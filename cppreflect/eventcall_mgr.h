#ifndef EVENTCALL_MGR_H
#define EVENTCALL_MGR_H

#include <string>
#include <vector>
#include <map>

typedef void (*EventCallbaclFuncType)();

struct EventCallMgr
{
public:
    /* GetInstance */
    static EventCallMgr* GetInstance();

    /* 注册event对应的回调函数 */
    void RegisterEventCall(const std::string& event, EventCallbaclFuncType callback);

    /* 触发event对应的回调函数 */
    void InvokeEvent(const std::string& event);

private:
    /* event索引回调函数集合 */
    std::map<std::string, std::vector<EventCallbaclFuncType> > m_eventCallbacks;
};

struct EventCall
{
    EventCall(const std::string& event, EventCallbaclFuncType callback)
    {
        if (event == "BeforeMain")
        {
            callback();
        }
        else
        {
            EventCallMgr::GetInstance()->RegisterEventCall(event, callback);
        }
    }
};


#endif /* EVENTCALL_MGR_H */