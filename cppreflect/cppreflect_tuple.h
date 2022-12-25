#ifndef CPPREFLECT_TUPLE_H
#define CPPREFLECT_TUPLE_H

#include "cppreflect/cppreflect_define.h"
#include <string>
#include <tuple>

namespace cppreflect {

namespace detail {

// ================================ //
// Tuple Helper                     //
// ================================ //

// ============ Get Args ============
template<std::size_t _Idx>
void _TupleHelper_GetArgs(std::vector<TemplateArgument*>& templateArgs) {}

template<std::size_t _Idx, typename _Head, typename... _Tail>
void _TupleHelper_GetArgs(std::vector<TemplateArgument*>& templateArgs)
{
    static TemplateArgument _templateArg{
        TemplateArgument::Tag::Type,
        GetType<_Head>(),
    };
    templateArgs.push_back(&_templateArg);
    _TupleHelper_GetArgs<_Idx+1, _Tail...>(templateArgs);
}

// ============ Dump ============
template<typename TupleType, std::size_t _Idx>
void _TupleHelper_Dump(const TupleType& t, std::string& output) {}

template<typename TupleType, std::size_t _Idx, typename _Head, typename... _Tail>
void _TupleHelper_Dump(const TupleType& t, std::string& output)
{
    const _Head& head = std::get<_Idx>(t);
    output.append(GetType<_Head>()->Dump(&head));
    output.push_back(',');
    _TupleHelper_Dump<TupleType, _Idx+1, _Tail...>(t, output);
}

// ============ SetValueFromLua ============
template<typename TupleType, std::size_t _Idx>
int _TupleHelper_SetValueFromLua(TupleType& t, lua_State* L, int table_index)
{
    return 0;
}

template<typename TupleType, std::size_t _Idx, typename _Head, typename... _Tail>
int _TupleHelper_SetValueFromLua(TupleType& t, lua_State* L, int table_index)
{
    if (lua_next(L, table_index) == 0)
    {
        return 0; // 遍历完table
    }

    _Head& head = std::get<_Idx>(t);
    int err = GetType<_Head>()->SetValueFromLua(L, -1, &head);
    if (err != 0)
    {
        return err;
    }

    lua_pop(L, 1);

    return _TupleHelper_SetValueFromLua<TupleType, _Idx+1, _Tail...>(t, L, table_index);
}

// ============ PushValueToLua ============
template<typename TupleType, std::size_t _Idx>
int _TupleHelper_PushValueToLua(const TupleType& t, lua_State *L)
{
    return 0;
}

template<typename TupleType, std::size_t _Idx, typename _Head, typename... _Tail>
int _TupleHelper_PushValueToLua(const TupleType& t, lua_State *L)
{
    lua_pushinteger(L, _Idx + 1);
    const _Head& head = std::get<_Idx>(t);
    int err = GetType<_Head>()->PushValueToLua(L, &head);
    if (err != 0)
    {
        return err;
    }

    lua_settable(L, -3);
    
    return _TupleHelper_PushValueToLua<TupleType, _Idx+1, _Tail...>(t, L);
}

// ============ PackValueToMsgpack ============
template<typename TupleType, std::size_t _Idx>
int _TupleHelper_PackValueToMsgpack(const TupleType& t, msgpack_packer* pk)
{
    return 0;
}

template<typename TupleType, std::size_t _Idx, typename _Head, typename... _Tail>
int _TupleHelper_PackValueToMsgpack(const TupleType& t, msgpack_packer* pk)
{
    const _Head& head = std::get<_Idx>(t);
    int err = GetType<_Head>()->PackValueToMsgpack(pk, &head);
    if (err != 0)
    {
        return err;
    }
    
    return _TupleHelper_PackValueToMsgpack<TupleType, _Idx+1, _Tail...>(t, pk);
}

// ============ SetValueFromMsgpack ============
template<typename TupleType, std::size_t _Idx>
int _TupleHelper_SetValueFromMsgpack(TupleType& t, const msgpack_object o)
{
    return 0;
}

template<typename TupleType, std::size_t _Idx, typename _Head, typename... _Tail>
int _TupleHelper_SetValueFromMsgpack(TupleType& t, const msgpack_object o)
{
    if (_Idx >= o.via.array.size)
    {
        return 0;
    }

    _Head& head = std::get<_Idx>(t);
    msgpack_object *p = &o.via.array.ptr[_Idx];
    int err = GetType<_Head>()->SetValueFromMsgpack(*p, &head);
    if (err != 0)
    {
        return err;
    }

    return _TupleHelper_SetValueFromMsgpack<TupleType, _Idx+1, _Tail...>(t, o);
}

} /* namespace detail */

template <typename TupleType, typename ... Args>
struct TupleClass : public ClassTemplate
{
    using ClassTemplate::ClassTemplate;

    virtual std::string Dump(const void *instance) const override
    {
        std::string output;
        output.push_back('{');
        detail::_TupleHelper_Dump<TupleType, 0, Args...>(*(TupleType*)instance, output);
        output.push_back('}');
        return output;
    }

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override
    {
        if (!lua_istable(L, index))
        {
            return -1;
        }

        int top = lua_gettop(L);
        lua_pushnil(L);
        int t = index > 0 ? index : index - 1; // 如果是负数情况下，pushnil之后table在index-1的位置上
        int err = detail::_TupleHelper_SetValueFromLua<TupleType, 0, Args...>(*(TupleType*)value, L, t);
        lua_settop(L, top);
        return err;
    }

    virtual int PushValueToLua(lua_State *L, const void* value) const override
    {
        size_t sz = std::tuple_size<TupleType>{};
        lua_createtable(L, sz, 0);
        int err = detail::_TupleHelper_PushValueToLua<TupleType, 0, Args...>(*(TupleType*)value, L);
        return err;
    }

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override
    {
        size_t sz = std::tuple_size<TupleType>{};
        msgpack_pack_array(pk, sz);
        int err = detail::_TupleHelper_PackValueToMsgpack<TupleType, 0, Args...>(*(TupleType*)value, pk);
        return err;
    }

    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override
    {
        if (o.type != MSGPACK_OBJECT_ARRAY)
        {
            return -1;
        }

        int err = detail::_TupleHelper_SetValueFromMsgpack<TupleType, 0, Args...>(*(TupleType*)value, o);
        return 0;
    }
};

namespace detail {

template<class ... Args>
const Class* GetClassImpl(ClassTag<std::tuple<Args...> >)
{
    static std::vector<TemplateArgument*> templateArgs;
    detail::_TupleHelper_GetArgs<0, Args...>(templateArgs);

    static TupleClass<std::tuple<Args...>, Args...> tupleClass {
        "std::tuple<T...>",             /* m_name */
        sizeof(std::tuple<Args...>),    /* m_size */
        0,                              /* m_hash */
        NULL,                           /* m_baseClass */
        {},                             /* m_fields */
        {},                             /* m_functions */
        templateArgs,                   /* m_templateArgs */
    };

    return &tupleClass;
}

template<class... Args>
const Type* GetTypeImpl(TypeTag<std::tuple<Args...> >)
{
    return GetClassImpl(ClassTag<std::tuple<Args...>>{});
}

} /* namespace detail */
} /* namespace cppreflect */

#endif /* CPPREFLECT_TUPLE_H */