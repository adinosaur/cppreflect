#ifndef VECTOR_3D_H
#define VECTOR_3D_H

#include "cppreflect/cppreflect.h"

struct STRUCT() Vector3d
{
    META_OBJECT

    PROPERTY(ScriptVisible)
    int x;

    PROPERTY(ScriptVisible)
    int y;

    PROPERTY(ScriptVisible)
    int z;
};

#endif