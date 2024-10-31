CC = gcc
CFLAGS = -Wall -g

TARGET = main

all: $(TARGET)

$(TARGET): hello.o
	$(CC) $(CFLAGS) -o main hello.o

hello.o: hello.c
	$(CC) $(CFLAGS) -c hello.c

valgrind: $(TARGET)
	valgrind --leak-check=full -s --track-origins=yes --show-leak-kinds=all --log-file=valgrind_log.log ./$(TARGET) hello world

debug: $(TARGET)
	valgrind --leak-check=full --vgdb=yes --vgdb-error=0 ./$(TARGET) hello world

clean:
	rm -f *.o $(TARGET)