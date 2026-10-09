#include <stdio.h>
#include <unistd.h>
#include "../include/my_ls.h"

extern int optind; // Khai báo biến của getopt()

int main(int argc, char *argv[]) {
    Options opts = {0};
    parse_options(argc, argv, &opts);

    // Xử lý các đường dẫn (operands) được truyền vào
    if (optind == argc) {
        process_path(".", &opts, 0);
    } else {
        for (int i = optind; i < argc; i++) {
            process_path(argv[i], &opts, argc - optind > 1);
        }
    }

    return 0;
}