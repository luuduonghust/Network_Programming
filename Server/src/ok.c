#include <stdio.h>
#include <string.h>

int main()
{
    char buffer_full[] = "login\n<fullname>\n<username>\n<pass>\r\n";

    // Kiểm tra xem chuỗi có bắt đầu bằng "login" hay không
    if (strncmp(buffer_full, "login", 5) == 0)
    {
        // Nếu có, bỏ qua "login" và ký tự '\n' kế tiếp
        char *remaining_data = strchr(buffer_full, '\n');
        if (remaining_data != NULL)
        {
            // Bỏ qua ký tự '\n'
            remaining_data++;

            // In ra phần còn lại của chuỗi
            printf("Remaining data:%s\n", remaining_data);
        }
    }
    else
    {
        printf("Chuỗi không bắt đầu bằng 'login'.\n");
    }

    return 0;
}