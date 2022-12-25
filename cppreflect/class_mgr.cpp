#include "cppreflect/class_mgr.h"

static ClassMgr* ClassMgrInstance = NULL;

ClassMgr* ClassMgr::GetInstance()
{
    if (ClassMgrInstance == NULL)
    {
        ClassMgrInstance = new ClassMgr();
    }
    return ClassMgrInstance;
}

void ClassMgr::RegisterClass(const std::string& ClassName, const cppreflect::Class* ClassObject)
{
    m_classMap[ClassName] = ClassObject;
}