#include "cppreflect/cppreflect_string.h"

namespace cppreflect {

std::string StringClass::Dump(const void *instance) const
{
    const std::string *str = (const std::string *)instance;
    return *str;
}

int StringClass::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isstring(L, index))
    {
        return -1;
    }
    else
    {
        *((std::string *)value) = lua_tostring(L, index);
        return 0;
    }
}

int StringClass::PushValueToLua(lua_State *L, const void* value) const
{
    const char* s = ((std::string*)value)->c_str();
    lua_pushstring(L, s);
    return 0;
}

int StringClass::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    std::string* s = (std::string*)value;
    msgpack_pack_str_with_body(pk, s->c_str(), s->size());
    return 0;
}

int StringClass::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_STR)
    {
        return -1;
    }
    else
    {
        *((std::string *)value) = std::string(o.via.str.ptr, o.via.str.size);
        return 0;
    }
}

namespace detail {

const Class* GetClassImpl(ClassTag<std::string>)
{
    static StringClass stringClass {
        "std::string",                  /* m_name */
        sizeof(std::string),            /* m_size */
        0,                              /* m_hash */
        NULL,                           /* m_baseClass */
        {},                             /* m_fields */
        {},                             /* m_functions */
    };

    return &stringClass;
}

template <>
const Type* GetTypeImpl(TypeTag<std::string>)
{
    return GetClassImpl(ClassTag<std::string>{});
}

} /* namespace: detail */

} /* namespace cppreflect */