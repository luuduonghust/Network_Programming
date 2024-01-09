#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sessionManager.h"



// xử lý phản hồi từ yêu cầu kết bạn và cập nhật danh sách bạn bè
void processResponse(char *message, int socket_fd, const struct Session* sessionList){
    char userId1[256];
    char userId2[256];
        if (sscanf(message, "%s\n%s\r\n",  userId1, userId2) == 2) {
          
            int socket_user1 = findSocketIdByUserId(sessionList, userId1);
            int socket_user2 = findSocketIdByUserId(sessionList, userId2);
           // Đồng ý kết bạn
                 FILE *file = fopen("friend.txt", "a");
                if (file == NULL) {
                perror("Error opening file");
                exit(1);
                }
                    fprintf(file, "%s %s\n", userId1, userId2);
                    fclose(file);

                send(socket_user1, "2108", sizeof("2108"), 0);
                send(socket_user2, "2108", sizeof("2108"), 0);
            
        
        }

}