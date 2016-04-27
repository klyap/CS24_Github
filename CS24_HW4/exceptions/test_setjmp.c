#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Used to jump back to main() if we get a bad input. */
int corruption_check_left = 0;
jmp_buf env;
int corruption_check_right = 0;

/* Computes sqrt(x – 3), as long as x >= 3. Otherwise,
* performs a longjmp() back to an error handler.
*/
/*double compute(double x) {
 if (x < 3)
 longjmp(env, 1); //Jump to error handler!

 return sqrt(x – 3);
}*/


/*void longjmp_return_1() {
    printf("longjmp(env, 0) returns 1:    ");
    if (longjmp(env, 0) == 1) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

void longjmp_return_n() {
    printf("longjmp(env, n) returns n:    ");
    if (longjmp(env, 5) == 5) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

void longjmp_between_multiple() {
    printf("longjmp across multiple functions:    ");
    if (longjmp(env, 5) == 5) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}*/


void corruption_check(){
    if (corruption_check_left == corruption_check_right){
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

}

int h(int x){
    if (x < 5){
        longjmp(env, 1);
    }

    return sqrtl(x-5);
}

int g(int x){
    return h(15 - x);
}

int f(int x){
    if (setjmp(env) == 0){
        printf("Regular setjmp 1\n");
        return g(3 * x);
        printf("Regular setjmp 2\n");
    } else {
        printf("Longjmped back: %d\n", setjmp(env));
        return -1;
    }
}








int main(int argc, char *argv[]) {
	// TODO: Need functions with try/catch blocks and env to test on
    printf("Testing plain setjmp: ");
    f(0);
    printf("Longjmp returns 1: ");
    f(5);
    printf("Longjmp returns n / jumps multiple functions: ");
    f(2);

	/*printf("Testing plain setjmp: ");
    test3(0);
    printf("Longjmp returns 1: ");
    test3(1);
    printf("Longjmp returns n / jumps multiple functions: ");
    test3(2);*/
    printf("No corruption: ");
    corruption_check();

    return 0;
}

