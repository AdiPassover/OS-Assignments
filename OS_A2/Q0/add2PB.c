#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define PB_PATH "PhoneBook.txt"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Invalid amount of arguments. You have to enter the name and phone separated by spaces.\n");
        return 1;
    }

    int fd = open(PB_PATH, O_WRONLY | O_APPEND | O_CREAT, 0644); // 0644 = rw-r--r--
    if (fd == -1) {
        perror("Error opening file");
        return 2;
    }

    for (int i = 1; i < argc; i++) {
        size_t size = 0;
        while (argv[i][size] != '\0') { size++; }
        write(fd, argv[i], size);
        if (i < argc-2)
            write(fd, " ", 1);
        else if (i == argc-2)
            write(fd, ",", 1);
        else
            write(fd, "\n", 1);
    }

    close(fd);
    return 0;
}