#include <stdio.h>

int MAX(int X, int Y) {
	if (X > Y) { return X; }
	return Y;
}

int main(int argc, char ** argv) {
	// these will be the three sides of our triangle
	int i, j, k;
	printf("Enter 3 sides of a triangle (integer lengths): ");
	scanf("%d %d %d", &i, &j, &k);

	if (i <= 0 || j <= 0 || k <= 0) {
		fprintf(stderr, "Error - all sides must be positive & nonzero!");
		return -1;
	}

	// the hypotenuse is the largest side entered
	int hyp = MAX(MAX(i, j), k);
	
	// check for a right triangle with the pythagorean theorem
	// a^2 + b^2 = c^2 
	// => a^2 + b^2 + c^2 = c^2 + c^2 
	// => a^2 + b^2 + c^2 = 2*c^2
	// note I don't know which i,j,k is hyp, 
	// but I do know it must be one of them

	if ((i*i + j*j + k*k) == 2*hyp*hyp) {
		printf("Nice Right Triangle!\n");
	} else {
		printf("Not a Right Triangle!\n");
	}

	return 0;
}
