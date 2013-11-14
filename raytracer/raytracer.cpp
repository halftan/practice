#include "raytrace.h"
#include "image.h"
#include "camera.h"
#include "scene.h"
#include "ray.h"
#include "intersection.h"

Image Raytrace (Camera cam, Scene scene, int width, int height)
{
    Image image = new Image(width, height);
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            Ray ray = RayThruPixel(cam, i, j);
            Intersection hit = Intersect(ray, scene);
            image[i][j] = FindColor(hit);
        }
    }
    return image;
}
