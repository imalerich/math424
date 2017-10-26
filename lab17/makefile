all: 5.1.bin omp_trap1.bin

5.1.bin: 5.1.c
	gcc -fopenmp -o 5.1 5.1.c

omp_trap1.bin: omp_trap1.c
	gcc -fopenmp -lm -o omp_trap1 omp_trap1.c

