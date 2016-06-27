//
//  ChatMessageManager.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/13.
//
//

#ifndef ChatMessageManager_hpp
#define ChatMessageManager_hpp

#include <stdio.h>
#include "msg.pb.h"
#include "cmd_data.h"
#include "events.h"

typedef ::google_public::protobuf::RepeatedPtrField<msg::ChatMessages> Messages;

#define CM ChatMessageManager::getInstance()
class ChatMessageManager {
public:
    static ChatMessageManager* getInstance() {
        if(!__ChatMessage_manager__) {
            __ChatMessage_manager__ = new ChatMessageManager();
        }
        return __ChatMessage_manager__;
    }
    
    static void destroy() {
        delete __ChatMessage_manager__;
        __ChatMessage_manager__ = nullptr;
    }
    
    void save_chat_record();
    enum MESSAGETYPE{
        GROUPCHAT = 0x01,
        FRIENDCHAT = 0x02
    };
    
    void send_chat_message(const msg::ChatMessage& ChatMessage);
    void resend_chat_message_by_subtime(const std::string& subtime);
    void resend_chat_message(msg::ChatMessage* chatMessage);
    void clear_chat_record(const uint32_t key_id, MESSAGETYPE type);
    const msg::ChatMessages* get_chats(uint32_t id, MESSAGETYPE type);
    msg::ChatMessage* get_message_with_submit_time(const std::string& subtime);
    const msg::ChatMessage* get_last_message(const uint32_t id, MESSAGETYPE type);
    const uint32_t get_unread_message_count(const uint32_t id, MESSAGETYPE type, bool isAll = false);
    void clear_unreal_message_by(const uint32_t id, MESSAGETYPE type);
    void init_chat_records();
    void set_chat_data_failed();
    void receive_chat_message(msg::Processor_161_ReceiveChatMessage processor);
    void update_chat_layer();
    void update_unread_message_count_layer();
    
    std::map<uint32_t, msg::ChatMessages>* get_messages_by_type(MESSAGETYPE type);
private:
    void insert_chat_data_to_record(const msg::ChatMessage& cd);
    void handle_send_chat_message(cmd_data_pointer data);
    void handle_receive_chat_message(cmd_data_pointer data);
    void handle_upload_file_success(cmd_data_pointer data);
    void handle_upload_file_failed(cmd_data_pointer data);
    
    static ChatMessageManager* __ChatMessage_manager__;
    //聊天记录
    std::map<uint32_t, msg::ChatMessages> friend_chat_records_;
    //聊天记录
    std::map<uint32_t, msg::ChatMessages> chat_records_;
    //附带文件的聊天消息
    std::map<std::string, std::string> need_upload_records_;
    
    events::event_dispatch dispatch_;
    void register_events();
    ChatMessageManager();
};

#endif /* ChatMessageManager_hpp */
