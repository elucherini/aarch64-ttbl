CC=gcc
CFLAGS=-I.

ttbl: ttbl.o
	$(CC) -o output.out ttbl.o -I.
