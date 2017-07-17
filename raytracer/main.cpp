#include "main.h"

#include "scene.h"

#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        print_usage();
        exit(3);
    }
    raytr::Scene sce;
    sce.readfile(argv[1]);
    sce.render();
    return 0;
}

void print_usage(void)
{
    printf("Please specify a scene file.\n");
    printf("Usage: raytracer <scene_file> [output filename]\n");
}
