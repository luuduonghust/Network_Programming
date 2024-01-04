//Server return message: "Protocol code\n Total Friend \n ID1 Status1 \n ID2 Status2 \n ID3 Status3 \n ID4 Status4 \n ID5 Status5 \n ..."
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sessionManager.h"

#define MAX_FRIENDS 100
#define MAX_LINE_LENGTH 256

void loadFriendData(char *filename, char *userId, char friends[MAX_FRIENDS][50], int *totalFriends) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    *totalFriends = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *friendId = strtok(line, " ");
        char *userIdInFile = strtok(NULL, " \n");

        if (strcmp(userId, userIdInFile) == 0) {
            // User_id matches, add friend to the list
            strcpy(friends[*totalFriends], friendId);
            (*totalFriends)++;
        } else if (strcmp(userId, friendId) == 0) {
            // User_id found in friend's column, add friend to the list
            strcpy(friends[*totalFriends], userIdInFile);
            (*totalFriends)++;
        }
    }

    fclose(file);
}

int findSocketIdByUserId(const struct Session *sessionList, const char *userId, int totalSessions) {
    for (int i = 0; i < totalSessions; i++) {
        if (strcmp(sessionList[i].userId, userId) == 0) {
            return sessionList[i].socketId;
        }
    }
    return -1;
}

void getFriendListAndStatus(char *user_id, int socket_fd) {
    // Load friend data from file
    char friends[MAX_FRIENDS][50];
    int totalFriends;
    loadFriendData("friend.txt", user_id, friends, &totalFriends);

    // Simulate session list (replace with your actual session list)
    struct Session sessionList[] = {
        {"ID1", 101},
        {"ID2", 102},
        {"ID3", 103},
        // Add more sessions as needed
    };
    int totalSessions = sizeof(sessionList) / sizeof(sessionList[0]);

    // Prepare the response message
    char response[1024];
    sprintf(response, "Protocol code\nTotal Friend\n");
    for (int i = 0; i < totalFriends; i++) {
        int status = findSocketIdByUserId(sessionList, friends[i], totalSessions) != -1 ? 1 : 0;
        sprintf(response + strlen(response), "%s %d\n", friends[i], status);
    }

    // Send the response to the client
    send(socket_fd, response, strlen(response), 0);
}