#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8081
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char user_input[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");
    printf("Type 'exit' to quit\n");

    // Chat loop
    while (1) {
        // Get user input
        printf("[Client]: ");
        fgets(user_input, sizeof(user_input), stdin);

        // Remove newline from fgets
        user_input[strcspn(user_input, "\n")] = 0;

        // Send message to server
        send(sock, user_input, strlen(user_input), 0);

        // Check for exit command
        if (strcmp(user_input, "exit") == 0) {
            printf("Disconnecting from server.\n");
            break;
        }

        // Clear buffer
        memset(buffer, 0, sizeof(buffer));

        // Receive response from server
        int recv_size = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (recv_size <= 0) break;

        printf("[Server]: %s\n", buffer);

        // Check if server sent exit
        if (strcmp(buffer, "exit") == 0) {
            printf("Server closed connection.\n");
            break;
        }
    }

    // Close socket
    close(sock);

    return 0;
}
