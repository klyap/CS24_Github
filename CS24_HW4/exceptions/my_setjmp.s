/* Arguments and where they are stored 
int my_setjmp(my_jmp_buf buf);
void my_longjmp(my_jmp_buf buf, int ret);
*/

.globl my_setjmp	       /* Ignore everything beginning with a "." */
.globl my_longjmp	       /* Ignore everything beginning with a "." */
.text
	.align 4
my_setjmp:
	pushl %ebp         	/* Push old base pointer. */
	movl %esp,%ebp	   	/* Current stack is new base. */
	movl 8(%ebp), %ecx	/* Store jmp_buf. */
	movl %esp, (%ecx)	/* Move to jmp_buf (and push stack pointer) */
	movl %ebx, 4(%ecx)  /* Push callee save registers. */
	movl %esi, 8(%ecx)  /* Push callee save registers. */
	movl %edi, 12(%ecx) /* Push callee save registers. */
	movl 4(%ebp), %edx	/* Temp storage for return address */
	movl %edx, 16(%ecx) /* Push return address. */
	movl (%ebp), %edx	/* Temp storage for caller return address */
	movl %edx, 20(%ecx) /* Push base pointer */

	/*movl %esp, 16(%ecx)         Push stack frame. */
	
	xor %eax, %eax		/* Set %eax to 0

	popl %ebp           /* Pop old base of frame. */
	ret 				/* Return */

my_longjmp:
	pushl %ebp         	/* Push old base pointer. */
	movl %esp,%ebp	   	/* Current stack is new base. */
	movl 12(%ebp), %eax	/* Store ret value */
	movl 8(%ebp), %ecx	/* Store jmp_buf */
	movl (%ecx), %esp	/* Restore stack pointer to before my_setjmp */
	movl 4(%ecx), %ebx  /* Restore callee save registers. */
	movl 8(%ecx), %esi  /* Restore callee save registers. */
	movl 12(%ecx), %edi /* Restore callee save registers. */
	movl 16(%ecx), %edx	/* Temp storage */
	movl %edx, 4(%esp)  /* Restore return address. */
	movl 20(%ecx), %edx	/* Temp storage for my_setjmp's caller's stack pointer */
	movl %edx, (%esp)	/* Restore my_setjmp's caller's stack pointer */
	cmpl $0, %eax		/* If ret != 0 */
	jnz done			/* go to return 0 */
	movl $1, %eax		/* Else go to return 1 */
done:
	popl %ebp           /* Pop old base of frame. */
	ret 				/* Return */

