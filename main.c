#include "ls_core.h"
#include "ls_format.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    LsOptions opts = {0, 0, 0, 0}; // Khởi tạo các cờ bằng 0
    int opt;

    // Đọc các cờ từ dòng lệnh
    while ((opt = getopt(argc, argv, "alih")) != -1) {
        switch (opt) {
            case 'a': opts.show_all = 1; break;
            case 'l': opts.long_fmt = 1; break;
            case 'i': opts.show_inode = 1; break;
            case 'h': opts.human_read = 1; break;
            default:
                fprintf(stderr, "Sử dụng: %s [-alih] [thư mục]\n", argv[0]);
                return 1;
        }
    }

    // Nếu không có thư mục nào được chỉ định, dùng thư mục hiện tại "."
    if (optind == argc) {
        process_directory(".", &opts);
    } else {
        // Hỗ trợ truyền nhiều thư mục
        for (int i = optind; i < argc; i++) {
            if (argc - optind > 1) {
                printf("%s:\n", argv[i]);
            }
            process_directory(argv[i], &opts);
            if (i < argc - 1) printf("\n");
        }
    }

    return 0;
}