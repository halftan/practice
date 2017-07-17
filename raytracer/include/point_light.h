#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H 1
#include "light.h"

namespace raytr
{
    class PointLight : public Light
    {
    public:
        PointLight(vec3 pos, Color color);
        ~PointLight();

        void generateLightRay(Intersection &inter,
                Ray *lRay, Color *lColor);
        Color calculateLightColor(float dist, Intersection &inter,
                Ray &eRay);
        vec3 getPosition();

        vec3 pos;
    };
}
#endif
