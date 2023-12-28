#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10

// Struct để lưu trữ thông tin của mỗi client
typedef struct {
    int socket;
    struct sockaddr_in address;
    int addr_len;
} Client;

Client clients[MAX_CLIENTS];  // Mảng lưu trữ thông tin của các client
pthread_t threads[MAX_CLIENTS];  // Mảng lưu trữ các thread

// Hàm xử lý các yêu cầu từ client
void handle_client(int client_socket) {
    char buffer[1024];
    int bytes_received;
    char *partial_data = NULL;

    while (1) {
        // Đọc dữ liệu từ client
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            // Client đã đóng kết nối hoặc có lỗi
            close(client_socket);
            return;
        }

        // Append received data to the existing partial_data
        partial_data = realloc(partial_data, partial_data_length + bytes_received);
        if (partial_data == NULL) {
            // Handle memory allocation error
            break;
        }
        memcpy(partial_data + partial_data_length, buffer, bytes_received);
        partial_data_length += bytes_received;

        char *crlf_position;
        // Process the data until a "\r\n" is found
        while ((crlf_position = strstr(partial_data, "\r\n")) != NULL) {
            *crlf_position = '\0'; // Null-terminate the line

        // Process the complete line

        // Create a buffer to store the complete data
        int buffer_full_size = partial_data_length;
        char *buffer_full = (char *)malloc(buffer_full_size);
        if (buffer_full == NULL) {
            perror("malloc");
            // Handle memory allocation error
        }
        strcpy(buffer_full, partial_data);

        //Handle

        // Move the remaining data to the beginning of the buffer
        int remaining_length = partial_data_length - (crlf_position - partial_data) - 2;
        memmove(partial_data, crlf_position + 2, remaining_length);
        partial_data_length = remaining_length;
        }
    }
}

// Hàm xử lý từng client trong một thread riêng biệt
void *client_handler(void *arg) {
    int client_index = *((int *)arg);
    handle_client(clients[client_index].socket);

    return NULL;
}

int main() {
    int server_socket, client_socket, addr_len;
    struct sockaddr_in server_addr, client_addr;

    // Khởi tạo server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Cấu hình địa chỉ và cổng của server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind server socket đến địa chỉ và cổng
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Lắng nghe kết nối từ client
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Error listening for connections");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Chấp nhận kết nối từ client và xử lý trong thread riêng biệt
    int client_index;
    for (client_index = 0; client_index < MAX_CLIENTS; ++client_index) {
        addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);

        // Lưu thông tin của client vào mảng clients
        clients[client_index].socket = client_socket;
        clients[client_index].address = client_addr;
        clients[client_index].addr_len = addr_len;

        // Tạo thread để xử lý client
        pthread_create(&threads[client_index], NULL, client_handler, &client_index);
    }

    // Chờ các thread kết thúc (hiện tại là vô hạn, bạn có thể thêm điều kiện dừng)
    for (client_index = 0; client_index < MAX_CLIENTS; ++client_index) {
        pthread_join(threads[client_index], NULL);
    }

    // Đóng server socket
    close(server_socket);

    return 0;
}
