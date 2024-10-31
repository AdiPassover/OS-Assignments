#include "async_handling.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Proactor::Proactor() : isRunning(false), listenerFd(-1) {}

Proactor::~Proactor() {
    stop();
}

void Proactor::run(int fd, proactorFunc handleClient) {
    isRunning = true;
    listenerFd = fd;

    // Start the accept thread to handle new client connections
    acceptThread = std::thread([this, handleClient] {
        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof their_addr;
        while (isRunning) {
            int new_fd = accept(listenerFd, (struct sockaddr *)&their_addr, &addr_size);
            if (new_fd == -1) {
                if (!isRunning) break;  // Exit if the server is stopping
                perror("accept");
                continue;
            }

            // Handle the client connection
            char buffer[INET6_ADDRSTRLEN];
            inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), buffer, sizeof buffer);
            std::cout << "server: got connection from " << buffer << std::endl;
            send(new_fd, ">> ", 3, 0);

            std::thread clientThread([new_fd, handleClient, clientMutexRef = std::ref(mtx)] {
                handleClient(new_fd, clientMutexRef);
            });
            clientThread.detach();
        }
    });

    if (acceptThread.joinable()) acceptThread.join();
}

void Proactor::stop() {
    if (isRunning) {
        isRunning = false;
        close(listenerFd);
        listenerFd = -1;
    }
}

void *Proactor::get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
