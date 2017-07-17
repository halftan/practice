#ifndef INTERSECTION_H
#define INTERSECTION_H 1

#include "localgeo.h"

namespace raytr
{
    class Shape;

    class Intersection {
    public:
        Intersection() {}
        ~Intersection() {}

        LocalGeo localgeo;
        Shape    *shape;
    };
}
#endif
