#include "ClientAPI.hpp"

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

void send_tcp_message(int sockfd, const char* message) {
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

int create_udss_socket(const char* uds_path, struct sockaddr_un** addr) {
    int sockfd;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror_and_exit("Creating UDS stream socket failed");
    }

    *addr = (struct sockaddr_un*) malloc(sizeof(struct sockaddr_un));
    if (*addr == NULL) {
        perror_and_exit("Memory allocation for sockaddr_un failed");
    }

    memset(*addr, 0, sizeof(struct sockaddr_un));
    (*addr)->sun_family = AF_UNIX;
    strncpy((*addr)->sun_path, uds_path, sizeof((*addr)->sun_path) - 1);

    if (connect(sockfd, (struct sockaddr*) *addr, sizeof(struct sockaddr_un)) == -1) {
        perror_and_exit("UDS stream connection failed");
    }

    return sockfd;
}

void send_udss_message(int sockfd, const char* message) {
    // Send the message
    ssize_t bytes_sent = send(sockfd, message, strlen(message), 0);
    if (bytes_sent == -1) {
        perror_and_exit("Sending UDS stream message failed");
    }

    // Receive server response
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(sockfd, buffer, sizeof buffer - 1, 0);
    if (bytes_received == -1) {
        perror_and_exit("Receiving UDS stream response failed");
    }

    buffer[bytes_received] = '\0';
    printf("Received from server (UDS stream): %s\n", buffer);
}

int create_udsd_socket(const char* uds_path, struct sockaddr_un** addr) {
    int sockfd;

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror_and_exit("Creating UDS datagram socket failed");
    }

    *addr = (struct sockaddr_un*) malloc(sizeof(struct sockaddr_un));
    if (*addr == NULL) {
        perror_and_exit("Memory allocation for sockaddr_un failed");
    }

    memset(*addr, 0, sizeof(struct sockaddr_un));
    (*addr)->sun_family = AF_UNIX;
    strncpy((*addr)->sun_path, uds_path, sizeof((*addr)->sun_path) - 1);

    return sockfd;
}

void send_udsd_message(int sockfd, struct sockaddr_un* addr, char* message) {
    const char* cd_message = "DELIVER CARBON DIOXIDE";
    if (strncmp(message, cd_message, strlen(cd_message)) == 0)
        message[14] = '_';

    // Send the message
    ssize_t bytes_sent = sendto(sockfd, message, strlen(message), 0, (struct sockaddr*) addr, sizeof(struct sockaddr_un));
    if (bytes_sent == -1) {
        perror_and_exit("Sending UDS datagram message failed");
    }

    printf("Sent to server (UDS datagram)\n");
}
