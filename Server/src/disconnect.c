#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

void processDisconnect(char *message, int socket_fd) {
    char userId1[256];
    char userId2[256];

    if (sscanf(message, "\n%s\n%s\n\r", userId1, userId2) == 2) {
        // Tạo đường dẫn cho file cần tìm kiếm
        char filePath1[256];
        char filePath2[256];
        sprintf(filePath1, "../message_private/%s_%s.txt", userId1, userId2);
        sprintf(filePath2, "../message_private/%s_%s.txt", userId2, userId1);

        // Kiểm tra xem file tồn tại hay không
        //Nếu thành công thì trả về 0. Nếu có lỗi, trả về -1 
         if (remove(filePath1) == 0 || remove(filePath2) == 0) {

            send(socket_fd, "2015", sizeof("2015"), 0);
        } else {
            // File không tồn tại, gửi response "4015"
            send(socket_fd, "4015", sizeof("4015"), 0);
        }
    }
}
