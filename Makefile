CFLAGS = -O2 -Wall
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

INSTALL = /usr/bin/install

.PHONY: all clean install

all: xorify

install: all
	$(INSTALL) --owner=root --group=root --mode=0755 xorify $(BINDIR)

clean:
	$(RM) -f *~ *.o xorify

xorify: xorify.o
	$(CC) $(CFLAGS) -o $@ $<

xorify.o: src/xorify.c
	$(CC) $(CFLAGS) -c -o $@ $<
