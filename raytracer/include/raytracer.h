#ifndef RAYTRACER_H
#define RAYTRACER_H 1

#include "color.h"

namespace raytr
{
    class Ray;
    class Scene;

    class Raytracer
    {
    public:
        void trace(Ray &ray, Color *color, int depth=0);
        Raytracer(Scene *scene);
        ~Raytracer();

    private:
        Scene *scene;
        int maxdepth;
    };
}
#endif
