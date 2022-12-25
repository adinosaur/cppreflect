#ifndef CPPREFLECT_STRING_H
#define CPPREFLECT_STRING_H

#include "cppreflect/cppreflect_define.h"
#include <string>

namespace cppreflect {

struct StringClass : public Class
{
    using Class::Class;

    virtual std::string Dump(const void *instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;
};

} /* namespace cppreflect */

#endif /* CPPREFLECT_VECTOR_H */