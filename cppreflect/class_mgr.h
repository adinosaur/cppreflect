#ifndef YGGD_CLASSMGR_H
#define YGGD_CLASSMGR_H

#include "cppreflect/cppreflect.h"

#include <string>
#include <map>

struct ClassMgr
{
    /* GetInstance */
    static ClassMgr* GetInstance();

    /* 注册Class对象 */
    void RegisterClass(const std::string& ClassName, const cppreflect::Class* ClassObject);

    /* Class名字索引Class对象 */
    std::map<std::string, const cppreflect::Class*> m_classMap;
};

#endif /* YGGD_CLASSMGR_H */