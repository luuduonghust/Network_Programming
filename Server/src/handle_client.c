#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../lib/menu.h"
#include "../lib/sessionManager.h"
#include "../lib/handle_client.h"
/**
 * handleClient - Function to handle client requests in a separate thread.
 *
 * This function handles incoming client requests in a separate thread, processing various commands and interacting with a session management system.
 *
 * @param arg               A pointer to a structure containing thread arguments.
 * @return                  NULL.
 */
void *handleClient(void *arg)
{
    // Extracting thread arguments
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    int conn_fd = args->conn_fd;
    FILE *file = args->file;
    struct sockaddr_in client = args->client_sock_addr;
    struct Session **sessionList = args->sessionList;
    int *gen_id = args->gen_id;

    int login = 0;
    char *partial_data = NULL;
    int partial_data_length = 0;
    char buffer[BUFF_SIZE];
    int bytes_received;

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client.sin_addr), client_ip, INET_ADDRSTRLEN);

    // Connected with client reply
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "2000");
    send(conn_fd, buffer, sizeof(buffer), 0);

    while (1)
    {
        bytes_received = recv(conn_fd, buffer, BUFF_SIZE, 0);
        if (bytes_received <= 0)
        {
            break; // Connection closed or error
        }

        // Append received data to the existing partial_data
        partial_data = realloc(partial_data, partial_data_length + bytes_received);
        if (partial_data == NULL)
        {
            // Handle memory allocation error
            break;
        }
        memcpy(partial_data + partial_data_length, buffer, bytes_received);
        partial_data_length += bytes_received;

        // Process the data until a "\r\n" is found
        char *crlf_position;
        while ((crlf_position = strstr(partial_data, "\r\n")) != NULL)
        {
            *crlf_position = '\0'; // Null-terminate the line
            // Process the complete line

            // Create a buffer to store the complete data
            int buffer_full_size = partial_data_length;
            char *buffer_full = (char *)malloc(buffer_full_size);
            if (buffer_full == NULL)
            {
                perror("malloc");
                // Handle memory allocation error
            }
            strcpy(buffer_full, partial_data);
            /*========================================*/
            // Function login
            if (strncmp(buffer_full, "login", 5) == 0)
            {
                // Nếu có, bỏ qua "login" và ký tự '\n' kế tiếp
                char *remaining_data = strchr(buffer_full, '\n');
                if (remaining_data == NULL)
                {
                    // Chuỗi không bắt đầu bằng login\n...
                }
                else
                {

                    // Bỏ qua ký tự '\n'
                    remaining_data++;
                    handleLogin(remaining_data, conn_fd, sessionList);
                }
            }
            // send(conn_fd, buffer, sizeof(buffer), 0);

            // Function register
            if (strncmp(buffer_full, "register", 8) == 0){
                char *remaining_data = strchr(buffer_full, '\n');
                 if (remaining_data == NULL)
                {
                    // Chuỗi không bắt đầu bằng login\n...
                }
                else
                {

                    // Bỏ qua ký tự '\n'
                    remaining_data++;
                    handleRegistration(remaining_data ,gen_id, conn_fd);
                }
            }
            // Move the remaining data to the beginning of the buffer
            int remaining_length = partial_data_length - (crlf_position - partial_data) - 2;
            memmove(partial_data, crlf_position + 2, remaining_length);
            partial_data_length = remaining_length;
        }
    }

    close(conn_fd);
    free(partial_data);
    free(args);
    pthread_exit(NULL);
}
