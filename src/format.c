#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <stdint.h>
#include "../include/my_ls.h"

// Tính số blocks (tính theo KB cho -k, hoặc mặc định 512 bytes)
static long long get_blocks(const struct stat *st, const Options *opts) {
    long long blk = st->st_blocks;
    if (opts->block_kb) return (blk * 512 + 1023) / 1024;
    return blk; 
}

void print_file_info(const FileEntry *entry, const Options *opts) {
    if (opts->show_inode) {
        printf("%7llu ", (unsigned long long)entry->st.st_ino);
    }
    if (opts->show_blocks) {
        printf("%4lld ", get_blocks(&entry->st, opts));
    }
    if (opts->long_format) {
        // Quyền truy cập
        char mode[] = "----------";
        mode_t m = entry->st.st_mode;
        if (S_ISDIR(m)) mode[0] = 'd';
        else if (S_ISLNK(m)) mode[0] = 'l';
        
        if (m & S_IRUSR) mode[1] = 'r'; if (m & S_IWUSR) mode[2] = 'w'; if (m & S_IXUSR) mode[3] = 'x';
        if (m & S_IRGRP) mode[4] = 'r'; if (m & S_IWGRP) mode[5] = 'w'; if (m & S_IXGRP) mode[6] = 'x';
        if (m & S_IROTH) mode[7] = 'r'; if (m & S_IWOTH) mode[8] = 'w'; if (m & S_IXOTH) mode[9] = 'x';
        
        printf("%s %2lu ", mode, (unsigned long)entry->st.st_nlink);

        // User & Group (-n hiển thị dạng số)
        if (opts->numeric_id) {
            printf("%u %u ", entry->st.st_uid, entry->st.st_gid);
        } else {
            struct passwd *pw = getpwuid(entry->st.st_uid);
            struct group *gr = getgrgid(entry->st.st_gid);
            printf("%-8s %-8s ", pw ? pw->pw_name : "???", gr ? gr->gr_name : "???");
        }

        // Kích thước (có cờ -h)
        if (opts->human_readable) {
            double size = entry->st.st_size;
            char *units[] = {"B", "K", "M", "G"};
            int u = 0;
            while (size >= 1024 && u < 3) { size /= 1024; u++; }
            printf("%6.1f%s ", size, units[u]);
        } else {
            printf("%8lld ", (long long)entry->st.st_size);
        }

        // Thời gian (-c, -u, -t)
        time_t t = entry->st.st_mtime;
        if (opts->time_ctime) t = entry->st.st_ctime;
        if (opts->time_atime) t = entry->st.st_atime;
        char timebuf[32];
        struct tm *tm_info = localtime(&t);
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
        printf("%s ", timebuf);
    }

    // Tên file (-q thay ký tự lạ bằng dấu ?)
    for (int i = 0; entry->name[i] != '\0'; i++) {
        unsigned char c = entry->name[i];
        if (opts->print_question && !isprint(c)) putchar('?');
        else putchar(c);
    }

    // Ký hiệu loại file (-F)
    if (opts->classify) {
        mode_t m = entry->st.st_mode;
        if (S_ISDIR(m)) putchar('/');
        else if (S_ISLNK(m)) putchar('@');
        else if (m & (S_IXUSR | S_IXGRP | S_IXOTH)) putchar('*');
    }
    printf("\n");
}