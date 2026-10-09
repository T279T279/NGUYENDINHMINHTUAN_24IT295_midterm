#include "ls_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Hàm so sánh dùng cho qsort (Sắp xếp theo bảng chữ cái)
static int cmp_names(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void process_directory(const char *dir_path, const LsOptions *opts) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("ls error");
        return;
    }

    char *entries[1024]; // Mảng lưu tên file tạm thời
    int count = 0;
    struct dirent *entry;

    // Đọc tên các file trong thư mục
    while ((entry = readdir(dir)) != NULL && count < 1024) {
        // Nếu không có cờ -a, bỏ qua file ẩn
        if (!opts->show_all && entry->d_name[0] == '.') {
            continue;
        }
        entries[count++] = strdup(entry->d_name);
    }
    closedir(dir);

    // Sắp xếp danh sách file
    qsort(entries, count, sizeof(char *), cmp_names);

    // In kết quả
    for (int i = 0; i < count; i++) {
        print_item_info(dir_path, entries[i], opts);
        free(entries[i]); // Giải phóng bộ nhớ chuỗi
    }
}