#ifndef SAMPLER_H
#define SAMPLER_H 1

#include <iostream>
#include <vector>

#include "sample.h"

namespace raytr
{
    class Scene; // Scene forward declaration.

    class Sampler
    {
    public:
        Sampler(const Scene &scene);
        ~Sampler() {}

        bool getSample(Sample *sample);
        void initFromScene(const Scene &scene);
    private:
        int max_x;
        int max_y;
        int current_x;
        int current_y;
    };
}
#endif
