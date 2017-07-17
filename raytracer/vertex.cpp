#include "vertex.h"

using raytr::Vertex;

Vertex::Vertex(const vec3 &pos) : pos(pos) {}

Vertex::Vertex(float x, float y, float z) :
    pos(x,y,z) {}

Vertex::~Vertex() {};

