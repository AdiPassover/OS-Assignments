#include "ClientAPI.hpp"

#define PORT "55555"


int main() {
    char buffer[BUFFER_SIZE];
    struct addrinfo *tcp_res, *udp_res;
    int tcp_sockfd = create_tcp_socket("127.0.0.1", PORT, &tcp_res);
    int udp_sockfd = create_udp_socket("127.0.0.1", PORT, &udp_res);

    printf("Enter commands or HELP for usage. To quit, type EXIT.\n");

    while (fgets(buffer, sizeof buffer, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (strncmp(buffer, "ADD", 3) == 0) {
            send_tcp_message(tcp_sockfd, buffer);
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
