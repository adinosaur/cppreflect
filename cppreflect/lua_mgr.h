#ifndef YGGD_LUA_MGR_H
#define YGGD_LUA_MGR_H

#include "lua.hpp"

struct LuaMgr
{
    /* GetInstance */
    static LuaMgr* GetInstance();

    /* 初始化Lua的cppmodule */
    int InitCppModule();

    /* Set全局的LuaState */
    void SetLuaState(lua_State* L) { m_L = L; }

    /* Get全局的LuaState */
    lua_State* GetLuaState() { return m_L; }

    /* 全局的LuaState  */
    lua_State* m_L = NULL;
};

#endif /* YGGD_LUA_MGR_H */