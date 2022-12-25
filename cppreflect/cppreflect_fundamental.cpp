#include "cppreflect/cppreflect.h"

namespace cppreflect
{

// ==================
// Bool
// ==================
std::string Bool::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%d", *((bool*)instance));
    return buff;
}

int Bool::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isboolean(L, index))
    {
        return -1;
    }
    else
    {
        *((bool*)value) = lua_toboolean(L, index) != 0;
        return 0;
    }
}

int Bool::PushValueToLua(lua_State *L, const void* value) const
{
    int b = *((bool*)value);
    lua_pushboolean(L, b);
    return 0;
}

int Bool::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    bool b = *((bool*)value);
    if (b)
    {
        msgpack_pack_true(pk);
    }
    else
    {
        msgpack_pack_false(pk);
    }
    return 0;
}

int Bool::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_BOOLEAN)
    {
        return -1;
    }
    else
    {
        *((bool*)value) = o.via.boolean;
        return 0;
    }
}

// ==================
// Char
// ==================
std::string Char::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%d", *((char*)instance));
    return buff;
}

int Char::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isinteger(L, index))
    {
        return -1;
    }
    else
    {
        *((char*)value) = (char)lua_tointeger(L, index);
        return 0;
    }
}

int Char::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Integer i = *((char*)value);
    lua_pushinteger(L, i);
    return 0;
}

int Char::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    char i = *((char*)value);
    msgpack_pack_char(pk, i);
    return 0;
}

int Char::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    switch (o.type)
    {
        case MSGPACK_OBJECT_POSITIVE_INTEGER: { *((char*)value) = o.via.u64; return 0; }
        case MSGPACK_OBJECT_NEGATIVE_INTEGER: { *((char*)value) = o.via.i64; return 0; }
        default: return -1;
    }
}

// ==================
// Short
// ==================
std::string Short::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%d", *((short*)instance));
    return buff;
}

int Short::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isinteger(L, index))
    {
        return -1;
    }
    else
    {
        *((short*)value) = (short)lua_tointeger(L, index);
        return 0;
    }
}

int Short::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Integer i = *((short*)value);
    lua_pushinteger(L, i);
    return 0;
}

int Short::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    short i = *((short*)value);
    msgpack_pack_short(pk, i);
    return 0;
}

int Short::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    switch (o.type)
    {
        case MSGPACK_OBJECT_POSITIVE_INTEGER: { *((short*)value) = o.via.u64; return 0; }
        case MSGPACK_OBJECT_NEGATIVE_INTEGER: { *((short*)value) = o.via.i64; return 0; }
        default: return -1;
    }
}

// ==================
// Int
// ==================
std::string Int::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%d", *((int*)instance));
    return buff;
}

int Int::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isinteger(L, index))
    {
        return -1;
    }
    else
    {
        *((int*)value) = (int)lua_tointeger(L, index);
        return 0;
    }
}

int Int::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Integer i = *((int*)value);
    lua_pushinteger(L, i);
    return 0;
}

int Int::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    int i = *((int*)value);
    msgpack_pack_int(pk, i);
    return 0;
}

int Int::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    switch (o.type)
    {
        case MSGPACK_OBJECT_POSITIVE_INTEGER: { *((int*)value) = o.via.u64; return 0; }
        case MSGPACK_OBJECT_NEGATIVE_INTEGER: { *((int*)value) = o.via.i64; return 0; }
        default: return -1;
    }
}

// ==================
// Long
// ==================
std::string Long::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%ld", *((long*)instance));
    return buff;
}

int Long::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isinteger(L, index))
    {
        return -1;
    }
    else
    {
        *((long*)value) = (long)lua_tointeger(L, index);
        return 0;
    }
}

int Long::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Integer i = *((int*)value);
    lua_pushinteger(L, i);
    return 0;
}

int Long::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    long i = *((long*)value);
    msgpack_pack_long(pk, i);
    return 0;
}

int Long::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    switch (o.type)
    {
        case MSGPACK_OBJECT_POSITIVE_INTEGER: { *((long*)value) = o.via.u64; return 0; }
        case MSGPACK_OBJECT_NEGATIVE_INTEGER: { *((long*)value) = o.via.i64; return 0; }
        default: return -1;
    }
}

// ==================
// LongLong
// ==================
std::string LongLong::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%lld", *((long long*)instance));
    return buff;
}

int LongLong::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isinteger(L, index))
    {
        return -1;
    }
    else
    {
        *((long long*)value) = (long long)lua_tointeger(L, index);
        return 0;
    }
}

int LongLong::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Integer i = *((long long*)value);
    lua_pushinteger(L, i);
    return 0;
}

int LongLong::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    long long i = *((long long*)value);
    msgpack_pack_long_long(pk, i);
    return 0;
}

int LongLong::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    switch (o.type)
    {
        case MSGPACK_OBJECT_POSITIVE_INTEGER: { *((long long*)value) = o.via.u64; return 0; }
        case MSGPACK_OBJECT_NEGATIVE_INTEGER: { *((long long*)value) = o.via.i64; return 0; }
        default: return -1;
    }
}

// ==================
// Float
// ==================
std::string Float::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%.9g", *((float*)instance));
    return buff;
}

int Float::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isnumber(L, index))
    {
        return -1;
    }
    else
    {
        *((float*)value) = (float)lua_tonumber(L, index);
        return 0;
    }
}

int Float::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Number n = *((float*)value);
    lua_pushnumber(L, n);
    return 0;
}

int Float::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    float n = *((float*)value);
    msgpack_pack_float(pk, n);
    return 0;
}

int Float::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_FLOAT)
    {
        return -1;
    }
    else
    {
        *((float*)value) = o.via.f64;
        return 0;
    }
}

// ==================
// Double
// ==================
std::string Double::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%.17g", *((double*)instance));
    return buff;
}

int Double::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isnumber(L, index))
    {
        return -1;
    }
    else
    {
        *((double*)value) = (double)lua_tonumber(L, index);
        return 0;
    }
}

int Double::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Number n = *((double*)value);
    lua_pushnumber(L, n);
    return 0;
}

int Double::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    double n = *((double*)value);
    msgpack_pack_double(pk, n);
    return 0;
}

int Double::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_FLOAT)
    {
        return -1;
    }
    else
    {
        *((double*)value) = o.via.f64;
        return 0;
    }
}

// ==================
// LongDouble
// ==================
std::string LongDouble::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%.21Lg", *((long double*)instance));
    return buff;
}

int LongDouble::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isnumber(L, index))
    {
        return -1;
    }
    else
    {
        *((long double*)value) = (long double)lua_tonumber(L, index);
        return 0;
    }
}

int LongDouble::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Number n = *((long double*)value);
    lua_pushnumber(L, n);
    return 0;
}

int LongDouble::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    double n = *((long double*)value);
    msgpack_pack_double(pk, n);
    return 0;
}

int LongDouble::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_FLOAT)
    {
        return -1;
    }
    else
    {
        *((long double*)value) = o.via.f64;
        return 0;
    }
}

// ==================
// UnsignedChar
// ==================
std::string UnsignedChar::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%u", *((unsigned char*)instance));
    return buff;
}

int UnsignedChar::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isinteger(L, index))
    {
        return -1;
    }
    else
    {
        *((unsigned char*)value) = (unsigned char)lua_tointeger(L, index);
        return 0;
    }
}

int UnsignedChar::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Integer i = *((unsigned char*)value);
    lua_pushinteger(L, i);
    return 0;
}

int UnsignedChar::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    unsigned char u = *((unsigned char*)value);
    msgpack_pack_unsigned_char(pk, u);
    return 0;
}

int UnsignedChar::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_POSITIVE_INTEGER)
    {
        return -1;
    }
    else
    {
        *((unsigned char*)value) = o.via.u64;
        return 0;
    }
}

// ==================
// UnsignedShort
// ==================
std::string UnsignedShort::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%u", *((unsigned short*)instance));
    return buff;
}

int UnsignedShort::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isinteger(L, index))
    {
        return -1;
    }
    else
    {
        *((unsigned short*)value) = (unsigned short)lua_tointeger(L, index);
        return 0;
    }
}

int UnsignedShort::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Integer i = *((unsigned short*)value);
    lua_pushinteger(L, i);
    return 0;
}

int UnsignedShort::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    unsigned short u = *((unsigned short*)value);
    msgpack_pack_unsigned_short(pk, u);
    return 0;
}

int UnsignedShort::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_POSITIVE_INTEGER)
    {
        return -1;
    }
    else
    {
        *((unsigned short*)value) = o.via.u64;
        return 0;
    }
}

// ==================
// UnsignedInt
// ==================
std::string UnsignedInt::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%u", *((unsigned int*)instance));
    return buff;
}

int UnsignedInt::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isinteger(L, index))
    {
        return -1;
    }
    else
    {
        *((unsigned int*)value) = (unsigned int)lua_tointeger(L, index);
        return 0;
    }
}

int UnsignedInt::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Integer i = *((unsigned int*)value);
    lua_pushinteger(L, i);
    return 0;
}

int UnsignedInt::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    unsigned int u = *((unsigned int*)value);
    msgpack_pack_unsigned_int(pk, u);
    return 0;
}

int UnsignedInt::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_POSITIVE_INTEGER)
    {
        return -1;
    }
    else
    {
        *((unsigned int*)value) = o.via.u64;
        return 0;
    }
}

// ==================
// UnsignedLong
// ==================
std::string UnsignedLong::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%lu", *((unsigned long*)instance));
    return buff;
}

int UnsignedLong::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isinteger(L, index))
    {
        return -1;
    }
    else
    {
        *((unsigned long*)value) = (unsigned long)lua_tointeger(L, index);
        return 0;
    }
}

int UnsignedLong::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Integer i = *((unsigned long*)value);
    lua_pushinteger(L, i);
    return 0;
}

int UnsignedLong::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    unsigned long u = *((unsigned long*)value);
    msgpack_pack_unsigned_long(pk, u);
    return 0;
}

int UnsignedLong::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_POSITIVE_INTEGER)
    {
        return -1;
    }
    else
    {
        *((unsigned long*)value) = o.via.u64;
        return 0;
    }
}

// ==================
// UnsignedLongLong
// ==================
std::string UnsignedLongLong::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%llu", *((unsigned long long*)instance));
    return buff;
}

int UnsignedLongLong::SetValueFromLua(lua_State *L, int index, void* value) const
{
    if (!lua_isinteger(L, index))
    {
        return -1;
    }
    else
    {
        *((unsigned long long*)value) = (unsigned long long)lua_tointeger(L, index);
        return 0;
    }
}

int UnsignedLongLong::PushValueToLua(lua_State *L, const void* value) const
{
    lua_Integer i = *((unsigned long long*)value);
    lua_pushinteger(L, i);
    return 0;
}

int UnsignedLongLong::PackValueToMsgpack(msgpack_packer* pk, const void* value) const
{
    unsigned long long u = *((unsigned long long*)value);
    msgpack_pack_unsigned_long_long(pk, u);
    return 0;
}

int UnsignedLongLong::SetValueFromMsgpack(const msgpack_object o, void* value) const
{
    if (o.type != MSGPACK_OBJECT_POSITIVE_INTEGER)
    {
        return -1;
    }
    else
    {
        *((unsigned long long*)value) = o.via.u64;
        return 0;
    }
}

// ==================
// Void
// ==================
std::string Void::Dump(const void *instance) const
{
    static char buff[64];
    snprintf(buff, sizeof(buff), "%p", instance);
    return buff;
}

// ==================
// GetTypeImpl
// ==================
namespace detail {

template <>
const Type* GetTypeImpl(TypeTag<bool>)
{
    static Bool type{ "Bool", sizeof(bool), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<char>)
{
    static Char type{ "Char", sizeof(char), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<short>)
{
    static Short type{ "Short", sizeof(short), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<int>)
{
    static Int type{ "Int", sizeof(int), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<long>)
{
    static Long type{ "Long", sizeof(long), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<float>)
{
    static Float type{ "Float", sizeof(float), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<double>)
{
    static Double type{ "Double", sizeof(double), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<long double>)
{
    static LongDouble type{ "LongDouble", sizeof(long double), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<long long>)
{
    static LongLong type{ "LongLong", sizeof(long long), 0 };
    return &type;
}


template <>
const Type* GetTypeImpl(TypeTag<unsigned char>)
{
    static UnsignedChar type{ "UnsignedChar", sizeof(unsigned char), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<unsigned short>)
{
    static UnsignedShort type{ "UnsignedShort", sizeof(unsigned short), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<unsigned int>)
{
    static UnsignedInt type{ "UnsignedInt", sizeof(unsigned int), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<unsigned long>)
{
    static UnsignedLong type{ "UnsignedLong", sizeof(unsigned long), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<unsigned long long>)
{
    static UnsignedLongLong type{ "UnsignedLongLong", sizeof(unsigned long long), 0 };
    return &type;
}

template <>
const Type* GetTypeImpl(TypeTag<void>)
{
    static Void type{ "Void", 0, 0 };
    return &type;
}

} /* namespace detail */
} /* namespace cppreflect */