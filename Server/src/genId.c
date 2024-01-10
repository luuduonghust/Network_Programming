#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *genId(int gen_code) {
    // Số chữ số tối đa trong gen_code
    int max_digits = 5;

    // Tính số chữ số trong gen_code
    int num_digits = 0;
    int temp_gen_code = gen_code;
    while (temp_gen_code > 0) {
        temp_gen_code /= 10;
        num_digits++;
    }

    // Kiểm tra xem số chữ số có vượt quá max_digits không
    if (num_digits > max_digits) {
        fprintf(stderr, "Error: Gen code has too many digits.\n");
        return NULL;
    }

    // Tạo chuỗi kết quả với đủ số lượng ký tự '0' ban đầu
    char *result = (char *)malloc((max_digits + 1) * sizeof(char));
    if (result == NULL) {
        perror("Memory allocation error");
        return NULL;
    }
    memset(result, '0', max_digits);
    result[max_digits] = '\0'; // Kết thúc chuỗi

    // Chuyển gen_code thành chuỗi và sao chép vào vị trí phù hợp
    snprintf(result + (max_digits - num_digits), num_digits + 1, "%d", gen_code);

    return result;
}