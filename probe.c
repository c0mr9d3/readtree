#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "probe.h"
#include "stack.h"

char *path_alloc(void)
{
    char *file_path;

    if ( (file_path = malloc(PATH_MAX + 1)) == NULL ) {
        perror("malloc");
        return NULL;
    }

    return file_path;
}

int inspect_directory(char *dir_name, ssize_t file_size, file_types disable_count)
{
    if ( dir_name == NULL ) {
        fprintf(stderr, "inspect_directory: dir_name is NULL\n");
        return -1;
    }

    if ( chdir(dir_name) < 0 ) {
        perror("chdir");
        return errno;
    }

    DIR *current_directory;
    struct stat statbuf, checkdir;
    struct dirent *file_info;
    int file_level = 1, i;
    char *full_path_cur_dir, *full_path_next_dir;
    char *target_file;
    filetype **filetypes_array;

    filetypes_array = malloc(sizeof(filetype *) * N_FILETYPES);
    full_path_cur_dir = path_alloc();
    target_file = path_alloc();

    if ( !filetypes_array || !full_path_cur_dir || !target_file ) {
        perror("malloc");
        return errno;
    }

    for (i = 0; i < N_FILETYPES; i++) {
        filetypes_array[i] = (filetype *) malloc(sizeof(filetype));
        filetypes_array[i]->type_name = (char *) malloc(100);
        filetypes_array[i]->count = 0;
    }

    sprintf(filetypes_array[dir_iter]->type_name, "\033[01;34mDirectory\033[0m");
    sprintf(filetypes_array[block_iter]->type_name, "\033[01;33mBlock device\033[0m");
    sprintf(filetypes_array[file_iter]->type_name, "\033[01;39mFile\033[0m");
    sprintf(filetypes_array[symlink_iter]->type_name, "\033[01;36mSymlink\033[0m");
    sprintf(filetypes_array[char_iter]->type_name, "\033[01;32mChar device\033[0m");
    sprintf(filetypes_array[fifo_iter]->type_name, "\033[01;30mFIFO\033[0m");

    if ( full_path_cur_dir == NULL ) {
        fprintf(stderr, "inspect_directory: path_alloc is NULL\n");
        return -3;
    }
    
    if ( getcwd(full_path_cur_dir, PATH_MAX) == NULL ) {
        perror("getcwd");
        free(full_path_cur_dir);
        return errno;
    }

    if ( (current_directory = opendir(full_path_cur_dir)) == NULL ) {
        perror("opendir");
        free(full_path_cur_dir);
        return errno;
    }

    if ( lstat(full_path_cur_dir, &checkdir) == 0 ) {
        if ( CHECK_FILE_SIZE(checkdir.st_size, file_size) )
            printf("\033[01;34mStart directory -> %s:\n", full_path_cur_dir);
    } else {
        perror("lstat");
        free(full_path_cur_dir);
        closedir(current_directory);
        return errno;
    }

    stack_value *next_dir_stack = NULL, *last_dir_stack, *tmp_stack;
    last_dir_stack = next_dir_stack;

    while ( (file_info = readdir(current_directory)) != NULL || 
            next_dir_stack != NULL ) {

        if ( file_info == NULL ) {
            closedir(current_directory);
            current_directory = opendir(next_dir_stack->dir_path);

            if ( !current_directory ) {
                perror("opendir");
                clear_stack(next_dir_stack);
                break;
            }

            file_level = next_dir_stack->level;
            free(full_path_cur_dir);
            full_path_cur_dir = next_dir_stack->dir_path;
            tmp_stack = next_dir_stack;
            next_dir_stack = next_dir_stack->next;
            free(tmp_stack);
            printf("\033[01;34m%s (level %d):\n", full_path_cur_dir, file_level);
            continue;
        }
        
        if ( strcmp(file_info->d_name, ".") == 0 || 
             strcmp(file_info->d_name, "..") == 0 )
            continue;

        if ( chdir(full_path_cur_dir) < 0 )
            continue;

        if ( lstat(file_info->d_name, &statbuf) != 0 ) {
            fprintf(stderr, "%s: ", file_info->d_name);
            perror("lstat");
            continue;
        }

        printf("\033[0m|");
        for (i = 0; i < file_level; i++) {
            printf("-");
        }

        switch ( statbuf.st_mode & S_IFMT ) {
            case S_IFBLK:
                if ( CHECK_FILE_SIZE(statbuf.st_size, file_size) && !disable_count.block ) {
                    filetypes_array[block_iter]->count++;
                    printf("\033[01;33m%s (Block)\n", file_info->d_name);
                }
                break;

            case S_IFIFO:
                if ( CHECK_FILE_SIZE(statbuf.st_size, file_size) && !disable_count.fifo ) {
                    filetypes_array[fifo_iter]->count++;
                    printf("\033[01;30m%s (FIFO)\n", file_info->d_name);
                }
                break;

            case S_IFLNK:
                if ( CHECK_FILE_SIZE(statbuf.st_size, file_size) && !disable_count.symlink ) {
                    filetypes_array[symlink_iter]->count++;
                    printf("\033[01;36m%s (Symlink)\n", file_info->d_name);
                }
                break;

            case S_IFREG:
                if ( CHECK_FILE_SIZE(statbuf.st_size, file_size) && !disable_count.file ) {
                    filetypes_array[file_iter]->count++;
                    printf("\033[01;39m%s (File)\n", file_info->d_name);
                }
                break;

            case S_IFCHR:
                if ( CHECK_FILE_SIZE(statbuf.st_size, file_size) && !disable_count.character ) {
                    filetypes_array[char_iter]->count++;
                    printf("\033[01;32m%s (Char)\n", file_info->d_name);
                }
                break;

            case S_IFDIR:
                filetypes_array[dir_iter]->count++;
                printf("\033[01;34m%s (Directory)\n", file_info->d_name);
                full_path_next_dir = path_alloc();
                snprintf(full_path_next_dir, PATH_MAX-1, "%s/%s", full_path_cur_dir, file_info->d_name);
                last_dir_stack = push_dir_path(last_dir_stack, full_path_next_dir, file_level+1);

                if ( next_dir_stack == NULL )
                    next_dir_stack = last_dir_stack;

                break;

            default:
                if ( CHECK_FILE_SIZE(statbuf.st_size, file_size) )
                    printf("\033[01;31m%s (Unknown type)\n", file_info->d_name);
                break;
        }

    }
    printf("\033[0m`-----------------------------------------------------------\n");
    closedir(current_directory);
    free(full_path_cur_dir);
    free(target_file);

    for (i = 0; i < N_FILETYPES; i++) {
        printf("%s: %zu\n", filetypes_array[i]->type_name, filetypes_array[i]->count);
        free(filetypes_array[i]->type_name);
        free(filetypes_array[i]);
    }

    free(filetypes_array);

    return 0;
}
