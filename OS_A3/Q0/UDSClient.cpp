#include "ClientAPI.hpp"

#define UDSS_PATH "uds_stream_socket"
#define UDSD_PATH "uds_dgram_socket"

int main() {
    char buffer[BUFFER_SIZE];
    struct sockaddr_un *udss_addr, *udsd_addr;
    int udss_sockfd = create_udss_socket(UDSS_PATH, &udss_addr);
    int udsd_sockfd = create_udsd_socket(UDSD_PATH, &udsd_addr);

    printf("Enter commands or HELP for usage. To quit, type EXIT.\n");

    while (fgets(buffer, sizeof buffer, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (strncmp(buffer, "ADD", 3) == 0) {
            send_udss_message(udss_sockfd, buffer);
        } else if (strncmp(buffer, "DELIVER", 7) == 0) {
            send_udsd_message(udsd_sockfd, udsd_addr, buffer);
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
    close(udss_sockfd);
    close(udsd_sockfd);
    free(udss_addr);
    free(udsd_addr);

    return 0;
}
