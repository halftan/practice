#include "sampler.h"
#include "scene.h"

using namespace raytr;

Sampler::Sampler(const Scene &scene)
{
    this->initFromScene(scene);
}

void Sampler::initFromScene(const Scene &scene)
{
    this->current_x = 0;
    this->current_y = 0;
    this->max_x = scene.wsizew;
    this->max_y = scene.wsizeh;
}

bool Sampler::getSample(Sample *sample)
{
    if (current_x == max_x
            && current_y == max_y)
        return false;
    if (current_x == max_x - 1) {
        current_x = 0;
        ++ current_y;
    }
    sample->x = current_x + 0.5;
    sample->y = current_y + 0.5;
    return true;
}
