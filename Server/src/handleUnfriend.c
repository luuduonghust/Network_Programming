#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct FriendRequest {
    char senderId[256];
    char receiverId[256];
};
