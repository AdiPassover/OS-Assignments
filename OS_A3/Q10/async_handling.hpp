#ifndef OS_A3_ASYNC_HANDLING_HPP
#define OS_A3_ASYNC_HANDLING_HPP

#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>

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

typedef void* (*proactorFunc)(int, std::mutex&);

class Proactor {
private:
    std::atomic<bool> isRunning;
    int listenerFd;
    std::thread acceptThread;
    std::mutex mtx;

public:

    Proactor();
    ~Proactor();

    void run(int fd, proactorFunc handleClient);
    void stop();

private:
    // Get sockaddr, IPv4 or IPv6:
    void *get_in_addr(struct sockaddr *sa);


};


#endif //OS_A3_ASYNC_HANDLING_HPP
