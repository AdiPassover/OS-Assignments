CC = gcc
CFLAGS = -Wall -Wextra -g --coverage

all: add2PB findPhone

add2PB: add2PB.o
	$(CC) $(CFLAGS) -o add2PB add2PB.o

findPhone: findPhone.o
	$(CC) $(CFLAGS) -o findPhone findPhone.o

add2PB.o: add2PB.c
	$(CC) $(CFLAGS) -c add2PB.c

findPhone.o: findPhone.c
	$(CC) $(CFLAGS) -c findPhone.c

clean:
	rm -f add2PB findPhone *.o *.gcda *.gcno