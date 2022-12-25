#include "example/base/service_demo.h"
#include "example/base/service_demo.generated.h"

#include <stdio.h>

/// 静态函数
/// 没有参数
/// 没有返回值
void ServiceDemo::Func1()
{
    printf("call ServiceDemo::Func1()\n");
}

/// 静态函数
/// 参数是基础类型
/// 没有返回值
void ServiceDemo::Func2(int x, unsigned int y, double z)
{
    printf("call ServiceDemo::Func2(int x, unsigned int y, double z)\n");
    printf("x=%d, y=%u, z=%f\n", x, y, z);
}

/// 静态函数
/// 参数是std::vector
/// 没有返回值
void ServiceDemo::Func3(std::vector<int> v)
{
    printf("ServiceDemo::Func3(std::vector<int> v)\n");
    for (int i = 0; i < v.size(); i++)
    {
        printf("[%d]=%d\n", i, v[i]);
    }
}

/// 静态函数
/// 参数是结构体/类
/// 没有返回值
void ServiceDemo::Func4(Vector3D v3d)
{
    printf("ServiceDemo::Func4(Vector3D v3d)\n");
    printf("v3d.x=%d, v3d.y=%d, v3d.z=%d\n", v3d.x, v3d.y, v3d.z);
}

/// 静态函数
/// 参数是std::string
/// 返回值是std::string
std::string ServiceDemo::Func5(std::string s)
{
    printf("ServiceDemo::Func5(std::string s)\n");
    printf("s=%s\n", s.c_str());
    return "hehe";
}

/// 成员函数
/// 参数是std::map
/// 返回值是字符串
std::string ServiceDemo::Func6(std::map<int, std::string> m)
{
    printf("ServiceDemo::Func6(std::map<int, std::string> m)\n");
    for (auto& it : m)
    {
        printf("[%d]=%s\n", it.first, it.second.c_str());
    }
    printf("inst.i=%d\n", i);
    return "keke";
}