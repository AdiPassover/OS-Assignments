#ifndef OS_A3_CLIENTAPI_HPP
#define OS_A3_CLIENTAPI_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUFFER_SIZE 100


int create_tcp_socket(const char* server_ip, const char* port, struct addrinfo** res);
void send_tcp_message(int sockfd, const char* message);

int create_udp_socket(const char* server_ip, const char* port, struct addrinfo** res);
void send_udp_message(int sockfd, struct addrinfo* res, char* message);

int create_udss_socket(const char* uds_path, struct sockaddr_un** addr);
void send_udss_message(int sockfd, const char* message);

int create_udsd_socket(const char* uds_path, struct sockaddr_un** res);
void send_udsd_message(int sockfd, struct sockaddr_un* res, char* message);

#endif //OS_A3_CLIENTAPI_HPP
