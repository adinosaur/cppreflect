#include "cppreflect/cppreflect_define.h"

#include <string>

namespace cppreflect {

// ==================
// Type
// ==================
std::string Type::Dump(const void* instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%s{%p}", m_name.c_str(), instance);
    return buff;
}

int Type::SetValueFromLua(lua_State *L, int index, void* value) const
{
    return -1;
}

int Type::PushValueToLua(lua_State *L, const void* value) const
{
    return -1;
}

int Type::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    return -1;
}

int Type::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    return -1;
}


// ==================
// Enum
// ==================
std::string Enum::GetNameByIndex(int index) const
{
    if (index >= 0 and index < m_items.size())
    {
        return m_items[index].first;
    }
    return "";
}

int64_t Enum::GetValueByIndex(int index) const
{
    if (index >= 0 and index < m_items.size())
    {
        return m_items[index].second;
    }
    return -1;
}

// ==================
// Class
// ==================
std::string Class::Dump(const void* instance) const
{
    std::string r;
    r.push_back('{');

    for (auto& field : GetFields())
    {
        const void* pointer = field->GetVoidPointer(instance);
        const char* name = field->GetName().c_str();

        r.append(field->GetName());
        r.push_back('=');
        r.append(field->GetType()->Dump(pointer));
        r.push_back(',');
    }
    r.push_back('}');
    return r;
}

int Class::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_istable(L, index))
    {
        return -1;
    }
    else
    {
        int err = 0;
        int top = lua_gettop(L);

        for (Field* field : m_fields)
        {
            lua_pushstring(L, field->GetName().c_str());
            lua_gettable(L, -2);
            void* member = const_cast<void*>(field->GetVoidPointer(value));
            err = field->m_type->SetValueFromLua(L, -1, member);
            if (err != 0)
            {
                break;
            }
            lua_pop(L, 1);
        }

        lua_settop(L, top);
        return err;
    }
}

int Class::PushValueToLua(lua_State *L, const void* value) const
{
    lua_createtable(L, 0, m_fields.size());
    for (Field* field : m_fields)
    {
        lua_pushstring(L, field->GetName().c_str());
        void* member = const_cast<void*>(field->GetVoidPointer(value));
        field->m_type->PushValueToLua(L, member);
        lua_settable(L, -3);
    }
    return 0;
}

int Class::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    size_t sz = m_fields.size();
    msgpack_pack_map(pk, sz);
    for (Field* field : m_fields)
    {
        msgpack_pack_str_with_body(pk, field->GetName().c_str(), field->GetName().size());
        void* member = const_cast<void*>(field->GetVoidPointer(value));
        field->m_type->PackValueToMsgpack(pk, member);
    }
    return 0;
}

int Class::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_MAP)
    {
        return -1;
    }
    else
    {
        int err = 0;

        std::map<std::string, msgpack_object_kv*> m;
        if (o.via.map.size != 0)
        {
            const msgpack_object_kv* pend = o.via.map.ptr + o.via.map.size;
            for (msgpack_object_kv *p = o.via.map.ptr; p < pend; p++)
            {
                if (p->key.type == MSGPACK_OBJECT_STR)
                {
                    std::string k(p->key.via.str.ptr, p->key.via.str.size);
                    m[k] = p;
                }
            }
        }

        for (Field* field : m_fields)
        {
            const auto& it = m.find(field->GetName());
            if (it == m.end())
            {
                continue;
            }

            const msgpack_object_kv* p = it->second;
            void* member = const_cast<void*>(field->GetVoidPointer(value));
            err = field->m_type->SetValueFromMsgpack(p->val, member);
            if (err != 0)
            {
                break;
            }
        }

        return err;
    }
}

const Field* Class::GetFieldByName(const std::string& name) const
{
    auto it = m_cacheFields.find(name);
    if (it == m_cacheFields.end())
    {
        return NULL;
    }
    return it->second;
}

const Function* Class::GetFunctionByName(const std::string& name) const
{
    auto it = m_cacheFunctions.find(name);
    if (it == m_cacheFunctions.end())
    {
        return NULL;
    }
    return it->second;
}

} /* namespace cppreflect */