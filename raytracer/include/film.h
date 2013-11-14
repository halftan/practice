#ifndef FILM_H
#define FILM_H 1
#include <string>

namespace raytr
{
    class Scene;
    class Sample;
    class Color;

    class Film
    {
    public:
        Film(const Scene &scene);
        ~Film();

        void initFromScene(const Scene &scene);

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
