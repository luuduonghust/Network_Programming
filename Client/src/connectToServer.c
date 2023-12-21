#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int connectToServer(char* server_add, int port){
    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_add);
    server_addr.sin_port = htons(port);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    return client_socket;
}