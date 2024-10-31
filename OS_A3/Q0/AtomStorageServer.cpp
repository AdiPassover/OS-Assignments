#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/un.h>
#include "MoleculeFactory.cpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"

#define PORT "55555"
#define BACKLOG 10
#define BUFFER_SIZE 100

Molecule atoms;

void perror_and_exit(const char* msg) {
    perror(msg);
    exit(1);
}

void process_add_command(int client_fd, char* atom_type, unsigned long long num_atoms) {
    if (strcmp(atom_type, "CARBON") == 0) {
        atoms.addCarbon(num_atoms);
    } else if (strcmp(atom_type, "OXYGEN") == 0) {
        atoms.addOxygen(num_atoms);
    } else if (strcmp(atom_type, "HYDROGEN") == 0) {
        atoms.addHydrogen(num_atoms);
    }
    send(client_fd, "OK\n", 3, 0);
}

void process_deliver_command(int udp_fd, struct sockaddr_storage* client_addr, socklen_t addr_len, char* molecule_name, unsigned long long num_molecules, int is_uds) {
    Molecule molecule;
    try {
        molecule = MoleculeFactory::getMolecule(molecule_name) * num_molecules;
    } catch (const std::exception& e) {
        const char* response = "ERROR: Invalid molecule name\n";
        sendto(udp_fd, response, strlen(response), 0, (struct sockaddr*)client_addr, addr_len);
        return;
    }

    char response[BUFFER_SIZE];

    if (molecule <= atoms) {
        atoms -= molecule;
        snprintf(response, sizeof(response), "OK\n");
    } else {
        snprintf(response, sizeof(response), "ERROR: Insufficient atoms\n");
    }
    if (is_uds) return;

    // Send response back to the UDP client
    ssize_t bytes_sent = sendto(udp_fd, response, strlen(response), 0, (struct sockaddr*)client_addr, addr_len);
    if (bytes_sent == -1) {
        perror("Sending response to UDP client failed");
    }
}


void process_gen_command(char* drink_name) {
    char* response = new char[301];
    response[0] = '\0';
    const char** drink = MoleculeFactory::getDrink(drink_name);
    for (unsigned int i = 0; i < 3; i++) {
        unsigned long long n = atoms / MoleculeFactory::getMolecule(drink[i]);
        char buffer[100];
        sprintf(buffer, "Can generate %llu of %s\n", n, drink[i]);
        strcat(response, buffer);
    }
    fprintf(stdin, "%s\n", response);
    fprintf(stdout, "%s\n", response);
}

void process_command_tcp(int client_fd, char* buffer) {
    printf("Processing command (TCP): %s\n", buffer);
    char copy[BUFFER_SIZE];
    strcpy(copy, buffer);
    char* command = strtok(buffer, " ");

    if (strcmp(command, "ADD") == 0) {
        char atom_type[10];
        unsigned long long num_atoms;
        if (sscanf(copy, "ADD %s %llu", atom_type, &num_atoms) == 2) {
            process_add_command(client_fd, atom_type, num_atoms);
        } else {
            send(client_fd, "ERROR: Invalid ADD command format\n", 34, 0);
        }
    } else {
        send(client_fd, "ERROR: Invalid command\n", 23, 0);
    }
    printf("Storage:\n%s\n", atoms.toString());
}

void process_command_udp(int udp_fd, struct sockaddr_storage* client_addr, socklen_t addr_len, char* buffer, int is_uds) {
    printf("Processing command (UDP): %s\n", buffer);
    char copy[BUFFER_SIZE];
    strcpy(copy, buffer);
    char* command = strtok(buffer, " ");

    if (strcmp(command, "DELIVER") == 0) {
        char molecule_name[16];
        unsigned long long num_molecules;
        if (sscanf(copy, "DELIVER %s %llu", molecule_name, &num_molecules) == 2) {
            process_deliver_command(udp_fd, client_addr, addr_len, molecule_name, num_molecules, is_uds);
        } else {
            char response[] = "ERROR: Invalid DELIVER command format\n";
            if (!is_uds) sendto(udp_fd, response, strlen(response), 0, (struct sockaddr*)client_addr, addr_len);
        }
    } else {
        char response[] = "ERROR: Invalid command\n";
        if (!is_uds) sendto(udp_fd, response, sizeof(response), 0, (struct sockaddr*)client_addr, sizeof(struct sockaddr_un));
    }
    printf("Storage:\n%s\n", atoms.toString());
}

void process_command_stdin(char* buffer) {
    if (strcmp(buffer,"GEN SOFT DRINK") == 0) {
        buffer[8] = '_';
    }

    char* command = strtok(buffer, " ");
    char* drink_type = strtok(NULL, " ");

    if (strcmp(command, "GEN") == 0 && drink_type != NULL) {
        try {
            process_gen_command(drink_type);
        } catch (const std::exception& e) {
            printf("ERROR: Unkown drink type\n");
        }
    } else {
        printf("ERROR: Invalid GEN command\n");
    }
}


int main(int argc, char* argv[]) {

    unsigned int TIMEOUT_SECONDS = 20;
    char* uds_stream_path = NULL;
    char* uds_dgram_path = NULL;
    int option;

    while ((option = getopt(argc, argv, "c:o:h:t:s:d:")) != -1) {
        switch (option) {
            case 'c':
                atoms.addCarbon(strtoull(optarg, NULL, 10));
                break;
            case 'o':
                atoms.addOxygen(strtoull(optarg, NULL, 10));
                break;
            case 'h':
                atoms.addHydrogen(strtoull(optarg, NULL, 10));
                break;
            case 't':
                TIMEOUT_SECONDS = atoi(optarg);
                break;
            case 's':
                uds_stream_path = optarg;
                break;
            case 'd':
                uds_dgram_path = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-c carbon] [-o oxygen] [-h hydrogen] [-t timeout] "
                                "[-udss uds stream path] [-udsd uds datagram path]\n", argv[0]);
                exit(1);
        }
    }

    int status, listener_fd, udp_fd, new_fd;
    struct addrinfo hints_tcp, hints_udp, *res_tcp, *res_udp;
    fd_set master, read_fds;
    int fd_max;
    struct timeval timeout;
    int activity_detected;
    int yes = 1;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    // TCP setup
    memset(&hints_tcp, 0, sizeof hints_tcp);
    hints_tcp.ai_family = AF_INET;  // IPv4
    hints_tcp.ai_socktype = SOCK_STREAM;
    hints_tcp.ai_flags = AI_PASSIVE;  // fill in my IP for me

    if ((status = getaddrinfo(NULL, PORT, &hints_tcp, &res_tcp)) != 0) {
        fprintf(stderr, "getaddrinfo error (TCP): %s\n", gai_strerror(status));
        exit(1);
    }

    listener_fd = socket(res_tcp->ai_family, res_tcp->ai_socktype, res_tcp->ai_protocol); // TCP socket
    if (listener_fd == -1)
        perror_and_exit("Creating TCP socket failed");

    if (setsockopt(listener_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
        perror("setsockopt");

    if (bind(listener_fd, res_tcp->ai_addr, res_tcp->ai_addrlen) == -1)
        perror_and_exit("Binding TCP socket failed");

    if (listen(listener_fd, BACKLOG) == -1)
        perror_and_exit("Listening on TCP socket failed");

    FD_SET(listener_fd, &master);
    fd_max = listener_fd;

    // UDP setup
    memset(&hints_udp, 0, sizeof hints_udp);
    hints_udp.ai_family = AF_INET;  // IPv4
    hints_udp.ai_socktype = SOCK_DGRAM;  // UDP
    hints_udp.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, PORT, &hints_udp, &res_udp)) != 0) {
        fprintf(stderr, "getaddrinfo error (UDP): %s\n", gai_strerror(status));
        exit(1);
    }

    udp_fd = socket(res_udp->ai_family, res_udp->ai_socktype, res_udp->ai_protocol); // UDP socket
    if (udp_fd == -1)
        perror_and_exit("Creating UDP socket failed");

    if (bind(udp_fd, res_udp->ai_addr, res_udp->ai_addrlen) == -1)
        perror_and_exit("Binding UDP socket failed");

    FD_SET(udp_fd, &master);
    if (udp_fd > fd_max) {
        fd_max = udp_fd;
    }

    // Listen to STDIN
    FD_SET(STDIN_FILENO, &master);
    if (STDIN_FILENO > fd_max) {
        fd_max = STDIN_FILENO;
    }

    int uds_stream_fd = -1, uds_dgram_fd = -1;
    struct sockaddr_un uds_stream_addr, uds_dgram_addr;

    // UDS Stream setup
    if (uds_stream_path) {
        uds_stream_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (uds_stream_fd == -1) {
            perror_and_exit("Creating UDS stream socket failed");
        }
        memset(&uds_stream_addr, 0, sizeof(uds_stream_addr));
        uds_stream_addr.sun_family = AF_UNIX;
        strncpy(uds_stream_addr.sun_path, uds_stream_path, sizeof(uds_stream_addr.sun_path) - 1);

        unlink(uds_stream_path);  // Remove any existing file
        if (bind(uds_stream_fd, (struct sockaddr*)&uds_stream_addr, sizeof(uds_stream_addr)) == -1) {
            perror_and_exit("Binding UDS stream socket failed");
        }
        if (listen(uds_stream_fd, BACKLOG) == -1) {
            perror_and_exit("Listening on UDS stream socket failed");
        }

        FD_SET(uds_stream_fd, &master);
        if (uds_stream_fd > fd_max) fd_max = uds_stream_fd;
    }

    // UDS Datagram setup
    if (uds_dgram_path) {
        uds_dgram_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (uds_dgram_fd == -1) {
            perror_and_exit("Creating UDS datagram socket failed");
        }
        memset(&uds_dgram_addr, 0, sizeof(uds_dgram_addr));
        uds_dgram_addr.sun_family = AF_UNIX;
        strncpy(uds_dgram_addr.sun_path, uds_dgram_path, sizeof(uds_dgram_addr.sun_path) - 1);

        unlink(uds_dgram_path);  // Remove any existing file
        if (bind(uds_dgram_fd, (struct sockaddr*)&uds_dgram_addr, sizeof(uds_dgram_addr)) == -1) {
            perror_and_exit("Binding UDS datagram socket failed");
        }

        FD_SET(uds_dgram_fd, &master);
        if (uds_dgram_fd > fd_max) fd_max = uds_dgram_fd;
    }

    printf("Server waiting for clients and keyboard...\n");
    bool finished = false;

    for (;;) {
        read_fds = master; // Copy the master set because select ruins it

        timeout.tv_sec = TIMEOUT_SECONDS;
        timeout.tv_usec = 0;

        activity_detected = select(fd_max + 1, &read_fds, NULL, NULL, &timeout);

        if (activity_detected == -1) {
            perror_and_exit("select");
        } else if (activity_detected == 0) { // Timeout occurred
            printf("No activity for %d seconds, shutting down server.\n", TIMEOUT_SECONDS);
            break;
        }

        for (int i = 0; i <= fd_max; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == uds_stream_fd) {  // New UDS stream connection
                    struct sockaddr_un client_addr;
                    socklen_t addr_size = sizeof(client_addr);
                    int new_fd = accept(uds_stream_fd, (struct sockaddr*)&client_addr, &addr_size);
                    if (new_fd == -1) {
                        perror("Accept failed (UDS stream)");
                    } else {
                        FD_SET(new_fd, &master);
                        if (new_fd > fd_max) fd_max = new_fd;
                        printf("New UDS stream connection\n");
                    }
                } else if (i == uds_dgram_fd) {  // UDS datagram data
                    char buffer[BUFFER_SIZE];
                    struct sockaddr_un client_addr;
                    socklen_t addr_len = sizeof(client_addr);
                    ssize_t bytes_received = recvfrom(uds_dgram_fd, buffer, sizeof buffer, 0, (struct sockaddr*)&client_addr, &addr_len);
                    if (bytes_received > 0) {
                        buffer[bytes_received] = '\0';
                        process_command_udp(uds_dgram_fd, (struct sockaddr_storage*)&client_addr, addr_len, buffer,1);
                    } else {
                        perror("recvfrom (UDS datagram)");
                    }
                }
                else if (i == listener_fd) {  // New TCP connection
                    // Existing code to handle TCP connections
                    struct sockaddr_storage client_addr;
                    socklen_t addr_size = sizeof client_addr;
                    new_fd = accept(listener_fd, (struct sockaddr*)&client_addr, &addr_size);
                    if (new_fd == -1) {
                        perror("Accept failed");
                    } else {
                        FD_SET(new_fd, &master);  // Add new client to master set
                        if (new_fd > fd_max) {
                            fd_max = new_fd;
                        }
                        printf("New TCP connection\n");
                    }
                } else if (i == udp_fd) {  // Handle UDP data
                    char buffer[BUFFER_SIZE];
                    struct sockaddr_storage client_addr;
                    socklen_t addr_len = sizeof client_addr;
                    ssize_t bytes_received = recvfrom(udp_fd, buffer, sizeof buffer, 0, (struct sockaddr*)&client_addr, &addr_len);
                    if (bytes_received > 0) {
                        buffer[bytes_received] = '\0';
                        process_command_udp(udp_fd, &client_addr, addr_len, buffer,0);
                    } else {
                        perror("recvfrom");
                    }
                } else if (i == STDIN_FILENO) {  // Handle STDIN
                    char buffer[BUFFER_SIZE];
                    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                        size_t len = strlen(buffer);
                        if (len <= 1) continue;
                        if (buffer[len - 1] == '\n') buffer[len - 1] = '\0';
                        process_command_stdin(buffer);
                    }
                    else {
                        printf("Processing command (STDIN): %s of length: %zu", buffer, strlen(buffer));
                    }
                } else {  // Handle TCP data from a client
                    char buffer[BUFFER_SIZE];
                    ssize_t bytes_received = recv(i, buffer, sizeof buffer, 0);
                    if (bytes_received <= 0) {
                        if (bytes_received == 0) {
                            printf("TCP client disconnected\n");
                        } else {
                            perror("recv");
                        }
                        close(i);  // Close the socket
                        FD_CLR(i, &master);  // Remove from master set
                    } else {
                        buffer[bytes_received] = '\0';
                        process_command_tcp(i, buffer);
                    }
                }
            }
        }
        if (finished) break;

    }

    // Close all open sockets and free resources
    for (int i = 0; i <= fd_max; i++) {
        if (FD_ISSET(i, &master)) close(i);
    }
    freeaddrinfo(res_tcp);
    freeaddrinfo(res_udp);
    if (uds_stream_path) unlink(uds_stream_path);
    if (uds_dgram_path) unlink(uds_dgram_path);

    return 0;
}
