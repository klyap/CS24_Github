.globl setjmp	       /* Ignore everything beginning with a "." */
.text
	.align 4
setjmp:
	pushl %ebx         /* Push callee save registers. */
	pushl %esi         /* Push callee save registers. */
	pushl %edi         /* Push callee save registers. */
	pushl %esp         /* Push stack frame. */
	pushl %ebp         /* Push old base pointer. */
	movl %esp,%ebp	   /* Current stack is new base. */
	movl $0, %eax

setjmp_return:
	movl %ebp,%esp     /* Pop local stack. */
	popl %ebp          /* Pop old base of frame. */
	ret

longjmp:
	pushl %ebp         /* Push old base pointer. */
	movl %esp,%ebp	   /* Current stack is new base. */
	/* Go to env */
	/* Restore callee save registers */
	/* Set return value to second argument, 8(%ebp)*/

longjmp_return:
	movl %ebp,%esp     /* Pop local stack. */
	popl %ebp          /* Pop old base of frame. */
	ret

