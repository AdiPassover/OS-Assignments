CC = gcc
CFLAGS = -g

all: signals

signals: SignalsHandling.o
	$(CC) $(CFLAGS) -o signals SignalsHandling.o

SignalsHandling.o: SignalsHandling.c
	$(CC) $(CFLAGS) -c SignalsHandling.c

clean:
	rm -f signals SignalsHandling.o