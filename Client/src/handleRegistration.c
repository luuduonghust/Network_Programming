#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../lib/sendMessageToServer.h"
#include "../lib/user.h"

void *handleRegistration(void *arg, int client_socket){

    struct User newUser;

    size_t input_buffer_size_full_name = 0;
    size_t input_buffer_size_account = 0;
    size_t input_buffer_size_password = 0;
    // Read user input
    printf("Nhap ho ten: ");
    ssize_t input_length_full_name = getline(newUser.fullname, &input_buffer_size_full_name, stdin);
    if (input_length_full_name <= 0) {
        free(newUser.fullname);
        break;
    }

    printf("Nhap tai khoan: ");
    ssize_t input_length_account = getline(newUser.fullname, &input_buffer_size_account, stdin);
    if (input_length_account <= 0) {
        free(newUser.username);
        break;
    }

     printf("Nhap mat khau: ");
    ssize_t input_length_password = getline(newUser.fullname, &input_buffer_size_password, stdin);
    if (input_length_password <= 0) {
        free(newUser.password);
        break;
    }
    int protocolCode = 200;
    char* message;  

    // Ghép mã giao thức
    sprintf(message, "register/n");

    // Ghép thông tin người dùng
    sprintf(message + strlen(message), "%s/n%s/n%s/r/n", newUser.fullname, newUser.username, newUser.password);

    // Gửi thông điệp đến server
    sendMessageToServer(message, client_socket);

    // send(client_socket, &protocolCode, sizeof(protocolCode))
}