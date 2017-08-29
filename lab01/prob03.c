#include <stdio.h>
#define PI 3.14159f

// problem 03
int main(int argc, char ** argv) {
	int r;
	fprintf(stdout, "Enter an integer: ");
	scanf("%d", &r);

	fprintf(stdout, "----------------------------------\n");
	fprintf(stdout, "Diameter:\t%d\n", 2*r);
	fprintf(stdout, "Circumference:\t%f\n", 2*PI*r);
	fprintf(stdout, "Area:\t\t%f\n", PI*r*r);
	fprintf(stdout, "----------------------------------\n");
	return 0;
}
