#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../lib/session_manager.h"

struct FriendRequest {
    char senderId[256];
    char receiverId[256];
};

void requestToFriend(char *message, int socket_fd){
    int protocolCode;
    char senderId[256];
    char receiverId[256];
    char requestType[256];

// Chưa có hàm xử lý để gửi cho Recieve
        if (sscanf(message, "%d/n%s/n%s/n%s/r/n", &protocolCode, senderId, receiverId, requestType) == 4) {
             if (strcmp(requestType, "send_friend_request") == 0){
                int socket_client = findSocketIdByUserId(sessionList, receiverId);
                send(socket_client, "311", sizeof("311"), 0);
             }
        }

}
void processResponse(char *message, int socket_fd){
    char responseCode[256];
    char userId1[256];
    char userId2[256];
        if (sscanf(message, "%d/n%s/n%s/n%s/r/n", responseCode, userId1, userId2) == 3) {
            if (strcmp(responseCode, "312") == 0) {
                 FILE *file = fopen("friend.txt", "a");
                if (file == NULL) {
                perror("Error opening file");
                exit(1);
                }
                    fprintf(file, "%s %s\n", userId1, userId2);
                    fprintf(file, "%s %s\n", userId2, userId1);
                    fclose(file);
                send(socket_fd, "312", sizeof("312"), 0);
            }
        }

}