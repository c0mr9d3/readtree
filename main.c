#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#include "stack.h"
#include "probe.h"

static void help(char *);

int main(int argc, char **argv)
{

    char options, *target_dir = NULL;
    int option_index = 0;
    ssize_t filesize = -1;
    file_types off_file_types;
    struct option long_options_args[] = {
        {"help", no_argument, 0, 'h'},
        {"target-dir", required_argument, 0, 't'},
        {"directory", no_argument, 0, 'd'},
        {"block", no_argument, 0, 'b'},
        {"file", no_argument, 0, 'r'},
        {"symlink", no_argument, 0, 'l'},
        {"char", no_argument, 0, 'c'},
        {"fifo", no_argument, 0, 'f'},
        {0, 0, 0, 0}
    };

    memset(&off_file_types, 0, sizeof(file_types));

    if (argc == 1)
        help(argv[0]);

    while ( (options = getopt_long(argc, argv, "t:s:bflrcdh", long_options_args, &option_index)) != EOF ) {
        switch (options) {
         case 'h':
             help(argv[0]);
         case 't':
             if ( !optarg )
                 help(argv[0]);
             target_dir = optarg;
             break;
         case 'd':
             off_file_types.directory = true;
             break;
         case 'b':
             off_file_types.block = true;
             break;
         case 'r':
             off_file_types.file = true;
             break;
         case 'l':
             off_file_types.symlink = true;
             break;
         case 'c':
             off_file_types.character = true;
             break;
         case 'f':
             off_file_types.fifo = true;
             break;
         case 's':
             if ( optarg && atoll(optarg) > 0 )
                 filesize = atoll(optarg);
             else
                 help(argv[0]);
             break;
        }
    }

    if ( !target_dir )
        help(argv[0]);

    inspect_directory(target_dir, filesize, off_file_types);
    return 0;
}

static void help(char *program_name)
{
    if ( program_name == NULL ) {
        fprintf(stderr, "Program name is NULL\n");
        exit(EXIT_FAILURE);
    }

    printf(
           "Usage: %s [options] -t <dir>\n\n"
           "Options:\n"
           "\t-t or --target-dir: start directory\n"
           "\t-h or --help: help\n"
           "\t-s <size> ( > 0) size of file\n"
           "\t-f or --fifo: exclude FIFO\n"
           "\t-d or --directory: exclude directory\n"
           "\t-b or --block: exclude block device\n"
           "\t-l or --symlink: exclude symlink\n"
           "\t-r or --file: exclude regular file\n"
           "\t-c or --char: exclude character device\n"
           "\n", program_name);
    exit(EXIT_FAILURE);
}
