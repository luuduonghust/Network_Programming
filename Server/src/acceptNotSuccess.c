#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sessionManager.h"



// xử lý phản hồi từ yêu cầu từ chối kết bạn 
void acceptNotSuccess(char *message, int socket_fd, const struct Session* sessionList){
    // char responseCode[256];
    char userId1[256];
    char userId2[256];
        if (sscanf(message, "%s\n%s\r\n", userId1, userId2) == 2) {
          
            int socket_user1 = findSocketIdByUserId(sessionList, userId1);
            int socket_user2 = findSocketIdByUserId(sessionList, userId2);
           //Từ chối kết bạn
          
                send(socket_user1, "2208", sizeof("2208"), 0);
                send(socket_user2, "2208", sizeof("2208"), 0);
        
        
        }

}