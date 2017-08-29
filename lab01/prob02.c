#include <stdio.h>

#define MIN(X, Y) (X < Y ? X : Y)
#define MAX(X, Y) (X > Y ? X : Y)

// problem 02
int main(int argc, char ** argv) {
	int i, j, k;
	fprintf(stdout, "Enter three different integers: \n");
	scanf("%d %d %d", &i, &j, &k);

	fprintf(stdout, "Sum is: %d\n", (i+j+k));
	fprintf(stdout, "Average is: %d\n", (i+j+k)/3);
	fprintf(stdout, "Product is: %d\n", (i*j*k));
	fprintf(stdout, "Smallest is: %d\n", MIN(MIN(i,j),k));
	fprintf(stdout, "Largest is: %d\n", MAX(MAX(i,j),k));
	return 0;
}
