// Include necessary headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sessionManager.h"
#include "../lib/limit_local.h"

// Define the maximum length of a line
#define MAX_LINE_LENGTH 12

// Function to get the friend list and status
void getFriendListAndStatus(char *user_id, int socket_fd, const struct Session *sessionList)
{
    // Open the friend file for reading
    FILE *friendFile = fopen("friend.txt", "r");
    if (!friendFile)
    {
        perror("Error opening friend.txt");
        return;
    }

    char line[MAX_LINE_LENGTH];
    // Initialize the response with the size of "Protocol code" and one newline character
    char *response = malloc(sizeof("308\n") + 1);
    strcpy(response, "308\n");

    int totalFriends = 0;

    // Iterate through the friend.txt file
    while (fgets(line, sizeof(line), friendFile) != NULL)
    {
        char *friend1 = strtok(line, " \n");
        char *friend2 = strtok(NULL, " \n");

        // Check if user_id is a friend of friend1 or friend2
        if (strcmp(user_id, friend1) == 0)
        {
            int status = findSocketIdByUserId(sessionList, friend2) != -1 ? 1 : 0;
            // Perform realloc to add new data
            response = realloc(response, strlen(response) + MAX_ID_LENGTH + 3);
            sprintf(response + strlen(response), "%s %d\n", friend2, status);
            totalFriends++;
        }
        else if (strcmp(user_id, friend2) == 0)
        {
            int status = findSocketIdByUserId(sessionList, friend1) != -1 ? 1 : 0;
            // Perform realloc to add new data
            response = realloc(response, strlen(response) + MAX_ID_LENGTH + 3);
            sprintf(response + strlen(response), "%s %d\n", friend1, status);
            totalFriends++;
        }
    }

    fclose(friendFile);

    // Add the total number of friends to the response
    // Perform realloc to add new data
    response = realloc(response, strlen(response) + MAX_LINE_LENGTH);
    sprintf(response + strlen(response), "%d\n", totalFriends);

    // Send the response back to the client
    send(socket_fd, response, strlen(response), 0);

    // Free the allocated memory after use
    free(response);
}
