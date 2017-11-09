all: 5.9.bin 5.10.bin

5.9.bin: 5.9.c
	gcc -fopenmp -o 5.9 5.9.c

5.10.bin: 5.10.c
	gcc -fopenmp -lm -o 5.10 5.10.c 

clean:
	rm -rf 5.9
	rm -rf 5.10
