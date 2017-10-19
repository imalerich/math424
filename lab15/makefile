all: mpi_vec.bin mpi_trap.bin

mpi_vec.bin: 3.13.vec.c
	mpicc -g -lm -Wall -o mpi_vec 3.13.vec.c

mpi_trap.bin: 3.20.trap.c
	mpicc -g -lm -Wall -o mpi_trap 3.20.trap.c

clean:
	rm -rf *.o
	rm -rf mpi_vec
	rm -rf mpi_trap
