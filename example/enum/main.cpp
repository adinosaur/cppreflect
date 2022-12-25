#include "example/enum/enum.h"
#include "cppreflect/lua_mgr.h"

#include <stdio.h>

int main()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // 初始化LuaCppModule
    LuaMgr::GetInstance()->SetLuaState(L);
    LuaMgr::GetInstance()->InitCppModule();

    const cppreflect::Type* t = cppreflect::GetType<MyEnum>();
    if (t->IsEnum())
    {
        const cppreflect::Enum* e = dynamic_cast<const cppreflect::Enum*>(t);
        for (int i = 0; i < e->GetItemsSize(); i++)
        {
            std::string enum_name = e->GetNameByIndex(i);
            int64_t enum_value = e->GetValueByIndex(i);
            printf("enum_name=%s, enum_value=%lld\n", enum_name.c_str(), enum_value);
        }
    }
    // output:
    // enum_name=ONE, enum_value=1
    // enum_name=TWO, enum_value=2
    // enum_name=THREE, enum_value=3

    luaL_dostring(L, "print(Cpp.MyEnum.ONE)");
    luaL_dostring(L, "print(Cpp.MyEnum.TWO)");
    luaL_dostring(L, "print(Cpp.MyEnum.THREE)");
    luaL_dostring(L, "print(Cpp.MyEnum.FOUR)");
    // output:
    // 1
    // 2
    // 3
    // nil

    return 0;
}