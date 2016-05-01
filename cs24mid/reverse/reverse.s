# reverse_list(LinkedList *list)
# This function reverses a linked list in place and returns it.
# 
# 8(%ebp) = LinkedList *list
# %eax = list
# %ebx = prev
# %ecx = head
# %edx = next

.globl reverse_list
.text
	.align 4

reverse_list:
	pushl %ebp			/* Push old base pointer */
	movl %esp,%ebp      /* Current stack is new base.*/
	movl 8(%ebp), %eax	/* Store pointer to the list */
	movl $0, %ecx		/* Temp storage for 0 so 0 is the right size */
	cmpl (%eax), %ecx	/* If list->head == NULL */
	je done				/* Then go to done */
	cmpl 4(%eax), %ecx	/* If list->tail == NULL */
	je done				/* Then go to done */

code:
	movl (%eax), %ecx	/* Set head = list->head */
	movl %ecx, 4(%eax)	/* Set list->tail = head */
	movl $0, %ebx		/* Set prev = NULL */

	cmpl %ecx, %ebx		/* If head == NULL */
	je done				/* Then go to done */

loop:
	movl (%ecx), %edx	/* Set next = head->next */
	movl %ebx, 4(%ecx)	/* Set head->next = prev */
	movl %ecx, %ebx		/* Set prev = head */
	movl %edx, %ecx		/* Set head = next */
	movl %ecx, (%eax)	/* Set list->head = head */

	cmpl %ecx, %ecx		/* If head == NULL */
	jz done				/* Then go to done */

done:
	popl %ebp           /* Pop old base of frame. */
	ret 				/* Return */

