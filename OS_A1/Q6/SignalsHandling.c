#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int count = 0;

void handleSigusr1(int sig) {
    printf("Received SIGUSR1\n");
    int *ptr = (int*)0x70707070;
    int val = *ptr;
}

void handleSigSegv(int sig) {
    printf("Received SIGSEGV\n");
    int a = 1/0;
}

void handleSigFpe(int sig) {
    printf("Received SIGFPE\n");
    alarm(1);
    pause();
}

void handleSigAlrm(int sig) {
    printf("Received SIGALRM\n");
    count++;
    printf("Count: %d\n", count);
    exit(0);
}

int main() {
    signal(SIGUSR1, handleSigusr1);
    signal(SIGSEGV, handleSigSegv);
    signal(SIGFPE, handleSigFpe);
    signal(SIGALRM, handleSigAlrm);

    printf("Count: %d\n", count);
    kill(getpid(), SIGUSR1);

    return 0;
}