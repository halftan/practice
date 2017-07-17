#ifndef LOCALGEO_H
#define LOCALGEO_H 1
#include <glm/glm.hpp>

using glm::vec3;
using glm::normalize;

namespace raytr
{
    class LocalGeo
    {
    public:
        LocalGeo()=default;
        LocalGeo(const vec3 &pos, const vec3 &normal);
        ~LocalGeo();

        vec3 position;
        vec3 normal;
    };
}
#endif
