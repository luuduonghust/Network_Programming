#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "../lib/function_chat.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

void displayMenu01()
{
    printf("\n----- Menu -----\n");
    printf("1. Đăng Ký\n");
    printf("2. Đăng nhập\n");
    printf("0. Thoát\n");
    printf("Nhập lựa chọn của bạn: ");
}

void displayMenu02()
{
    printf("\n----- Menu -----\n");
    printf("1. Gửi lời mời kết bạn\n");
    printf("2. Thông báo lời mời kết bạn\n");
    printf("3. Xem danh sách bạn bè\n");
    printf("4. Chat giữa hai người\n");
    printf("5. Tạo nhóm chat\n");
    printf("6. Thêm người dùng khác vào nhóm chat\n");
    printf("7. Xóa người dùng khỏi nhóm chat\n");
    printf("8. Rời nhóm chat\n");
    printf("9. Chat trong nhóm\n");
    printf("0. Thoát\n");
    printf("Nhập lựa chọn của bạn: ");
}

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Tạo socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Cấu hình địa chỉ server
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    // Kết nối đến server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    int choice;
    do
    {
        displayMenu();
        scanf("%d", &choice);

        // Khi người dùng chưa đăng nhập
        switch (choice)
        {
        case 1:
        {
            // Xử lý đăng ký
            // ...
            // break;
            // Dăng ký xong vẫn phải yêu cầu đăng nhập hoặc đăng ký nên chỗ này không có break
        }
        case 2:
        {
            // Xử lý đăng nhập
            // ...
            break;
        }
        // Thêm các trường hợp xử lý cho các chức năng khác tại đây
        case 0:
        {
            printf("Thoát khỏi ứng dụng.\n");
            break;
        }
        default:
        {
            printf("Lựa chọn không hợp lệ.\n");
        }
        }

    } while (choice != 0);

    // Đóng kết nối
    close(clientSocket);

    return 0;
}
