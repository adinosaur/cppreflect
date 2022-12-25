#include "ClassFinder.h"

#include "llvm/ADT/SmallString.h"
#include "llvm/Support/raw_ostream.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>

QualType GetDesugaredType(ASTContext *ctx, QualType t)
{
    auto type = t.split().Ty;

    clang::BuiltinType const *builtin = type->getAs<clang::BuiltinType>();
    if (builtin) {
        return QualType(builtin, 0);
    }

    clang::RecordType const *record = type->getAs<clang::RecordType>();
    if (record) {
        return QualType(record, 0);
    }

    /* Fallback to traversing the type manually. */
    // TypeVisitor visitor(ctx);
    // visitor.TraverseType(t);
    // return QualType(visitor.m_leaf, 0);
    return QualType();
}

inline void GetRealTypeName(ASTContext *ctx, QualType t, raw_ostream &os)
{
    QualType retType = GetDesugaredType(ctx, t);
    retType.print(os, ctx->getPrintingPolicy());
}

inline std::string ConvertString(StringRef raw)
{
    std::string out;
    for (char c : raw)
    {
        if (c == '\"')
            out.push_back('\\');
        out.push_back(c);
    }
    return out;
}

void ReflectedClass::Generate(ASTContext* Context, raw_ostream &OS)
{
    for (const FunctionDecl* Function : m_Functions)
    {
        GenerateInvokeCxxFunction(Context, OS, Function);
        GenerateInvokeRpcFunction(Context, OS, Function);
    }
    GenerateLuaFunctions(Context, OS);
    GenerateMetaClass(Context, OS);
}


void GetExtendAnnotationAttr(std::map<StringRef, StringRef>& OutAttrMap, const Decl* InMemberDecl)
{
    AnnotateAttr* Attr = InMemberDecl->getAttr<AnnotateAttr>();
    if (Attr == NULL)
    {
        return;
    }
    
    StringRef Annotation = Attr->getAnnotation();

    size_t i;
    for (i = 0; i < Annotation.size(); i++)
    {
        if (Annotation[i] == ';')
        {
            break;
        }
    }

    StringRef ExtendAnnotation = Annotation.slice(i+1, Annotation.size());
    SmallVector<StringRef, 10> ExtendAnnotationVec;
    ExtendAnnotation.split(ExtendAnnotationVec, ',');
    for (size_t i = 0; i < ExtendAnnotationVec.size(); i++)
    {
        std::pair<StringRef, StringRef> p = ExtendAnnotationVec[i].trim().split('=');
        if (!p.first.empty())
            OutAttrMap.insert(p);
    }
}


// =================
// 为Class生成运行时动态调用c++函数的代码
// =================
void ReflectedClass::GenerateInvokeCxxFunction(ASTContext* Context, raw_ostream &OS, const FunctionDecl* Function)
{
    SmallString<64> TypeName;
    raw_svector_ostream TypeNameOS(TypeName);
    m_Record->printQualifiedName(TypeNameOS);

    SmallString<64> FunctionName;
    SmallString<64> ReturnTypeName;

    raw_svector_ostream FunctionNameOS{FunctionName};
    raw_svector_ostream ReturnTypeNameOS{ReturnTypeName};
    
    Function->printName(FunctionNameOS);
    GetRealTypeName(Context, Function->getReturnType(), ReturnTypeNameOS);

    OS << "// ===========================\n"
       << "// CxxFunction (" << TypeName << "::" << FunctionName << ")\n"
       << "// ===========================\n"
       << "static int ExecFunction_" << TypeName << "_" << FunctionName << "(void* object, const std::vector<cppreflect::AnyObject>& params, cppreflect::AnyObject& ret)\n"
       << "{\n"
       << "    int err;\n\n";
    
    const CXXMethodDecl* method = static_cast<const CXXMethodDecl*>(Function);

    int ParamsNum = Function->getNumParams();
    if (ParamsNum == 0)
    {
        OS << "    /* call cxx function */\n";
        if (Function->getReturnType()->isVoidType())
        {
            if (method->isStatic())
            {
                OS << "    " << TypeName << "::" << FunctionName << "();\n";
            }
            else
            {
                OS << "    ((" << TypeName << "*)object)->" << FunctionName << "();\n";
            }
        }
        else
        {
            if (method->isStatic())
            {
                OS << "    ret = " << TypeName << "::" << FunctionName << "();\n";
            }
            else
            {
                OS << "    ret = ((" << TypeName << "*)object)->" << FunctionName << "();\n";
            }
        }
        OS << "\n"
           << "    return 0;\n"
           << "}\n\n";
    }
    else
    {
        OS << "    /* params */\n";
        for (int i = 0; i < ParamsNum; ++i)
        {
            const clang::ParmVarDecl *Param = Function->getParamDecl(i);

            SmallString<64> ParamName;
            SmallString<64> ParamTypeName;

            raw_svector_ostream ParamNameOS{ParamName};
            raw_svector_ostream ParamTypeNameOS{ParamTypeName};

            Param->printName(ParamNameOS);
            GetRealTypeName(Context, Param->getOriginalType(), ParamTypeNameOS);

            OS << "    " << ParamTypeName << " " << ParamName << ";\n";
        }

        OS << "\n"
           << "    /* check params count */\n"
           << "    if (params.size() < " << ParamsNum << ")\n"
           << "    {\n"
           << "        return -1;\n"
           << "    }\n\n";

        for (int i = 0; i < ParamsNum; ++i)
        {
            const clang::ParmVarDecl *Param = Function->getParamDecl(i);

            SmallString<64> ParamName;
            SmallString<64> ParamTypeName;

            raw_svector_ostream ParamNameOS{ParamName};
            raw_svector_ostream ParamTypeNameOS{ParamTypeName};

            Param->printName(ParamNameOS);
            GetRealTypeName(Context, Param->getOriginalType(), ParamTypeNameOS);

            OS << "    /* param" << i+1 << ": " << ParamName << " */\n"
               << "    if (cppreflect::GetType<" << ParamTypeName << ">() != params[" << i << "].GetType())\n"
               << "    {\n"
               << "        return -" << i+2 << ";\n"
               << "    }\n"
               << "\n"
               << "    " << ParamName << " = params[" << i << "].Get<" << ParamTypeName << ">();\n"
               << "\n";
        }

        OS << "    /* call cxx function */\n";
        if (Function->getReturnType()->isVoidType())
        {
            if (method->isStatic())
            {
                OS << "    " << TypeName << "::" << FunctionName << "(";
            }
            else
            {
                OS << "    ((" << TypeName << "*)object)->" << FunctionName << "(";
            }
        }
        else
        {
            if (method->isStatic())
            {
                OS << "    ret = " << TypeName << "::" << FunctionName << "(";
            }
            else
            {
                OS << "    ret = ((" << TypeName << "*)object)->" << FunctionName << "(";
            }
        }

        for (int i = 0; i < ParamsNum; ++i)
        {
            const clang::ParmVarDecl *Param = Function->getParamDecl(i);

            SmallString<64> ParamName;
            SmallString<64> ParamTypeName;

            raw_svector_ostream ParamNameOS{ParamName};
            raw_svector_ostream ParamTypeNameOS{ParamTypeName};

            Param->printName(ParamNameOS);
            GetRealTypeName(Context, Param->getOriginalType(), ParamTypeNameOS);

            OS << ParamName;
            if (i + 1 < ParamsNum)
            {
                OS << ", ";
            }
        }

        OS << ");\n"
           << "\n"
           << "    return 0;\n"
           << "}\n\n";
    }
}


// =================
// 为Class生成RPC函数的代码（msgpack-c）
// =================
void ReflectedClass::GenerateInvokeRpcFunction(ASTContext* Context, raw_ostream &OS, const FunctionDecl* Function)
{
    SmallString<64> TypeName;
    raw_svector_ostream TypeNameOS(TypeName);
    m_Record->printQualifiedName(TypeNameOS);

    SmallString<64> FunctionName;
    SmallString<64> ReturnTypeName;

    raw_svector_ostream FunctionNameOS{FunctionName};
    raw_svector_ostream ReturnTypeNameOS{ReturnTypeName};
    
    Function->printName(FunctionNameOS);
    GetRealTypeName(Context, Function->getReturnType(), ReturnTypeNameOS);

    std::map<StringRef, StringRef> OutAttrMap;
    GetExtendAnnotationAttr(OutAttrMap, Function);
    auto it = OutAttrMap.find("RpcCallable");
    if (it == OutAttrMap.end())
    {
        return;
    }

    OS << "// ===========================\n"
       << "// RpcFunction (" << TypeName << "::" << FunctionName << ")\n"
       << "// ===========================\n"
       << "static int ExecRpcFunction_" << TypeName << "_" << FunctionName << "(void* object, const char* data, std::size_t len)\n"
       << "{\n"
       << "    int err;\n\n";
    
    int ParamsNum = Function->getNumParams();
    if (ParamsNum == 0)
    {
        OS << "    /* call rpc function */\n"
           << "    int ret =  " << TypeName << "::" << FunctionName << "();\n"
           << "\n"
           << "    return ret;\n";
    }
    else
    {
        OS << "    /* params */\n";
        for (int i = 0; i < ParamsNum; ++i)
        {
            const clang::ParmVarDecl *Param = Function->getParamDecl(i);

            SmallString<64> ParamName;
            SmallString<64> ParamTypeName;

            raw_svector_ostream ParamNameOS{ParamName};
            raw_svector_ostream ParamTypeNameOS{ParamTypeName};

            Param->printName(ParamNameOS);
            GetRealTypeName(Context, Param->getOriginalType(), ParamTypeNameOS);

            OS << "    " << ParamTypeName << " " << ParamName << ";\n";
        }

        OS << "\n"
           << "    /* unpack */\n"
           << "    msgpack_zone z;\n"
           << "    msgpack_object obj;\n"
           << "\n"
           << "    msgpack_zone_init(&z, 1023);\n"
           << "    msgpack_unpack(data, len, NULL, &z, &obj);\n"
           << "\n"
           << "    /* check params type */\n"
           << "    if (obj.type != MSGPACK_OBJECT_MAP)\n"
           << "    {\n"
           << "        msgpack_zone_destroy(&z);\n"
           << "        return -1;\n"
           << "    }\n"
           << "\n"
           << "    /* param_name -> msgpack_object_kv* */\n"
           << "    std::map<std::string, msgpack_object_kv*> m;\n"
           << "    if (obj.via.map.size != 0)\n"
           << "    {\n"
           << "        const msgpack_object_kv* pend = obj.via.map.ptr + obj.via.map.size;\n"
           << "        for (msgpack_object_kv *p = obj.via.map.ptr; p < pend; p++)\n"
           << "        {\n"
           << "            if (p->key.type == MSGPACK_OBJECT_STR)\n"
           << "            {\n"
           << "                std::string k(p->key.via.str.ptr, p->key.via.str.size);\n"
           << "                m[k] = p;\n"
           << "            }\n"
           << "        }\n"
           << "    }\n"
           << "\n";

        for (int i = 0; i < ParamsNum; ++i)
        {
            const clang::ParmVarDecl *Param = Function->getParamDecl(i);

            SmallString<64> ParamName;
            SmallString<64> ParamTypeName;

            raw_svector_ostream ParamNameOS{ParamName};
            raw_svector_ostream ParamTypeNameOS{ParamTypeName};

            Param->printName(ParamNameOS);
            GetRealTypeName(Context, Param->getOriginalType(), ParamTypeNameOS);

            OS << "    /* param" << i+1 << ": " << ParamName << " */\n"
               << "    {\n"
               << "        const std::map<std::string, msgpack_object_kv*>::iterator it = m.find(\"" << ParamName << "\");\n"
               << "        if (it == m.end())\n"
               << "        {\n"
               << "            msgpack_zone_destroy(&z);\n"
               << "            return -1;\n"
               << "        }\n"
               << "\n"
               << "        const msgpack_object_kv* p = it->second;\n"
               << "        err = cppreflect::GetType<" << ParamTypeName << ">()->SetValueFromMsgpack(p->val, (void*)&" << ParamName << ");\n"
               << "        if (err != 0)\n"
               << "        {\n"
               << "            msgpack_zone_destroy(&z);\n"
               << "            return -1;\n"
               << "        }\n"
               << "    }\n"
               << "\n";
        }

        OS << "    /* call rpc function */\n";
        const CXXMethodDecl* method = static_cast<const CXXMethodDecl*>(Function);
        if (method->isStatic())
        {
            OS << "    int ret = " << TypeName << "::" << FunctionName << "(";
        }
        else
        {
            OS << "    int ret = CPPREFLECT_INVOKE_NON_STATIC_RPC(" << TypeName << ", " << FunctionName << ", object, ";
        }

        for (int i = 0; i < ParamsNum; ++i)
        {
            const clang::ParmVarDecl *Param = Function->getParamDecl(i);

            SmallString<64> ParamName;
            SmallString<64> ParamTypeName;

            raw_svector_ostream ParamNameOS{ParamName};
            raw_svector_ostream ParamTypeNameOS{ParamTypeName};

            Param->printName(ParamNameOS);
            GetRealTypeName(Context, Param->getOriginalType(), ParamTypeNameOS);

            OS << ParamName;
            if (i + 1 < ParamsNum)
            {
                OS << ", ";
            }
        }
        OS << ");\n"
           << "\n"
           << "    /* free */\n"
           << "    msgpack_zone_destroy(&z);\n"
           << "    return ret;\n"
           << "}\n\n";
    }

    OS << "static EventCall RegisterRpcFunction_" << TypeName << "_" << FunctionName << "(\"BeforeMain\", [](){\n"
       << "    RpcStruct rpcStruct;\n\n"
       << "    rpcStruct.m_RpcName = \"" << FunctionName << "\"; /* Rpc Name */\n"
       << "    rpcStruct.m_ExecFunc = ExecRpcFunction_" << TypeName << "_" << FunctionName << "; /* EntryPoint */\n"
       << "    rpcStruct.m_Clazz = cppreflect::GetClass<" << TypeName << ">(); /* reflect class object */ \n"
       << "    rpcStruct.m_Func = cppreflect::GetClass<" << TypeName << ">()->GetFunctionByName(\"" << FunctionName << "\"); /* reflect function object */\n\n";

    OS << "    RpcMgr::GetInstance()->RegisterRpc(\"" << FunctionName << "\", rpcStruct);\n"
       << "});\n\n\n";
}


// =================
// 为Class生成元数据代码
// =================
void ReflectedClass::GenerateMetaClass(ASTContext* Context, raw_ostream &OS)
{
    SmallString<64> TypeName;
    raw_svector_ostream TypeNameOS(TypeName);
    m_Record->printQualifiedName(TypeNameOS);

    OS << "// ===========================\n"
       << "// MetaClass (" << TypeName << ")\n"
       << "// ===========================\n"
       << "namespace cppreflect\n"
       << "{\n"
       << "namespace detail\n"
       << "{\n\n"
       << "template <>\n"
       << "const Type* GetTypeImpl(TypeTag<" << TypeName << ">)\n"
       << "{\n"
       << "    return GetClass<" << TypeName << ">();\n"
       << "}\n\n"
       << "template <>\n"/* m_name */
       << "const Class* GetClassImpl(ClassTag<" << TypeName << ">)\n"
       << "{\n"
       << "    std::vector<Field*> fields;\n"
       << "    std::vector<Function*> functions;\n"
       << "\n";
    
    for (const FieldDecl* Field : m_Fields)
    {
        SmallString<64> FieldName;
        SmallString<64> FieldTypeName;

        raw_svector_ostream FieldNameOS{FieldName};
        raw_svector_ostream FieldTypeNameOS{FieldTypeName};
        
        Field->printName(FieldNameOS);
        GetRealTypeName(Context, Field->getType(), FieldTypeNameOS);

        OS << "    /* field: " << FieldTypeName << " " << FieldName << " */\n"
           << "    static Field __field_" << FieldName << " {\n"
           << "        \"" << FieldName << "\", /* m_name */\n"
           << "        GetType<" << FieldTypeName << ">(), /* m_type */\n"
           << "        offsetof(" << TypeName << ", " << FieldName << "), /* m_offset */\n"
           << "    };\n"
           << "    fields.push_back(&__field_" << FieldName <<");\n\n";
    }

    for (const FunctionDecl* Function : m_Functions)
    {
        SmallString<64> FunctionName;
        SmallString<64> ReturnTypeName;

        raw_svector_ostream FunctionNameOS{FunctionName};
        raw_svector_ostream ReturnTypeNameOS{ReturnTypeName};
        
        Function->printName(FunctionNameOS);
        GetRealTypeName(Context, Function->getReturnType(), ReturnTypeNameOS);

        // 函数参数
        OS << "    {\n"
           << "        std::vector<Field*> __function_params;\n\n";

        for (int i = 0, n = Function->getNumParams(); i < n; ++i)
        {
            const clang::ParmVarDecl *Param = Function->getParamDecl(i);

            SmallString<64> ParamName;
            SmallString<64> ParamTypeName;

            raw_svector_ostream ParamNameOS{ParamName};
            raw_svector_ostream ParamTypeNameOS{ParamTypeName};

            Param->printName(ParamNameOS);
            GetRealTypeName(Context, Param->getOriginalType(), ParamTypeNameOS);

            OS << "        /* function param: " << ParamTypeName << " " << ParamName << " */\n"
               << "        static Field __function_param_" << FunctionName << "_" << ParamName << " {\n"
               << "            \"" << ParamName << "\", /* m_name */\n"
               << "            GetType<" << ParamTypeName << ">(), /* m_type */\n"
               << "            0, /* m_offset */\n"
               << "        };\n"
               << "        __function_params.push_back(&__function_param_" << FunctionName << "_" << ParamName << ");\n\n";
        }

        // 函数属性
        std::map<StringRef, StringRef> OutAttrMap;
        GetExtendAnnotationAttr(OutAttrMap, Function);

        OS << "        std::map<std::string, std::string> __function_attributes;\n";
        if (!OutAttrMap.empty())
        {
            for (std::pair<StringRef, StringRef> attr : OutAttrMap)
            {
                OS << "        __function_attributes[\"" << ConvertString(attr.first) << "\"] = \"" << ConvertString(attr.second) << "\";\n";
            }
        }

        OS << "\n"
           << "        /* function: " << ReturnTypeName << " " << FunctionName << " */\n"
           << "        static Function __function_" << FunctionName << " {\n"
           << "            \"" << FunctionName << "\", /* m_name */\n"
           << "            GetType<" << ReturnTypeName << ">(), /* m_returnType */\n"
           << "            __function_params, /* m_parameters */ \n"
           << "            ExecFunction_" << TypeName << "_" << FunctionName << ", /* m_functionWrapper */ \n"
           << "            __function_attributes, /* m_attributes */\n"
           << "        };\n"
           << "        functions.push_back(&__function_" << FunctionName <<");\n"
           << "    }\n\n";
    }

    OS << "    /* class: " << TypeName << " */\n"
       << "    static Class __class_" << TypeName <<" {\n"
       << "        \"" << TypeName << "\", /* m_name */\n"
       << "        sizeof(" << TypeName << "), /* m_size */\n"
       << "        0, /* m_hash */\n"
       << "        NULL, /* m_baseClass NOT SUPPORT NOW!!! */\n"
       << "        fields, /* m_fields */\n"
       << "        functions, /* m_functions */\n"
       << "    };\n"
       << "    return &__class_" << TypeName << ";\n"
       << "}\n"
       << "\n"
       << "} /* namespace: detail */\n"
       << "} /* namespace: cppreflect */\n\n"
       << "static EventCall RegisterClass_" << TypeName << "(\"BeforeMain\", [](){\n"
       << "    const cppreflect::Class* clazz = cppreflect::GetClass<" << TypeName << ">();\n"
       << "    ClassMgr::GetInstance()->RegisterClass(\"" << TypeName << "\", clazz);\n"
       << "});\n\n";
}

// =================
// 为Class生成Lua接口函数的代码
// =================
void ReflectedClass::GenerateLuaFunctions(ASTContext* Context, raw_ostream &OS)
{
    SmallString<64> TypeName;
    raw_svector_ostream TypeNameOS(TypeName);
    m_Record->printQualifiedName(TypeNameOS);

    std::vector<const FunctionDecl*> LuaFunctions;

    for (const FunctionDecl* Function : m_Functions)
    {
        if (GenerateLuaFunction(Context, OS, Function) == 1)
        {
            // 如果是Lua函数记录到LuaFunctions
            LuaFunctions.push_back(Function);
        }
    }

    // 没有暴露给Lua的函数，直接返回
    if (LuaFunctions.empty())
    {
        return;
    }

    OS << "static void InitLuaCppModule_" << TypeName << "()\n"
       << "{\n"
       << "    lua_State* L = LuaMgr::GetInstance()->GetLuaState();\n"
       << "    luaL_Reg l[] =\n"
       << "    {\n";

    for (const FunctionDecl* Function : LuaFunctions)
    {
        SmallString<64> FunctionName;
        raw_svector_ostream FunctionNameOS{FunctionName};
        Function->printName(FunctionNameOS);
        OS << "        { \"" << FunctionName << "\", " << "ExecLuaFunction_" << TypeName << "_" << FunctionName << " },\n";
    }

    OS << "        { NULL, NULL },\n"
       << "    };\n\n"
       << "    lua_getglobal(L, \"Cpp\");\n"
       << "    lua_pushstring(L, \"" << TypeName << "\");\n"
       << "    luaL_newlib(L, l);\n"
       << "    lua_settable(L, -3);\n"
       << "}\n\n"
       << "static EventCall InitLuaCppMoudle_" << TypeName << "(\"InitLua\", InitLuaCppModule_" << TypeName << ");\n\n\n";
}

int ReflectedClass::GenerateLuaFunction(ASTContext* Context, raw_ostream &OS, const FunctionDecl* Function)
{
    SmallString<64> TypeName;
    raw_svector_ostream TypeNameOS(TypeName);
    m_Record->printQualifiedName(TypeNameOS);

    SmallString<64> FunctionName;
    SmallString<64> ReturnTypeName;

    raw_svector_ostream FunctionNameOS{FunctionName};
    raw_svector_ostream ReturnTypeNameOS{ReturnTypeName};

    Function->printName(FunctionNameOS);
    GetRealTypeName(Context, Function->getReturnType(), ReturnTypeNameOS);

    std::map<StringRef, StringRef> OutAttrMap;
    GetExtendAnnotationAttr(OutAttrMap, Function);
    auto it = OutAttrMap.find("ScriptCallable");
    if (it == OutAttrMap.end())
    {
        return 0;
    }

    const CXXMethodDecl* method = static_cast<const CXXMethodDecl*>(Function);
    if (!method->isStatic())
    {
        SourceRange Range = Function->getSourceRange();
        std::cerr << "\033[1;31m" << Range.getBegin().printToString(*m_SourceManager) << ": "
                  << "error: Invalid cppreflect function(" << FunctionName.c_str() << "). "
                  << "attribute ’ScriptCallable’ only support static method now\033[0m" << std::endl;
        return 0;
    }

    OS << "// ===========================\n"
       << "// LuaFunction (" << TypeName << "::" << FunctionName << ")\n"
       << "// ===========================\n"
       << "static int ExecLuaFunction_" << TypeName << "_" << FunctionName << "(lua_State* L)\n"
       << "{\n"
       << "    int err;\n\n";

    int ParamsNum = Function->getNumParams();
    if (ParamsNum > 0)
    {
        OS << "    /* params */\n";
        for (int i = 0; i < ParamsNum; ++i)
        {
            const clang::ParmVarDecl *Param = Function->getParamDecl(i);

            SmallString<64> ParamName;
            SmallString<64> ParamTypeName;

            raw_svector_ostream ParamNameOS{ParamName};
            raw_svector_ostream ParamTypeNameOS{ParamTypeName};

            Param->printName(ParamNameOS);
            GetRealTypeName(Context, Param->getOriginalType(), ParamTypeNameOS);

            OS << "    " << ParamTypeName << " " << ParamName << ";\n";
        }
        OS << "\n";

        OS << "    /* params count */\n"
           << "    int nargs = lua_gettop(L);\n"
           << "    if (nargs < " << ParamsNum << ")\n"
           << "    {\n"
           << "        luaL_error(L, \"parameter error: need 2 params\");\n"
           << "        return 0;\n"
           << "    }\n\n";

        for (int i = 0; i < ParamsNum; ++i)
        {
            const clang::ParmVarDecl *Param = Function->getParamDecl(i);

            SmallString<64> ParamName;
            SmallString<64> ParamTypeName;

            raw_svector_ostream ParamNameOS{ParamName};
            raw_svector_ostream ParamTypeNameOS{ParamTypeName};

            Param->printName(ParamNameOS);
            GetRealTypeName(Context, Param->getOriginalType(), ParamTypeNameOS);

            OS << "    /* param" << i+1 << ": " << ParamName << " */\n"
               << "    err = cppreflect::GetType<" << ParamTypeName << ">()->SetValueFromLua(L, " << i+1 << ", (void*)&" << ParamName << ");\n"
               << "    if (err != 0)\n"
               << "    {\n"
               << "        luaL_error(L, \"param " << i+1 << " error\");\n"
               << "        return 0;\n"
               << "    }\n\n";
        }
    }

    OS << "    /* call cpp function */\n";
    if (Function->getReturnType()->isVoidType())
    {
        OS << "    " << TypeName << "::" << FunctionName << "(";
    }
    else
    {
        OS << "    " << ReturnTypeName << " r = " << TypeName << "::" << FunctionName << "(";
    }
    
    for (int i = 0; i < ParamsNum; ++i)
    {
        const clang::ParmVarDecl *Param = Function->getParamDecl(i);

        SmallString<64> ParamName;
        SmallString<64> ParamTypeName;

        raw_svector_ostream ParamNameOS{ParamName};
        raw_svector_ostream ParamTypeNameOS{ParamTypeName};

        Param->printName(ParamNameOS);
        GetRealTypeName(Context, Param->getOriginalType(), ParamTypeNameOS);

        OS << ParamName;
        if (i + 1 < ParamsNum)
        {
            OS << ", ";
        }
    }
    OS << ");\n\n";

    if (Function->getReturnType()->isVoidType())
    {
        OS << "    /* return 0 param to lua */\n"
           << "    return 0;\n";
    }
    else
    {
        OS << "    /* return value to lua */\n"
           << "    cppreflect::GetType<" << ReturnTypeName << ">()->PushValueToLua(L, &r);\n"
           << "\n"
           << "    /* return 1 param to lua */\n"
           << "    return 1;\n";
    }
    OS << "}\n\n";

    return 1;
}


// =================
// 为Enum生成反射代码
// =================
void ReflectedEnum::Generate(ASTContext* Context, raw_ostream &OS)
{
    GenerateMetaData(Context, OS);
    GenerateLuaBind(Context, OS);
}


// =================
// 为Enum生成元信息
// =================
void ReflectedEnum::GenerateMetaData(ASTContext* Context, raw_ostream &OS)
{
    SmallString<64> TypeName;
    raw_svector_ostream TypeNameOS(TypeName);
    m_Enum->printQualifiedName(TypeNameOS);

    OS << "// ===========================\n"
       << "// MetaEnum (" << TypeName << ")\n"
       << "// ===========================\n"
       << "namespace cppreflect\n"
       << "{\n"
       << "namespace detail\n"
       << "{\n\n"
       << "template <>\n"
       << "const Type* GetTypeImpl(TypeTag<" << TypeName << ">)\n"
       << "{\n"
       << "    return GetEnum<" << TypeName << ">();\n"
       << "}\n\n"
       << "template <>\n"/* m_name */
       << "const Enum* GetEnumImpl(EnumTag<" << TypeName << ">)\n"
       << "{\n"
       << "    static std::vector<std::pair<std::string, int64_t> > items {\n";;

    for (EnumDecl::enumerator_iterator it = m_Enum->enumerator_begin(); it != m_Enum->enumerator_end(); it++)
    {
        SmallString<64> EnumName;
        raw_svector_ostream EnumNameOS{EnumName};
        it->printName(EnumNameOS);
        int64_t EnumVal = it->getInitVal().getExtValue();
        OS << "        { \"" << EnumName << "\", " << EnumVal << " },\n";
    }

    OS << "    };\n\n"
       << "    /* enum: " << TypeName << " */\n"
       << "    static Enum __enum_" << TypeName << " {\n"
       << "        \"" << TypeName << "\", /* m_name */\n"
       << "        sizeof(" << TypeName << "), /* m_size */\n"
       << "        0, /* m_hash */\n"
       << "        items, /* m_items */\n"
       << "    };\n"
       << "    return &__enum_" << TypeName << ";\n"
       << "}\n\n"
       << "} /* namespace: detail */\n"
       << "} /* namespace: cppreflect */\n\n";
}

// =================
// 为Enum生成LuaBind代码
// =================
void ReflectedEnum::GenerateLuaBind(ASTContext* Context, raw_ostream &OS)
{
    SmallString<64> TypeName;
    raw_svector_ostream TypeNameOS(TypeName);
    m_Enum->printQualifiedName(TypeNameOS);

    OS << "static void InitLuaCppModule_" << TypeName << "()\n"
       << "{\n"
       << "    lua_State* L = LuaMgr::GetInstance()->GetLuaState();\n\n"
       << "    lua_getglobal(L, \"Cpp\");\n\n"
       << "    /* Cpp." << TypeName << " = {} */\n"
       << "    lua_pushstring(L, \"" << TypeName << "\");\n"
       << "    lua_createtable(L, 0, 0);\n"
       << "    lua_settable(L, -3);\n\n"
       << "    /* local t = Cpp." << TypeName << " */\n"
       << "    lua_pushstring(L, \"" << TypeName << "\");\n"
       << "    lua_gettable(L, -2);\n\n";

    for (EnumDecl::enumerator_iterator it = m_Enum->enumerator_begin(); it != m_Enum->enumerator_end(); it++)
    {
        SmallString<64> EnumName;
        raw_svector_ostream EnumNameOS{EnumName};
        it->printName(EnumNameOS);
        int64_t EnumVal = it->getInitVal().getExtValue();
        OS << "    /* t." << EnumName << " = " << EnumVal <<" */\n"
           << "    lua_pushstring(L, \"" << EnumName << "\");\n"
           << "    lua_pushinteger(L, " << EnumVal << ");\n"
           << "    lua_settable(L, -3);\n\n";
    }

    OS << "    lua_pop(L, 1);\n"
       << "}\n\n"
       << "static EventCall InitLuaCppMoudle_" << TypeName << "(\"InitLua\", InitLuaCppModule_" << TypeName << ");\n\n\n";
}