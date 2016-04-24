.globl gcd	           /* Ignore everything beginning with a "." */
.text
	.align 4
gcd:
	pushl %ebp          /* Push old base pointer. */
	movl %esp,%ebp		/* Current stack is new base. */
	cmpl $0, 12(%ebp)	/* Compare 0 and arg2 */
	jne gcd_recurse		/* If arg2 != 0, go to gcd_recurse */
	movl 8(%ebp), %eax	/* Otherwise, arg1 is output */
	jmp gcd_return		/* Go to gcd_return */
gcd_recurse:
	movl $0, %edx		/* Set up all zeroes in %edx of %edx:%eax */
	movl 8(%ebp), %eax	/* Set up %eax = arg1 s.t. %edx:%eax = arg1 */
	movl 12(%ebp), %ecx	/* %ecx = arg2 */
	divl %ecx           /* Divide arg1 and arg2 (automatically puts remainder in %edx)*/
	movl %edx, %eax		/* %eax = arg2 % arg1 (aka remainder from prev operation) */
	push %eax           /* Push %eax (turns into arg2)*/
	push %ecx           /* Push %ecx (turns into arg1)*/
	call gcd            /* Recursive call */

gcd_return:
	movl %ebp, %esp		/* Pop local stack */
	popl %ebp           /* Pop old base of frame */
	ret
