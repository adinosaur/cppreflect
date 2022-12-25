#ifndef CPPREFLECT_VECTOR_H
#define CPPREFLECT_VECTOR_H

#include "cppreflect/cppreflect_define.h"
#include <string>
#include <vector>

namespace cppreflect {

template <typename VecT>
struct VectorClass : public ClassTemplate
{
    using ClassTemplate::ClassTemplate;

    virtual std::string Dump(const void *instance) const override
    {
        const Type *type = GetType<typename VecT::value_type>();
        const VecT *vec = (const VecT *)instance;

        std::string r;
        r.push_back('{');
        for (auto& element : *vec)
        {
            r.append(type->Dump(&element));
            r.push_back(',');
        }
        r.push_back('}');

        return r;
    }

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override
    {
        if (!lua_istable(L, index))
        {
            return -1;
        }
        else
        {
            const Type *type = GetType<typename VecT::value_type>();
            int err = 0;
            int top = lua_gettop(L);
            lua_pushnil(L);
            int t = index > 0 ? index : index - 1; // 如果是负数情况下，pushnil之后table在index-1的位置上
            while (lua_next(L, t) != 0)
            {
                typename VecT::value_type inst;
                err = type->SetValueFromLua(L, -1, &inst);
                if (err != 0)
                {
                    break;
                }
                ((VecT*)value)->push_back(inst);
                lua_pop(L, 1);
            }
            lua_settop(L, top);
            return err;
        }
    }

    virtual int PushValueToLua(lua_State *L, const void* value) const override
    {
        const Type *type = GetType<typename VecT::value_type>();
        size_t sz = ((VecT*)value)->size();
        lua_createtable(L, sz, 0);
        for (int i = 0; i < sz; i++)
        {
            typename VecT::value_type inst = ((VecT*)value)->at(i);
            lua_pushinteger(L, i + 1);
            type->PushValueToLua(L, &inst);
            lua_settable(L, -3);
        }
        return 0;
    }

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override
    {
        const Type *type = GetType<typename VecT::value_type>();
        size_t sz = ((VecT*)value)->size();
        msgpack_pack_array(pk, sz);
        for (int i = 0; i < sz; i++)
        {
            typename VecT::value_type inst = ((VecT*)value)->at(i);
            type->PackValueToMsgpack(pk, &inst);
        }
        return 0;
    }

    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override
    {
        if (o.type != MSGPACK_OBJECT_ARRAY)
        {
            return -1;
        }
        else
        {
            const Type *type = GetType<typename VecT::value_type>();
            int err = 0;
            if (o.via.array.size != 0)
            {
                const msgpack_object* pend = o.via.array.ptr + o.via.array.size;
                for (msgpack_object *p = o.via.array.ptr; p < pend; p++)
                {
                    typename VecT::value_type inst;
                    err = type->SetValueFromMsgpack(*p, (void*)&inst);
                    if (err != 0)
                    {
                        break;
                    }
                    ((VecT*)value)->push_back(inst);
                }
            }
            return err;
        }
    }
};

namespace detail {

template<class T>
const Class* GetClassImpl(ClassTag<std::vector<T> >)
{
    static TemplateArgument vectorTemplateArg{
        TemplateArgument::Tag::Type,
        GetType<T>(),
    };

    static VectorClass<std::vector<T>> vectorClass {
        "std::vector<T>",               /* m_name */
        sizeof(std::vector<T>),         /* m_size */
        0,                              /* m_hash */
        NULL,                           /* m_baseClass */
        {},                             /* m_fields */
        {},                             /* m_functions */
        { &vectorTemplateArg, },        /* m_templateArgs */
    };

    return &vectorClass;
}

template<class T>
const Type* GetTypeImpl(TypeTag<std::vector<T> >)
{
    return GetClassImpl(ClassTag<std::vector<T>>{});
}

} /* namespace detail */
} /* namespace cppreflect */

#endif /* CPPREFLECT_VECTOR_H */