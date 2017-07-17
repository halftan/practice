#include "ray.h"

using raytr::Ray;

Ray::Ray(const vec3 &pos, const vec3 &dirn, float min, float max) :
    position(pos), dirn(dirn), min(min), max(max) {}

Ray::Ray() {}
Ray::~Ray() {}

Ray Ray::transform(mat4 transf) const
{
    Ray ret;
    mat4 M = glm::inverse(transf);
    mat3 N(M);
    vec4 pos(position, 1.0);
    ret.position = vec3(M * pos);
    ret.dirn = N * dirn;

    return ret;
}
