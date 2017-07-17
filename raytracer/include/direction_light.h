#ifndef DIRECTION_LIGHT
#define DIRECTION_LIGHT 1
#include "light.h"

namespace raytr
{
    class DirectionLight : public Light
    {
    public:
        DirectionLight(vec3 dirn, Color color);
        ~DirectionLight();

        void generateLightRay(Intersection &inter,
                Ray *lRay, Color *lColor);
        Color calculateLightColor(float dist, Intersection &inter,
                Ray &eRay);
        bool isDirectional();

        vec3 dirn;
    };
}
#endif
