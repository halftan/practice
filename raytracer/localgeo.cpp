#include "localgeo.h"

#include <glm/glm.hpp>

using raytr::LocalGeo;

using glm::vec3;
using glm::normalize;

LocalGeo::LocalGeo(const vec3 &pos, const vec3 &normal) :
    position(pos), normal(normal) {}

LocalGeo::~LocalGeo() {}
