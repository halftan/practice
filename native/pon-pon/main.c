#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "main.h"

int main(int argc, char *argv[]) {
    init(argc, argv);
    update();
    draw();
    return 0;
}
