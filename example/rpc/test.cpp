#include "example/rpc/test.h"
#include "example/rpc/test.generated.h"

#include <stdio.h>

static TestService* g_inst = nullptr;

TestService* TestService::GetInstance()
{
    if (g_inst == nullptr)
        g_inst = new TestService();
    return g_inst;
}

int TestService::Print(int i, double d, Vector3d v3d)
{
    printf("i=%d, d=%f, v3d.x=%d, v3d.y=%d, v3d.z=%d\n", i, d, v3d.x, v3d.y, v3d.z);
    return 0;
}

int TestService::Print2(std::map<int, int> int2intmap)
{
    printf("call rpc: iTestService::Print2(std::map<int, int> data)\n");
    for (auto& it : int2intmap)
    {
        printf("k=%d, v=%d\n", it.first, it.second);
    }
    return 0;
}

int TestService::Print3(int i, std::tuple<int, int> t, std::string s)
{
    printf("call rpc: iTestService::Print3(std::tuple<int, int> t)\n");
    printf("i=%d\n", i);
    printf("t[0]=%d\n", std::get<0>(t));
    printf("t[1]=%d\n", std::get<1>(t));
    printf("s=%s\n", s.c_str());
    return 0;
}

int TestService::Print4(int i, std::tuple<int, int> t, std::string s)
{
    printf("call rpc: iTestService::Print4(std::tuple<int, int> t)\n");
    printf("i=%d\n", i);
    printf("t[0]=%d\n", std::get<0>(t));
    printf("t[1]=%d\n", std::get<1>(t));
    printf("s=%s\n", s.c_str());
    printf("this->m_name=%s\n", this->m_name.c_str());
    return 0;
}