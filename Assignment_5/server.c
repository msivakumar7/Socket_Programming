#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8084
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

// Structure to pass to thread
struct client_info {
    int socket;
    struct sockaddr_in address;
};

// Thread function to handle individual client
void *handle_client(void *arg) {
    struct client_info *client = (struct client_info *)arg;
    char buffer[BUFFER_SIZE] = {0};
    int recv_size;

    // Receive message from client
    recv_size = recv(client->socket, buffer, BUFFER_SIZE - 1, 0);
    if (recv_size > 0) {
        buffer[recv_size] = '\0';
        printf("Client %d: %s\n", client->socket, buffer);
    }

    // Close socket for this client
    close(client->socket);
    free(client);

    return NULL;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t thread_id;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Enable socket reuse
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
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
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Multi-Client Server listening on port %d...\n", PORT);
    printf("Waiting for clients (up to %d)...\n", MAX_CLIENTS);

    // Accept and handle multiple clients
    while (1) {
        // Accept client connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        printf("New client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port));

        // Create structure for thread
        struct client_info *client = (struct client_info *)malloc(sizeof(struct client_info));
        if (client == NULL) {
            perror("malloc failed");
            close(client_fd);
            continue;
        }

        client->socket = client_fd;
        client->address = client_addr;

        // Create thread to handle this client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client) != 0) {
            perror("pthread_create failed");
            free(client);
            close(client_fd);
            continue;
        }

        // Detach thread so resources are freed automatically
        pthread_detach(thread_id);
    }

    // Close server socket (will not be reached in normal operation)
    close(server_fd);

    return 0;
}
