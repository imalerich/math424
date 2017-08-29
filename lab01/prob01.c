#include <stdio.h>

// problem 01
int main(int argc, char ** argv) {
	int i, j;
	fprintf(stdout, "Enter two integers: ");
	scanf("%d %d", &i, &j);

	fprintf(stdout, "----------------------------------\n");
	fprintf(stdout, "The sum is:\t\t%d\n", (i+j));
	fprintf(stdout, "The difference is:\t%d\n", (i-j));
	fprintf(stdout, "The product is:\t\t%d\n", (i*j));
	fprintf(stdout, "The quotient is:\t%f\n", ((float)i)/j);
	fprintf(stdout, "The remainder is:\t%d\n", (i%j));
	fprintf(stdout, "----------------------------------\n");
	return 0;
}
