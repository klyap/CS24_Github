/*! \file
 * Implementation of a simple memory allocator.  The allocator manages a small
 * pool of memory, provides memory chunks on request, and reintegrates freed
 * memory back into the pool.
 *
 * Adapted from Andre DeHon's CS24 2004, 2006 material.
 * Copyright (C) California Institute of Technology, 2004-2010.
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>

#include "myalloc.h"


/*!
 * These variables are used to specify the size and address of the memory pool
 * that the simple allocator works against.  The memory pool is allocated within
 * init_myalloc(), and then myalloc() and free() work against this pool of
 * memory that mem points to.
 */
int MEMORY_SIZE;
unsigned char *mem;

/* Made a struct for boundary tags. This stores the header. */
typedef struct header {
    int size; // Size of payload; Negative if used, positive if free
}header;

/* TODO:  The unacceptable allocator uses an external "free-pointer" to track
 *        where free memory starts.  If your allocator doesn't use this
 *        variable, get rid of it.
 *
 *        You can declare data types, constants, and statically declared
 *        variables for managing your memory pool in this section too.
 */
static header *freeptr;
header *bestfit;
int min_space;


/*!
 * This function initializes both the allocator state, and the memory pool.  It
 * must be called before myalloc() or myfree() will work at all.
 *
 * Note that we allocate the entire memory pool using malloc().  This is so we
 * can create different memory-pool sizes for testing.  Obviously, in a real
 * allocator, this memory pool would either be a fixed memory region, or the
 * allocator would request a memory region from the operating system (see the
 * C standard function sbrk(), for example).
 */
void init_myalloc() {

    /*
     * Allocate the entire memory pool, from which our simple allocator will
     * serve allocation requests.
     */
    mem = (unsigned char *) malloc(MEMORY_SIZE);
    if (mem == 0) {
        fprintf(stderr,
                "init_myalloc: could not get %d bytes from the system\n",
                MEMORY_SIZE);
        abort();
    }

    /* Initialize the initial state of your memory pool. */
    // Make a header struct.
    header *h = (header *) mem; 
    // Initialize it to whole memory pool.
    h->size = MEMORY_SIZE - sizeof(struct header);

}


/*!
 * Attempt to allocate a chunk of memory of "size" bytes.  Return 0 if
 * allocation fails.
 */
unsigned char *myalloc(int size) {

    /* TODO:  The unacceptable allocator simply checks to see if there are at
     *        least "size" bytes left in the pool, and if so, the caller gets
     *        the current "free-pointer" value, and then freeptr is incremented
     *        by size bytes.
     *
     *        Your allocator will be more sophisticated!
     */
    
    /* Use First Fit
     * Go to current pointer in memory pool pointing to next free space:
     * If header > size, then set header = size + sizeof(header) + sizeof(footer)
     *     return pointer to this header
     * Else, increment pointer to next block and check again.
     * If reach end of memory pool, return error
     * 
     * This is inefficient in cases where 
     */

    // Flag for whether allocation succeeded.
    // 1 means there's an error. 0 means it's successful.
    int err = 1; 

    header *ret;
    min_space = MEMORY_SIZE;

    // Start at beginning of total memory pool
    freeptr = (header *) mem;
    int space;
    fprintf(stderr,"------------- %d \n", size);
    // While the freeptr is still within the bounds of allocated memory pool
    while ( (unsigned char *)(freeptr + 1) + size < (mem + MEMORY_SIZE)){
        
        // If the block pointed at by freeptr is big enough:
        space = freeptr->size - (size + (int)sizeof(header));
        if (space > 0 && space < min_space ){
            bestfit = freeptr;
            min_space = space;
            fprintf(stderr,"myalloc: found %p %d\n", bestfit, space);

        } 
        // If it doesn't fit, go to next block by incrementing by
        // size of header and payload of current block
        freeptr = (header *) ((unsigned char *) freeptr + sizeof(header) + abs(freeptr->size));
        fprintf(stderr,"myalloc: moving from %p \n", freeptr); 
    }
    
    fprintf(stderr,"myalloc: is %p %d\n", bestfit, space);
    // Adjust current block's header to reflect size allocated
    int old_block_size = bestfit->size;
    bestfit->size = -1 * size;
    ret = bestfit;

    // Block splitting
    // Move to leftover region of the old block and add header there
    bestfit = (header *) ((unsigned char *) (bestfit + 1) + size);
    // Adjust header value to reflect remaining free block size
    bestfit->size = abs(old_block_size) - sizeof(header) - size;

    err = 0; // Succesfully allocated memory
    
    if (err == 1){
        fprintf(stderr, "myalloc: cannot service request of size %d with"
                " %d bytes allocated\n", size, ((int)freeptr - (int)mem));
        return (unsigned char *) 0;
    }

    // Return pointer to payload of allocated block
    return (unsigned char *) (ret + 1);
    
}


/*!
 * Free a previously allocated pointer.  oldptr should be an address returned by
 * myalloc().
 */
void myfree(unsigned char *oldptr) {

    /* Simple free
     * Go to header of oldptr and set to negative (ie. free it)
     */
    oldptr -= sizeof(header);
    header *oldptr_h = (header *) oldptr;
    oldptr_h->size = oldptr_h->size * -1;

    /* Coalescing
     * Coalesce with free adjacent blocks.
     * Find previous block by using a runner pointer starting at the beginning
     * of our memory pool and moving along with another pointer.
     */

    /* Find next block */
    header *next = (header *)((char *)(oldptr_h + 1) + abs(oldptr_h->size));

    /* Find previous block */
    // prevptr stores address of block just before the freed block
    header *prevptr = (header *) mem;

    // prev is the runner pointer that checks if the block next to prevptr
    // is the freed block
    header *prev = prevptr;

    // While we haven't found the block before the freed block
    while (prev != oldptr_h){
        // Move both prevptr and prev ahead to next block
        prevptr = prev;
        prev = (header *)((char *)(prevptr + 1) + abs(prevptr->size)); 
    }

    // Coalesce freed block with next block if its free 
    // and the next block is still in our allocated memory pool
    if (next->size > 0 && next < (header *)(mem + MEMORY_SIZE)){
        oldptr_h->size = oldptr_h->size + next->size + sizeof(header);
    }

    // Coalesce freed block with prev block if its free
    // and the prev block is still in our allocated memory pool
    if (prevptr->size > 0 && prevptr != (header *)mem){
        prevptr->size = oldptr_h-> size + prevptr->size + sizeof(header);
    }
     


}

