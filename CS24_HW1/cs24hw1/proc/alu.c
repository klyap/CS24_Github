/*! \file
 *
 * This file contains definitions for an Arithmetic/Logic Unit of an
 * emulated processor.s
 */


#include <stdio.h>
#include <stdlib.h>   /* malloc(), free() */
#include <string.h>   /* memset() */

#include "alu.h"
#include "instruction.h"


/*!
 * This function dynamically allocates and initializes the state for a new ALU
 * instance.  If allocation fails, the program is terminated.
 */
ALU * build_alu() {
    /* Try to allocate the ALU struct.  If this fails, report error then exit. */
    ALU *alu = malloc(sizeof(ALU));
    if (!alu) {
        fprintf(stderr, "Out of memory building an ALU!\n");
        exit(11);
    }

    /* Initialize all values in the ALU struct to 0. */
    memset(alu, 0, sizeof(ALU));
    return alu;
}


/*! This function frees the dynamically allocated ALU instance. */
void free_alu(ALU *alu) {
    free(alu);
}


/*!
 * This function implements the logic of the ALU.  It reads the inputs and
 * opcode, then sets the output accordingly.  Note that if the ALU does not
 * recognize the opcode, it should simply produce a zero result.
 */
void alu_eval(ALU *alu) {
    uint32_t A, B, aluop;
    uint32_t result;

    A = pin_read(alu->in1);
    B = pin_read(alu->in2);
    aluop = pin_read(alu->op);

    result = 0;

    /*======================================*/
    /* TODO:  Implement the ALU logic here. */
    /*======================================*/
    switch (aluop) {
        
        case ALUOP_ADD:
        /*The opcode for simple addition*/
            result = A + B;
            break;
        case ALUOP_INV:
        /*The opcode for bitwise invert*/
            /*if (A) {
                result = ~A;
            } else if (B) {
                result = ~B;
            } else {
                result = 0;
            };*/
            result = ~A;
            break;
        case ALUOP_SUB:
        /*!< The opcode for simple subtraction. */
            result = A - B;
            break;
        case ALUOP_XOR:
        /*!< The opcode for bitwise exclusive-or. */
            result = A ^ B;
            break;
        case ALUOP_OR:
        /*!< The opcode for bitwise or. */
            result = A | B;
            break;
        case ALUOP_INCR:
        /*!< The opcode for incrementing a register. */
            result = A + 1;
            break;
        case ALUOP_AND:
        /*!< The opcode for bitwise and. */
            result = A & B;
            break;
        case ALUOP_SRA:
        /*!< The opcode for arithmetic shift-right. */
            result = A >> 1;
            result[31] = A[31];
            break;
        case ALUOP_SRL:
        /*!< The opcode for logical shift-right. */
            result = A >> 1;
            break;
        case ALUOP_SLA:
        /*!< The opcode for arithmetic shift-left. */
            result = A << 1;
            break;
        case ALUOP_SLL:
        /*!< The opcode for logical shift-left. */
            result = A << 1;
            break;
        default:
            result = 0;
            break;
    }
    /*======================================*/
    /* END:  Implement the ALU logic. */
    /*======================================*/

    pin_set(alu->out, result);
}

