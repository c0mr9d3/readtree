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
    struct option long_options_args[] = {
        {"help", no_argument, 0, 'h'},
        {"target-dir", required_argument, 0, 't'},
        {0, 0, 0, 0}
    };

    if (argc == 1)
        help(argv[0]);

    while ( (options = getopt_long(argc, argv, "t:h", long_options_args, &option_index)) != EOF ) {
        switch (options) {
         case 'h':
             help(argv[0]);
         case 't':
             if ( !optarg )
                 help(argv[0]);
             target_dir = optarg;
             break;
        }
    }

    if ( !target_dir )
        help(argv[0]);

    inspect_directory(target_dir);
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
            "\n", program_name);
    exit(EXIT_FAILURE);
}
