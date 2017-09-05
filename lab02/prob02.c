#include <stdio.h>

int main(int argc, char ** argv) {
	// these will be the three sides of our triangle
	int i, j, k;
	printf("Enter 3 sides of a triangle (integer lengths): ");
	scanf("%d %d %d", &i, &j, &k);

	if (i <= 0 || j <= 0 || k <= 0) {
		fprintf(stderr, "Error - all sides must be positive & nonzero!");
		return -1;
	}

	// not really sure what I'm allowed to use to get credit
	// so let's just try all the possible cases one by one
	
	// check for a triangle with the triangle inequality theorem
	// by checking to make sure that the smaller two sides
	// add up to and are greater than the hypotenuse
	
	// I think you don't want logical AND's
	// but nesting all these cases would be a pain in the butt
	if (i <= k && j <= k && (i + j) > k) {
		// k is the hypotenuse
		printf("Nice Triangle!\n");
		return 0;

	} else if (i <= j && k <= j && (i + k) > j) {
		// j is the hypotenuse
		printf("Nice Triangle!\n");
		return 0;
		
	} if (j <= i && k <= i && (k + j) > i) {
		// i is the hypotenuse
		printf("Nice Triangle!\n");
		return 0;
	}

	printf("Not a Triangle!\n");
	return 0;
}
