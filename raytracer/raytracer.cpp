#include "raytracer.h"
#include "camera.h"
#include "shape.h"
#include "color.h"
#include "scene.h"
#include "ray.h"
#include "intersection.h"
#include "light.h"

#include <vector>

using namespace raytr;
using namespace std;

void Raytracer::trace(Ray &ray, Color *color, int depth)
{
    Intersection intersection, t_int;
    float thit = -1, min = 9999999;

    if (depth > maxdepth) {
        return;
    }

    for (auto i = scene->objects.cbegin();
            i != scene->objects.cend(); ++i) {
        Ray oRay = ray.transform((*i)->transform);
        if ((*i)->intersectT(oRay)) {
            (*i)->intersect(oRay, &thit, &t_int);
            if (thit < min && thit > 0) {
                intersection = t_int;
                min = thit;
            }
        }
    }

    if (thit >= 0) {
        int light_hit = 0;
        *color = intersection.shape->material.ambient +
            intersection.shape->material.emission;

        for (auto i = scene->lights.cbegin();
                i != scene->lights.cend(); ++i) {
            // lighting
            Ray lRay;
            Color lColor;
            float lhit = -1;

            (*i)->generateLightRay(intersection, &lRay, &lColor);
            for (auto j = scene->objects.cbegin();
                    j != scene->objects.cend(); ++j) {
                Ray oRay = lRay.transform((*j)->transform);
                if ((*j)->intersect(oRay, &lhit, nullptr)) {
                    if (! (*i)->isDirectional()
                            && lhit > glm::length((*i)->getPosition() - lRay.position)){
                        // Point Light source between two objects
                        lhit = -1.f;
                        continue;
                    } else
                        // blocked. shadowing
                        break;
                }
            }
            if (lhit < 0) {
                // not hit any object
                Color ret = (*i)->calculateLightColor(glm::length(
                            (*i)->getPosition() - lRay.position
                            ), intersection, ray);
                *color += ret;
                ++light_hit;
            }
        }

        if (1) {
            // recursive raytracing
            Ray nRay;
            Color nColor;
            nRay.position = intersection.localgeo.position +
                Light::bias * intersection.localgeo.normal;
            nRay.dirn = glm::normalize(ray.dirn - glm::dot(
                    ray.dirn, intersection.localgeo.normal
                    ) * intersection.localgeo.normal * 2.f);
            this->trace(nRay, &nColor, depth + 1);
            *color += intersection.shape->material.specular * nColor;
        }
    }
}

Raytracer::Raytracer(Scene *scene)
{
    this->scene = scene;
    this->maxdepth = scene->maxdepth;
}

Raytracer::~Raytracer() {}
