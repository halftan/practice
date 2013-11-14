#ifndef READFILE_H
#define READFILE_H 1

#include <iostream>
#include <sstream>
#include <glm/ext.hpp>

namespace raytr
{
    const int MAX_VAL = 10;
    const int UNKNOWN_PRIM = 4;

    bool readvals(std::stringstream &s, const int numvals, double *values);
}

#endif
