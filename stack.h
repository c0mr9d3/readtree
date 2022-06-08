#ifndef _FILES_STACK
#define _FILES_STACK

typedef struct stack_struct {
    char *dir_path;
    size_t level;
    struct stack_struct *next;
} stack_value;

stack_value *push_dir_path(stack_value *stack, char *dir_path, size_t level);
stack_value *delete_first(stack_value *stack);
void clear_stack(stack_value *stack);

#endif
