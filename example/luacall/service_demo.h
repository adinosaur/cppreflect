#ifndef SERVICE_DEMO_H
#define SERVICE_DEMO_H

#include "cppreflect/cppreflect.h"
#include "example/luacall/vector3d.h"

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <tuple>

struct STRUCT() ServiceDemo
{
    META_OBJECT

    // 函数参数是基础类型
    FUNCTION(ScriptCallable)
    static int Func1(int x, int y, int z);

    // 函数参数是std::vector
    FUNCTION(ScriptCallable)
    static int Func2(std::vector<int> v);

    // 函数参数是std::string
    FUNCTION(ScriptCallable)
    static int Func3(std::string s);

    // 函数返回值是std::string
    FUNCTION(ScriptCallable)
    static std::string Func4();

    // 函数返回值是std::vector<std::string>
    FUNCTION(ScriptCallable)
    static std::vector<std::string> Func5();

    // 函数参数是std::vector<std::string>
    FUNCTION(ScriptCallable)
    static int Func6(std::vector<std::string> v);

    // 函数无参数，没有返回值
    FUNCTION(ScriptCallable)
    static void Func7();

    // 参数是struct/class
    FUNCTION(ScriptCallable)
    static void Func8(Vector3d v3d);

    // 返回值是struct/class
    FUNCTION(ScriptCallable)
    static Vector3d Func9();

    // 函数不是static，暂时不支持并且会在编译时出现一个报错
    // FUNCTION(ScriptCallable)
    void FuncX();

    // 函数参数是std::map<int, int>
    FUNCTION(ScriptCallable)
    static int Func10(std::map<int, int> m);

    // 函数参数是std::map<std::string, int>
    FUNCTION(ScriptCallable)
    static int Func11(std::map<std::string, int> m);

    // 函数参数是std::unordered_map<int, int>
    FUNCTION(ScriptCallable)
    static int Func12(std::unordered_map<int, int> m);

    // 函数参数是std::tuple<int, double, std::string>
    FUNCTION(ScriptCallable)
    static int Func13(std::tuple<int, double, std::string> t);

    // 函数返回值是std::tuple<int, double>
    FUNCTION(ScriptCallable)
    static std::tuple<int, double> Func14();
};

#endif