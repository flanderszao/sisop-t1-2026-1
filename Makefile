CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -std=c11
LDFLAGS ?=

BINARIES := p1 p2 t1 t2

.PHONY: all clean

all: $(BINARIES)

p1: p1.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

p2: p2.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) -pthread -lrt

t1: t1.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) -pthread

t2: t2.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) -pthread

clean:
	rm -f $(BINARIES)
