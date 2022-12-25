#ifndef SERVICE_DEMO_H
#define SERVICE_DEMO_H

#include "cppreflect/cppreflect.h"
#include "example/base/vector3d.h"

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

struct STRUCT() ServiceDemo
{
    META_OBJECT
    
    /// 静态函数
    /// 没有参数
    /// 没有返回值
    FUNCTION()
    static void Func1();

    /// 静态函数
    /// 参数是基础类型
    /// 没有返回值
    FUNCTION()
    static void Func2(int x, unsigned int y, double z);

    /// 静态函数
    /// 参数是std::vector
    /// 没有返回值
    FUNCTION()
    static void Func3(std::vector<int> v);

    /// 静态函数
    /// 参数是结构体/类
    /// 没有返回值
    FUNCTION()
    static void Func4(Vector3D v3d);

    /// 静态函数
    /// 参数是std::string
    /// 返回值是std::string
    FUNCTION()
    static std::string Func5(std::string s);

    /// 成员函数
    /// 参数是std::map
    /// 返回值是字符串
    FUNCTION()
    std::string Func6(std::map<int, std::string> m);

    int i = 999;
};

#endif