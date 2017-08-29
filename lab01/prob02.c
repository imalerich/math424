#include <stdio.h>
#define MIN(X, Y) (X < Y ? X : Y)
#define MAX(X, Y) (X > Y ? X : Y)

// problem 02
int main(int argc, char ** argv) {
	int i, j, k;
	fprintf(stdout, "Enter three different integers: ");
	scanf("%d %d %d", &i, &j, &k);

	fprintf(stdout, "----------------------------------\n");
	fprintf(stdout, "Sum is:\t\t%d\n", (i+j+k));
	fprintf(stdout, "Average is:\t%d\n", (i+j+k)/3);
	fprintf(stdout, "Product is:\t%d\n", (i*j*k));
	fprintf(stdout, "Smallest is:\t%d\n", MIN(MIN(i,j),k));
	fprintf(stdout, "Largest is:\t%d\n", MAX(MAX(i,j),k));
	fprintf(stdout, "----------------------------------\n");
	return 0;
}
