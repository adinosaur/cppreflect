#include "example/luacall/service_demo.h"
#include "example/luacall/service_demo.generated.h"

#include <stdio.h>

int ServiceDemo::Func1(int x, int y, int z)
{
    printf("ServiceDemo::Func1(). x=%d, y=%d, z=%d\n", x, y, z);
    return 0;
}

int ServiceDemo::Func2(std::vector<int> v)
{
    printf("ServiceDemo::Func2(). v={ ");
    for (int i = 0; i < v.size(); i++)
    {
        printf("[%d]=%d, ", i, v[i]);
    }
    printf("}\n");
    return 0;
}

int ServiceDemo::Func3(std::string s)
{
    printf("ServiceDemo::Func3(). s=%s\n", s.c_str());
    return 0;
}

std::string ServiceDemo::Func4()
{
     printf("ServiceDemo::Func4().\n");
    return "return std::string\n";
}

std::vector<std::string> ServiceDemo::Func5()
{
    printf("ServiceDemo::Func5().\n");
    return { "one", "two", "three", "four", };
}

int ServiceDemo::Func6(std::vector<std::string> v)
{
    printf("ServiceDemo::Func6(). v={ ");
    for (int i = 0; i < v.size(); i++)
    {
        printf("[%d]=%s, ", i, v[i].c_str());
    }
    printf("}\n");
    return 0;
}

void ServiceDemo::Func7()
{
    printf("ServiceDemo::Func7().\n");
}

void ServiceDemo::Func8(Vector3d v3d)
{
    printf("ServiceDemo::Func8(). v3d.x=%d, v3d.y=%d, v3d.z=%d\n", v3d.x, v3d.y, v3d.z);
}

Vector3d ServiceDemo::Func9()
{
    Vector3d v3d;
    v3d.x = 100;
    v3d.y = 200;
    v3d.z = 300;
    return v3d;
}

void ServiceDemo::FuncX()
{
    printf("ServiceDemo::FuncX().\n");
}

int ServiceDemo::Func10(std::map<int, int> m)
{
    printf("ServiceDemo::Func10().\n");
    for (auto& it : m)
    {
        printf("k=%d, v=%d\n", it.first, it.second);
    }
    return 0;
}

int ServiceDemo::Func11(std::map<std::string, int> m)
{
    printf("ServiceDemo::Func11().\n");
    for (auto& it : m)
    {
        printf("k=%s, v=%d\n", it.first.c_str(), it.second);
    }
    return 2;
}

int ServiceDemo::Func12(std::unordered_map<int, int> m)
{
    printf("ServiceDemo::Func12().\n");
    for (auto& it : m)
    {
        printf("k=%d, v=%d\n", it.first, it.second);
    }
    return 0;
}

int ServiceDemo::Func13(std::tuple<int, double, std::string> t)
{
    printf("ServiceDemo::Func13().\n");
    printf("[0]=%d\n", std::get<0>(t));
    printf("[1]=%f\n", std::get<1>(t));
    printf("[2]=%s\n", std::get<2>(t).c_str());
    return 0;
}

std::tuple<int, double> ServiceDemo::Func14()
{
    return std::make_tuple(111, 222.22);
}