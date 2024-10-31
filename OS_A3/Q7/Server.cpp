#include "ConvexHull.hpp"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>

#define PORT "9034"
#define BUFFER_SIZE 50

using std::cout;
using std::endl;
using std::string;

int sock_fd;
pthread_t accepting_thread;
pthread_mutex_t mutex;  // Mutex for protecting the points vector

vector<Point> points;
size_t points_to_receive = 0;
int creator_fd = -1;

void handle_sigint(int sig) {
    cout << endl << "Received SIGINT (" << sig << "), shutting off the server..." << endl;

    // Close the listener socket
    close(sock_fd);

    // Cancel and join the accepting thread
    pthread_cancel(accepting_thread);
    pthread_join(accepting_thread, nullptr);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    exit(0);
}

string process_command(char* line, int sender_fd) {
    if (points_to_receive > 0) {
        if (creator_fd != sender_fd) {
            return "Another client is creating a graph";
        }

        float x, y;
        if (sscanf(line, "%f,%f", &x, &y) != 2) {
            return "Invalid coordinates format while waiting for points";
        }
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
        if (sscanf(line, "Newgraph %zu", &n) != 1 || n == 0) {
            return "Invalid Newgraph command format";
        }

        points.clear();
        points.reserve(n);
        creator_fd = sender_fd;
        points_to_receive = n;

        return "Expecting points for new graph";
    }
    else if (strncmp(line,"CH",2) == 0) {

        float area = ConvexHull::convexHullArea(points);

        return "Convex hull area: " + std::to_string(area);
    }
    else if (strncmp(line,"Newpoint", 8) == 0) {
        float x, y;
        if (sscanf(line, "Newpoint %f,%f", &x, &y) != 2) {
            return "Invalid coordinates format";
        }

        points.emplace_back(x, y);

        return "Point added";
    }
    else if (strncmp(line,"Removepoint", 11) == 0) {
        float x, y;
        if (sscanf(line, "Removepoint %f,%f", &x, &y) != 2) {
            return "Invalid coordinates format";
        }

        for (unsigned int i = 0; i < points.size(); i++) {
            if (points[i].getX() == x && points[i].getY() == y) {
                points[i] = points.back();
                points.pop_back();
                return "Point removed";
            }
        }
        return "Point not found";
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

void* handle_client_message(void* client_fd_arg) {
    int client_fd = *(int*)client_fd_arg;

    while (true) {
        char buffer[BUFFER_SIZE] = {0};
        ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_read > 0) {
            std::cout << "Message from client " << client_fd << ": " << buffer;

            pthread_mutex_lock(&mutex);
            std::string response = process_command(buffer, client_fd) + "\n>> ";
            pthread_mutex_unlock(&mutex);

            send(client_fd, response.c_str(), response.size(), 0);
        }
        else if (bytes_read == 0) {
            std::cout << "Client " << client_fd << " disconnected." << std::endl;
            close(client_fd);
            break;
        }
        else {
            perror("recv");
            break;
        }
    }
    return nullptr;
}

void* handle_new_connection(void* server_fd_arg) {
    int server_fd = *(int*)server_fd_arg;

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int new_client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (new_client_fd == -1) {
            perror("accept");
            continue;
        }

        std::cout << "New client connected: " << new_client_fd << std::endl;
        send(new_client_fd, ">> ", 3, 0);

        pthread_t client_thread;
        if (pthread_create(&client_thread, nullptr, &handle_client_message, &new_client_fd) != 0) {
            perror("pthread_create");
            close(new_client_fd);
            continue;
        }

        pthread_detach(client_thread);  // Allow the thread to run independently
    }

    return nullptr;
}

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
    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

int main() {
    signal(SIGINT, handle_sigint);

    if (pthread_mutex_init(&mutex, nullptr) != 0) {
        perror("mutex init failed");
        return 1;
    }

    sock_fd = get_listener_socket();
    if (sock_fd == -1) {
        perror("Failed to create listener socket");
        return 1;
    }

    if (pthread_create(&accepting_thread, nullptr, &handle_new_connection, &sock_fd) != 0) {
        perror("pthread_create");
        return 1;
    }

    pthread_join(accepting_thread, nullptr);

    pthread_mutex_destroy(&mutex);
    return 0;
}
