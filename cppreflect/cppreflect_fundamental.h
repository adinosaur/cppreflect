#ifndef CPPREFLECT_FUNDAMENTAL_H
#define CPPREFLECT_FUNDAMENTAL_H

#include "cppreflect/cppreflect_define.h"

namespace cppreflect
{

// ==================
// Bool
// ==================
struct Bool : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// Char
// ==================
struct Char : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// Short
// ==================
struct Short : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// Int
// ==================
struct Int : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// Long
// ==================
struct Long : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// LongLong
// ==================
struct LongLong : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// Float
// ==================
struct Float : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;
    
    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// Double
// ==================
struct Double : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// LongDouble
// ==================
struct LongDouble : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// UnsignedChar
// ==================
struct UnsignedChar : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// UnsignedShort
// ==================
struct UnsignedShort : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// UnsignedInt
// ==================
struct UnsignedInt : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// UnsignedLong
// ==================
struct UnsignedLong : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// UnsignedLongLong
// ==================
struct UnsignedLongLong : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

// ==================
// Void
// ==================
struct Void : public Type
{
    using Type::Type;
    virtual std::string Dump(const void *instance) const override;
};

} /* namespace cppreflect */

#endif /* CPPREFLECT_FUNDAMENTAL_H */