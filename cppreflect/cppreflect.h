#ifndef CPPREFLECT_H
#define CPPREFLECT_H

namespace cppreflect
{

#ifdef __CPPREFLECT__
    #define STRUCT(...) __attribute__((annotate("reflect-class;" #__VA_ARGS__)))
    #define CLASS(...) __attribute__((annotate("reflect-class;" #__VA_ARGS__)))
    #define UNION(...) __attribute__((annotate("reflect-class;" #__VA_ARGS__)))
    #define ENUM(...) __attribute__((annotate("reflect-enum;" #__VA_ARGS__)))
    #define PROPERTY(...) __attribute__((annotate("reflect-property;" #__VA_ARGS__)))
    #define FUNCTION(...) __attribute__((annotate("reflect-function;" #__VA_ARGS__)))
    #define META_OBJECT
#else
    #define STRUCT(...)
    #define CLASS(...)
    #define UNION(...)
    #define ENUM(...)
    #define PROPERTY(...)
    #define FUNCTION(...)
    #define META_OBJECT \
        static constexpr bool IsReflected = true; \
        template<class T> \
        friend const cppreflect::Class* cppreflect::detail::GetClassImpl(cppreflect::ClassTag<T>);
#endif /* __CPPREFLECT__ */

} /* namespace cppreflect */

#include "cppreflect/cppreflect_define.h"
#include "cppreflect/cppreflect_fundamental.h"
#include "cppreflect/cppreflect_string.h"
#include "cppreflect/rpc_mgr.h"
#include "cppreflect/lua_mgr.h"
#include "cppreflect/class_mgr.h"
#include "cppreflect/eventcall_mgr.h"

#endif /* CPPREFLECT_H */