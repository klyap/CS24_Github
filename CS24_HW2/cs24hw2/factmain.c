#include<stdio.h>
#include<stdlib.h>
#include"fact.h"

int main(int argc, char *argv[]) {
  if (argc != 2){
  	printf("error: need 1 argument");
  }

  int arg = atoi(argv[1]);

  if (arg < 0){
  	printf("error: argument must be non-negative");
  	return -1;
  }

  // If there are no errors, run the factorial instructions
  // in fact.s and print result.
  int out = fact(arg);
  printf("%d\n", out);
  
}
