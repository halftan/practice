#include "solve.h"

using glm::mat3;

void raytr::solve_quadratic(float a, float b, float c,
        raytr::quadRoot *out)
{
    float delta = b*b - 4*a*c;

    if (delta < 0) {
        out->real_root_count = 0;
        return;
    } else if (delta == 0) {
        out->real_root_count = 1;
        delta = sqrt(delta);
        out->ra = out->rb = -b / (2*a);
        return;
    } else {
        out->real_root_count = 2;
        delta = sqrt(delta);
        out->ra = (-b + delta) / (2*a);
        out->rb = (-b - delta) / (2*a);
        return;
    }
}


vec3 raytr::simul3(vec3 a, vec3 b, vec3 c, vec3 s)
{
    mat3 A(a,b,c);
    vec3 solve;
    float delta = glm::determinant(A);
    if (delta == 0)
        return vec3(-1);
    for (int i = 0; i < 3; ++i) {
        mat3 rep = A;
        rep[i] = s;
        solve[i] = glm::determinant(rep) / delta;
    }
    return solve;
}

vec2 raytr::simul2(vec2 a, vec2 b, vec2 s)
{
    float D = glm::determinant(mat2(a, b));
    if (D == 0)
        return vec2(-1);
    float Da = glm::determinant(mat2(s, b));
    float Db = glm::determinant(mat2(a, s));
    return vec2(Da/D, Db/D);
}
