#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <error.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PATH_MAX 4096

typedef struct {
    uint32_t count;
    char name[256];
    bool off;
} filetype;

static char *path_alloc(size_t *);
static void recur_dir(char *, uint64_t);
static void help(char **);
static bool sizeof_file(uint64_t, uint64_t);
static void count_files(filetype *, uint8_t);
static char *fullpath;
static char *curdir;
static size_t pathlen;
static bool yes_size;
static uint64_t filesize;
static filetype if_dir = { .name = "\033[01;34mDirectory" }, if_block = { .name = "\033[01;33mBlock device" }, if_file = { .name = "\033[01;39mFile" }, 
                if_link = { .name = "\033[01;36mSymlink" }, if_char = { .name = "\033[01;32mChar device" }, if_fifo = { .name = "\033[01;30mFIFO" };

int main(int argc, char **argv)
{
    char options;
    if (argc == 1)
        help(argv);
    while ( (options = getopt(argc, argv, "s:bflrcdh")) != EOF ) {
        switch (options) {
         case 'h':
             help(argv);
         case 'b':
             if_block.off = true;
             break;
         case 'f':
             if_fifo.off = true;
             break;
         case 'l':
             if_link.off = true;
             break;
         case 'c':
             if_char.off = true;
             break;
         case 'r':
             if_file.off = true;
             break;
         case 'd':
             if_dir.off = true;
             break;
         case 's':
             if ( optarg && atoll(optarg) > 0 ) {
                 yes_size = true;
                 filesize = atoll(optarg);
             }
             else help(argv);
             break;
        }
    }

    if ( yes_size )
        recur_dir(argv[argc-1], filesize);
    else
        recur_dir(argv[argc-1], -1);
    filetype mm[6] = { if_dir, if_file, if_block, if_fifo, if_char, if_link };
    count_files(mm, 6);
    return 0;
}

static void recur_dir(char *directory, uint64_t sizefile)
{
    if ( chdir(directory) == -1 ) {
        perror("chdir");
        exit(-1);
    }
    DIR *tmpdir;
    struct stat statbuf, checkdir;
    struct dirent *rd;
    fullpath = path_alloc(&pathlen); 
    curdir = path_alloc(&pathlen);
    getcwd(fullpath, pathlen);
    if ((tmpdir = opendir(fullpath)) == NULL) {
        fprintf(stderr, "%s: ", fullpath);
        perror("opendir");
        exit(-1);
    }
    if ( lstat(fullpath, &checkdir) == 0 )
        if ( sizeof_file(checkdir.st_size, sizefile) == true && if_dir.off != true ) {
            printf("\033[01;34mdir -> %s\n", fullpath);
            if_dir.count++;
        }
    while ((rd = readdir(tmpdir)) != NULL) {
        if (strcmp(rd->d_name, ".") == 0 || strcmp(rd->d_name, "..") == 0) continue;
        if (lstat(rd->d_name, &statbuf) != 0) {
            fprintf(stderr, "%s: ", rd->d_name);
            perror("lstat");
            continue;
        } 
        getcwd(curdir, pathlen);
        switch (statbuf.st_mode & S_IFMT) {
            case S_IFBLK: 
                if ( sizeof_file(statbuf.st_size, sizefile) == true && if_block.off != true ) {
                    if_block.count++;
                    printf("\033[01;33mBlock device -> %s\n", rd->d_name);
                }
                break;
            case S_IFIFO: 
                if ( sizeof_file(statbuf.st_size, sizefile) == true && if_fifo.off != true ) {
                    if_fifo.count++;
                    printf("\033[01;30mFIFO -> %s/%s\n", curdir, rd->d_name);
                }
                break;
            case S_IFLNK: 
                if ( sizeof_file(statbuf.st_size, sizefile) == true && if_link.off != true ) {
                    if_link.count++;
                    printf("\033[01;36mSymlink -> %s/%s\n", curdir, rd->d_name); 
                }
                break;
            case S_IFREG: 
                if ( sizeof_file(statbuf.st_size, sizefile) == true && if_file.off != true ) {
                    if_file.count++;
                    printf("\033[01;39mFile -> %s/%s\n", curdir, rd->d_name); 
                }
                break;
            case S_IFCHR: 
                if ( sizeof_file(statbuf.st_size, sizefile) == true && if_char.off != true ) {
                    if_char.count++;
                    printf("\033[01;32mCharacter file -> %s/%s\n", curdir, rd->d_name); 
                }
                break;
            case S_IFDIR: 
                recur_dir(rd->d_name, sizefile);
                break;
            default:
                if ( sizeof_file(statbuf.st_size, sizefile) == true ) 
                    printf("\033[01;31mUnknown filetype -> %s/%s\n", curdir, rd->d_name);
                break;
        }
    }
    chdir("..");
    closedir(tmpdir);
}

static bool sizeof_file(uint64_t filesize, uint64_t check_size)
{
    if ( filesize <= check_size || check_size < 0 )
        return true;
    else
        return false;
}

static void count_files(filetype *mass, uint8_t sz_mass)
{
    printf("\033[0m--------------------------------------------------------\n");
    for (int i=0; i < sz_mass; i++) {
       if ( mass[i].count != 0 )
          printf("%s = %d\n", mass[i].name, mass[i].count); 
    }
    printf("\033[0m");
}

static void help(char **argv)
{
    printf(
           "Usage: %s [options] <dir>\n\n"
           "Options:\n"
           "\t-h help\n"
           "\t-s <size> ( > 0) size of file\n"
           "\t-f exclude FIFO\n"
           "\t-d exclude directory\n"
           "\t-b exclude block device\n"
           "\t-l exclude symlink\n"
           "\t-r exclude regular file\n"
           "\t-c exclude character device\n"
           "\n", argv[0]);
    exit(1);
}

static char *path_alloc(size_t *size)
{
    char *ptr;
    if ((ptr = malloc(PATH_MAX + 1)) == NULL) {
        perror("malloc");
        exit(-1);
    }
    if (size != NULL)
        *size = PATH_MAX + 1;
    return ptr;
}
