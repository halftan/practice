#ifndef FILM_H
#define FILM_H 1
#include <string>

#include "color.h"

namespace raytr
{
    class Scene;
    class Sample;

    class Film
    {
    public:
        Film(Scene *scene);
        ~Film();

        void commit(const Sample &sample, const Color &color);
        void savePNG();
        Color* at(int x, int y);
    private:
        Color *image;
        int width;
        int height;
        std::string ofilename;
        static const int bpp; // Bits per pixel
    };
}
#endif
