#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "cppreflect/cppreflect.h"
#include <vector>

struct STRUCT() Vector3D
{
    META_OBJECT

    PROPERTY()
    int x;

    PROPERTY()
    int y;

    PROPERTY()
    int z;

    PROPERTY()
    std::vector<int> array;

    PROPERTY()
    std::string name;
};

#endif