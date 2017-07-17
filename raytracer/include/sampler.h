#ifndef SAMPLER_H
#define SAMPLER_H 1

#include <iostream>
#include <mutex>
#include <vector>

#include "sample.h"

namespace raytr
{
    class Scene; // Scene forward declaration.

    class Sampler
    {
    public:
        Sampler(Scene *scene);
        ~Sampler() {}

        bool getSample(Sample *sample);
        float progress();
    private:
        int max_x;
        int max_y;
        int current_x;
        int current_y;
        std::mutex lock;
    };
}
#endif
