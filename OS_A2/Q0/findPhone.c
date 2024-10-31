#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define PB_PATH "PhoneBook.txt"
#define MAX_NAME_SIZE 100

void pipeline(char*** commands, size_t n) {
    int pipe_fd[2];  // Pipe file descriptors
    int curr_fd = 0; // Current input file descriptor, initially stdin

    for (size_t i = 0; i < n; i++) {
        if (pipe(pipe_fd) == -1) {
            perror("Error opening pipe.");
            exit(2);
        }

        pid_t fork_id = fork();
        if (fork_id == -1) {
            perror("Error forking.");
            exit(3);
        }

        if (fork_id == 0) { // Child process
            if (curr_fd != 0) {  // Not the first command, redirect input
                dup2(curr_fd, STDIN_FILENO); 
                close(curr_fd);
            }
            if (i != n - 1) { // Not the last command, redirect output
                dup2(pipe_fd[1], STDOUT_FILENO);
            }

            // Close pipe ends after redirecting
            close(pipe_fd[0]);
            close(pipe_fd[1]);

            execvp(commands[i][0], commands[i]); // Execute the command
            perror("Command failed");
            exit(4);
        } else { // Parent process
            wait(NULL); // Wait for child to finish
            close(pipe_fd[1]); // Close the write end of the pipe in the parent
            if (curr_fd != 0) {
                close(curr_fd); // Close the previous read end
            }
            curr_fd = pipe_fd[0]; // Save the read end for the next command
        }
    }
    // Close the last read end in the parent
    if (curr_fd != 0) {
        close(curr_fd);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Please write the name you want the phone of.\n");
        return 1;
    }

    char name[MAX_NAME_SIZE];
    name[0] = '\0'; // Initialize the string
    for (int i = 1; i < argc; i++) {
        strcat(name, argv[i]);
        if (i < argc - 1) {
            strcat(name, " "); // Add space between words
        }
    }

    // The commands we want to run in the pipeline:
    // grep [name] PhoneBook.txt | sed 's/ /#/g' | sed 's/,/ /' | awk '{print $2}'
    char* c1[] = {"grep", name, PB_PATH, NULL};
    char* c2[] = {"sed", "s/ /#/g", NULL};
    char* c3[] = {"sed", "s/,/ /", NULL};
    char* c4[] = {"awk", "{print $2}", NULL};
    char** commands[] = { c1, c2, c3, c4};
    
    pipeline(commands, 4);

    return 0;
}
