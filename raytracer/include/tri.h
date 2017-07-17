#ifndef TRI_H
#define TRI_H 1

#include "shape.h"
#include "intersection.h"
#include <glm/glm.hpp>

using glm::vec3;

namespace raytr
{
    class Vertex;
    class Tri : public Shape
    {
    public:
        Tri(Vertex *a, Vertex *b, Vertex *c);
        ~Tri();

        virtual bool intersect(const Ray &ray, float *thit, Intersection *inter);
        virtual bool intersectT(const Ray &ray);

        Vertex *verts[3]; // vertices are stored in counter-clockwise order
        vec3 normal;

        vec3 A();
        vec3 B();
        vec3 C();
    };
}
#endif
