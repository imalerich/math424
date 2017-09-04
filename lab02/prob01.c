#include <stdio.h>

// this is in math.h as 'pow', but I assume I'm not allowed to use that
// not really sure if I'm allowed to create my own functions either
// but you did technically briefly touch on those one day
int impow(int a, int b) {
	if (b == 0) { return 1; }
	// would be a lot nicer recursively, or at least a for loop
	// but I've only seen you use a while loop, so here ya go
	while (b > 1) {
		a *= a;
		b--;
	}

	return a;
}

int main(int argc, char ** argv) {
	int bin;
	printf("Enter a binary number (5 bits or fewer): ");
	scanf("%d", &bin);

	// working decimal value 
	int dec = 0;

	// the offset of the current bit we are looking at
	int bit = 4; // 4th bit (0 based, so 5 max bits)
	// can use the following instead to support any number of bits ;)
	// int bit = log10(dec) + 1;

	while (bit >= 0) {
		// set = 1 if 'bit' is set, 0 otherwise
		int set = bin / impow(10, bit);
		bin -= set * impow(10, bit); // remove the information we retreived
	
		// now update our decimal, using powers of 2
		dec += set * impow(2, bit);
		bit--;
	}

	printf("You're binary number in decimal is: %d\n", dec);
	
	return 0;
}
