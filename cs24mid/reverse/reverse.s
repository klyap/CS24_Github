# 8(%ebp) = LinkedList *list
# %eax = list
# %ebx = prev
# %ecx = head
# %edx = next

.globl reverse	           /* Ignore everything beginning with a "." */
.text
	.align 4

reverse:
	pushl %ebp			/* Push old base pointer */
	movl %esp,%ebp      /* Current stack is new base.*/

code:
	movl 8(%ebp), %eax	/* Store list */
	movl (%eax), %ecx	/* Set head = list->head */
	
	movl %ecx, 4(%eax)	/* Set list->tail = head */

	movl NULL, %ebx		/* Set prev = NULL */

	cmpl %ecx, NULL		/* If head == NULL */
	je done				/* Then go to done */

loop:
	movl (%ecx), %edx	/* Set next = head->next */
	movl %ebx, 4(%ecx)	/* Set head->next = prev */
	movl %ecx, %ebx		/* Set prev = head */
	movl %edx, %ecx		/* Set head = next */
	movl %ecx, (%eax)	/* Set list->head = head */

	cmpl %ecx, NULL		/* If head == NULL */
	je done				/* Then go to done */

done:
	popl %ebp           /* Pop old base of frame. */
	ret 				/* Return */

