#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "c_except.h"
/* Used to jump back to main() if we get a bad input. */
int corruption_check_left = 0;
jmp_buf env;
int corruption_check_right = 0;

/*
Helper functions that long jumps back if x < 5 without returning.
Otherwise, it returns x - 5.
*/
int h(int x){
    if (x < 5){
        longjmp(env, 1);
    }

    printf("    PASS: Testing regular setjmp with no exceptions thrown\n");
    return x - 5;
}

/*
Helper function that calls h(x)
*/
int g(int x){
    return h(15 - x);
}

/*
  Tests case where no exceptions are thrown. It should call h(x) succesfully.
*/
int no_exceptions(){
    int x = 1;
    printf("Testing regular setjmp with no exceptions thrown: \n");
    if (setjmp(env) == 0){
        return g(3 * x);
    } else {
        printf("    FAIL: Testing regular setjmp with no exceptions thrown\n");
        return -1;
    }
}

/* 
  Tests whether you can long jump across the various functions
  (ie. g() and h()). h(x) should thrown an exception that
  this function catches.
*/
int jump_across(){
    int x = 5;
    printf("Testing jumping across functions: \n");
    
    if (setjmp(env) == 0){
        return g(3 * x);
    } else {
        printf("    PASS: Testing jumping across functions\n");
        return -1;
    }
}

/* 
  Tests whether you can long jump into back within
  the same function.
*/
void jump_within(){
    int x = 1;
    printf("Test jump within function: \n");

    if (setjmp(env) == 0){
        if (x == 1){
            longjmp(env, 1);
        }
        printf("    FAIL: Test jump within function\n");
    } else {
        printf("    PASS: Test jump within function\n");
    }

}

/* Tests whether long jump returns n, which is defined
  to be 2 in this test case.
*/
int jump_return_n(){
    printf("Test jump return n: \n");
    
    int curr_env = setjmp(env);
    if (curr_env == 0){
        longjmp(env, 2);
    } else if (curr_env == 2){
        printf("    PASS: Test jump within function\n");
        return curr_env;
    }
    printf("    FAIL: Test jump within function\n");
    return curr_env;
}

/* Tests whether the jump buffer was corrupted by checking
 whether the two local variables on either side of it
 are still what I had defined them to be initially.
*/
void corruption_check(){
    printf("Testing jmp_buf is uncorrupted: \n");
    if (corruption_check_left == corruption_check_right){
        printf("    PASS: Testing jmp_buf is uncorrupted\n");
    } else {
        printf("    FAIL: Testing jmp_buf is uncorrupted\n");
    }

}

int main(int argc, char *argv[]) {
    // Test 1
    if (jump_across() != -1){
        printf("    FAIL: Testing jumping across functions\n");
    };
    // Test 2
    no_exceptions();
    // Test 3
    jump_within();
    // Test 4
    corruption_check();
    // Test 5
    jump_return_n();


    return 0;
}

