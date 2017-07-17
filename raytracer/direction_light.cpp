#include "direction_light.h"
#include "intersection.h"
#include "color.h"
#include "ray.h"
#include "shape.h"

#include <glm/glm.hpp>

using namespace raytr;
using namespace glm;

DirectionLight::DirectionLight(vec3 dirn, Color color) :
    Light(color), dirn(glm::normalize(dirn)) {}

DirectionLight::~DirectionLight() {}

void DirectionLight::generateLightRay(Intersection &inter,
        Ray *lRay, Color *lColor)
{
    *lColor = this->color;
    lRay->position = inter.localgeo.position + Light::bias * inter.localgeo.normal;
    lRay->dirn = this->dirn;
}

Color DirectionLight::calculateLightColor(float dist, Intersection &inter, Ray &eRay)
{
    Color result = this->color;
    vec3 half_vec = glm::normalize(this->dirn - eRay.dirn);

    result *= inter.shape->material.diffuse * (MAX(
        glm::dot(inter.localgeo.normal, this->dirn), .0f
        )) + inter.shape->material.specular * glm::pow(MAX(
            glm::dot(inter.localgeo.normal, half_vec), .0f
            ), inter.shape->material.shininess);

    return result;
}

bool DirectionLight::isDirectional()
{
    return true;
}
