#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char ** argv) {
	// seed the random number generator using the system clock
	srand(time(NULL));

	for (int point = 0;;) {
		// roll two dice, get their values as (1-6)
		int d0 = rand() % 6 + 1;
		int d1 = rand() % 6 + 1;
		int sum = d0 + d1;
		printf("You rolled %d + %d = %d\n", d0, d1, sum);

		// extra roles, we are either going to lose (7), win (point)
		// or role again
		if (point > 0 && sum == 7) {
			printf("You lose\n");
			break;
		} else if (point > 0 && sum == point) {
			printf("You win\n");
			break;
		} else if (point > 0) {
			continue; // need to keep rolling
			break;
		}

		// first role
		if (sum == 2 || sum == 3 || sum == 12) {
			printf("You lose\n");
			break;
		} else if (sum == 7 || sum == 11) {
			printf("You win\n");
			break;
		} else {
			printf("point is %d\n", sum);
			point = sum;
		}
	}

	return 0;
}
