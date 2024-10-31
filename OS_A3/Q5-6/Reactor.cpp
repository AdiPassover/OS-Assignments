#include "Reactor.hpp"

Reactor::Reactor() : isRunning(false), pollFdsCount(0) {
    pollFds = new struct pollfd[MAX_EVENTS];
    fdMap.reserve(MAX_EVENTS);
}

Reactor::~Reactor() {
    isRunning = false;
    for (unsigned int i = 0; i < pollFdsCount; i++)
        close(pollFds[i].fd);
    delete[] pollFds;
}

void Reactor::addFd(int fd, reactorFunc func) {
    pollFds[pollFdsCount++] = {fd, POLLIN, 0};
    fdMap[fd] = func;
}

void Reactor::removeFd(int fd) {
    unsigned int index = MAX_EVENTS+1;
    for (unsigned int i = 0; i < pollFdsCount; i++)
        if (pollFds[i].fd == fd) { index = i; break; }
    if (index == MAX_EVENTS+1) return;

    pollFds[index] = pollFds[pollFdsCount-1];
    pollFdsCount--;
    fdMap.erase(fd);
}

void Reactor::run() {
    isRunning = true;

    while (isRunning) {
        int numEvents = poll(pollFds, pollFdsCount, 10);

        if (numEvents == -1) {
            if (errno == EINTR) { continue; }
            else { perror("poll"); break; }
        } else if (numEvents > 0) {
            for (unsigned int i = 0; i < pollFdsCount; i++) {
                if (pollFds[i].revents & POLLIN) {
                    pollFds[i].revents = 0; // Clear the event flag after handling
                    fdMap[pollFds[i].fd](pollFds[i].fd);
                }
            }
        }
    }
}

void Reactor::stop() {
    isRunning = false;
}
