#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include<unistd.h>
#define BUFFER_SIZE 256

int unFriendFunction(int clientSocket){

     char user_id_1[256];
    char user_id_2[256];

    // Nhập thông tin từ người dùng
    printf("Nhap id cua ban: ");
    scanf("%s", user_id_1);

    printf("Nhap id nguoi muon huy ket ban ");
    scanf("%s", user_id_2);

    // Tạo thông điệp gửi lời mời kết bạn
    char message[512];
    sprintf(message, "un_friend\n%s\n%s\r\n", user_id_1, user_id_2);

    send(clientSocket, message, strlen(message), 0);

     char response[BUFFER_SIZE];
    if (recv(clientSocket, response, BUFFER_SIZE, 0) == -1)
    {
        perror("Error receiving response from server");
        return 0; // Trả về 0 để báo lỗi
    }

      // Xử lý phản hồi từ serve
        if (strcmp(response, "2009") == 0) {
            printf("Huy ket ban thanh cong\n");
            // Thực hiện các công việc cần thiết sau khi gửi lời mời thành công
        } else if (strcmp(response, "4009") == 0) {
            printf("Khong tim thay ID hoac nguoi dung chua ket ban\n");
            // Thực hiện các công việc cần thiết khi không tìm thấy ID hoặc người dùng không online
        } 

}