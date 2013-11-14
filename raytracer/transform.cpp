#include <glm/glm.hpp>

#include "transform.h"

using namespace raytr;

mat4 lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
    vec3 w,u,v,t;
    w = eye / glm::length(eye);
    t = glm::cross(up, w);
    u = t / glm::length(t);
    v = glm::cross(w, u);

    mat4 ret(
        u[0], v[0], w[0], 0,
        u[1], v[1], w[1], 0,
        u[2], v[2], w[2], 0,
        -u[0]*eye[0] - u[1]*eye[1] - u[2]*eye[2],
        -v[0]*eye[0] - v[1]*eye[1] - v[2]*eye[2],
        -w[0]*eye[0] - w[1]*eye[1] - w[2]*eye[2],
        1);
    return ret;
}

mat4 perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    float theta = fovy / 2;
    float d = 1 / glm::tan(glm::radians(theta));
    float A = - (zFar + zNear) / (zFar - zNear);
    float B = - (2 * zFar * zNear) / (zFar - zNear);
    ret = mat4(
        d/aspect,0,0,0,
        0,d,0,0,
        0,0,A,-1,
        0,0,B,0
        );
    return ret;
}

mat4 scale(const float &sx, const float &sy, const float &sz)
{
    mat4 ret;
    ret = mat4(
        sx,0,0,0,
        0,sy,0,0,
        0,0,sz,0,
        0,0,0,1
        );
    return ret;
}

mat4 translate(const float &tx, const float &ty, const float &tz)
{
    mat4 ret;
    ret = mat4(
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        tx,ty,tz,1
        );
    return ret;
}

mat3 raytr::rotate3(const float degrees, const vec3 &axis)
{
    float radian     = glm::radians(degrees);
    float cosine_val = cos(radian);
    float sine_val   = sin(radian);
    vec3 _axis = glm::normalize(axis);
    float ux = _axis[0];
    float uy = _axis[1];
    float uz = _axis[2];
    mat3 ret;
    mat3 I(1);
    mat3 A_star(0., uz, -uy, -uz, 0., ux, uy, -ux, 0.);
    mat3 tensor_product(
        ux*ux, ux*uy, ux*uz,
        ux*uy, uy*uy, uy*uz,
        ux*uz, uy*uz, uz*uz
        );
    ret = I * cosine_val + sine_val * A_star + (1. - cosine_val) * tensor_product;
    return ret;
}

mat4 raytr::rotate4(const float degrees, const vec3 &axis)
{
    mat4 ret;
    ret = mat4(raytr::rotate3(degrees, axis));
    ret[3] = vec4(.0,.0,.0,1.0);
    return ret;
}

void rightMultiply(const mat4 &M, stack<mat4> &transfstack)
{
    mat4 &T = transfstack.top();
    T = T * M;
}
