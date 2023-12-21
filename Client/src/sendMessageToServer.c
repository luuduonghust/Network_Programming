#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void sendMessageToServer(const char *message, const int socket_fd) {

    // Send the message to the server
    send(socket_fd, message, strlen(message), 0);

}