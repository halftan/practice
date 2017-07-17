#include "point_light.h"
#include "intersection.h"
#include "color.h"
#include "ray.h"
#include "shape.h"

#include <glm/glm.hpp>

using namespace raytr;
using namespace glm;

PointLight::PointLight(vec3 pos, Color color) :
    Light(color), pos(pos) {}

PointLight::~PointLight() {}

void PointLight::generateLightRay(Intersection &inter,
        Ray *lRay, Color *lColor)
{
    *lColor = color;
    lRay->position = inter.localgeo.position + Light::bias * inter.localgeo.normal;
    lRay->dirn = glm::normalize(pos - lRay->position);
}

Color PointLight::calculateLightColor(float dist, Intersection &inter, Ray &eRay)
{
    Color result = this->color / (
            Light::attenuation[0] + Light::attenuation[1] * dist +
            Light::attenuation[2] * dist * dist
            );
    vec3 position = glm::normalize(pos -
            inter.localgeo.position + Light::bias * inter.localgeo.normal);
    vec3 half_vec = glm::normalize(position - eRay.dirn);

    result *= inter.shape->material.diffuse * (MAX(
        glm::dot(inter.localgeo.normal, position), .0f
        )) + inter.shape->material.specular * glm::pow(MAX(
            glm::dot(inter.localgeo.normal, half_vec), .0f
            ), inter.shape->material.shininess);

    return result;
}

vec3 PointLight::getPosition()
{
    return this->pos;
}
