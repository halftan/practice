#ifndef MATERIAL_H
#define MATERIAL_H 1
#include <glm/glm.hpp>

#include "color.h"

using glm::vec3;

namespace raytr
{
    class Material {
    public:
        Material() : diffuse(0), specular(0), ambient(0),
                emission(0), attenuation(1,0,0), shininess(1) {}
        Material(Color diffuse, Color specular, Color ambient) :
            diffuse(diffuse), specular(specular),
            ambient(ambient) {}
        ~Material() = default;

        Color diffuse;
        Color specular;
        Color ambient;
        Color emission;
        vec3 attenuation;
        float shininess;
    };
}
#endif
