#include <stdio.h>

int min(int i, int j, int k);
int max(int i, int j, int k);

// problem 02
int main(int argc, char ** argv) {
	int i, j, k;
	fprintf(stdout, "Enter three different integers: \n");
	scanf("%d %d %d", &i, &j, &k);

	fprintf(stdout, "Sum is: %d\n", (i+j+k));
	fprintf(stdout, "Average is: %d\n", (i+j+k)/3);
	fprintf(stdout, "Product is: %d\n", (i*j*k)/3);
	fprintf(stdout, "Smallest is: %d\n", min(i,j,k));
	fprintf(stdout, "Largest is: %d\n", max(i,j,k));
	return 0;
}

int min(int i, int j, int k) {
	int tmp = (i < j) ? i : j;
	return (tmp < k) ? tmp : k;
}


int max(int i, int j, int k) {
	int tmp = (i > j) ? i : j;
	return (tmp > k) ? tmp : k;
}
