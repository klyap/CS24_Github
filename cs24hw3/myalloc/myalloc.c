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

static header *freeptr;


/*!
 * This function initializes both the allocator state, and the memory pool.  It
 * must be called before myalloc() or myfree() will work at all.
 *
 * Note that we allocate the entire memory pool using malloc().  This is so we
 * can create different memory-pool sizes for testing.  Obviously, in a real
 * allocator, this memory pool would either be a fixed memory region, or the
 * allocator would request a memory region from the operating system (see the
 * C standard function sbrk(), for example).
 * 
 * Initalization: O(1)
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
    h->size = MEMORY_SIZE - 2 * sizeof(header);
    // Make footer
    mem + sizeof(header) + MEMORY_SIZE = h->size;
}


/*!
 * Attempt to allocate a chunk of memory of "size" bytes.  Return 0 if
 * allocation fails.
 * 
 * N = # of memory blocks
 * Allocation: O(N)
 * Block splitting: O(1)
 */
unsigned char *myalloc(int size) {

    
    /* Use Best Fit
     * Use an iterator pointer, freeptr, that iterates through all 
     * memory blocks in our allocated pool.
     * The goal is to find the smallest free block that fits 
     * the size we want to allocate (ie. the argument to myalloc()).
     * If freeptr finds a block that fits and is smaller than 
     * previously found blocks that also fit,
     * then record the address of this block in a pointer and
     * the amount of space this block has in a local variable.
     * 
     * If it reaches the end of memory pool and haven't found
     * a suitable block, 
     * print error message and return (unsigned char *) 0.
     * 
     * This method uses implicit free list, which is generally slow,
     * especially when there are a lot of memory blocks.
     * 
     * However, it's relatively good compared to other implicit
     * free list methods when there are a lot allocation calls because
     * it tries to minimize fragmentation.
     */

    // Flag for whether allocation succeeded.
    // 1 means there's an error. 0 means it's successful.
    int err = 1; 

    header *ret;

    int min_space = MEMORY_SIZE;
    int space;
    
    // Start at beginning of total memory pool
    freeptr = (header *) mem; // Iterator pointer
    header *bestfit = (header *) mem; // Free block that best fits so far


    // While the freeptr is still within the bounds of allocated memory pool
    while ( (unsigned char *)(freeptr + 2) + size < (mem + MEMORY_SIZE)){
        
        // Find out how much free space there is
        space = freeptr->size - (size + 2 * (int)sizeof(header));
        
        // If the block pointed at by freeptr is big enough:
        if (space > 0 && space < min_space ){
            bestfit = freeptr;
            min_space = space;
            err = 0; // Succesfully found free memory that fits
        }

        // Increment to next block by incrementing by
        // size of header and footer and payload of current block
        freeptr = (header *) ((unsigned char *) freeptr + 
            2 * sizeof(header) + abs(freeptr->size));
    }

    // If we haven't found a suitable block, return error
    if (err == 1){
        fprintf(stderr, "myalloc: cannot service request of size %d with"
                " %d bytes allocated\n", size, ((int)freeptr - (int)mem));
        return (unsigned char *) 0;
    }
    
    // Otherwise:
    // Adjust current block's header to reflect size allocated
    int old_block_size = bestfit->size;
    bestfit->size = -1 * size;
    (char *)bestfit + bestfit->size + sizeof(header) = bestfit->size;
    ret = bestfit;

    // Block splitting
    // Move to leftover region of the old block and add header there
    bestfit = (header *) ((unsigned char *) (bestfit + 1) + size);
    // Adjust header value to reflect remaining free block size
    bestfit->size = abs(old_block_size) - sizeof(header) - size;
    // Add footer
    *((char *)bestfit + bestfit->size + sizeof(header)) = bestfit->size;


    // Return pointer to payload of allocated block
    return (unsigned char *) (ret + 1);
    
}


/*!
 * Free a previously allocated pointer.
 * oldptr should be an address returned by myalloc().
 * 
 * 
 * N = # of memory blocks
 * Deallocation: O(1)
 * Coalescing: O(N)
 */
void myfree(unsigned char *oldptr) {

    /* Deallocation
     * Go to header of oldptr and flip its sign to positive
     * (ie. mark it as free)
     */
    oldptr -= sizeof(header);
    header *oldptr_h = (header *) oldptr;
    oldptr_h->size = oldptr_h->size * -1;
    *(oldptr + sizeof(header) + oldptr_h->size) = oldptr_h->size;

    /* Coalescing
     * Coalesce with free adjacent blocks.
     * Find previous block by using a runner pointer starting at 
     * the beginning of our memory pool and 
     * moving along with another pointer.
     */

    /* Find next block */
    header *next = (header *)((char *)(oldptr_h + 2) + abs(oldptr_h->size));

    /* Find previous block */
    // prevptr stores address of block just before the freed block
    header *prevptr = (header *)((char *)(oldptr_h - 2) - abs(oldptr_h->size))

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

