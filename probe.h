#ifndef _PROBE_DIR
#define _PROBE_DIR

#define N_FILETYPES 6
#define PATH_MAX 4096

typedef struct {
    size_t count;
    char *type_name;
} filetype;

enum {dir_iter, block_iter, file_iter, symlink_iter, char_iter, fifo_iter};

char *path_alloc(void);
int inspect_directory(char *);

#endif
