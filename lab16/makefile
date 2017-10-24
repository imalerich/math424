all: mpi_sort.bin

mpi_sort.bin: sqd.c
	mpicc -g -lm -Wall -o mpi_sort sqd.c

clean:
	rm -rf *.o
	rm -rf mpi_sort
