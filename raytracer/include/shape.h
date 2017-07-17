#ifndef SHAPE_H
#define SHAPE_H 1
#include "ray.h"
#include "material.h"
#include "intersection.h"

#include <glm/glm.hpp>

namespace raytr
{
    class Shape
    {
    public:
        virtual bool intersect(const Ray &ray, float *thit, Intersection *intersect)=0;
        virtual bool intersectT(const Ray &ray)=0;
        virtual ~Shape()=default;
        Shape() : transform(1) {}
        Material material;
        glm::mat4 transform;
    };
}
#endif
