#include "tri.h"
#include "vertex.h"
#include "solve.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <cstdio>
#include <cmath>

#define WITHIN0_1(x) x <= 1 && x >= 0

using namespace raytr;

Tri::Tri(Vertex *a, Vertex *b, Vertex *c)
{
    this->verts[0] = a;
    this->verts[1] = b;
    this->verts[2] = c;
    this->normal = glm::normalize(glm::cross(
                vec3(b->pos - a->pos),
                vec3(c->pos - a->pos)
                ));
}

Tri::~Tri() {}

bool Tri::intersect(const Ray &ray, float *thit, Intersection *inter)
{
    float t = (glm::dot(A(), normal) - glm::dot(ray.position, normal))
        / glm::dot(ray.dirn, normal);
    vec3 point = ray.position + ray.dirn * t;
    vec3 vec_ap = point - A();
    vec3 vec_ab = B() - A();
    vec3 vec_ac = C() - A();
    vec2 solv(0);

    for (int i = 0; i < 3; ++i) {
        solv = raytr::simul2(
                vec2(vec_ab[i], vec_ab[(i+1)%3]),
                vec2(vec_ac[i], vec_ac[(i+1)%3]),
                vec2(vec_ap[i], vec_ap[(i+1)%3])
                );
        if (WITHIN0_1(solv[0])
                    && WITHIN0_1(solv[1])
                    && WITHIN0_1(solv[0] + solv[1]))
            break;
    }

    if (WITHIN0_1(solv[0])
                && WITHIN0_1(solv[1])
                && WITHIN0_1(solv[0] + solv[1])) {
        if (t <= 0)
            return false;

        if (thit != nullptr) *thit = t;
        if (inter != nullptr) {
            inter->shape = this;
            inter->localgeo.position = vec3(transform * vec4(point, 1.0));
            inter->localgeo.normal = glm::normalize(
                    vec3(glm::inverseTranspose(mat3(transform))
                    * normal));
        }
        // printf("Hit!!!!\n");
        return true;
    } else {
        return false;
    }
}
bool Tri::intersectT(const Ray &ray)
{
    return intersect(ray, nullptr, nullptr);
}

vec3 Tri::A()
{
    return verts[0]->pos;
}

vec3 Tri::B()
{
    return verts[1]->pos;
}

vec3 Tri::C()
{
    return verts[2]->pos;
}
