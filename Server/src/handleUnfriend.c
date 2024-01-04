#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct FriendRequest {
    char senderId[256];
    char receiverId[256];
};
void processRemoveFriend(char *message, int socket_fd) {
    int protocolCode;
    char removeCode[256];
    char userId1[256];
    char userId2[256];
        if (sscanf(message, "%d\n%s\n%s\n%s", &protocolCode, removeCode, userId1, userId2) == 4) {
            if (strcmp(removeCode, "remove_friend") == 0){
                 FILE *file = fopen("friend.txt", "r");
                if (file == NULL) {
                perror("Error opening file");
                exit(1);
                }
                int found = 0;
                FILE *tempFile = fopen("temp.txt", "w");
                    if (tempFile == NULL) {
                perror("Error opening file");
                fclose(file);
                exit(1);
            }
                char tempUserId1[256];
                char tempUserId2[256];
                while (fscanf(file, "%s %s", tempUserId1, tempUserId2) != EOF) {
                if ((strcmp(userId1, tempUserId1) == 0 && strcmp(userId2, tempUserId2) == 0) ||
                    (strcmp(userId2, tempUserId1) == 0 && strcmp(userId1, tempUserId2) == 0)) {
                    found = 1; 
                } else {
                    fprintf(tempFile, "%s %s\n", tempUserId1, tempUserId2);
                    }
                }
                  fclose(file);
                fclose(tempFile);
                 remove("friend.txt");
            rename("temp.txt", "friend.txt");
             if (found) {
                send(socket_fd, "315",sizeof("315"), 0);
             }
            }
        }
}