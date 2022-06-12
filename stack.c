#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

stack_value *push_dir_path(stack_value *stack, char *dir_path, size_t level)
{
    if ( dir_path == NULL ) {
        fprintf(stderr, "Error in push: dir_path is NULL");
        return NULL;
    }

    if ( stack == NULL ) {
        stack = (stack_value *) malloc(sizeof(stack_value));
        
        if ( stack == NULL ) {
            fprintf(stderr, "Error in push: cannot allocate memory for stack\n");
            return NULL;
        }

        stack->dir_path = dir_path;
        stack->level = level;
        stack->next = NULL;
    } else {
        stack->next = (stack_value *) malloc(sizeof(stack_value));
        
        if ( stack->next == NULL ) {
            fprintf(stderr, "Error in push: cannot allocate memory for stack next\n");
            return NULL;
        }

        stack = stack->next;
        stack->next = NULL;
        stack->dir_path = dir_path;
        stack->level = level;
    }

    return stack;
}

void clear_stack(stack_value *stack)
{
    if ( stack == NULL )
        return;

    stack_value *temp;
    temp = stack;

    while ( stack != NULL ) {
        free(stack->dir_path);
        temp = stack;
        stack = stack->next;
        free(temp);
    }
}
