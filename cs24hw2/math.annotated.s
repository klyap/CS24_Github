/* This file contains IA32 assembly-language implementations of three
 * basic, very common math operations.
 *
 * These operations all avoid using jumps. 
 * Processors often try to optimize computations by
 * preprocessing instructions. However, when jumps are used, these
 * precalculated values are discarded and all that work is wasted.
 * Additionally, jumps divert processes to branches of different
 * instructions and creates code with variable runtimes. 
 */

    .text

/*====================================================================
 * int f1(int x, int y)
 * min(x,y)
 */
.globl f1
f1:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx	/* Store x in %edx */
	movl	12(%ebp), %eax	/* Store y in %eax */
	cmpl	%edx, %eax		/* Compare y and x */
	cmovg	%edx, %eax		/* If x > y, move y into %eax */
	popl	%ebp
	ret


/*====================================================================
 * int f2(int x)
 * abs(x)
 * By doing an arithmetic shift right, %edx becomes either 32 ones or
 * 32 zeroes, depending on whether x is negative or positive/zero, respectively
 * This does nothing to x when it is XORed and subtracted if x >= 0.
 * However, these 2 operations basically do a twos complement on x
 * when x < 0, since %edx represents -1 due to overflow,
 * returning the positive version of x.
 */
.globl f2
f2:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax	/* Store x in %eax */
	movl	%eax, %edx		/* Copy x into %edx */
	sarl	$31, %edx		/* If x < 0, store all ones in %edx, otherwise store all zeroes */
	xorl	%edx, %eax		/* If x < 0, flip bits in x */
	subl	%edx, %eax		/* If x < 0, add 1 to flipped bits */
	popl	%ebp
	ret


/*====================================================================
 * int f3(int x)
 * sgn(x)
 * If x < 0, %eax becomes all ones (ie -1 due to overflow). 
 * If x >= 0, %eax becomes all zeroes.
 * If x <= 0, the testl operation sets flags st cmovg will not be satisfied.
 * 0x1 is stored in %edx, and if x > 0, this value is put in %eax and returned.
 * This leaves the cases where x = 0, and %eax remains 0x0 and is returned.
 * The last case is where x < 0, and -1 is returned.
 * 
 */
.globl f3
f3:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx	/* Store x in %edx */
	movl	%edx, %eax		/* Copy x into %eax */
	sarl	$31, %eax		/* If x < 0, store all ones in %eax. Else store all zeroes. */
	testl	%edx, %edx		/* Sets flags as if we did x AND x */
	movl	$1, %edx		/* Put 1 in %edx (does not affect flags)*/
	cmovg	%edx, %eax		/* If x > 0, return 1; if x = 0, return 0, if x < 0, return -1 */
	popl	%ebp
	ret

