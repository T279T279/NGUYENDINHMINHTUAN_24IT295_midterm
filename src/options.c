#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/my_ls.h"

void parse_options(int argc, char *argv[], Options *opts) {
    // Khởi tạo mặc định
    opts->print_question = isatty(STDOUT_FILENO) ? 1 : 0;
    opts->raw_print = !opts->print_question;

    int opt;
    while ((opt = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (opt) {
            case 'A': opts->show_almost_all = 1; break;
            case 'a': opts->show_all = 1; break;
            case 'c': opts->time_ctime = 1; opts->time_atime = 0; break;
            case 'u': opts->time_atime = 1; opts->time_ctime = 0; break;
            case 'd': opts->list_dir_plain = 1; opts->recursive = 0; break;
            case 'F': opts->classify = 1; break;
            case 'f': opts->unsorted = 1; opts->show_all = 1; break;
            case 'h': opts->human_readable = 1; opts->block_kb = 0; break;
            case 'i': opts->show_inode = 1; break;
            case 'k': opts->block_kb = 1; opts->human_readable = 0; break;
            case 'l': opts->long_format = 1; opts->numeric_id = 0; break;
            case 'n': opts->long_format = 1; opts->numeric_id = 1; break;
            case 'q': opts->print_question = 1; opts->raw_print = 0; break;
            case 'w': opts->raw_print = 1; opts->print_question = 0; break;
            case 'R': opts->recursive = 1; opts->list_dir_plain = 0; break;
            case 'r': opts->reverse_sort = 1; break;
            case 'S': opts->sort_size = 1; opts->sort_time = 0; break;
            case 's': opts->show_blocks = 1; break;
            case 't': opts->sort_time = 1; opts->sort_size = 0; break;
            default:
                fprintf(stderr, "Usage: %s [-AacdFfhiklnqRrSstuw] [file...]\n", argv[0]);
                exit(1);
        }
    }
}