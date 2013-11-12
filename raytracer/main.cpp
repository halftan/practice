#include "main.h"
#include "readfile.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage();
        exit(3);
    }
    return 0;
}

void print_usage(void) {
    printf("Please specify a scene file.\n");
}
