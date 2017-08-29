#include <stdio.h>

// problem 01
int main(int argc, char ** argv) {
	int i, j;
	fprintf(stdout, "Enter two integers: \n");
	scanf("%d %d", &i, &j);

	fprintf(stdout, "The sum is: %d\n", (i+j));
	fprintf(stdout, "The difference is: %d\n", (i-j));
	fprintf(stdout, "The product is: %d\n", (i*j));
	fprintf(stdout, "The quotient is: %f\n", ((float)i)/j);
	fprintf(stdout, "The remainder is: %d\n", (i%j));
	return 0;
}
