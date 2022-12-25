#ifndef TEST_H
#define TEST_H

#include "cppreflect/cppreflect.h"

#include <map>
#include <tuple>

struct STRUCT() Vector3d
{
    META_OBJECT

    PROPERTY(Serialize)
    int x;

    PROPERTY(Serialize)
    int y;

    PROPERTY(Serialize)
    int z;

    PROPERTY()
    int k;
};

struct STRUCT() TestService
{
    META_OBJECT

    std::string m_name;
    TestService(): m_name("Test") {}

    static TestService* GetInstance();

    FUNCTION(RpcCallable)
    static int Print(int i, double d, Vector3d v3d);

    FUNCTION(RpcCallable)
    static int Print2(std::map<int, int> int2intmap);

    FUNCTION(RpcCallable)
    static int Print3(int i, std::tuple<int, int> t, std::string s);

    FUNCTION(RpcCallable)
    int Print4(int i, std::tuple<int, int> t, std::string s);
};

struct STRUCT() PrintParams
{
    META_OBJECT

    PROPERTY(Serialize)
    int i;

    PROPERTY(Serialize)
    double d;

    PROPERTY(Serialize)
    Vector3d v3d;
};

struct STRUCT() Print2Params
{
    META_OBJECT

    PROPERTY(Serialize)
    std::map<int, int> int2intmap;
};

struct STRUCT() Print3Params
{
    META_OBJECT
    
    PROPERTY(Serialize)
    int i;

    PROPERTY(Serialize)
    std::tuple<int, int> t;

    PROPERTY(Serialize)
    std::string s;
};

struct STRUCT() Print4Params
{
    META_OBJECT
    
    PROPERTY(Serialize)
    int i;

    PROPERTY(Serialize)
    std::tuple<int, int> t;

    PROPERTY(Serialize)
    std::string s;
};

#endif /* TEST_H */