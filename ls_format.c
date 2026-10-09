#include "ls_format.h"
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

// Bỏ qua thư viện Linux nếu đang chạy trên Windows
#ifndef _WIN32
#include <pwd.h>
#include <grp.h>
#endif

// Bổ sung các hằng số phân quyền bị thiếu trên trình biên dịch Windows (MinGW)
#ifndef S_IRUSR
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100
#endif

#ifndef S_IRGRP
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010
#endif

#ifndef S_IROTH
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001
#endif

// Hàm in quyền truy cập trực tiếp
static void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void print_item_info(const char *dir_path, const char *filename, const LsOptions *opts) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dir_path, filename);

    struct stat st;
    if (stat(path, &st) == -1) return; // Dùng stat thay lstat cho an toàn trên Windows

    // Cờ -i: In số inode
    if (opts->show_inode) {
        printf("%8lu ", (unsigned long)st.st_ino);
    }

    // Cờ -l: In định dạng dài
    if (opts->long_fmt) {
        print_permissions(st.st_mode);
        
        printf(" %2lu ", (unsigned long)st.st_nlink);

        // Xử lý lấy user/group tương thích đa nền tảng
#ifndef _WIN32
        struct passwd *pw = getpwuid(st.st_uid);
        struct group *gr = getgrgid(st.st_gid);
        printf("%s %s ", pw ? pw->pw_name : "user", gr ? gr->gr_name : "group");
#else
        // Trên Windows in thẳng mã ID dạng số do không có getpwuid
        printf("%lu %lu ", (unsigned long)st.st_uid, (unsigned long)st.st_gid);
#endif

        // Cờ -h: In kích thước dễ đọc
        if (opts->human_read) {
            double size = st.st_size;
            char unit = 'B';
            if (size >= 1024) { size /= 1024; unit = 'K'; }
            if (size >= 1024) { size /= 1024; unit = 'M'; }
            printf("%6.1f%c ", size, unit);
        } else {
            printf("%8lld ", (long long)st.st_size);
        }

        // Thời gian chỉnh sửa
        char time_buf[64];
        struct tm *tm_info = localtime(&st.st_mtime);
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);
        printf("%s ", time_buf);
    }

    // In tên file
    printf("%s\n", filename);
}