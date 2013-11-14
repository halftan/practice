#ifndef SCENE_H
#define SCENE_H 1

#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "main.h"
#include "sampler.h"
#include "film.h"

namespace raytr
{
    class Scene
    {
    public:
        Scene();
        ~Scene();
        bool readfile(const char *filename);
        bool render();

        friend void Sampler::initFromScene(const Scene &scene);
        friend void Film::initFromScene(const Scene &scene);

    private:
        Sampler *sampler;
        Film *film;

        int wsizew, wsizeh; // window size width & height
        int maxdepth;       // maximum depth for a ray. default to 5;
        std::string ofilename;

        vec3 eyeinit;
        vec3 upinit;
        vec3 center;
        float fovy;
    };

}
#endif
