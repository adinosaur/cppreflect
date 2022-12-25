#include "cppreflect/eventcall_mgr.h"

static EventCallMgr* EventCallMgrInstance = NULL;

EventCallMgr* EventCallMgr::GetInstance()
{
    if (EventCallMgrInstance == NULL)
    {
        EventCallMgrInstance = new EventCallMgr();
    }
    return EventCallMgrInstance;
}

void EventCallMgr::RegisterEventCall(const std::string& event, EventCallbaclFuncType callback)
{
    m_eventCallbacks[event].push_back(callback);
}

void EventCallMgr::InvokeEvent(const std::string& event)
{
    std::map<std::string, std::vector<EventCallbaclFuncType> >::iterator it = m_eventCallbacks.find(event);
    if (it != m_eventCallbacks.end())
    {
        for (EventCallbaclFuncType f : it->second)
        {
            f();
        }
    }
}