CC = gcc
CFLAGS = -Wall -g

MAIN = Main
LIB = libpoisson.so
LIB_SRC = Poisson

.PHONY: all clean

all: Poisson

Poisson: $(MAIN).o $(LIB)
	$(CC) $(CFLAGS) $^ -lm -o $@ -Wl,-rpath=.

PoissonMain.o: $(MAIN).c $(LIB_SRC).h
	$(CC) $(CFLAGS) -c $<

libpoisson.so: $(LIB_SRC).o
	$(CC) -shared -Wall -o $@ $^

$(LIB_SRC).o: $(LIB_SRC).c
	$(CC) $(CFLAGS) -fPIC -c $^

clean:
	rm -f Poisson *.o *.so