#ifndef _PROBE_DIR
#define _PROBE_DIR

#define N_FILETYPES 6
#define PATH_MAX 4096
#define CHECK_FILE_SIZE(filesize, max_size) \
    (filesize <= max_size || max_size < 0) ? true : false

typedef struct {
    size_t count;
    char *type_name;
    bool off;
} filetype;

typedef struct {
    char directory:1;
    char block:1;
    char file:1;
    char symlink:1;
    char character:1;
    char fifo:1;
} file_types;

enum {dir_iter, block_iter, file_iter, symlink_iter, char_iter, fifo_iter};

char *path_alloc(void);
int inspect_directory(char *, ssize_t, file_types);

#endif
