#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    char user_input[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept client connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client Connected: %s\n", inet_ntoa(client_addr.sin_addr));

    // Chat loop
    while (1) {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));

        // Receive message from client
        int recv_size = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (recv_size <= 0) break;

        printf("[Client]: %s\n", buffer);

        // Check for exit command
        if (strcmp(buffer, "exit") == 0) {
            printf("Client disconnected.\n");
            break;
        }

        // Get server response
        printf("[Server]: ");
        fgets(user_input, sizeof(user_input), stdin);

        // Remove newline from fgets
        user_input[strcspn(user_input, "\n")] = 0;

        // Send response
        send(client_fd, user_input, strlen(user_input), 0);

        // Check if server wants to exit
        if (strcmp(user_input, "exit") == 0) {
            printf("Server closing connection.\n");
            break;
        }
    }

    // Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
