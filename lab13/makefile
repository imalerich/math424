all: 3.1.bin 3.2.bin 3.4.bin

3.1.bin: 3.1.c
	mpicc -g -Wall -o mpi_hello 3.1.c

3.2.bin: 3.2.c
	mpicc -g -Wall -o mpi_trap 3.2.c

3.4.bin: 3.4.c
	mpicc -g -Wall -o mpi_print 3.4.c

clean:
	rm -rf *.o
	rm -rf mpi_hello
	rm -rf mpi_trap
