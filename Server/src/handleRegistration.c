#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct UserInfo {
    char fullname[256];
    char username[256];
    char password[256];
    int gen_id;
};
// Lưu thông tin người dùng
int save(char* fullname, char* username, char* passsword, int *gen_id);
// Kiểm tra người dùng có tồn tại hay không
int check(struct UserInfo user);
// Lưu giá trị gen_id vào tệp
void saveGenId(int gen_id);
int readGenId();
// Xử lý đăng ký người dùng
char *genId(int gen_code);
void handleRegistration(char *message, int *gen_id, int socket_fd){
    // Khai báo biến. Khi server đọc message đầu tiên thì sẽ truyền vào các module tương ứng
    char* fullname;
    char* username;
    char* password;
    // Phân tích thông điệp
    if (sscanf(message, "%s\n%s\n%s\r\n", fullname, username, password) == 3)
    {
        // Đọc giá trị gen_id từ tệp khi khởi động chương trình
        *gen_id = readGenId();
        // Gọi hàm lưu thông tin người dùng
       int registrationResult = save(fullname, username, password, gen_id);
        // Kiểm tra kết quả đăng ký
       if (registrationResult == 0)
       {
    
        // Kiểm tra xem người dùng đã đăng ký có tồn tại trong cơ sở dữ liệu hay không
        struct UserInfo newUser;
        strcpy(newUser.username, username);
        if (check(newUser))
        {
             // Gửi mã phản hồi 201 nếu thành công
            send(socket_fd, "2001", sizeof("2001"), 0);
        }
       }else
       {
        // Gửi mã 401 nếu không thành công
        send(socket_fd, "4001", sizeof("4001"), 0 );
       }
    }
    
}

int save(char *fullname, char *username, char *password, int *gen_id){
    // Mở file cơ sở dữ liệu để ghi
    FILE *dbFile = fopen("../database/account.txt", "a");
    if (dbFile == NULL) {
        perror("Error opening database file");
        return -1; 
    }
    // Tăng id lên 1 đơn vị với 1 tài khoản đăng ký thành công
    char genID[10];
    strcpy(genID, genId(*gen_id));
    (*gen_id)++;
     // Ghi thông tin người dùng vào file
    fprintf(dbFile, "%s %s %s %s  1\n", fullname, username, password, genID);
     // Lưu giá trị gen_id vào tệp
    saveGenId(*gen_id);
    fclose(dbFile);
    return 0; 
}
int check(struct UserInfo user){
    FILE *file = fopen("../database/account.txt", "r"); 
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
     // Khai báo biến tạm thời để lưu thông tin người dùng từ file
    struct UserInfo temp;
    while (fscanf(file, "%s %s %s ", temp.fullname, temp.username, temp.password, temp.gen_id) != EOF)
    {
     // So sánh username của người dùng với username trong file
      if (strcmp(user.username, temp.username) == 0)
      {
        // Nếu tìm thấy, đóng file và trả về 1
          fclose(file);
            return 1; 
      }
      
    }
     fclose(file);
      return 0; 

}
// Lưu giá trị gen_id vào tệp
void saveGenId(int gen_id) {
    FILE *genId = fopen("../database/gen_id.txt", "w");//Chế độ w trong fopen sẽ ghi đè 
    if (genId != NULL) {
        fprintf(genId, "%d", gen_id);
        fclose(genId);
    }
}
// Đọc giá trị gen_id từ tệp
int readGenId() {
    int gen_id;
    FILE *genId = fopen("../database/gen_id.txt", "r"); 
    if (genId != NULL) {
        fscanf(genId, "%d", &gen_id);
        fclose(genId);
    }
    return gen_id;
}
