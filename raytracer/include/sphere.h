#ifndef SPHERE_H
#define SPHERE_H 1
#include "shape.h"
#include <glm/glm.hpp>

using glm::vec3;

namespace raytr
{
    class Sphere : public Shape
    {
    public:
        Sphere(const vec3 &center, float radius);
        Sphere(float x, float y, float z, float radius);
        ~Sphere();

        virtual bool intersectT(const Ray &ray);
        virtual bool intersect(const Ray &ray, float *thit, Intersection *inter);

        vec3 center;
        float radius;
    };
}
#endif
