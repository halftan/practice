CC=gcc
CFLAGS=-c -Wall -std=gnu99 -g
LDFLAGS=-lm

myls: myls.o
	$(CC) myls.o -o myls $(LDFLAGS)

myls.o: myls.c
	$(CC) $(CFLAGS) myls.c

clean:
	rm -r *.o myls
