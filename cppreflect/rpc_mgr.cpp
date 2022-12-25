#include "cppreflect/rpc_mgr.h"

static RpcMgr* RpcMgrInstance = NULL;

RpcMgr* RpcMgr::GetInstance()
{
    if (RpcMgrInstance == NULL)
    {
        RpcMgrInstance = new RpcMgr();
    }
    return RpcMgrInstance;
}

void RpcMgr::RegisterRpc(const std::string& RpcName, const RpcStruct& RpcStruct)
{
    m_RpcMap[RpcName] = RpcStruct;
}

int RpcMgr::InvokeRpcFunction(const std::string& RpcName, void* object, const char* data, std::size_t len)
{
    std::map<std::string, RpcStruct>::iterator it = m_RpcMap.find(RpcName);
    if (it != m_RpcMap.end())
    {
        RpcStruct* rpc = &it->second;
        return rpc->m_ExecFunc(object, data, len);
    }
    return -1;
}