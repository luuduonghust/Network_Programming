#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sessionManager.h"

//Tìm kiếm socket ID của một user trong danh sách phiên. danh sách phiên, userId cần tìm kiếm và tổng số phiên
int findSocketIdByUserId(const struct Session *sessionList, const char *userId, int totalSessions) {
    //Lặp qua các phiên
    for (int i = 0; i < totalSessions; i++) {
        //Kiểm tra userid trùng với userid cần tìm
        if (strcmp(sessionList[i].userId, userId) == 0) {
           //Trả về socket ID của phiên nếu userId được tìm thấy.
            return sessionList[i].socketId;
        }
    }
    return -1; //Trả về -1 nếu không tìm thấy userI
}

// xử lý phản hồi từ yêu cầu kết bạn và cập nhật danh sách bạn bè
void processResponse(char *message, int socket_fd){
    char responseCode[256];
    char userId1[256];
    char userId2[256];
        if (sscanf(message, "%s/n%s/n%s/r/n", responseCode, userId1, userId2) == 3) {
          
            int socket_user1 = findSocketIdByUserId(sessionList, userId1);
            int socket_user2 = findSocketIdByUserId(sessionList, userId2);
           // Đồng ý kết bạn
            if (strcmp(responseCode, "312") == 0) {
                 FILE *file = fopen("friend.txt", "a");
                if (file == NULL) {
                perror("Error opening file");
                exit(1);
                }
                    fprintf(file, "%s %s\n", userId1, userId2);
                    fprintf(file, "%s %s\n", userId2, userId1);
                    fclose(file);

                send(socket_user1, "312", sizeof("312"), 0);
                send(socket_user2, "312", sizeof("312"), 0);
            }
        
        }

}