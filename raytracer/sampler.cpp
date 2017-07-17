#include "sampler.h"
#include "scene.h"

#include <cstdio>

using namespace raytr;

Sampler::Sampler(Scene *scene)
{
    this->current_x = 0;
    this->current_y = 0;
    this->max_x     = scene->wsizew;
    this->max_y     = scene->wsizeh;
}

bool Sampler::getSample(Sample *sample)
{
    this->lock.lock();
    if (current_x == max_x - 1
            && current_y == max_y - 1) {
        this->lock.unlock();
        return false;
    }
    if (current_x == max_x - 1) {
        current_x = 0;
        ++ current_y;
        printf("Progress: %-2.3f\n", progress()*100.0);
    }
    sample->x  = current_x + 0.5;
    sample->y  = current_y + 0.5;
    sample->ix = current_x;
    sample->iy = current_y;
    ++ current_x;
    this->lock.unlock();

    return true;
}

float Sampler::progress()
{
    return (float) current_y / (float) max_y;
}
