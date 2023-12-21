#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char message[1024];

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Send a message to the server
    strcpy(message, "Hello, server! This is the client.");
    send(client_socket, message, strlen(message), 0);
    printf("Message sent to server: %s\n", message);

    // Receive a response from the server
    if (recv(client_socket, message, sizeof(message), 0) <= 0) {
        perror("Error receiving data from server");
    } else {
        printf("Server response: %s\n", message);
    }

    // Close the socket
    close(client_socket);

    return 0;
}



