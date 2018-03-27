CC=gcc
CFLAGS=-I.
DEPS=ttbl_entry.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ttbl: ttbl.o ttbl_entry.o
	$(CC) -o output.out ttbl.o ttbl_entry.o $(CFLAGS)

clean:
	rm -fr *.o
