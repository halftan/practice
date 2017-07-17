#include "camera.h"
#include "scene.h"
#include "ray.h"
#include "sample.h"

#include <glm/glm.hpp>

using namespace raytr;

Camera::Camera(Scene *scene)
{
    this->eyepos    = scene->eyeinit;
    this->upvec     = glm::normalize(scene->upinit);
    this->centerpos = scene->center;
    this->width     = scene->wsizew;
    this->height    = scene->wsizeh;
    this->aspect    = this->width / this->height;
    this->fovy      = glm::radians(scene->fovy);
    this->tan_hfovy = glm::tan(this->fovy / 2);
    this->tan_hfovx = this->tan_hfovy * this->aspect;
    this->lookAt    = glm::normalize(centerpos - eyepos);

    this->imagePaneCenter = eyepos + lookAt;
    this->imagePaneHori   = glm::normalize(glm::cross(
                lookAt, upvec
                ));
    this->imagePaneVert   = glm::normalize(glm::cross(
                imagePaneHori, lookAt
                ));

    this->half_width  = width / 2;
    this->half_height = height / 2;
}

Camera::~Camera() {}

void Camera::generateRay(const Sample &sample, Ray *ray)
{
    float a_u, a_v;
    vec3 rayCenter;
    a_u = tan_hfovx * (sample.x - half_width) / half_width;
    a_v = tan_hfovy * (half_height - sample.y)/ half_height;
    rayCenter = imagePaneCenter +
        a_u * imagePaneHori + a_v * imagePaneVert;

    ray->position = eyepos;
    ray->dirn = glm::normalize(rayCenter - eyepos);
}
