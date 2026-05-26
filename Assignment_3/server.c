#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8082
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    char response[BUFFER_SIZE];

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    // Receive message from client
    int recv_size = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, 
                             (struct sockaddr *)&client_addr, &addr_len);
    if (recv_size < 0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    buffer[recv_size] = '\0';
    printf("Received from %s:%d - %s\n", inet_ntoa(client_addr.sin_addr), 
           ntohs(client_addr.sin_port), buffer);

    // Convert message to uppercase
    strcpy(response, buffer);
    for (int i = 0; response[i]; i++) {
        response[i] = toupper((unsigned char)response[i]);
    }

    printf("Sending: %s\n", response);

    // Send response back to client
    sendto(sock, response, strlen(response), 0, 
           (struct sockaddr *)&client_addr, addr_len);

    // Close socket
    close(sock);

    return 0;
}
