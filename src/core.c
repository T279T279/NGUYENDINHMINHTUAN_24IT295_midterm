#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../include/my_ls.h"

// Biến toàn cục tạm để hàm qsort dùng options
static const Options *g_opts = NULL;

static int compare_entries(const void *a, const void *b) {
    const FileEntry *fa = (const FileEntry *)a;
    const FileEntry *fb = (const FileEntry *)b;
    int res = 0;

    if (g_opts->sort_size) {
        res = (fa->st.st_size < fb->st.st_size) ? 1 : -1;
    } else if (g_opts->sort_time) {
        time_t t_a = g_opts->time_atime ? fa->st.st_atime : (g_opts->time_ctime ? fa->st.st_ctime : fa->st.st_mtime);
        time_t t_b = g_opts->time_atime ? fb->st.st_atime : (g_opts->time_ctime ? fb->st.st_ctime : fb->st.st_mtime);
        res = (t_a < t_b) ? 1 : -1;
    } else {
        res = strcmp(fa->name, fb->name);
    }
    return g_opts->reverse_sort ? -res : res;
}

void process_path(const char *path, const Options *opts, int is_command_arg) {
    struct stat st;
    if (lstat(path, &st) != 0) {
        perror("my_ls: stat error");
        return;
    }

    // Nếu là file, hoặc có cờ -d thì in ra rồi thoát
    if (!S_ISDIR(st.st_mode) || opts->list_dir_plain) {
        FileEntry fe;
        fe.name = (char*)path; // In trực tiếp đường dẫn
        fe.st = st;
        print_file_info(&fe, opts);
        return;
    }

    // Xử lý thư mục
    DIR *dir = opendir(path);
    if (!dir) { perror("my_ls: opendir error"); return; }

    if (is_command_arg || opts->recursive) printf("\n%s:\n", path);

    FileEntry entries[1024];
    int count = 0;
    struct dirent *ent;

    while ((ent = readdir(dir)) != NULL && count < 1024) {
        if (!opts->show_all && ent->d_name[0] == '.') {
            if (!opts->show_almost_all || strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                continue;
        }

        entries[count].name = strdup(ent->d_name);
        char fullpath[2048];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, ent->d_name);
        entries[count].path = strdup(fullpath);
        lstat(fullpath, &entries[count].st);
        count++;
    }
    closedir(dir);

    if (!opts->unsorted) {
        g_opts = opts;
        qsort(entries, count, sizeof(FileEntry), compare_entries);
    }

    for (int i = 0; i < count; i++) {
        print_file_info(&entries[i], opts);
    }

    // Xử lý cờ đệ quy (-R)
    if (opts->recursive) {
        for (int i = 0; i < count; i++) {
            if (S_ISDIR(entries[i].st.st_mode) && strcmp(entries[i].name, ".") != 0 && strcmp(entries[i].name, "..") != 0) {
                process_path(entries[i].path, opts, 0);
            }
        }
    }

    for (int i = 0; i < count; i++) { free(entries[i].name); free(entries[i].path); }
}