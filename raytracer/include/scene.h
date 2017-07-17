#ifndef SCENE_H
#define SCENE_H 1

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <vector>

#include "main.h"

#ifndef MAXTHREAD
#define MAXTHREAD 4
#endif

using std::vector;

namespace raytr
{
    class Sampler;
    class Film;
    class Camera;
    class Raytracer;
    class Shape;
    class Vertex;
    class Light;

    class Scene
    {
    public:
        Scene();
        ~Scene();
        bool readfile(const char *filename);
        bool render();

        Sampler   *sampler;
        Film      *film;
        Camera    *camera;
        Raytracer *raytracer;

        Vertex **vertices;
        vector <Shape*> objects;
        vector <Light*> lights;

        int wsizew, wsizeh; // window size width & height
        int maxdepth;       // maximum depth for a ray. default to 5;
        std::string ofilename;

        vec3 eyeinit;
        vec3 upinit;
        vec3 center;
        float fovy;

    private:
        int maxverts;
        int usedverts;
        void render_thread();
    };

}
#endif
