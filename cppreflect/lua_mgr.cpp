#include "cppreflect/lua_mgr.h"
#include "cppreflect/eventcall_mgr.h"
#include <assert.h>

static LuaMgr* LuaMgrInstance = NULL;

LuaMgr* LuaMgr::GetInstance()
{
    if (LuaMgrInstance == NULL)
    {
        LuaMgrInstance = new LuaMgr();
    }
    return LuaMgrInstance;
}

int LuaMgr::InitCppModule()
{
    assert(m_L != NULL);

    int top = lua_gettop(m_L);
    lua_newtable(m_L);
    lua_setglobal(m_L, "Cpp");
    EventCallMgr::GetInstance()->InvokeEvent("InitLua");
    lua_settop(m_L, top);
    return 0;
}