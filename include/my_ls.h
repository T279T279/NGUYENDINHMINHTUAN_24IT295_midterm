#ifndef MY_LS_H
#define MY_LS_H

#include <sys/types.h>
#include <sys/stat.h>

// Định nghĩa 19 cờ
typedef struct {
    int show_all;         // -a
    int show_almost_all;  // -A
    int time_ctime;       // -c
    int time_atime;       // -u
    int list_dir_plain;   // -d
    int classify;         // -F
    int unsorted;         // -f
    int human_readable;   // -h
    int show_inode;       // -i
    int block_kb;         // -k
    int long_format;      // -l
    int numeric_id;       // -n
    int print_question;   // -q
    int raw_print;        // -w
    int recursive;        // -R
    int reverse_sort;     // -r
    int sort_size;        // -S
    int show_blocks;      // -s
    int sort_time;        // -t
} Options;

typedef struct {
    char *name;
    char *path;
    struct stat st;
} FileEntry;

// Khai báo hàm
void parse_options(int argc, char *argv[], Options *opts);
void print_file_info(const FileEntry *entry, const Options *opts);
void process_path(const char *path, const Options *opts, int is_command_arg);

#endif