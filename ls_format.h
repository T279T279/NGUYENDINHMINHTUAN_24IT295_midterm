#ifndef LS_FORMAT_H
#define LS_FORMAT_H

#include <sys/stat.h>

// Các cờ cấu hình từ người dùng
typedef struct {
    int show_all;     // cờ -a
    int long_fmt;     // cờ -l
    int show_inode;   // cờ -i
    int human_read;   // cờ -h
} LsOptions;

// In thông tin chi tiết của 1 file
void print_item_info(const char *dir_path, const char *filename, const LsOptions *opts);

#endif