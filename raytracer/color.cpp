#include "color.h"

using namespace raytr;

Color::Color()
    : r(.0), g(.0), b(.0) {}

Color::Color(float x)
    : r(x), g(x), b(x) {}

Color::Color(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

Color Color::operator+(const Color &other)
{
    this->r += other.r;
    this->g += other.g;
    this->b += other.b;
    return *this;
}

Color Color::operator-(const Color &other)
{
    this->r -= other.r;
    this->g -= other.g;
    this->b -= other.b;
    return *this;
}

Color Color::operator+(const vec3 &other)
{
    this->r += other.x;
    this->g += other.y;
    this->b += other.z;
    return *this;
}

Color Color::operator-(const vec3 &other)
{
    this->r -= other.x;
    this->g -= other.y;
    this->b -= other.z;
    return *this;
}

Color Color::operator=(const Color &other)
{
    if (this != &other) {
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
    }
    return *this;
}

Color Color::operator*(const float k)
{
    if (k > 0) {
        this->r *= k;
        this->g *= k;
        this->b *= k;
    }
    if (this->r > 1.0) {
        this->r = 1.0;
    }
    if (this->g > 1.0) {
        this->g = 1.0;
    }
    if (this->b > 1.0) {
        this->b = 1.0;
    }
    return *this;
}

Color Color::operator/(const float k)
{
    if (k > 0) {
        this->r /= k;
        this->g /= k;
        this->b /= k;
    }
    if (this->r > 1.0) {
        this->r = 1.0;
    }
    if (this->g > 1.0) {
        this->g = 1.0;
    }
    if (this->b > 1.0) {
        this->b = 1.0;
    }
    return *this;
}
