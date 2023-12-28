#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../lib/session_manager.h"

// Structure to store account information
typedef struct {
    char *fullname;
    char *username;
    char *password;
    char *ID;
    int active; // 1: active, 0: inactive
} Account;

// Function to check if a username and password are valid and active
int isValidAccount(char *inputUsername, char *inputPassword, Account *accounts, int numAccounts) {
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(inputUsername, accounts[i].username) == 0 &&
            strcmp(inputPassword, accounts[i].password) == 0 &&
            accounts[i].active == 1) {
            return 1; // Valid and active account
        }
    }
    return 0; // Invalid or inactive account
}

// Function to read accounts from file
Account *readAccountsFromFile(const char *filename, int *numAccounts) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Count the number of accounts in the file
    *numAccounts = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            (*numAccounts)++;
        }
    }
    rewind(file);

    // Allocate memory for accounts
    Account *accounts = malloc(*numAccounts * sizeof(Account));

    // Read accounts from file
    for (int i = 0; i < *numAccounts; i++) {
        char fullname[100], username[100], password[100], ID[100];
        int active;
        fscanf(file, "%s %s %s %s %d", fullname, username, password, ID, &active);

        accounts[i].fullname = strdup(fullname);
        accounts[i].username = strdup(username);
        accounts[i].password = strdup(password);
        accounts[i].active = active;
    }

    fclose(file);
    return accounts;
}

void handleLogin(char *message, int socket_fd){

    // Cấp phát bộ nhớ động cho các chuỗi con
    char *command = NULL;
    char *username = NULL;
    char *password = NULL;

    // Tách chuỗi bằng strtok
    char *token = strtok(message, " ");
    if (token != NULL) {
        command = malloc(strlen(token) + 1);
        strcpy(command, token);

        token = strtok(NULL, " ");
        if (token != NULL) {
            username = malloc(strlen(token) + 1);
            strcpy(username, token);

            token = strtok(NULL, " ");
            if (token != NULL) {
                password = malloc(strlen(token) + 1);
                strcpy(password, token);
            }
        }
    }
    //Kiem tra tai tai khoan

    // Giải phóng bộ nhớ đã cấp phát
    free(command);

    // Kiểm tra tài khoản từ file
    int numAccounts;
    Account *accounts = readAccountsFromFile("account.txt", &numAccounts);

    // Kiểm tra tài khoản và mật khẩu
    if (isValidAccount(username, password, accounts, numAccounts)) {
        //Thanh cong
        send(socket_fd, "203", sizeof("203"), 0 );
    } else {
        //Khong thanh cong
        send(socket_fd, "402", sizeof("402"), 0 );
    }
    //Tao session, mot tai khoan chi cho phep mot session
    addSession(&sessionList, socket_fd, accounts[0].ID);
    // Giải phóng bộ nhớ đã cấp phát
    for (int i = 0; i < numAccounts; i++) {
        free(accounts[i].fullname);
        free(accounts[i].username);
        free(accounts[i].password);
    }
    free(accounts);

    //Tra ve thong diep client
}