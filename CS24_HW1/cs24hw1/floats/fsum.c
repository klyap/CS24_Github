#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "ffunc.h"


/* This function takes an array of single-precision floating point values,
 * and computes a sum in the order of the inputs.  Very simple.
 */
float fsum(FloatArray *floats) {
    float sum = 0;
    int i;

    for (i = 0; i < floats->count; i++)
        sum += floats->values[i];

    return sum;
}


/* TODO:  IMPLEMENT my_fsum() HERE, AND DESCRIBE YOUR APPROACH. */
/* My approach uses a compensation term to make up
for the rounding inaccuracies. Algebraically, c should always be 0,
but when sum is a lot bigger than the next value to be added to it,
the less significant digtis will be rounded away. 
To recover this value, we subtract nextsum from sum to get a
number closer in magnitude to floats->values[i], so that when we 
subtract the two, the result won't be rounded away.
The compensationary terms are kept track of separately in a sum of
other compensationary terms, which are closer in range to
each other. This sum of c's is then added to the main sum at the end.
 */
float my_fsum(FloatArray *floats) {
    float sum = 0;
    float nextsum = 0;
    float sumc = 0;
    int i, c;

    for (i = 0; i < floats->count; i++){
        nextsum = sum + floats->values[i];
        c = (nextsum - sum) - floats->values[i];
        sumc += c;
    }
    return sum + c;
}


int main() {
    FloatArray floats;
    float sum1, sum2, sum3, my_sum;

    load_floats(stdin, &floats);
    printf("Loaded %d floats from stdin.\n", floats.count);

    /* Compute a sum, in the order of input. */
    sum1 = fsum(&floats);

    /* Use my_fsum() to compute a sum of the values.  Ideally, your
     * summation function won't be affected by the order of the input floats.
     */
    my_sum = my_fsum(&floats);

    /* Compute a sum, in order of increasing magnitude. */
    sort_incmag(&floats);
    sum2 = fsum(&floats);

    /* Compute a sum, in order of decreasing magnitude. */
    sort_decmag(&floats);
    sum3 = fsum(&floats);

    /* %e prints the floating-point value in full precision,
     * using scientific notation.
     */
    printf("Sum computed in order of input:  %e\n", sum1);
    printf("Sum computed in order of increasing magnitude:  %e\n", sum2);
    printf("Sum computed in order of decreasing magnitude:  %e\n", sum3);

    /* TODO:  UNCOMMENT WHEN READY!
    printf("My sum:  %e\n", my_sum);
    */

    return 0;
}

