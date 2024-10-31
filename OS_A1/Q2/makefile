CC = gcc
CFLAGS = -Wall -g

.PHONY: all clean

all: Poisson

Poisson: Poisson.c
	$(CC) $(CFLAGS) $^ -lm -o $@

clean:
	rm -f Poisson