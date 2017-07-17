#ifndef VERTEX_H
#define VERTEX_H 1
#include <glm/glm.hpp>

using glm::vec3;

namespace raytr
{
    class Vertex
    {
    public:
        Vertex(const vec3 &pos);
        Vertex(float x, float y, float z);
        Vertex() = default;
        ~Vertex();

        vec3 pos;
    };
}
#endif
