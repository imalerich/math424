all: 4.7.1.bin 4.7.2.bin 4.7.3.bin

4.7.1.bin: 4.7.1.c
	gcc -o bin.4.7.1 4.7.1.c -pthread

4.7.2.bin: 4.7.2.c
	gcc -o bin.4.7.2 4.7.2.c -pthread

4.7.3.bin: 4.7.3.c
	gcc -o bin.4.7.3 4.7.3.c -pthread

clean:
	rm -rf bin.4.7.1
	rm -rf bin.4.7.2
	rm -rf bin.4.7.3
