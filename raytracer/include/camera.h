#ifndef CAMERA_H
#define CAMERA_H 1

#include <glm/glm.hpp>

using glm::vec3;

namespace raytr
{
    class Scene;
    class Ray;
    class Sample;

    class Camera
    {
    public:
        Camera(Scene *scene);
        ~Camera();

        void generateRay(const Sample &sample, Ray *ray);

        vec3 eyepos;
        vec3 upvec;
        vec3 centerpos;
        vec3 lookAt;
        vec3 imagePaneCenter;
        vec3 imagePaneVert;
        vec3 imagePaneHori;
        float fovy;
        float aspect;
        float width, height;
        float tan_hfovy;
        float tan_hfovx;

    private:
        float half_width, half_height;
    };
}
#endif
