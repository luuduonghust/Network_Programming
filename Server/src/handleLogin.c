#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../lib/sessionManager.h"

#define MAX_LINE_LENGTH 256

void handleLogin(char *message, int socket_fd, const struct Session *sessionList)
{

    char *username = NULL;
    char *password = NULL;
    char line[MAX_LINE_LENGTH];
    int status = 0; // Giả sử tất cả đều không đúng ban đầu
    int find_username = 0;
    char userId_find[MAX_LINE_LENGTH];

    // Tách username và password từ message
    if (sscanf(message, "%m[^\n]\n%m[^\r]", &username, &password) != 2)
    {
        fprintf(stderr, "Error parsing message.\n");
        return;
    }

    // Mở file để đọc
    FILE *file = fopen("../database/account.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        free(username);
        free(password);
        return;
    }

    // Đọc từng dòng từ file
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char fullname[MAX_LINE_LENGTH];
        char fileUsername[MAX_LINE_LENGTH];
        char filePassword[MAX_LINE_LENGTH];
        char userId[MAX_LINE_LENGTH];
        int fileStatus;

        // Phân tích thông tin từ dòng
        if (sscanf(line, "%s %s %s %s %d", fullname, fileUsername, filePassword, userId, &fileStatus) != 5)
        {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }
        if (strcmp(username, fileUsername) == 0)
        {
            find_username = 1;
        }
        // Kiểm tra thông tin đăng nhập
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0)
        {
            status = fileStatus;
            strcpy(userId_find, userId);
            break; // Tìm thấy thông tin đăng nhập, thoát khỏi vòng lặp
        }
    }

    // Đóng file
    fclose(file);

    // Xử lý kết quả đăng nhập
    if (status == 1)
    {
        // Đăng nhập thành công
        send(socket_fd, "2002", sizeof("2002"), 0);
        addSession(sessionList, socket_fd, userId_find);
    }
    else if (find_username == 1)
    {
        send(socket_fd, "4202", sizeof("2002"), 0);
    }
    else
    {
        send(socket_fd, "4102", sizeof("2002"), 0);
    }

    // Giải phóng bộ nhớ
    free(username);
    free(password);
}