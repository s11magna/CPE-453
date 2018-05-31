CC=gcc

liblwp.so:
	$(CC) -Wall -g -fpic -c lwp.c -o liblwp.o
	$(CC) -Wall -g -fpic -c scheduler.c -o scheduler.o
	$(CC) -o magic64.o -c magic64.S
	$(CC) -Wall -g -fpic -shared -o liblwp.so liblwp.o scheduler.o magic64.o

clean:
	rm liblwp.so
	rm liblwp.o
