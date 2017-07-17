#include "sphere.h"
#include "ray.h"
#include "solve.h"

#include <glm/gtc/matrix_inverse.hpp>

using namespace raytr;

Sphere::Sphere(const vec3 &center, float radius) :
    center(center), radius(radius) {}

Sphere::Sphere(float x, float y, float z, float radius) :
    center(x,y,z), radius(radius) {}

Sphere::~Sphere() {}

bool Sphere::intersectT(const Ray &ray)
{
    return this->intersect(ray, nullptr, nullptr);
}

bool Sphere::intersect(const Ray &ray, float *thit, Intersection *inter)
{
    quadRoot root;
    vec3 dist = ray.position - center;
    solve_quadratic(
            glm::dot(ray.dirn, ray.dirn),
            2*glm::dot(ray.dirn, dist),
            glm::dot(dist, dist) - radius*radius,
            &root
            );
    if (root.real_root_count == 0)
        return false;
    if (root.ra <= 0 && root.rb <= 0)
        return false;
    if (thit != nullptr)
        *thit = root.ra < root.rb ? root.ra < 0 ? root.rb : root.ra : root.rb;
    if (inter != nullptr) {
        vec3 position = ray.position + *thit * ray.dirn;
        vec3 normal = position - center;
        inter->shape = this;
        inter->localgeo.position = vec3(transform * vec4(position, 1.0));
        inter->localgeo.normal = glm::normalize(
                glm::inverseTranspose(mat3(transform)) * normal);
    }
    return true;
}
