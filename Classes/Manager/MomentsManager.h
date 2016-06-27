//
//  MomentsManager.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/14.
//
//

#ifndef MomentsManager_hpp
#define MomentsManager_hpp

#include <stdio.h>
#include "msg.pb.h"
#include "cmd_data.h"
#include "events.h"

#define MM MomentsManager::getInstance()

class MomentsManager {
public:
    static MomentsManager* getInstance() {
        if(!__Moments_manager__) {
            __Moments_manager__ = new MomentsManager();
        }
        return __Moments_manager__;
    }
    
    static void destroy() {
        delete __Moments_manager__;
        __Moments_manager__ = nullptr;
    }
    
    const std::map<uint32_t, std::vector<msg::Moment>>& get_moment_record() { return moment_records_; }
    void save_moment_record();
    void add_moment(const msg::Moment& moment);
    void init_moment_records();
private:
    static MomentsManager* __Moments_manager__;
    
    //朋友圈
    std::map<uint32_t, msg::CommentInfo> comment_;
    //朋友圈记录
    std::map<uint32_t, std::vector<msg::Moment>> moment_records_;
    
    events::event_dispatch dispatch_;
    void register_events();
    MomentsManager();
};

#endif /* MomentsManager_hpp */
