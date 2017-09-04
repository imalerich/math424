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

	// check for a triangle with the triangle inequality theorem
	// by checking to make sure that the smaller two sides
	// add up to and are greater than the hypotenuse
	
	// the hypotenuse is the largest side entered
	int hyp = MAX(MAX(i, j), k);
	
	// one of i,j,k is the hypotenuse, 
	// subtract it to get the sum of the smaller two sides
	// then compare it to the largest side
	if ((i + j + k) - hyp > hyp) {
		printf("Nice Triangle!\n");
	} else {
		printf("Not a Triangle!\n");
	}

	return 0;
}
