#ifndef MY_SERVICE_H
#define MY_SERVICE_H

#include "cppreflect/cppreflect.h"

struct STRUCT() MyStruct
{
    META_OBJECT

    PROPERTY()
    int x;

    PROPERTY()
    int y;

    PROPERTY()
    int z;
};

#endif