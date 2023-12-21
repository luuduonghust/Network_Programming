#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../lib/sendMessageToServer.h"
#include "../lib/user.h"
#include <sys/socket.h>

// Khai báo cấu trúc người dùng
struct User {
    char* fullname;
    char* username;
    char* password;
};

// Hàm đăng nhập
int login(struct User *users, int numUsers, char *username, char *password) {
    for (int i = 0; i < numUsers; ++i) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1; // Đăng nhập thành công
        }
    }
    return 0; // Đăng nhập thất bại
}

// Hàm đăng ký
void registerUser(struct User *users, int *numUsers) {
    printf("Nhap ho ten: ");
    scanf("%s", users[*numUsers].fullname);

    printf("Nhap tai khoan: ");
    scanf("%s", users[*numUsers].username);

    printf("Nhap mat khau: ");
    scanf("%s", users[*numUsers].password);

    (*numUsers)++;
    printf("Dang ky thanh cong!\n");
}

// Hàm chính
int main() {
    struct User users[100]; // Giả sử có tối đa 100 người dùng
    int numUsers = 0;
    int client_socket;

    int choice;
    char username[50];
    char password[50];

    client_socket = 

    do {
        printf("\nChon chuc nang:\n");
        printf("1. Dang ky\n");
        printf("2. Dang nhap\n");
        printf("3. Thoat\n");

        printf("Chon chuc nang: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Dang ky\n");
                registerUser(users, &numUsers);
                break;
            case 2:
                // Màn hình 1: Đăng nhập

                printf("Nhap tai khoan: ");
                scanf("%s", username);

                printf("Nhap mat khau: ");
                scanf("%s", password);

                // Kiểm tra đăng nhập
                if (login(users, numUsers, username, password)) {
                    // Màn hình 2: Hiển thị menu chọn
                    int choice2;
                    do {
                        printf("\nMàn hình 2:\n");
                        printf("1. Tạo Nhóm Chat\n");
                        printf("2. Chat với bạn\n");
                        printf("3. Danh sách bạn bè\n");
                        printf("4. Gửi lời mời kết bạn\n");
                        printf("5. Thoát\n");

                        printf("Chon chuc nang: ");
                        scanf("%d", &choice2);

                        switch (choice2) {
                            case 1:
                                printf("Tạo Nhóm Chat\n");
                                // Thêm mã chức năng tạo nhóm chat
                                break;
                            case 2:
                                printf("Chat với bạn\n");
                                // Thêm mã chức năng chat với bạn
                                break;
                            case 3:
                                printf("Danh sách bạn bè\n");
                                // Thêm mã chức năng hiển thị danh sách bạn bè
                                break;
                            case 4:
                                printf("Gửi lời mời kết bạn\n");
                                // Thêm mã chức năng gửi lời mời kết bạn
                                break;
                            case 5:
                                printf("Thoát\n");
                                break;
                            default:
                                printf("Chuc nang khong hop le\n");
                        }

                    } while (choice2 != 5);
                } else {
                    printf("Dang nhap that bai\n");
                }
                break;
            case 3:
                printf("Thoat\n");
                break;
            default:
                printf("Chuc nang khong hop le\n");
        }

    } while (choice != 3);

    return 0;
}
