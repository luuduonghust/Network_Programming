#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../lib/sendMessageToServer.h"
#include "../lib/user.h"

void *sendFriendRequest(void *arg, int client_socket, int sender_ID){
    char friendName[50];
    // Nhập tên của bạn bè
    printf("Nhap ten cua ban be: ");
    scanf("%s", friendName);

    char message[256];
    // char message[] = "311/nid/r/n";
    sprintf(message, "311/n%d/n%s/r/n", sender_ID, friendName);
    sendMessageToServer(message, client_socket);
}