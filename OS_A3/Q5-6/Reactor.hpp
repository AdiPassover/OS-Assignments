#ifndef OS_A3_ASYNC_HANDLING_HPP
#define OS_A3_ASYNC_HANDLING_HPP

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <signal.h>

#define MAX_EVENTS 10

typedef void (*reactorFunc)(int fd);


class Reactor {
private:
    bool isRunning;
    struct pollfd* pollFds;
    unsigned int pollFdsCount;
    std::unordered_map<int, reactorFunc> fdMap;

public:

    Reactor();
    ~Reactor();

    void addFd(int fd, reactorFunc func);
    void removeFd(int fd);
    void run();
    void stop();

};


#endif //OS_A3_ASYNC_HANDLING_HPP
