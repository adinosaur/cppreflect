#ifndef CPPREFLECT_DEFINE_H
#define CPPREFLECT_DEFINE_H

#include "msgpack.h"
#include "lua.hpp"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>

namespace cppreflect
{

// ==================
// Type
// ==================
struct Type
{
    std::string m_name;
    uint64_t m_size;
    uint64_t m_hash;

    Type()
    : m_name("")
    , m_size(0)
    , m_hash(0)
    {}

    Type(const std::string& name, int size, uint64_t hash)
    : m_name(name)
    , m_size(size)
    , m_hash(hash)
    {}

    virtual bool IsClass() const { return false; }
    virtual bool IsEnum() const { return false; }

    virtual std::string Dump(const void* instance) const;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const;
    virtual int PushValueToLua(lua_State *L, const void* value) const;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const;

    const std::string& GetName() const { return m_name; }
    uint64_t GetSize() const { return m_size; }
    uint64_t GetHash() const { return m_hash; }

    bool operator==(const Type& other) const { return m_hash == other.m_hash; }
    bool operator!=(const Type& other) const { return !(*this == other); }
};


// ==================
// Enum
// ==================
struct Enum : public Type
{
    Enum(  const std::string& name,
            int size,
            uint64_t hash,
            const std::vector<std::pair<std::string, int64_t> >& items)
    : Type(name, size, hash)
    , m_items(items)
    {
    }

    virtual bool IsEnum() const override { return true; }
    
    std::vector<std::pair<std::string, int64_t> > m_items;

    /** global list of all value names used by all enums in memory */
    static std::map<std::string, Enum*> m_allEnums;

    int GetItemsSize() const { return m_items.size(); }

    std::string GetNameByIndex(int index) const;
    int64_t GetValueByIndex(int index) const;
};

// ==================
// Field
// ==================
struct Field
{
    std::string m_name;
    const Type* m_type;
    unsigned m_offset;

    Field(const std::string& name, const Type* type, unsigned offset)
    : m_name(name)
    , m_type(type)
    , m_offset(offset)
    {}

    const std::string& GetName() const { return m_name; }
    const Type* GetType() const { return m_type; }
    unsigned GetOffset() const { return m_offset; }

    template <typename T>
    void SetValue(void* instance, const T& value) const
    {
        /* assert(*TypeResolver<T>::Get() == *m_type && "type mismatch"); */
        memcpy(((char *)instance + m_offset), &value, sizeof(T));
    }

    template<class T>
    T GetAs(void const *instance) const
    {
        /* assert(*TypeResolver<T>::Get() == *m_type && "type mismatch"); */
        T ret;
        memcpy(&ret, ((char *)instance + m_offset), sizeof(T));
        return ret;
    }

    template<class T>
    T* GetPointer(void *instance) const
    {
        /* assert(*TypeResolver<T>::Get() == *m_type && "type mismatch"); */
        return static_cast<T *>((char *)instance + m_offset);
    }

    template<class T>
    const T* GetConstPointer(const void *instance) const
    {
        /* assert(*TypeResolver<T>::Get() == *m_type && "type mismatch"); */
        return static_cast<const T*>((char *)instance + m_offset);
    }

    const void* GetVoidPointer(const void *instance) const
    {
        /* assert(*TypeResolver<T>::Get() == *m_type && "type mismatch"); */
        return static_cast<void const *>((char *)instance + m_offset);
    }
};


// ==================
// Function
// ==================
template<class T>
const Type* GetType();

struct AnyObject
{
    const Type* m_type;
    std::shared_ptr<void> m_data;

    AnyObject()
    : m_type(cppreflect::GetType<void>())
    , m_data()
    {}

    template <typename T>
    AnyObject(const T& t)
    {
        m_type = cppreflect::GetType<T>();
        m_data = std::make_shared<T>(t);
    }

    const Type* GetType() const { return m_type; }

    template <typename T>
    const T& Get() const
    {
        return *(const T*) m_data.get();
    }
};

template <typename T>
AnyObject MakeAny(const T& x)
{
    return AnyObject(x);
}

typedef int (*FunctionWrapper)(void* object, const std::vector<cppreflect::AnyObject>& params, cppreflect::AnyObject& ret);

struct Function
{
    std::string m_name;
    const Type* m_returnType;
    std::vector<Field*> m_parameters;
    FunctionWrapper m_functionWrapper;
    std::map<std::string, std::string> m_attributes;

    Function(const std::string& name, const Type* returntype, const std::vector<Field*>& parameters, FunctionWrapper functionwrapper, const std::map<std::string, std::string>& attributes)
    : m_name(name)
    , m_returnType(returntype)
    , m_parameters(parameters)
    , m_functionWrapper(functionwrapper)
    , m_attributes(attributes)
    {}

    const std::string& GetName() const { return m_name; }
    const Type* GetReturnType() const { return m_returnType; }
    const std::vector<Field*>& GetParameters() const { return m_parameters; }
    int Invoke(void* object, const std::vector<AnyObject>& params, cppreflect::AnyObject& ret) const
    {
        return m_functionWrapper(object, params, ret);
    }
};


// ==================
// Class
// ==================
struct Class : public Type
{
    Class* m_baseClass;
    std::vector<Field*> m_fields;
    std::vector<Function*> m_functions;

    // cache field and function for search by name
    std::unordered_map<std::string, Field*> m_cacheFields; // name -> field
    std::unordered_map<std::string, Function*> m_cacheFunctions; // name -> function

    Class(  const std::string& name, 
            int size,
            uint64_t hash,
            Class* baseclass,
            const std::vector<Field*>& fields,
            const std::vector<Function*>& functions)
    : Type(name, size, hash)
    , m_baseClass(baseclass)
    , m_fields(fields)
    , m_functions(functions)
    {
        for (Field* field : m_fields)
            m_cacheFields.insert(std::make_pair(field->GetName(), field));
        
        for (Function* funtion : m_functions)
            m_cacheFunctions.insert(std::make_pair(funtion->GetName(), funtion));
    }

    virtual bool IsClass() const override { return true; }

    const std::string& GetName() const { return m_name; }
    const Class* GetBaseClass() const { return m_baseClass; }
    const std::vector<Field*>& GetFields() const { return m_fields; }
    const std::vector<Function*>& GetFunctions() const { return m_functions; }

    virtual std::string Dump(const void* instance) const override;

    virtual int SetValueFromLua(lua_State *L, int index, void* value) const override;
    virtual int PushValueToLua(lua_State *L, const void* value) const  override;

    virtual int PackValueToMsgpack(msgpack_packer* pk, const void* value) const override;
    virtual int SetValueFromMsgpack(const msgpack_object o, void* value) const override;

    const Field* GetFieldByName(const std::string& name) const;
    const Function* GetFunctionByName(const std::string& name) const;
};


// ==================
// Template Class
// ==================
struct TemplateArgument
{
    enum class Tag {
        Type,
        Number,
    } tag;

    union {
        Type const *type;
        uint64_t number;
    } u;
};

struct ClassTemplate : public Class
{
    ClassTemplate(  const std::string& name,
                    int size,
                    uint64_t hash,
                    Class* baseclass,
                    const std::vector<Field*>& fields,
                    const std::vector<Function*>& functions,
                    const std::vector<TemplateArgument*>& templateArgs)
    : Class(name, size, hash, baseclass, fields, functions)
    , m_templateArgs(templateArgs)
    {}

    std::vector<TemplateArgument*> m_templateArgs;
};

// ==================
// Public API
// ==================

template<class T> struct TypeTag {};
template<class T> struct ClassTag {};
template<class T> struct EnumTag {};

namespace detail {

template<class T>
const Class* GetClassImpl(ClassTag<T>);

template<class T>
const Type* GetTypeImpl(TypeTag<T>);

template<class T>
const Enum* GetEnumImpl(EnumTag<T>);

} /* namespace detail */

template<class T>
const Class* GetClass();

template<class T>
const Type* GetType();

template<class T>
const Enum* GetEnum();

} /* namespace cppreflect */

#include "cppreflect/cppreflect_vector.h"
#include "cppreflect/cppreflect_map.h"
#include "cppreflect/cppreflect_unordered_map.h"
#include "cppreflect/cppreflect_tuple.h"

namespace cppreflect {

template<class T>
const Class* GetClass()
{
    return detail::GetClassImpl(ClassTag<T>{});
}

template<class T>
const Type* GetType()
{
    return detail::GetTypeImpl(TypeTag<T>{});
}

template<class T>
const Enum* GetEnum()
{
    return detail::GetEnumImpl(EnumTag<T>{});
}



} /* namespace cppreflect */

#endif /* CPPREFLECT_DEFINE_H */