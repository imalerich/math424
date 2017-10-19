all: mpi_vec.bin

mpi_vec.bin: vec.c
	mpicc -g -lm -Wall -o mpi_vec vec.c

clean:
	rm -rf *.o
	rm -rf mpi_vec
