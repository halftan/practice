#ifndef ARGUMENTS
#define ARGUMENTS 1

typedef struct _arguments {
    char **argv;
    int argc;
    int _alloc;
} arguments;

arguments *init_args(arguments*);
arguments *add_arg(arguments*, char*);
void del_arg(arguments*);
void clear_arg(arguments*);
static void _arg_alloc(arguments*);
void print_arg(arguments*);

#define INIT_ARGC 5
#define STEP_ARGC 5

#endif
