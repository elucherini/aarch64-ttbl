CC=gcc
CFLAGS=-I.
DEPS=ttbl_entry.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ttbl: ttbl_entry_test.o ttbl_entry.o
	$(CC) -o output.out ttbl_entry_test.o ttbl_entry.o $(CFLAGS)
