CC = gcc
CFLAGS =  -g -c -o
test: object magic64.o 
	$(CC) -L ~/cpe453/test/lib64 -o test test.o magic64.o -llwp 
object:
	$(CC) $(CFLAGS) test.o test.c 
magic64.o: magic64.S 
clean:
	rm -f test.o  *~ TAGS 
