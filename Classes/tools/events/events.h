//
//  event.h
//  GodOfWar
//
//  Created by MirrorMiko on 13-9-24.
//
//

#ifndef __GodOfWar__event__
#define __GodOfWar__event__

#include <mutex>
#include <iostream>
#include <thread>
#include <memory>
#include <map>
#include "cmd_data.h"
#include "macro.h"
#include "manager_interface.h"

#define BIND_FUNC(obj, func) events::handler_pointer(new events::handler(std::bind(&func, obj, std::placeholders::_1)))

namespace events{
    typedef std::function<void (cmd_data_pointer data)> handler;
    typedef std::shared_ptr<handler> handler_pointer;
    typedef std::weak_ptr<handler> handler_weak_pointer;
    class event_post_dispatch {
    protected:
        std::mutex tex;
    public:
        void raise_event(uint64_t cmd, cmd_data_pointer data = cmd_data_pointer());
    };
    
    class event_dispatch : public event_post_dispatch {
    protected:
        std::map<uint64_t, handler_pointer> _event_handlers;
    public:
        virtual ~event_dispatch();
        void register_event(uint64_t cmd, handler_pointer fn);
    };
    
    class event_manager : public SINGLETON<event_manager> , public manager_interface {
        friend class event_post_dispatch;
    protected:
        std::map<uint64_t, std::list<handler_weak_pointer>> _all_events;
        std::list<std::pair<uint64_t, cmd_data_pointer>> _wait_post_events;
    protected:
        void call_immediately(uint64_t, cmd_data_pointer);
        void add_event_call(uint64_t, cmd_data_pointer);
    public:
        void register_dispatch(uint64_t, handler_weak_pointer);
    public:
        virtual void init(){}
        virtual void update();
    protected:
        std::mutex tex;
    };
    
    static event_post_dispatch global_dispatch;
}

#endif /* defined(__GodOfWar__event__) */
