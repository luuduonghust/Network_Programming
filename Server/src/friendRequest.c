#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../lib/sessionManager.h"



int checkFriendship( const char *userId1, const char *userId2) {
    FILE *file = fopen("../database/friend.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {//// Đọc từng dòng của tệp
        char *friendId1 = strtok(line, " ");// Tách lấy userId1 
        char *friendId2 = strtok(NULL, " \n");//Tách lấy userId2
        // Kiểm tra xem dòng hiện tại có chứa cả hai userId1 và userId2 hay không
      if ((strcmp(friendId1, userId1) == 0 && strcmp(friendId2, userId2) == 0) ||
            (strcmp(friendId1, userId2) == 0 && strcmp(friendId2, userId1) == 0)) {
            fclose(file);
            return 1; // Nếu tìm thấy, trả về 1 (true)
        }
    }

    fclose(file);
    return 0; // Nếu không tìm thấy, trả về 0 (false)
}

void requestToFriend(char *message, int socket_fd, const struct Session *sessionList){
    char senderId[256];
    char receiverId[256];

// Phân tích thông điệp
        if (sscanf(message, "%s/n%s/r/n", senderId, receiverId) == 2) {
                // Tìm socket ID của người nhận thông qua danh sách phiên
                int socket_client = findSocketIdByUserId(sessionList, receiverId);
                
                if (isFriend( senderId, receiverId) == 1 || isFriend( receiverId, senderId) == 1) {
                    // Nếu đã là bạn bè, gửi mã phản hồi "411" cho người gửi
                    send(socket_fd, "4108", sizeof("4108"), 0);
                }
                if (socket_client == -1)
                {
                    int socket_sender = findSocketIdByUserId(sessionList, senderId);
                    send(socket_sender,"4008", sizeof("4008"), 0);
                } else {
                   //Thông điệp gửi cho người nhận
                char send_message[512];
                //Tạo chuỗi với mã giao thức "311" và senderId
                sprintf(send_message, "2008/n%s", senderId);
                // Gửi thông điệp tới người nhận
                send(socket_client, send_message, sizeof(send_message), 0);
                }
                
        }

}
