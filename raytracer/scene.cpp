#include "scene.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "readfile.h"
#include "transform.h"

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
    wsizew(100), wsizeh(100), maxdepth(5),
    ofilename("raytrace.png"), eyeinit(0.0,0.0,5.0),
    upinit(0.0,1.0,0.0), center(0.0,0.0,0.0), fovy(90.0)
{
    sampler = nullptr;
    film = nullptr;
}

Scene::~Scene()
{
    if (sampler != nullptr)
        delete sampler;
    if (film != nullptr)
        delete film;
}

bool Scene::readfile( const char *filename )
{
    string str, cmd;
    ifstream in;
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
                } else if (cmd == "") {
                    if (readvals(ss, 0, values)) {
                        /* code */
                    }
                // Materials
                } else if (cmd == "") {
                    if (readvals(ss, 0, values)) {
                        /* code */
                    }
                // Geometry
                } else if (cmd == "sphere") {
                    if (readvals(ss, 0, values)) {
                        /* code */
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
    sampler = new Sampler(*this);
    film = new Film(*this);
    cout << "The size of the output image is:" << endl << "Width: "
        << this->wsizew << "\tHeight: " << this->wsizeh << endl;
    film->savePNG();
    return true;
}

