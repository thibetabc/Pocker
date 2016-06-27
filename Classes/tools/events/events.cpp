//
//  event.cpp
//  GodOfWar
//
//  Created by MirrorMiko on 13-9-24.
//
//

#include "events.h"
#include <set>
#include "utils.h"
#include "threads.h"

events::event_dispatch::~event_dispatch(){
    _event_handlers.clear();
}

void events::event_post_dispatch::raise_event(uint64_t cmd, cmd_data_pointer data){
    
    
    event_manager::shared_instence().add_event_call(cmd, data);
}

void events::event_dispatch::register_event(uint64_t cmd, handler_pointer fn){
    _event_handlers[cmd] = fn;
    event_manager::shared_instence().register_dispatch(cmd, handler_weak_pointer(fn));
}

void events::event_manager::call_immediately(uint64_t cmd, cmd_data_pointer data){
    
    try {
        auto iter = _all_events.find(cmd);
        if(iter != _all_events.end()) {
            std::list<handler_weak_pointer> dispatchs = iter->second;
            for (auto dip = dispatchs.begin(); dip != dispatchs.end(); dip++) {
                handler_weak_pointer pointer = *dip;
                auto p = pointer.lock();
                if(p) {
                    p->operator()(data);

                }

            }
        }
        else {
            cocos2d::log("warnning: cmd %llu not found! at file %s line %d.", cmd, __FILE__, __LINE__);
        }
        
        std::lock_guard<std::mutex> s(tex);
        std::remove_if(_all_events[cmd].begin(), _all_events[cmd].end(), [](std::list<handler_weak_pointer>::value_type v){
            return !v.lock();
        });
        
    } catch (std::out_of_range) {
        //no hanlders for the event
    }
}

void events::event_manager::add_event_call(uint64_t cmd, cmd_data_pointer data){
    if (threads::thread_manager::shared_instence().is_main_thread()) {
        call_immediately(cmd, data);
    }else{
        _wait_post_events.push_back(std::make_pair(cmd, data));
    }
    
}

void events::event_manager::register_dispatch(uint64_t cmd, handler_weak_pointer pointer){
    std::lock_guard<std::mutex> s(tex);

    std::list<handler_weak_pointer>& list = _all_events[cmd];
    if (pointer.lock()) {
        list.push_back(pointer);
    }
}

void events::event_manager::update(){
    if (!_wait_post_events.empty()) {
        auto tmp = _wait_post_events;
        _wait_post_events.clear();
        for (auto iter = tmp.begin(); iter != tmp.end(); iter++) {
            call_immediately(iter->first, iter->second);
        }
    }
}