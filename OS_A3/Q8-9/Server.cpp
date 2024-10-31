#include "ConvexHull.hpp"
#include "async_handling.hpp"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define PORT "9034"
#define BUFFER_SIZE 50

using std::cout;
using std::endl;
using std::string;

Proactor proactor;
vector<Point> points;
size_t points_to_receive = 0;
int creator_fd = -1;

void handle_sigint(int sig) {
    cout << "\nReceived SIGINT (" << sig << "), shutting down server..." << endl;
    proactor.stop();
}

string process_command(char* line, int sender_fd) {
    if (points_to_receive > 0) {
        if (creator_fd != sender_fd)
            return "Another client is creating a graph";

        float x, y;
        if (sscanf(line, "%f,%f", &x, &y) != 2)
            return "Invalid coordinates format while waiting for points";
        points.emplace_back(x, y);
        points_to_receive--;
        if (points_to_receive == 0) {
            creator_fd = -1;
            return "Graph creation complete";
        }
        return "Point added";
    }

    if (strncmp(line, "Newgraph", 8) == 0) {
        size_t n;
        if (sscanf(line, "Newgraph %zu", &n) != 1)
            return "Invalid Newgraph command format";
        if (n == 0)
            return "Graph must have at least one point";

        points.clear();
        points.reserve(n);
        creator_fd = sender_fd;
        points_to_receive = n;
        return "Expecting points for new graph";
    }
    else if (strncmp(line,"CH",2) == 0) {
        float area = 0;
        if (points.size() > 2)
            area = ConvexHull::convexHullArea(points);
        return "Convex hull area: " + std::to_string(area);
    }
    else if (strncmp(line,"Newpoint", 8) == 0) {
        float x, y;
        if (sscanf(line, "Newpoint %f,%f", &x, &y) != 2)
            return "Invalid coordinates format";
        points.emplace_back(x, y);
        return "Point added";
    }
    else if (strncmp(line,"Removepoint", 11) == 0) {
        float x, y;
        if (sscanf(line, "Removepoint %f,%f", &x, &y) != 2)
            return "Invalid coordinates format";

        for (unsigned int i = 0; i < points.size(); i++) {
            if (points[i].getX() == x && points[i].getY() == y) {
                points[i] = points.back();
                points.pop_back();
                break;
            }
        }
        return "Point removed";
    } else if (strncmp(line, "Random", 6) == 0) {

        points.clear();
        points.reserve(10000000);
        for (size_t i = 0; i < 10000000; i++) {
            points.emplace_back((float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
        }

        return "Random points generated";
    }

    return "Unknown command";
}

// Return a listening socket
int get_listener_socket(void)
{
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
    if (listen(listener, MAX_EVENTS) == -1) {
        return -1;
    }

    return listener;
}

void* handle_client_message(int clientFd, std::mutex &mutex) {
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytesRead;
    while ((bytesRead = recv(clientFd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "Message from client " << clientFd << ": " << buffer;

        std::string response;
        mutex.lock();
        response = process_command(buffer, clientFd) + "\n>> ";
        mutex.unlock();

        send(clientFd, response.c_str(), response.size(), 0);
        memset(buffer, 0, sizeof(buffer));
    }
    if (bytesRead == 0) {
        std::cout << "Client " << clientFd << " disconnected." << std::endl;
        close(clientFd);
    } else {
        perror("recv");
    }
    return nullptr;
}


int main() {
    int listener = get_listener_socket();
    if (listener == -1) {
        perror("Error getting listener socket");
        return 1;
    }

    std::cout << "Server started, listening on port " << PORT << std::endl;
    proactor.run(listener, handle_client_message);

    return 0;
}

