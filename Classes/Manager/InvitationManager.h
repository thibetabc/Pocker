//
//  InvitationManager.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/13.
//
//

#ifndef InvitationManager_hpp
#define InvitationManager_hpp

#include <stdio.h>
#include "msg.pb.h"
#include "cmd_data.h"
#include "events.h"
#include "cocos2d.h"

#define IM InvitationManager::getInstance()

class InvitationManager {

    
private:
    InvitationManager();
    ~InvitationManager();
//    void request_new_friend_msg_list();
    void handle_receive_request_list(cmd_data_pointer data);
    //实时更新好友列表
    void handle_get_friends_list(cmd_data_pointer data);
    void register_events();
private:
    static InvitationManager* __Invitation_manager__;
    std::map<uint32_t, msg::Invitation> invitations_;
    uint32_t unread_invitation_count_ = 0;
    events::event_dispatch dispatch_;
    
 
    
public:
    static InvitationManager* getInstance() {
        if(!__Invitation_manager__) {
            __Invitation_manager__ = new InvitationManager();
        }
        return __Invitation_manager__;
    }
    
    static void destroy() {
        delete __Invitation_manager__;
        __Invitation_manager__ = nullptr;
    }
    
    void add_request(msg::Processor_123_ReceiveRequestList processor);
    void deal_request(uint32_t requestid, bool result);
    const std::map<uint32_t, msg::Invitation>& get_invitations_() { return invitations_;}
    const std::vector<msg::Invitation> get_vec_invitations_();
    
    void update_unread_invitation_count();
    
    void clear_unread_invitation_count();
    const uint32_t get_unread_invitation_count() {return unread_invitation_count_;};
    void save_invitation_record();
    void init_invitation_records();
    
   
    //下发的新消息id，添加好友时上传
    CC_SYNTHESIZE(int, ID, ID);
    
    
};

#endif /* InvitationManager_hpp */
