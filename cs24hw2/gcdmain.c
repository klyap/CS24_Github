#include <stdio.h>
#include <stdlib.h>
#include "gcd.h"

int main(int argc, char **argv){
	if (argc != 3){
		printf("Need 2 arguments");
	}
	int arg1 = atoi(argv[1]);
	int arg2 = atoi(argv[2]);
	int out;

	// Arrange arguments s.t. the first one is larger than the second.
	// And then print the result.
	if (arg1 < arg2){
		out = gcd(arg1, arg2);
		printf("%d", out);
	} else {
		out = gcd(arg2, arg1);
		printf("%d", out);
	}
	
}
