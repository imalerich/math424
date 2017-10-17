all: 3.9.bin 3.3.bin

3.9.bin: 3.9.c
	mpicc -g -Wall -o mpi_vec 3.9.c

3.3.bin: 3.3.c
	mpicc -g -Wall -o mpi_global_sum 3.3.c

clean:
	rm -rf *.o
	rm -rf mpi_vec
