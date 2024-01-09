#include <stdio.h>
#include <stdlib.h>
#include <string.h>

addUserToGroup(char *message, int socket_fd)
{
    // Extract GROUP_ID and USER_ID from the message
    char *ID_GROUP = strtok(message, "\n");
    char *ID_USER = strtok(NULL, "\n");

    FILE *groupFile = fopen("group.txt", "r+");
    if (!groupFile)
    {
        perror("Error opening group.txt");
        return;
    }

    char *line = NULL;
    size_t line_length = 0;
    int groupExists = 0;

    // Iterate through the group.txt file
    while (getline(&line, &line_length, groupFile) != -1)
    {
        char *groupID = strtok(line, " \n");
        if (strcmp(ID_GROUP, groupID) == 0)
        {
            // GROUP_ID exists, append USER_ID to the line
            fseek(groupFile, -1, SEEK_CUR);
            fprintf(groupFile, "%s\n", ID_USER);
            /*
            fseek(groupFile, -1, SEEK_CUR);
            fprintf(groupFile, "%s", ID_USER);
            */
            groupExists = 1;
            break;
        }
    }

    free(line);
    fclose(groupFile);

    // If the GROUP_ID exists, send the success code 317
    if (groupExists)
    {
        send(socket_fd, "317", sizeof("317"), 0);
    }
    else
    {
        // If GROUP_ID doesn't exist, send the failure code 417
        send(socket_fd, "417", sizeof("417"), 0);
    }
}