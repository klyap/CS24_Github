#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "c_except.h"
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
    printf("Testing jmp_buf is uncorrupted: \n");
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

    printf("PASS\n");
    return x-5;
}

int g(int x){

    return h(15 - x);
}

int no_exceptions(){
    int x = 1;
    printf("Testing regular setjmp with no exceptions thrown: \n");
    if (setjmp(env) == 0){
        //printf("Regular setjmp 1\n");
        return g(3 * x);
    } else {
        //printf("Longjmped back: %d\n", setjmp(env));
        printf("FAIL\n");
        return -1;
    }
}

int jump_across(){
    int x = 5;
    printf("Testing jumping across functions: \n");
    if (setjmp(env) == 0){
        //printf("Regular setjmp 1\n");
        return g(3 * x);
    } else {
        //printf("Longjmped back: %d\n", setjmp(env));
        printf("PASS\n");
        return -1;
    }
}

/*int no_exceptions(){
    
    printf("Test no exceptions: \n");
    if (setjmp(env) == 0){
        printf("PASS\n");
        return 1;
    } else {
        printf("FAIL\n");
        return -1;
    }
}*/

void jump_within(){
    printf("Test jump within function: \n");
    int x = 1;

    if (setjmp(env) == 0){
        if (x == 1){
            longjmp(env, 1);
        }
        printf("FAIL\n");
    } else {
        printf("PASS\n");
    }

}

/*void test_within(double x){
    TRY (
        double n1 = 1;
        double n2 = x;
        printf("The quotient of %lg / %lg is: %lg\n", n1, n2, n1/n2);
    )
    CATCH (NUMBER_PARSE_ERROR,
        printf("Ack!! I couldn't parse what you entered!\n");
    )
    CATCH (DIVIDE_BY_ZERO,
        printf("Ack!! You entered 0 for the divisor!\n");
        RETHROW;
    )
    END_TRY;
}*/



int main(int argc, char *argv[]) {
	// TODO: Need functions with try/catch blocks and env to test on
    // printf("Test within on DIVIDE_BY_ZERO\n");
    // test_within(0.0);
    // printf("Test within on 1.0\n");
    // test_within(1.0);
    // printf("Test within on 1\n");
    // test_within(1);
    // printf("Test within on char\n");
    // test_within('a');

    /*int a = f(0);
    printf("Testing plain setjmp: %d", a);
    int b = f(5);
    printf("Longjmp returns 1: %d", b);
    int c = f(2);
    printf("Longjmp returns n / jumps multiple functions: %d", c);*/
    
    /*printf("/n/n/n/n");
    int d = f(1);
    printf("Longjmp within function: %d", d);*/

    jump_across();
    no_exceptions();
    jump_within();
    corruption_check();

    /*printf("Testing plain setjmp: ");
    test3(0);
    printf("Longjmp returns 1: ");
    test3(1);
    printf("Longjmp returns n / jumps multiple functions: ");
    test3(2);
    printf("No corruption: ");
    corruption_check();*/

    return 0;
}

