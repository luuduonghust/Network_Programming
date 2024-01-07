#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../lib/sessionManager.h"

// Structure to store account information
typedef struct
{
    char *fullname;
    char *username;
    char *password;
    char *ID;
    int active; // 1: active, 0: inactive
} Account;

// Function to check if a username and password are valid and active
int isValidAccount(char *inputUsername, char *inputPassword, Account *accounts, int numAccounts)
{
    for (int i = 0; i < numAccounts; i++)
    {
        if (strcmp(inputUsername, accounts[i].username) == 0 &&
            strcmp(inputPassword, accounts[i].password) == 0 &&
            accounts[i].active == 1)
        {
            return 1; // Valid and active account
        }
    }
    return 0; // Invalid or inactive account
}

// Function to read accounts from file
Account *readAccountsFromFile(const char *filename, int *numAccounts)
{
    FILE *file = fopen("../database/account.txt", "r");
    size_t len = 0;
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Count the number of accounts in the file
    *numAccounts = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            (*numAccounts)++;
        }
    }
    rewind(file);

    // Allocate memory for accounts
    Account *accounts = malloc(*numAccounts * sizeof(Account));

    // Read accounts from file
    for (int i = 0; i < *numAccounts; i++)
    {
        char *line = NULL;
        char fullname = NULL;
        char username = NULL;
        char password = NULL;
        char ID = NULL;
        int active;
        // Đọc một dòng từ tệp
        if (getline(&line, &len, file) != -1)
        {
            // Hiển thị dòng vừa đọc
            printf("Read line: %s", line);

            // Bạn có thể sử dụng 'line' ở đây cho mục đích khác
        }
        else
        {
            // Xử lý khi không đọc được dòng
            perror("Error reading line");
        }
        fscanf(file, "%s\n%s\n%s\n%s\n%d", fullname, username, password, ID, &active);

        accounts[i].fullname = strdup(fullname);
        accounts[i].username = strdup(username);
        accounts[i].password = strdup(password);
        accounts[i].active = active;
    }

    fclose(file);
    return accounts;
}

void handleLogin(char *message, int socket_fd, const struct Session *sessionList)
{

    // Cấp phát bộ nhớ động cho các chuỗi con
    char *command = NULL;
    char *username = NULL;
    char *password = NULL;

    // Tách chuỗi bằng strtok
    char *token = strtok(message, " ");
    if (token != NULL)
    {
        command = malloc(strlen(token) + 1);
        strcpy(command, token);

        token = strtok(NULL, " ");
        if (token != NULL)
        {
            username = malloc(strlen(token) + 1);
            strcpy(username, token);

            token = strtok(NULL, " ");
            if (token != NULL)
            {
                password = malloc(strlen(token) + 1);
                strcpy(password, token);
            }
        }
    }
    // Kiem tra tai tai khoan

    // Giải phóng bộ nhớ đã cấp phát
    free(command);

    // Kiểm tra tài khoản từ file
    int numAccounts;
    Account *accounts = readAccountsFromFile("account.txt", &numAccounts);

    // Kiểm tra tài khoản và mật khẩu
    if (isValidAccount(username, password, accounts, numAccounts))
    {
        // Thanh cong
        send(socket_fd, "203", sizeof("203"), 0);
        // Tao session, mot tai khoan chi cho phep mot session
        addSession(&sessionList, socket_fd, accounts[0].ID);
    }
    else
    {
        // Khong thanh cong
        send(socket_fd, "402", sizeof("402"), 0);
    }
    // Giải phóng bộ nhớ đã cấp phát
    for (int i = 0; i < numAccounts; i++)
    {
        free(accounts[i].fullname);
        free(accounts[i].username);
        free(accounts[i].password);
    }
    free(accounts);

    // Tra ve thong diep client
}