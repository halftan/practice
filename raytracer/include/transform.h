#ifndef TRANSFORM_H
#define TRANSFORM_H 1
#include <glm/glm.hpp>
#include <stack>

using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec4;

using std::stack;

namespace raytr
{
    mat4 lookAt(const vec3 &eye, const vec3 &center, const vec3 &up);
    mat4 perspective(float fovy, float aspect, float zNear, float zFar);
    vec3 upvector(const vec3 &up, const vec3 &zvec);

    mat4 translate(const float &tx, const float &ty, const float &tz);
    mat4 scale(const float &sx, const float &sy, const float &sz);
    mat3 rotate3(const float degress, const vec3 &axis);
    mat4 rotate4(const float degrees, const vec3 &axis);

    void rightMultiply(const mat4 &M, stack<mat4> &transfstack);
}
#endif
