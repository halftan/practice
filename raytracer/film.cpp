#include "scene.h"
#include "film.h"
#include "sampler.h"
#include "color.h"

#include <iostream>
#include <FreeImage.h>

using namespace raytr;

const int Film::bpp = 24; // Set bits per pixel to 24 for regular RGB

Film::Film(const Scene &scene)
{
    this->initFromScene(scene);
}

Film::~Film()
{
    if (this->image)
        delete[] this->image;
}

void Film::initFromScene(const Scene &scene)
{
    this->image = new Color[scene.wsizew * scene.wsizeh];
    this->width = scene.wsizew;
    this->height = scene.wsizeh;
    this->ofilename = scene.ofilename;
}

void Film::commit(const Sample &sample, const Color &color)
{
    int x = sample.x - 0.5;
    int y = sample.y - 0.5;

    if (this->width * y + x < this->width * this->height)
        this->image[this->width * y + x] = color;
}

Color* Film::at(int x, int y)
{
    return this->image + y*width + x;
}

void Film::savePNG()
{
    FreeImage_Initialise();
    FIBITMAP *bitmap = FreeImage_Allocate(width, height, bpp);
    RGBQUAD cquad;
    Color *color;

    if (! bitmap) {
        std::cerr << "Cannot allocate bitmap." << std::endl;
    } else {
        // Draw the image to file
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                color = this->at(i,j);
                cquad.rgbRed = color->r * 255;
                cquad.rgbGreen = color->g * 255;
                cquad.rgbBlue = color->b * 255;
                FreeImage_SetPixelColor(bitmap, i, j, &cquad);
            }
        }

        if (FreeImage_Save(FIF_PNG, bitmap, ofilename.c_str(), 0)) {
            std::cout << "Successfully saved png." << std::endl;
        } else {
            std::cout << "Output png failed." << std::endl;
        }
    }

    FreeImage_DeInitialise();
}
