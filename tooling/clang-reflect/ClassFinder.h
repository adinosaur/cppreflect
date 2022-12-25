#ifndef CLASS_FINDER_H
#define CLASS_FINDER_H

#include <vector>
#include <string>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

struct ReflectedClass
{
    const CXXRecordDecl* m_Record;
    SourceManager* m_SourceManager;
    std::vector<const FieldDecl*> m_Fields;
    std::vector<const FunctionDecl*> m_Functions;

    ReflectedClass(const CXXRecordDecl* Record, SourceManager* sourceManager)
    : m_Record(Record)
    , m_SourceManager(sourceManager)
    , m_Fields()
    , m_Functions()
    {}

    std::string GetSource()
    {
        return m_SourceManager->getFilename(m_Record->getLocation());
    }

    void Generate(ASTContext* Context, raw_ostream &OS);

    void GenerateInvokeCxxFunction(ASTContext* Context, raw_ostream &OS, const FunctionDecl* Function);
    void GenerateInvokeRpcFunction(ASTContext* Context, raw_ostream &OS, const FunctionDecl* Function);
    
    void GenerateMetaClass(ASTContext* Context, raw_ostream &OS);
    void GenerateLuaFunctions(ASTContext* Context, raw_ostream &OS);
    int GenerateLuaFunction(ASTContext* Context, raw_ostream &OS, const FunctionDecl* Function);
};

struct ReflectedEnum
{
    const EnumDecl* m_Enum;
    SourceManager* m_SourceManager;

    ReflectedEnum(const EnumDecl* Enum, SourceManager* sourceManager)
    : m_Enum(Enum)
    , m_SourceManager(sourceManager)
    {}

    std::string GetSource()
    {
        return m_SourceManager->getFilename(m_Enum->getLocation());
    }

    void Generate(ASTContext* Context, raw_ostream &OS);
    void GenerateMetaData(ASTContext* Context, raw_ostream &OS);
    void GenerateLuaBind(ASTContext* Context, raw_ostream &OS);
};

class ClassFinder : public MatchFinder::MatchCallback
{
public:
    ClassFinder()
    : m_Context(NULL)
    , m_FileName()
    , m_Classes()
    {}

    virtual void run(const MatchFinder::MatchResult &Result) override
    {
        m_Context = Result.Context;
        m_SourceManager = Result.SourceManager;

        const CXXRecordDecl* Record = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("id");
        if (Record)
            return FoundRecord(Record);

        const FieldDecl* Field = Result.Nodes.getNodeAs<clang::FieldDecl>("id");
        if (Field)
            return FoundField(Field);

        const FunctionDecl* Function = Result.Nodes.getNodeAs<clang::FunctionDecl>("id");
        if (Function)
            return FoundFunction(Function);
        
        const EnumDecl* Enum = Result.Nodes.getNodeAs<clang::EnumDecl>("id");
        if (Enum)
            return FoundEnum(Enum);
    }

    virtual void onStartOfTranslationUnit() override
    {
    }

    virtual void onEndOfTranslationUnit() override
    {
        raw_fd_ostream OS(1, false);

        OS << "/* THIS FILE IS AUTO_GENERATED. DO NOT EDIT!!! */\n"
           << "#pragma once\n"
           << "#include \"" << m_FileName << "\"\n"
           << "\n"
           << "#include <string>\n"
           << "#include <vector>\n"
           << "#include <map>\n"
           << "\n"
           << "#ifndef CPPREFLECT_INVOKE_NON_STATIC_RPC\n"
           << "#define CPPREFLECT_INVOKE_NON_STATIC_RPC(Type, Method, Instance, ...) (((Type*)Instance)->Method(__VA_ARGS__))\n"
           << "#endif\n\n";

        for (auto &ref : m_Classes)
        {
            if (ref.GetSource() == m_FileName)
            {
                ref.Generate(m_Context, OS);
            }
        }

        for (auto &ref : m_Enums)
        {
            if (ref.GetSource() == m_FileName)
            {
                ref.Generate(m_Context, OS);
            }
        }
    }

    void GenerateEnumMetaData(const EnumDecl* Enum, raw_ostream &OS);

    void SetSouceFileName(const std::string& FileName)
    {
        m_FileName = FileName;
    }

protected:
    void FoundRecord(const CXXRecordDecl* Record)
    {
        m_Classes.emplace_back(ReflectedClass(Record, m_SourceManager));
    }

    void FoundField(const FieldDecl* Field)
    {
        m_Classes.back().m_Fields.push_back(Field);
    }

    void FoundFunction(const FunctionDecl* Function)
    {
        m_Classes.back().m_Functions.push_back(Function);
    }

    void FoundEnum(const EnumDecl* Enum)
    {
        m_Enums.emplace_back(ReflectedEnum(Enum, m_SourceManager));
    }

protected:
    ASTContext *m_Context;
    SourceManager *m_SourceManager;
    std::string m_FileName;
    std::vector<ReflectedClass> m_Classes;
    std::vector<ReflectedEnum> m_Enums;
};

#endif /* CLASS_FINDER_H */