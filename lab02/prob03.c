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

	// the hypotenuse is the largest side entered
	int hyp = MAX(MAX(i, j), k);

	if (i <= 0 || j <= 0 || k <= 0) {
		fprintf(stderr, "Error - all sides must be positive & nonzero!");
		return -1;
	}

	// not really sure what I'm allowed to use to get credit
	// so let's just try all the possible cases one by one
	
	// check for a right triangle with the pythagorean theorem

	// I think you don't want logical AND's
	// but nesting all these cases would be a pain in the butt
	if (i*i + j*j == hyp*hyp) {
		printf("Nice Right Triangle!\n");
		return 0;
	} else if (i*i + k*k == hyp*hyp) {
		printf("Nice Right Triangle!\n");
		return 0;
	} else if (j*j + k*k == hyp*hyp) {
		printf("Nice Right Triangle!\n");
		return 0;
	}

	printf("Not a Right Triangle!\n");
	return 0;
}
