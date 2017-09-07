#include <stdio.h>
#include <stdlib.h>

void print_spaces(int n) {
	for (int i=0; i<n; i++) { printf("   "); }
}

int main(int argc, char ** argv) {
	int count;
	printf("Enter the number of lines: ");
	scanf("%d", &count);

	// number of characters to output for each line
	int length = count*2 - 1;

	for (int i=1; i<=count; i++) {
		// how many numbers do we need to output
		int numbers = i*2-1;
		int spaces = (length - numbers);

		print_spaces(spaces/2);
		int center = (numbers-1)/2;
		for (int k=0; k<numbers; k++) { printf("%2d ", 1 + abs(center - k)); }

		printf("\n");
	}

	return 0;
}
