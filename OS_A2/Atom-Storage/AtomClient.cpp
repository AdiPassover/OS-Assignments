#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define PORT "55555"
#define BUFFER_SIZE 100

void perror_and_exit(const char* msg) {
    perror(msg);
    exit(1);
}

int create_tcp_socket(const char* server_ip, const char* port, struct addrinfo** res) {
    struct addrinfo hints;
    int sockfd;

    // Prepare connection
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(server_ip, port, &hints, res) != 0) {
        perror("getaddrinfo failed");
        exit(1);
    }

    sockfd = socket((*res)->ai_family, (*res)->ai_socktype, (*res)->ai_protocol);
    if (sockfd == -1) {
        perror_and_exit("Creating TCP socket failed");
    }

    if (connect(sockfd, (*res)->ai_addr, (*res)->ai_addrlen) == -1) {
        perror_and_exit("TCP connection failed");
    }

    return sockfd;
}

int create_udp_socket(const char* server_ip, const char* port, struct addrinfo** res) {
    struct addrinfo hints;
    int sockfd;

    // Prepare connection
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(server_ip, port, &hints, res) != 0) {
        perror_and_exit("getaddrinfo failed");
    }

    sockfd = socket((*res)->ai_family, (*res)->ai_socktype, (*res)->ai_protocol);
    if (sockfd == -1) {
        perror_and_exit("Creating UDP socket failed");
    }

    return sockfd;
}

void send_tcp_message(int sockfd, struct addrinfo* res, const char* message) {
    // Send the message
    ssize_t bytes_sent = send(sockfd, message, strlen(message), 0);
    if (bytes_sent == -1) {
        perror_and_exit("Sending TCP message failed");
    }

    // Receive server response
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(sockfd, buffer, sizeof buffer - 1, 0);
    if (bytes_received == -1) {
        perror_and_exit("Receiving TCP response failed");
    }

    buffer[bytes_received] = '\0';
    printf("Received from server (TCP): %s\n", buffer);
}

void send_udp_message(int sockfd, struct addrinfo* res, char* message) {
    const char* cd_message = "DELIVER CARBON DIOXIDE";
    if (strncmp(message,cd_message, strlen(cd_message)) == 0)
        message[14] = '_';

    // Send the message
    ssize_t bytes_sent = sendto(sockfd, message, strlen(message), 0, res->ai_addr, res->ai_addrlen);
    if (bytes_sent == -1) {
        perror_and_exit("Sending UDP message failed");
    }

    // Receive server response
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recvfrom(sockfd, buffer, sizeof buffer - 1, 0, res->ai_addr, &res->ai_addrlen);
    if (bytes_received == -1) {
        perror_and_exit("Receiving UDP response failed");
    }

    buffer[bytes_received] = '\0';
    printf("Received from server (UDP): %s\n", buffer);
}


int main() {
    char buffer[BUFFER_SIZE];
    struct addrinfo *tcp_res, *udp_res;
    int tcp_sockfd = create_tcp_socket("127.0.0.1", PORT, &tcp_res);
    int udp_sockfd = create_udp_socket("127.0.0.1", PORT, &udp_res);

    printf("Enter commands or HELP for usage. To quit, type EXIT.\n");

    while (fgets(buffer, sizeof buffer, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (strncmp(buffer, "ADD", 3) == 0) {
            send_tcp_message(tcp_sockfd, tcp_res, buffer);
        } else if (strncmp(buffer, "DELIVER", 7) == 0) {
            send_udp_message(udp_sockfd, udp_res, buffer);
        } else if (strcmp(buffer, "EXIT") == 0) {
            printf("Shutting down client.\n");
            break;
        } else if (strcmp(buffer, "HELP") == 0) {
            printf("Usage:\n"
                   "ADD [CARBON|OXYGEN|HYDROGEN] [number]\n"
                   "DELIVER [WATER|CARBON DIOXIDE|ALCOHOL|GLUCOSE] [number]\n");
        } else {
            printf("Unknown command. Use ADD or DELIVER or type EXIT to quit.\n");
        }
    }

    // Clean up
    close(tcp_sockfd);
    close(udp_sockfd);
    freeaddrinfo(tcp_res);
    freeaddrinfo(udp_res);

    return 0;
}
