
#ifndef YGGD_RPC_MGR_H
#define YGGD_RPC_MGR_H

#include <string>
#include <map>
#include "cppreflect/cppreflect.h"

typedef int (*RpcFunctionType)(void* object, const char* data, std::size_t len);

struct RpcStruct
{
    std::string m_RpcName;
    RpcFunctionType m_ExecFunc;
    const cppreflect::Class* m_Clazz;
    const cppreflect::Function* m_Func;
    std::map<std::string, std::string> m_Attributes;
};

struct RpcMgr
{
    /* GetInstance */
    static RpcMgr* GetInstance();

    /* 注册RPC函数 */
    void RegisterRpc(const std::string& RpcName, const RpcStruct& RpcStruct);

    /* 调用RPC函数 */
    int InvokeRpcFunction(const std::string& RpcName, void* object, const char* data, std::size_t len);

    /* RPC名字索引函数 */
    std::map<std::string, RpcStruct> m_RpcMap;
};

#endif /* YGGD_RPC_MGR_H */