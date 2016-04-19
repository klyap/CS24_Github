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

/* Made a struct for header */
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

    /* TODO:  You can initialize the initial state of your memory pool here. */
    //freeptr = (header *) mem; // Create pointer to space of size of header struct
    header *h = (header *) mem; // Make a header struct.
    h->size = MEMORY_SIZE - sizeof(struct header); // Initialize it.
    //freeptr = h; // Point freeptr to h

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
    /*if (freeptr + size < mem + MEMORY_SIZE) {
        unsigned char *resultptr = freeptr;
        freeptr += size;
        return resultptr;
    }
    else {
        fprintf(stderr, "myalloc: cannot service request of size %d with"
                " %d bytes allocated\n", size, (freeptr - mem));
        return (unsigned char *) 0;
    }*/

    /// Use Next Fit
    // Go to current pointer in memory pool pointing to next free space:
    // If header > size, then set header = size + sizeof(header) + sizeof(footer)
    //     return pointer to this header
    // Else, increment pointer to next block and check again.
    // If reach end of memory pool, return error

    int err = 1;
    header *ret;

    freeptr = (header *) mem;
    fprintf(stderr, "alloc: Looking for block size: %d \n", size);
    while ( (freeptr + 1) + size/sizeof(header) < (header *) (mem + MEMORY_SIZE)){
        fprintf(stderr, "alloc: size of block (freeptr): %d, %p \n", freeptr->size, freeptr);
        if (freeptr->size > MEMORY_SIZE){
            fprintf(stderr, "alloc: pointer is too big: %d, %p \n", freeptr->size, freeptr);
        }

        if (freeptr->size > size + (int)sizeof(header)){
            fprintf(stderr, "alloc: block found: %d, %d \n", freeptr->size, size + sizeof(header));
            // If it fits:
            int old_block_size = freeptr->size;
            freeptr->size = -1 * size;
            ret = freeptr;
            freeptr = (header *) ((unsigned char *) (freeptr + 1) + size); // at begining of next block
            freeptr->size = abs(old_block_size) - sizeof(header) - size;

            err = 0;
            break;
        } else {
            // If it doesn't fit, go to next block by incrementing by
            // size of header and payload of current block
            //err = 3;
            freeptr = (header *) ((unsigned char *) freeptr + sizeof(header) + abs(freeptr->size));
        }
    }
    
    if (err == 1){
        fprintf(stderr, "myalloc: cannot service request of size %d with"
                " %d bytes allocated\n", size, ((int)freeptr - (int)mem));
        return (unsigned char *) 0;
    }


    return (unsigned char *) (ret + 1);

    /// Block splitting
    // If header >= 2 * size, then:
    // Go to size + header: add footer (size of header + footer + size)
    // Go to size + header + footer: new header for empty block
    //    (orignal header - new header)
    
}


/*!
 * Free a previously allocated pointer.  oldptr should be an address returned by
 * myalloc().
 */
void myfree(unsigned char *oldptr) {
    /* TODO:
     *
     * The unacceptable allocator does nothing -- that's part of why this is
     * unacceptable!
     *
     * Allocations will succeed for a little while...
     */

     /// Simple free
     // Go to header of oldptr and set to negative
     // Go to footer and set to negative
     oldptr -= sizeof(header);
     header *oldptr_h = (header *) oldptr;
     if (oldptr_h->size > 0){
        fprintf(stderr, "myfree: pointer is too big: %d, %p \n", oldptr_h->size, oldptr_h);
     }

     oldptr_h->size = oldptr_h->size * -1;
     fprintf(stderr, "myfree: freed: %d, %p \n", oldptr_h->size, oldptr_h);
     
     /// Coalescing
     // Go to header of block we just freed.
     // if header - 1 (prev's tail) > 0, then it is free so:
     //  set prev_free to footer value
     // if header + size + footer + 1 (next's header) > 0, then it is free so:
     //  set next_free to footer value 
     // Set this block's header and footer = prev_free + next_free

     header *prevptr = (header *) mem;
     header *prev = prevptr;
     while (prev != oldptr_h){
        prevptr = prev;
        prev = (header *)((char *)(prevptr + 1) + abs(prevptr->size));
     }

     header *next = (header *)((char *)(oldptr_h + 1) + abs(oldptr_h->size));

     
     if (next->size > 0){
        oldptr_h->size = oldptr_h->size + next->size + sizeof(next);
        fprintf(stderr, "myfree: coalesced next: %d, %p \n", oldptr_h->size, oldptr_h);
     }

     if (prevptr->size > 0){
        prevptr->size = oldptr_h-> size + prevptr->size + sizeof(prevptr);
     }
     




}

