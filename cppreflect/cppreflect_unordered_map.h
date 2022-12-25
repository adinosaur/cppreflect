#ifndef CPPREFLECT_UNORDERED_MAP_H
#define CPPREFLECT_UNORDERED_MAP_H

#include "cppreflect/cppreflect_define.h"
#include <unordered_map>

namespace cppreflect {

template <typename MapT>
struct UnorderedMapClass : public ClassTemplate
{
    using ClassTemplate::ClassTemplate;

    virtual std::string Dump(const void *instance) const override
    {
        const Type *key_type = GetType<typename MapT::key_type>();
        const Type *mapped_type = GetType<typename MapT::mapped_type>();
        const MapT *m = (const MapT *)instance;

        std::string r;
        r.push_back('{');
        for (auto& p : *m)
        {
            r.append(key_type->Dump(&p.first));
            r.push_back('=');
            r.append(mapped_type->Dump(&p.second));
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
            int err = 0;

            const Type *key_type = GetType<typename MapT::key_type>();
            const Type *mapped_type = GetType<typename MapT::mapped_type>();

            int top = lua_gettop(L);
            lua_pushnil(L);
            int t = index > 0 ? index : index - 1; // 如果是负数情况下，pushnil之后table在index-1的位置上
            while (lua_next(L, t) != 0)
            {
                typename MapT::key_type k;
                err = key_type->SetValueFromLua(L, -2, &k);
                if (err != 0)
                {
                    break;
                }

                typename MapT::mapped_type v;
                err = mapped_type->SetValueFromLua(L, -1, &v);
                if (err != 0)
                {
                    break;
                }

                ((MapT*)value)->insert(std::make_pair(k, v));
                lua_pop(L, 1);
            }
            lua_settop(L, top);
            return err;
        }
    }

    virtual int PushValueToLua(lua_State *L, const void* value) const override
    {
        const Type *key_type = GetType<typename MapT::key_type>();
        const Type *mapped_type = GetType<typename MapT::mapped_type>();
        size_t sz = ((MapT*)value)->size();
        lua_createtable(L, 0, sz);
        for (auto& it : *(MapT*)value)
        {
            key_type->PushValueToLua(L, &it.first);
            mapped_type->PushValueToLua(L, &it.second);
            lua_settable(L, -3);
        }
        return 0;
    }

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override
    {
        const Type *key_type = GetType<typename MapT::key_type>();
        const Type *mapped_type = GetType<typename MapT::mapped_type>();

        size_t sz = ((MapT*)value)->size();
        msgpack_pack_map(pk, sz);
        for (auto& it : *(MapT*)value)
        {
            key_type->PackValueToMsgpack(pk, &it.first);
            mapped_type->PackValueToMsgpack(pk, &it.second);
        }
        return 0;
    }

    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override
    {
        if (o.type != MSGPACK_OBJECT_MAP)
        {
            return -1;
        }
        else
        {
            int err = 0;

            const Type *key_type = GetType<typename MapT::key_type>();
            const Type *mapped_type = GetType<typename MapT::mapped_type>();

            if (o.via.map.size != 0)
            {
                const msgpack_object_kv* pend = o.via.map.ptr + o.via.map.size;
                for (msgpack_object_kv *p = o.via.map.ptr; p < pend; p++)
                {
                    typename MapT::key_type k;
                    err = key_type->SetValueFromMsgpack(p->key, &k);
                    if (err != 0)
                    {
                        break;
                    }

                    typename MapT::mapped_type v;
                    err = mapped_type->SetValueFromMsgpack(p->val, &v);
                    if (err != 0)
                    {
                        break;
                    }

                    ((MapT*)value)->insert(std::make_pair(k, v));
                }
            }
            return err;
        }
    }
};

namespace detail {

template<class K, class V>
const Class* GetClassImpl(ClassTag<std::unordered_map<K, V> >)
{
    static TemplateArgument mapKeyTemplateArg{
        TemplateArgument::Tag::Type,
        GetType<K>(),
    };

    static TemplateArgument mapValueTemplateArg{
        TemplateArgument::Tag::Type,
        GetType<V>(),
    };

    static MapClass<std::unordered_map<K, V> > mapClass {
        "std::unordered_map<K, V>",               /* m_name */
        sizeof(std::unordered_map<K, V>),         /* m_size */
        0,                              /* m_hash */
        NULL,                           /* m_baseClass */
        {},                             /* m_fields */
        {},                             /* m_functions */
        { &mapKeyTemplateArg, &mapValueTemplateArg },        /* m_templateArgs */
    };

    return &mapClass;
}

template<class K, class V>
const Type* GetTypeImpl(TypeTag<std::unordered_map<K, V> >)
{
    return GetClassImpl(ClassTag<std::unordered_map<K, V> >{});
}

} /* namespace detail */
} /* namespace cppreflect */

#endif /* CPPREFLECT_UNORDERED_MAP_H */