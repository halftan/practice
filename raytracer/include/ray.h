#ifndef RAY_H
#define RAY_H 1
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

namespace raytr
{
    class Ray
    {
    public:
        Ray(const vec3 &pos, const vec3 &dirn, float min, float max);
        Ray();
        ~Ray();

        Ray transform(mat4) const;
        vec3 position;
        vec3 dirn;
        float min, max;
    };
}
#endif
