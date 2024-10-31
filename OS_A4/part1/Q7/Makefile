CC = gcc
CFLAGS = -Wall -g

SRC = race.c
OBJ = $(SRC:.c=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

helgrind: $(TARGET)
	valgrind --tool=helgrind --log-file=helgrind_log.txt ./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full -s --track-origins=yes --show-leak-kinds=all --log-file=valgrind_log.txt ./$(TARGET)

clean:
	rm -f *.o $(TARGET)