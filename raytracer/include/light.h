#ifndef LIGHT_H
#define LIGHT_H 1
#include <glm/glm.hpp>
#include "color.h"

using glm::vec3;

#define MAX(x,y) x > y ? x : y

namespace raytr
{
    class Intersection;
    class Ray;

    class Light
    {
    public:
        Light()=default;
        Light(Color color) : color(color) {}
        virtual ~Light()=default;

        virtual void generateLightRay(Intersection &inter,
                Ray *lRay, Color *lColor)=0;
        virtual Color calculateLightColor(float dist, Intersection &inter,
                Ray &eRay)=0;
        virtual bool isDirectional() {return false;}
        virtual vec3 getPosition() { return vec3(0); };

        static vec3 attenuation;
        static float bias;
        Color color;
    };
}
#endif
