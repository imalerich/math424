#include <stdio.h>

#define PI 3.14159f

// problem 03
int main(int argc, char ** argv) {
	int r;
	fprintf(stdout, "Enter an integer: \n");
	scanf("%d", &r);

	fprintf(stdout, "Diameter %d\n", 2*r);
	fprintf(stdout, "Circumference %f\n", 2*PI*r);
	fprintf(stdout, "Area %f\n", PI*r*r);
	return 0;
}
