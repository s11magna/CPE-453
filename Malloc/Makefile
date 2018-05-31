C = gcc
CFLAGS = -Wall -g -fpic
CFLAGS1 = -Wall -g
intel-all: lib/libmalloc.so lib64/libmalloc.so

lib/libmalloc.so: lib header32.o malloc32.o 
	$(CC) $(CFLAGS) -m32 -shared -o $@ malloc32.o header32.o

lib64/libmalloc.so: lib64 header64.o malloc64.o 
	$(CC) $(CFLAGS) -shared -o $@ malloc64.o header64.o

lib:
	mkdir lib

lib64:
	mkdir lib64

malloc32.o: malloc.c
	$(CC) $(CFLAGS) -m32 -c -o malloc32.o malloc.c

malloc64.o: malloc.c
	$(CC) $(CFLAGS) -m64 -c -o malloc64.o malloc.c

header32.o : header.c 
	$(CC) $(CFLAGS) -m32 -c -o header32.o header.c 
	
header64.o : header.c 
	$(CC) $(CFLAGS) -m64 -c -o header64.o header.c 
	
clean: rm -f malloc.o *~ TAGS


