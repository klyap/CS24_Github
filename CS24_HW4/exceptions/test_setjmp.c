#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

/* 
 */
void longjmp_return_1() {
    printf("longjmp(buf, 0) returns 1:    ");
    if (longjmp(buf, 0) == 1) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

void longjmp_return_n() {
    printf("longjmp(buf, n) returns n:    ");
    if (longjmp(buf, 5) == 5) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

void longjmp_between_multiple() {
    printf("longjmp across multiple functions:    ");
    if (longjmp(buf, 5) == 5) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

// TODO: 4th test: check functions don't corrupt stack


int main(int argc, char *argv[]) {
	// TODO: Need functions with try/catch blocks and buf to test on
	// Use divider and ptr_vector??
    longjmp_return_1();
    longjmp_return_n();
    longjmp_between_multiple();

    return 0;
}

