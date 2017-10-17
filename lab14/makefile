all: 3.9.bin 3.3.bin 3.4.bin

3.9.bin: 3.9.c
	mpicc -g -lm -Wall -o mpi_vec 3.9.c

3.3.bin: 3.3.c
	mpicc -g -lm -Wall -o mpi_global_sum 3.3.c

3.4.bin: 3.4.c
	mpicc -g -lm -Wall -o mpi_butterfly 3.4.c

clean:
	rm -rf *.o
	rm -rf mpi_vec
	rm -rf mpi_global_sum
	rm -rf mpi_butterfly
