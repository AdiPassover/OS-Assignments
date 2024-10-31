#include "server.hpp"
#include "../colors.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <utility>

void print_error(const std::string& message) {
    std::cerr << Colors::RED_BG << message << ": " << strerror(errno) << Colors::RESET_BG << std::endl;
}


void *Server::get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int Server::get_listener_socket() {
    int listener;     // Listening socket descriptor
    int yes=1;        // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "pollserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
    if (p == NULL) {
        return -1;
    }

    // Listen
    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

void Server::stop() {
    _running = false;
}

Server::~Server() {
    stop();

    for (auto& poll_fd : _poll_fds) {
        close(poll_fd.fd);
    }
}

Server::Server() : _running(false), _listener(get_listener_socket()), _poll_fds(), _client_graphs(0) {
    if (_listener == -1) {
        print_error("Failed to create listener socket");
        exit(1);
    }

    _poll_fds.emplace_back(pollfd{.fd = _listener, .events = POLLIN, .revents = 0});

}

void Server::run() {
    _running = true;

    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;
    char buf[BUFFER_SIZE];    // Buffer for client data
    char remoteIP[INET6_ADDRSTRLEN];

    std::cout << "Waiting for connections..." << std::endl;
    // Main loop
    while (_running) {
        int poll_count = poll(_poll_fds.data(), _poll_fds.size(), -1);

        if (poll_count == -1) {
            if (errno == EINTR) continue;
            print_error("poll");
            exit(1);
        }

        // Run through the existing connections looking for data to read
        for (unsigned int i = 0; i < _poll_fds.size(); i++) {
            auto& poll_fd = _poll_fds[i];
            if (!(poll_fd.revents & POLLIN)) continue;

            if (poll_fd.fd == _listener) {
                // If listener is ready to read, handle new connection

                addrlen = sizeof remoteaddr;
                int new_fd = accept(_listener,
                                    (struct sockaddr *)&remoteaddr,
                                    &addrlen);

                if (new_fd == -1) {
                    print_error("accept");
                } else {
                    _poll_fds.emplace_back(pollfd{.fd = new_fd, .events = POLLIN, .revents = 0});
                    {
                        std::lock_guard<std::mutex> lock(_graphs_mutex);
                        _client_graphs.emplace(new_fd, std::make_pair(Graph(), std::make_unique<std::mutex>()));
                    }

                    std::cout << Colors::GREEN
                              << "New connection from " <<  inet_ntop(remoteaddr.ss_family,get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN)
                              << " on socket " << new_fd << Colors::RESET << std::endl;

                    send(new_fd, ">> ", 3, 0);
                }
            } else {
                // If not the listener, we're just a regular client
                ssize_t n_bytes = recv(poll_fd.fd, buf, sizeof buf, 0);

                int sender_fd = poll_fd.fd;

                if (n_bytes <= 0) {
                    if (n_bytes == 0) {
                        // Connection closed
                        std::cout << Colors::MAGENTA << "Socket " << sender_fd << " hung up" << Colors::RESET << std::endl;
                    } else {
                        print_error("recv");
                    }
                    close(poll_fd.fd); // Bye!
                    std::swap(_poll_fds[i], _poll_fds.back());
                    _poll_fds.pop_back();

                } else {
                    // We got some good data from a client
                    buf[n_bytes - 1] = '\0';
                    std::cout << Colors::CYAN << "Socket " << sender_fd << " sent " << Colors::RESET
                              << Colors::LIGHT_CYAN << buf << Colors::RESET << std::endl;

                    handle_message(buf, sender_fd);
                }
            }
        }
    }
}

void Server::send_message(int fd, const std::string &message) {
    send(fd, message.c_str(), message.size(), 0);
}

std::unique_ptr<Server> ServerFactory::get_server(char type) {
    switch (type) {
        case 'l':
            return std::make_unique<LFServer>(DEFAULT_NUM_THREADS);
        case 'p':
            return std::make_unique<PipelineServer>();
        default:
            throw std::invalid_argument("Invalid server type. Use 'l' for leader follower or 'p' for pipeline");
    }
}