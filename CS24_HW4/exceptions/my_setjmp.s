/* Arguments and where they are stored 
int my_setjmp(my_jmp_buf buf);
void my_longjmp(my_jmp_buf buf, int ret);
*/

.globl setjmp	       /* Ignore everything beginning with a "." */
.text
	.align 4
my_setjmp:
	pushl %ebp         	/* Push old base pointer. */
	movl %esp,%ebp	   	/* Current stack is new base. */
	movl 8(%ebp), %ecx	/* Store jmp_buf. */
	movl %esp, (%ecx)	/* Move to jmp_buf */
	movl %ebx, 4(%ecx)         	/* Push callee save registers. */
	movl %esi, 8(%ecx)         /* Push callee save registers. */
	movl %edi, 12(%ecx)        /* Push callee save registers. */
	movl %esp, 16(%ecx)        /* Push stack frame. */
	
	xor %eax, %eax		/* Set %eax to 0

	popl %ebp          /* Pop old base of frame. */
	ret

my_longjmp:
	pushl %ebp         	/* Push old base pointer. */
	movl %esp,%ebp	   	/* Current stack is new base. */
	movl 12(%ebp), %eax	/* Store ret */
	movl 8(%ebp), %ecx	/* Store jmp_buf */
	movl 4(%ecx), %ebx         	/* Restore callee save registers. */
	movl 8(%ecx), %esi          /* Restore callee save registers. */
	movl 12(%ecx), %edi         /* Restore callee save registers. */
	movl 16(%ecx), %esp        /* Restore stack frame. */
	cmpl $0, %eax		/* If ret != 0 */
	jnz done			/* go to return 0 */
	movl $1, %eax		/* Else go to return 1 */
done:
	popl %ebp          /* Pop old base of frame. */
	ret

