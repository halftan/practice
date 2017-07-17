#include "scene.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "readfile.h"
#include "transform.h"

#include "sampler.h"
#include "camera.h"
#include "color.h"
#include "vertex.h"
#include "shape.h"
#include "sphere.h"
#include "tri.h"
#include "film.h"
#include "ray.h"
#include "raytracer.h"
#include "material.h"
#include "point_light.h"
#include "direction_light.h"

using namespace raytr;
using namespace std;

bool raytr::readvals(stringstream &s, const int numvals, double *values)
{
    for (int i = 0; i < numvals; ++i) {
        s >> values[i];
        if (s.fail()) {
            cerr << "Failed reading value " << i << " skips this line." << endl;
            return false;
        }
    }
    return true;
}

Scene::Scene() :
    objects(vector <Shape*>()),
    lights(vector <Light*>()),
    wsizew(100), wsizeh(100), maxdepth(5),
    ofilename("raytrace.png"), eyeinit(0.0,0.0,5.0),
    upinit(0.0,1.0,0.0), center(0.0,0.0,0.0), fovy(90.0)
{
    sampler  = nullptr;
    film     = nullptr;
    camera   = nullptr;
    vertices = nullptr;
}

Scene::~Scene()
{
    delete sampler;
    delete film;
    delete camera;
    delete raytracer;
    if (vertices != nullptr) {
        for (int i = 0; i < usedverts; ++i) {
            delete vertices[i];
        }
        delete [] vertices;
    }
    for (unsigned int i = 0; i < objects.size(); ++i) {
        delete objects[i];
    }

    for (unsigned int i = 0; i < lights.size(); ++i) {
        delete lights[i];
    }
}

bool Scene::readfile( const char *filename )
{
    string str, cmd;
    ifstream in;
    Material material;
    in.open(filename);
    if (in.is_open()) {

        stack<mat4> transfstack;
        transfstack.push(mat4(1.0));

        /* start reading command. */
        for (getline(in, str); in; getline(in, str)) {
            // cout << "Line: " << str << endl;
            if ((str.find_first_not_of(" \t\r\n") != string::npos)
                    && (str[0] != '#')) {

                /* ripped all comment and blank lines. */
                stringstream ss(str);
                ss >> cmd;
                double values[raytr::MAX_VAL];

                // General
                if (cmd == "size") {
                    if (readvals(ss, 2, values)) {
                        wsizew= values[0];
                        wsizeh= values[1];
                    }
                } else if (cmd == "maxdepth") {
                    if (readvals(ss, 1, values)) {
                        maxdepth = values[0];
                    }
                } else if (cmd == "output") {
                    ss >> ofilename;
                // Camera
                } else if (cmd == "camera") {
                    if (readvals(ss, 10, values)) {
                        for (int i = 0; i < 3; ++i) {
                            eyeinit[i] = values[i];
                            center[i] = values[3+i];
                            upinit[i] = values[6+i];
                        }
                        fovy = values[9];
                    }
                // Lights
                } else if (cmd == "directional") {
                    if (readvals(ss, 6, values)) {
                        DirectionLight *plight = new DirectionLight(
                                vec3(values[0], values[1], values[2]),
                                vec3(values[3], values[4], values[5])
                                );
                        lights.push_back(plight);
                    }
                } else if (cmd == "point") {
                    if (readvals(ss, 6, values)) {
                        PointLight *plight = new PointLight(
                                vec3(values[0], values[1], values[2]),
                                vec3(values[3], values[4], values[5])
                                );
                        lights.push_back(plight);
                    }
                } else if (cmd == "attenuation") {
                    if (readvals(ss, 3, values)) {
                        Light::attenuation = vec3(
                                values[0], values[1], values[2]
                                );
                    }
                } else if (cmd == "ambient") {
                    if (readvals(ss, 3, values)) {
                        material.ambient = Color(
                                values[0], values[1], values[2]
                                );
                    }
                // Materials
                } else if (cmd == "diffuse") {
                    if (readvals(ss, 3, values)) {
                        material.diffuse = Color(
                                values[0], values[1], values[2]
                                );
                    }
                } else if (cmd == "specular") {
                    if (readvals(ss, 3, values)) {
                        material.specular = Color(
                                values[0], values[1], values[2]
                                );
                    }
                } else if (cmd == "emission") {
                    if (readvals(ss, 3, values)) {
                        material.emission = Color(
                                values[0], values[1], values[2]
                                );
                    }
                } else if (cmd == "shininess") {
                    if (readvals(ss, 1, values)) {
                        material.shininess = values[0];
                    }
                // Geometry
                } else if (cmd == "sphere") {
                    if (readvals(ss, 4, values)) {
                        Sphere *sphere = new Sphere(vec3(
                                values[0], values[1], values[2]),
                                    values[3]
                                    );
                        sphere->material = material;
                        sphere->transform = transfstack.top();
                        objects.push_back(sphere);
                    }
                } else if (cmd == "tri") {
                    if (readvals(ss, 3, values)) {
                        Tri *tri = new Tri(
                                    vertices[(int)values[0]],
                                    vertices[(int)values[1]],
                                    vertices[(int)values[2]]
                                    );
                        tri->transform = transfstack.top();
                        tri->material = material;
                        objects.push_back(tri);
                    }
                } else if (cmd == "maxverts") {
                    if (readvals(ss, 1, values)) {
                        maxverts = values[0];
                        usedverts = 0;
                        vertices = new Vertex* [maxverts];
                    }
                } else if (cmd == "vertex") {
                    if (readvals(ss, 3, values)) {
                        if (usedverts < maxverts) {
                            vertices[usedverts++] =
                                new Vertex(values[0], values[1], values[2]);
                        } else {
                            cerr << "Exceeded maxverts." << endl;
                        }
                    }
                // Transformations
                } else if (cmd == "translate") {
                    if (readvals(ss, 3, values)) {
                        rightMultiply(raytr::translate(
                                    values[0], values[1], values[2]
                                    ), transfstack);
                    }
                } else if (cmd == "scale") {
                    if (readvals(ss, 3, values)) {
                        rightMultiply(raytr::scale(
                                    values[0], values[1],values[2]
                                    ), transfstack);
                    }
                } else if (cmd == "rotate") {
                    if (readvals(ss, 4, values)) {
                        vec3 axis;
                        for (int i = 0; i < 3; ++i) {
                            axis[i] = values[i];
                        }
                        rightMultiply(raytr::rotate4(values[3], axis),
                                transfstack);
                    }
                // push and pop transfstack
                } else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack is empty. Will ignore." << endl;
                    } else {
                        transfstack.pop();
                    }
                } else {
                    cerr << "Unknown primitive " << cmd << endl
                        << "Skipping...." << endl;
                    // exit(raytr::UNKNOWN_PRIM);
                }
            }
        }
    }

    return true;
}

bool Scene::render()
{
    sampler = new Sampler(this);
    film    = new Film(this);
    camera  = new Camera(this);
    raytracer = new Raytracer(this);

    int max_thread = thread::hardware_concurrency();
    std::thread *threads = new std::thread[max_thread];

    for (int i = 0; i < max_thread; ++i) {
        threads[i] = std::thread(&Scene::render_thread, this);
    }

    for (int i = 0; i < max_thread; ++i)
        threads[i].join();

    film->savePNG();

    delete []threads;

    delete sampler;
    sampler = nullptr;
    delete film;
    film = nullptr;
    delete camera;
    camera = nullptr;
    delete raytracer;
    raytracer = nullptr;

    return true;
}

void Scene::render_thread()
{
    Sample sample;
    Ray ray;

    // Main render loop
    while(sampler->getSample(&sample)) {
        Color color;
        camera->generateRay(sample, &ray);
        raytracer->trace(ray, &color);
        film->commit(sample, color);
    }
}
