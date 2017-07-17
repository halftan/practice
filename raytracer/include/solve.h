#ifndef SOLVE_H
#define SOLVE_H 1
#include <glm/glm.hpp>

using namespace glm;

namespace raytr
{
    struct quadRoot {
        float ra, rb;
        int real_root_count;
    };

    void solve_quadratic(float a, float b, float c, quadRoot *out);
    void reorder_vec(vec4 *invec);
    vec3 simul3(vec3 a, vec3 b, vec3 c, vec3 s);
    vec2 simul2(vec2 a, vec2 b, vec2 s);
}
#endif
