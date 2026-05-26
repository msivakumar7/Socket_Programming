#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8082
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    char *message = "Networking Lab";
    char buffer[BUFFER_SIZE] = {0};

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Send message to server
    printf("Sent: %s\n", message);
    sendto(sock, message, strlen(message), 0, 
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Receive response from server
    int recv_size = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, 
                             (struct sockaddr *)&server_addr, &addr_len);
    if (recv_size < 0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    buffer[recv_size] = '\0';
    printf("Received: %s\n", buffer);

    // Close socket
    close(sock);

    return 0;
}
