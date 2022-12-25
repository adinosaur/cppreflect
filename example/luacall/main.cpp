#include "cppreflect/lua_mgr.h"

int main()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // 初始化LuaCppModule
    LuaMgr::GetInstance()->SetLuaState(L);
    LuaMgr::GetInstance()->InitCppModule();

    // 测试lua调用
    luaL_dofile(L, "example/luacall/main.lua");
    return 0;
}