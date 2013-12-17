#include "err_handler.h"

int exit_error(int errno, int exitno) {
    fprintf(stderr, "%s\n", strerror(errno));
    exit(exitno);
}
