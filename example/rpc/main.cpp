#include "example/rpc/test.h"
#include "cppreflect/rpc_mgr.h"

#include <map>

int main()
{
    Vector3d v3d{1,2,3,4};
    TestService::Print(1, 2.33, v3d);

    {
        PrintParams params;
        params.i = 3;
        params.d = 6.55;
        params.v3d = v3d;

        msgpack_sbuffer* buffer = msgpack_sbuffer_new();
        msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

        cppreflect::GetType<PrintParams>()->PackValueToMsgpack(pk, (void*)&params);

        int err = RpcMgr::GetInstance()->InvokeRpcFunction("Print", nullptr, buffer->data, buffer->size);
        printf("err:%d\n", err);
    }

    {
        Print2Params params;
        params.int2intmap[10] = 999;
        params.int2intmap[20] = 1111;

        msgpack_sbuffer* buffer = msgpack_sbuffer_new();
        msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

        cppreflect::GetType<Print2Params>()->PackValueToMsgpack(pk, (void*)&params);
        int err = RpcMgr::GetInstance()->InvokeRpcFunction("Print2", nullptr, buffer->data, buffer->size);
        printf("err:%d\n", err);
    }

    {
        Print3Params params;
        params.i = 999;
        params.t = std::make_tuple(1001, 2002);
        params.s = "hehh";

        msgpack_sbuffer* buffer = msgpack_sbuffer_new();
        msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

        cppreflect::GetType<Print3Params>()->PackValueToMsgpack(pk, (void*)&params);
        int err = RpcMgr::GetInstance()->InvokeRpcFunction("Print3", nullptr, buffer->data, buffer->size);
        printf("err:%d\n", err);

        const cppreflect::Class* clazz = cppreflect::GetClass<Print3Params>();
        for (const cppreflect::Field* field : clazz->m_fields)
        {
            printf("field name=%s\n", field->m_name.c_str());
            printf("field offset=%u\n", field->m_offset);
        }

        const cppreflect::Class* c = cppreflect::GetClass<std::tuple<int, std::string>>();
        const cppreflect::ClassTemplate* tc = dynamic_cast<const cppreflect::ClassTemplate*>(c);
        for (const cppreflect::TemplateArgument* ta : tc->m_templateArgs)
        {
            printf("type: %s\n", ta->u.type->m_name.c_str());
        }
    }

    {
        Print4Params params;
        params.i = 999;
        params.t = std::make_tuple(1001, 2002);
        params.s = "hehh";

        msgpack_sbuffer* buffer = msgpack_sbuffer_new();
        msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

        cppreflect::GetType<Print4Params>()->PackValueToMsgpack(pk, (void*)&params);
        int err = RpcMgr::GetInstance()->InvokeRpcFunction("Print4", TestService::GetInstance(), buffer->data, buffer->size);
        printf("err:%d\n", err);

        const cppreflect::Class* clazz = cppreflect::GetClass<Print4Params>();
        for (const cppreflect::Field* field : clazz->m_fields)
        {
            printf("field name=%s\n", field->m_name.c_str());
            printf("field offset=%u\n", field->m_offset);
        }

        const cppreflect::Class* c = cppreflect::GetClass<std::tuple<int, std::string>>();
        const cppreflect::ClassTemplate* tc = dynamic_cast<const cppreflect::ClassTemplate*>(c);
        for (const cppreflect::TemplateArgument* ta : tc->m_templateArgs)
        {
            printf("type: %s\n", ta->u.type->m_name.c_str());
        }
    }

    return 0;
}