//
//  ChatMessageManager.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/13.
//
//

#include "ChatMessageManager.h"
#include "GameDataManager.h"
#include "file_utils.h"
#include "utils.h"
#include "net_manager.h"
#include "NET_CMD.h"
#include "FileManager.h"
#include "ProcessorManager.h"
#include "SceneManager.h"

#include "cmd_def.h"

#include "LayerChat.h"
#include "LayerMain.h"
#include "LayerGroupList.h"
#include "LayerFriendList.h"
#include "LayerGameChat.h"
#include "OfcBackGroundLayer.h"
#include "newOfcPokerRoom.h"

ChatMessageManager* ChatMessageManager::__ChatMessage_manager__ = nullptr;

ChatMessageManager::ChatMessageManager()
{
    register_events();
}

void ChatMessageManager::register_events()
{
    dispatch_.register_event(REQ_SEND_CHAT_MESSAGE,
                             BIND_FUNC(this, ChatMessageManager::handle_send_chat_message));
    dispatch_.register_event(RECEIVE_CHAT_MESSAGE,
                             BIND_FUNC(this, ChatMessageManager::handle_receive_chat_message));
    dispatch_.register_event(EVENT_UPLOAD_FILE_SUCCESS,
                             BIND_FUNC(this, ChatMessageManager::handle_upload_file_success));
    dispatch_.register_event(EVENT_UPLOAD_FILE_FAILED,
                             BIND_FUNC(this, ChatMessageManager::handle_upload_file_failed));
}

std::map<uint32_t, msg::ChatMessages>* ChatMessageManager::get_messages_by_type(MESSAGETYPE type)
{
    if (type == GROUPCHAT)
        return &chat_records_;
    else
        return &friend_chat_records_;
}

const msg::ChatMessages* ChatMessageManager::get_chats(uint32_t id, MESSAGETYPE type)
{
    auto messages = get_messages_by_type(type);
    auto iter = messages->find(id);
    if(iter != messages->end())
        return &iter->second;
    else
        return nullptr;
    
}

msg::ChatMessage* ChatMessageManager::get_message_with_submit_time(const std::string& subtime)
{
    for (auto iter = friend_chat_records_.begin(); iter != friend_chat_records_.end(); ++iter) {
        for (int i=0; i<iter->second.message_size(); i++) {
            if(iter->second.message(i).subtime() == subtime)
            {
                return iter->second.mutable_message(i);
            }
        }
    }
    
    for (auto iter = chat_records_.begin(); iter != chat_records_.end(); ++iter) {
        for (int i=0; i<iter->second.message_size(); i++) {
            if(iter->second.message(i).subtime() == subtime)
            {
                return iter->second.mutable_message(i);
            }
        }
    }
    
    return nullptr;
}

const msg::ChatMessage* ChatMessageManager::get_last_message(const uint32_t id, MESSAGETYPE type)
{
    auto messages = get_messages_by_type(type);
    auto iter = messages->find(id);
    if(iter != messages->end()) {
        if(iter->second.message_size() > 0) {
            return &iter->second.message(iter->second.message_size() - 1);
        }
    }
    return nullptr;
}

const uint32_t ChatMessageManager::get_unread_message_count(const uint32_t id, MESSAGETYPE type, bool isAll)
{
    auto messages = get_messages_by_type(type);
    if (isAll)
    {
        uint32_t intUnreadCount = 0;
        for (auto iter : *messages)
        {
            intUnreadCount += iter.second.count();
        }
        return intUnreadCount;
    }else
    {
        auto iter = messages->find(id);
        if(iter != messages->end())
        {
            return iter->second.count();
        }
    }
    
    return 0;
}

void ChatMessageManager::clear_unreal_message_by(const uint32_t id, MESSAGETYPE type)
{
    auto messages = get_messages_by_type(type);
    auto iter = messages->find(id);
    if(iter != messages->end())
        iter->second.set_count(0);
    save_chat_record();
    update_unread_message_count_layer();
}

void ChatMessageManager::save_chat_record()
{
    if(chat_records_.empty() && friend_chat_records_.empty()) {
        std::string user_cache = GDM->get_user_directory();
        file_utils::delete_directory(user_cache + "chat_data_record.a");
        file_utils::delete_directory(user_cache + "chat_data_record_new.a");
    }
    
    msg::ChatMessageForSave pb_chat_datas;
    
    for (auto iter = chat_records_.begin(); iter != chat_records_.end(); ++iter)
        pb_chat_datas.add_groupmessage()->CopyFrom(iter->second);
    
    for (auto iter = friend_chat_records_.begin(); iter != friend_chat_records_.end(); ++iter)
        pb_chat_datas.add_friendmessage()->CopyFrom(iter->second);
    
    if(chat_records_.size() > 0 || friend_chat_records_.size() > 0) {
        std::string result = "";
        
        bool succ = pb_chat_datas.SerializeToString(&result);
        
        //cocos2d::log("ChatMessageManager::save_chat_record() \nSerializeToString : %s", result.c_str());
        if(succ) {
            std::string user_cache = GDM->get_user_directory();
            file_utils::write_file(user_cache + "chat_data_record_new.a", result);
        }
    }
}

void ChatMessageManager::clear_chat_record(const uint32_t key_id, MESSAGETYPE cleartype)
{
    auto messages = get_messages_by_type(cleartype);
    messages->erase(key_id);
    save_chat_record();
}

void ChatMessageManager::init_chat_records()
{
    chat_records_.clear();
    friend_chat_records_.clear();
    std::string file_path = GDM->get_user_directory() + "chat_data_record.a";
    std::string new_file_path = GDM->get_user_directory() + "chat_data_record_new.a";
    if(file_utils::file_exists(new_file_path)) {
        auto content = file_utils::read_file(new_file_path);
        msg::ChatMessageForSave pb_chat_datas;
        bool succ = pb_chat_datas.ParseFromString(content);
        if(succ) {
            for (auto iter: pb_chat_datas.groupmessage()){
                msg::ChatMessages messages;
                messages.CopyFrom(iter);
                chat_records_.insert(std::make_pair(messages.id(), messages));
            }
            for (auto iter: pb_chat_datas.friendmessage()){
                msg::ChatMessages messages;
                messages.CopyFrom(iter);
                friend_chat_records_.insert(std::make_pair(messages.id(), messages));
            }
        }
    }
    else if(file_utils::file_exists(file_path)) {
        auto content = file_utils::read_file(file_path);
        msg::OldChatMessageForSave pb_chat_datas;
        bool succ = pb_chat_datas.ParseFromString(content);
        if(succ) {
            auto friendmessage = pb_chat_datas.friendmessage();
            for (int j = 0; j < friendmessage.message().size(); j++)
            {
                auto pb_cb = friendmessage.message(j);
                msg::ChatMessage cd;
                cd.CopyFrom(pb_cb);
                uint32_t keyid;
                if (cd.userdetailinfo().userid() == GDM->get_user_id())
                {
                    keyid = cd.receiveuserid();
                }else{
                    keyid = cd.userdetailinfo().userid();
                }
                
                auto iter = friend_chat_records_.find(keyid);
                if(iter != friend_chat_records_.end()) {
                    iter->second.add_message()->CopyFrom(cd);
                }
                else {
                    msg::ChatMessages messages;
                    messages.set_id(keyid);
                    messages.set_count(0);
                    messages.add_message()->CopyFrom(cd);
                    friend_chat_records_.insert(std::make_pair(keyid, messages));
                }
            }
            
            auto groupmessage = pb_chat_datas.groupmessage();
            for (int j = 0; j < groupmessage.message().size(); j++)
            {
                auto pb_cb = groupmessage.message(j);
                msg::ChatMessage cd;
                cd.CopyFrom(pb_cb);
                
                auto iter = chat_records_.find(cd.groupid());
                if(iter != chat_records_.end()) {
                    iter->second.add_message()->CopyFrom(cd);
                }
                else {
                    msg::ChatMessages messages;
                    messages.set_id(cd.groupid());
                    messages.set_count(0);
                    messages.add_message()->CopyFrom(cd);
                    chat_records_.insert(std::make_pair(cd.groupid(), messages));
                }
            }
        }
        else {
            cocos2d::log("Init Chat Records : succ error!");
        }
    }
    set_chat_data_failed();
}

void ChatMessageManager::set_chat_data_failed()
{
    bool bNeedRefresh = false;
    for (auto iter = friend_chat_records_.begin(); iter != friend_chat_records_.end(); ++iter) {
        for (int i=0; i<iter->second.message_size(); i++) {
            if(iter->second.message(i).status() == msg::MessageStatus::Sending)
            {
                iter->second.mutable_message(i)->set_status(msg::MessageStatus::Failed);
                bNeedRefresh = true;
            }
        }
    }
    
    for (auto iter = chat_records_.begin(); iter != chat_records_.end(); ++iter) {
        for (int i=0; i<iter->second.message_size(); i++) {
            if(iter->second.message(i).status() == msg::MessageStatus::Sending)
            {
                iter->second.mutable_message(i)->set_status(msg::MessageStatus::Failed);
                bNeedRefresh = true;
            }
        }
    }
    if (bNeedRefresh){
        save_chat_record();
    }
    
}

void ChatMessageManager::insert_chat_data_to_record(const msg::ChatMessage& cd)
{
    if (cd.has_receiveuserid()){
        uint32_t keyid;
        if (cd.userdetailinfo().userid() == GDM->get_user_id())
        {
            keyid = cd.receiveuserid();
        }else{
            keyid = cd.userdetailinfo().userid();
        }
        auto iter = friend_chat_records_.find(keyid);
        if(iter != friend_chat_records_.end()) {
            bool isMessageHasRecord = false;
            for (auto iter_user_chats : iter->second.message()) {
                if (iter_user_chats.has_id() &&  iter_user_chats.id() == cd.id())
                {
                    iter_user_chats = cd;
                    isMessageHasRecord = true;
                    break;
                }
            }
            if (!isMessageHasRecord)
                iter->second.add_message()->CopyFrom(cd);
        }
        else {
            msg::ChatMessages messages;
            messages.set_id(keyid);
            messages.set_count(0);
            messages.add_message()->CopyFrom(cd);
            friend_chat_records_.insert(std::make_pair(keyid, messages));        }
    }
    else if (cd.has_groupid()){
        auto iter = chat_records_.find(cd.groupid());
        if(iter != chat_records_.end()) {
            bool isMessageHasRecord = false;
            if (cd.has_id())
            {
                for (auto iter_group_chats : iter->second.message())
                {
                    if (iter_group_chats.has_id() && iter_group_chats.id() == cd.id())
                    {
                        iter_group_chats = cd;
                        isMessageHasRecord = true;
                        break;
                    }
                }
            }
            if (!isMessageHasRecord)
                iter->second.add_message()->CopyFrom(cd);
        }
        else {
            msg::ChatMessages messages;
            messages.set_id(cd.groupid());
            messages.set_count(0);
            messages.add_message()->CopyFrom(cd);
            chat_records_.insert(std::make_pair(cd.groupid(), messages));
        }
    }
    else{
        cocos2d::log("Insert Chat Data To Record : error!");
    }
}

void ChatMessageManager::send_chat_message(const msg::ChatMessage& ChatMessage)
{
    double t = tools::date_double() * 1000;
    char time_str[1024];
    sprintf(time_str, "%f", t);
    
    msg::ChatMessage temp_chat_data_;
    temp_chat_data_.CopyFrom(ChatMessage);
    temp_chat_data_.set_subtime(time_str);
    temp_chat_data_.set_status(msg::MessageStatus::Sending);
    
    msg::UserDetailInfo * userDetailInfo = GDM->get_userinfo();
    if (userDetailInfo)
    {
        temp_chat_data_.mutable_userdetailinfo()->set_userid(userDetailInfo->userid());
        temp_chat_data_.mutable_userdetailinfo()->set_picname(userDetailInfo->picname());
        temp_chat_data_.mutable_userdetailinfo()->set_nickname(userDetailInfo->nickname());
    }else
    {
        cocos2d::log("ChatMessageManager::send_chat_message UserDetailInfo Error!");
    }
    
    
    insert_chat_data_to_record(temp_chat_data_);
    save_chat_record();
    //android照相返回-要在主线程操作
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&]()
    {
        update_chat_layer();
    });
    
    if (ChatMessage.messageinfo().type() == msg::MessageType::Voice)
    {
        std::string up_file = ChatMessage.messageinfo().voiceinfo().voicename() + ".amr";
        need_upload_records_.insert(std::make_pair(up_file, time_str));
        FM.upload_file(up_file);
        
        return;
    }else if (ChatMessage.messageinfo().type() == msg::MessageType::Picture)
    {
        auto vecPic = tools::split(ChatMessage.messageinfo().picinfo().picname(), "#");
        if (vecPic.size() == 1)
        {
            std::string path = FileUtils::getInstance()->fullPathForFilename("emotion/" + vecPic[0]);
            
            if (path.empty())
            {
                std::string up_file = vecPic[0];
                need_upload_records_.insert(std::make_pair(up_file, time_str));
                FM.upload_file(up_file);
                
                return;
            }
        }
        
        
    }
    msg::Processor_160_SendChatMessage processor;
    processor.mutable_chatmessage()->CopyFrom(temp_chat_data_);
    net_manager::shared_instence().send_data(processor, REQ_SEND_CHAT_MESSAGE);
    
}


void ChatMessageManager::resend_chat_message_by_subtime(const std::string& subtime)
{
    msg::ChatMessage* chatMessage = get_message_with_submit_time(subtime);
    if (chatMessage)
    {
        chatMessage->set_status(::msg::MessageStatus::Sending);
        
        save_chat_record();
        update_chat_layer();
        
        if (chatMessage->messageinfo().type() == msg::MessageType::Voice)
        {
            std::string up_file = chatMessage->messageinfo().voiceinfo().voicename() + ".amr";
            need_upload_records_.insert(std::make_pair(up_file, chatMessage->subtime()));
            FM.upload_file(up_file);
            
            return;
        }else if (chatMessage->messageinfo().type() == msg::MessageType::Picture)
        {
            auto vecPic = tools::split(chatMessage->messageinfo().picinfo().picname(), "#");
            if (vecPic.size() == 1)
            {
                std::string path = FileUtils::getInstance()->fullPathForFilename("emotion/" + vecPic[0]);
                
                if (path.empty())
                {
                    std::string up_file = vecPic[0];
                    need_upload_records_.insert(std::make_pair(up_file, chatMessage->subtime()));
                    FM.upload_file(up_file);
                    
                    return;
                }
            }
            
            
        }
        
        msg::Processor_160_SendChatMessage processor;
        processor.mutable_chatmessage()->CopyFrom(*chatMessage);
        net_manager::shared_instence().send_data(processor, REQ_SEND_CHAT_MESSAGE);
        
    }
}

void ChatMessageManager::resend_chat_message(msg::ChatMessage* chatMessage)
{
    chatMessage->set_status(::msg::MessageStatus::Sending);
    msg::Processor_160_SendChatMessage processor;
    processor.mutable_chatmessage()->CopyFrom(*chatMessage);
    save_chat_record();
    net_manager::shared_instence().send_data(processor, REQ_SEND_CHAT_MESSAGE);
    update_chat_layer();
}

void ChatMessageManager::handle_send_chat_message(cmd_data_pointer data)//160
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->SendChatMessage_Down(rec);
    msg::ChatMessage* msg = get_message_with_submit_time(processor.submittime());
    if(msg) {
        if (processor.response().status() == msg::Status::SUCCESS)
            msg->set_status(::msg::MessageStatus::Success);
        else
            msg->set_status(::msg::MessageStatus::Failed);
        save_chat_record();
    }
    update_chat_layer();
}

void ChatMessageManager::handle_receive_chat_message(cmd_data_pointer data )
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->ReceiveChatMessage_Down(rec);
    receive_chat_message(processor);
    
    update_chat_layer();
    
}

void ChatMessageManager::receive_chat_message(msg::Processor_161_ReceiveChatMessage processor)
{
    bool bHasFriendMsg = false;
    bool bHasGroupMsg = false;
    std::map<int, msg::UserDetailInfo> userinfos;
    for(int i = 0 ; i < processor.chatmessage().size();i++){
        msg::ChatMessage cd;
        cd.CopyFrom(processor.chatmessage(i));
        if (cd.messageinfo().type() == msg::MessageType::Voice)
        {
            FM.download_file(cd.messageinfo().voiceinfo().voicename() + ".amr");
        }
        insert_chat_data_to_record(cd);
        if (cd.has_userdetailinfo()){
            msg::UserDetailInfo info;
            info.CopyFrom(cd.userdetailinfo());
            userinfos[cd.userdetailinfo().userid()] = info;
        }
        if (cd.has_receiveuserid())
        {
            bHasFriendMsg = true;
            auto iter = friend_chat_records_.find(cd.userdetailinfo().userid());
            if(iter != friend_chat_records_.end())
                iter->second.set_count(iter->second.count() + 1);
        }
        else if (cd.has_groupid())
        {
            bHasGroupMsg = true;
            
            auto iter = chat_records_.find(cd.groupid());
            if(iter != chat_records_.end())
                iter->second.set_count(iter->second.count() + 1);
        }
    }
    
    for (auto iter = userinfos.begin(); iter != userinfos.end(); iter++){
        for (auto msgs = chat_records_.begin(); msgs != chat_records_.end(); msgs++){
            for (int i=0; i< msgs->second.message_size(); i++)
                if (msgs->second.message(i).userdetailinfo().userid() == iter->second.userid())
                    msgs->second.mutable_message(i)->mutable_userdetailinfo()->CopyFrom(iter->second);
        }
        for (auto msgs = friend_chat_records_.begin(); msgs != friend_chat_records_.end(); msgs++){
            for (int i=0; i< msgs->second.message_size(); i++)
                if (msgs->second.message(i).userdetailinfo().userid() == iter->second.userid())
                    msgs->second.mutable_message(i)->mutable_userdetailinfo()->CopyFrom(iter->second);
        }
    }
    
    for (auto iter = chat_records_.begin(); iter != chat_records_.end(); iter++)
        std::sort(iter->second.mutable_message()->begin(), iter->second.mutable_message()->end(), [](const msg::ChatMessage& c1, const msg::ChatMessage& c2)->bool
                  {
                      return atol(c1.subtime().c_str()) < atol(c2.subtime().c_str());
                  });
    for (auto iter = friend_chat_records_.begin(); iter != friend_chat_records_.end(); iter++)
        std::sort(iter->second.mutable_message()->begin(), iter->second.mutable_message()->end(), [](const msg::ChatMessage& c1, const msg::ChatMessage& c2)->bool
                  {
                      return atol(c1.subtime().c_str()) < atol(c2.subtime().c_str());
                  });
    save_chat_record();
    
    if (processor.chatmessage().size() < 1)
    {
        cocos2d::log("Error : Processor.ChatMessage Size is %d", processor.chatmessage().size());
    }else
    {
        auto processorUp = PM->ReceiveChatMessage_UP(processor.chatmessage(processor.chatmessage_size()-1).id());
        net_manager::shared_instence().send_data(processorUp, RECEIVE_CHAT_MESSAGE);
    }
    
    update_unread_message_count_layer();
}

void ChatMessageManager::handle_upload_file_success(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (need_upload_records_.find(s->get_value()) == need_upload_records_.end())
    {
        return;
    }
    
    auto upPair = need_upload_records_.find(s->get_value());
    msg::ChatMessage* chatMessage = get_message_with_submit_time(upPair->second);
    if (chatMessage)
    {
        msg::Processor_160_SendChatMessage processor;
        processor.mutable_chatmessage()->CopyFrom(*chatMessage);
        net_manager::shared_instence().send_data(processor, REQ_SEND_CHAT_MESSAGE);
        need_upload_records_.erase(upPair->first);
    }
}

void ChatMessageManager::handle_upload_file_failed(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (need_upload_records_.find(s->get_value()) == need_upload_records_.end())
    {
        return;
    }
    
    auto upPair = need_upload_records_.find(s->get_value());
    msg::ChatMessage* chatMessage = get_message_with_submit_time(upPair->second);
    if (chatMessage)
    {
        chatMessage->set_status(::msg::MessageStatus::Failed);
        
        save_chat_record();
        update_chat_layer();
    }
}

void ChatMessageManager::update_chat_layer()
{
    LayerFriendList* lf = dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()));
    if (lf)
        lf->RefreshMessage();
    
    LayerGroupList* lg = dynamic_cast<LayerGroupList*>(SceneManager::get_layer_by_name(typeid(LayerGroupList).name()));
    if (lg)
        lg->RefreshMessage();
    
    LayerChat * lc = dynamic_cast<LayerChat*>(SceneManager::get_layer_by_name(typeid(LayerChat).name()));
    
    newOfcPokerRoom* opr = dynamic_cast<newOfcPokerRoom*>(SceneManager::GetRoomLayer());
    
    if (lc)
    {
        switch (lc->get_chat_status()) {
            case LayerChat::CHAT_STATUS::GROUP_CHAT:
            {
                uint32_t group_id = lc->get_chat_group_id();
                clear_unreal_message_by(group_id, MESSAGETYPE::GROUPCHAT);
                std::vector<msg::ChatMessage> vec_message;
                const msg::ChatMessages* chatmessages = get_chats(group_id, MESSAGETYPE::GROUPCHAT);
                if (chatmessages)
                {
                    for (auto messageIter : chatmessages->message())
                    {
                        vec_message.push_back(messageIter);
                    }
                }
                lc->update_chat_message_datas(vec_message);
                
                break;
            }
            case LayerChat::CHAT_STATUS::FRIEND_CHAT:
            {
                uint32_t friend_id = lc->get_chat_friend_id();
                clear_unreal_message_by(friend_id, MESSAGETYPE::FRIENDCHAT);
                std::vector<msg::ChatMessage> vec_message;
                const msg::ChatMessages* chatmessages = get_chats(friend_id, MESSAGETYPE::FRIENDCHAT);
                if (chatmessages)
                {
                    for (auto messageIter : chatmessages->message())
                    {
                        vec_message.push_back(messageIter);
                    }
                }
                lc->update_chat_message_datas(vec_message);
                
                break;
            }
            default:
                break;
        }
    }
    
    if (opr)
    {
        OfcBackGroundLayer* bg = dynamic_cast<OfcBackGroundLayer*>(opr->getChildByName("BACKGROUND_LAYER_TAG"));
        if (bg)
        {
            LayerGameChat* lgc = dynamic_cast<LayerGameChat*>(bg->getChildByName("LayerGameChat"));
            if (lgc)
            {
                switch (lgc->get_chat_status())
                {
                    case LayerChat::CHAT_STATUS::GROUP_CHAT:
                    {
                        uint32_t group_id = lgc->get_chat_group_id();
                        clear_unreal_message_by(group_id, MESSAGETYPE::GROUPCHAT);
                        std::vector<msg::ChatMessage> vec_message;
                        const msg::ChatMessages* chatmessages = get_chats(group_id, MESSAGETYPE::GROUPCHAT);
                        if (chatmessages)
                        {
                            for (auto messageIter : chatmessages->message())
                            {
                                vec_message.push_back(messageIter);
                            }
                        }
                        lgc->update_chat_message_datas(vec_message);
                        
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
}

void ChatMessageManager::update_unread_message_count_layer()
{
    LayerMain * lm = dynamic_cast<LayerMain*>(SceneManager::get_layer_by_name(typeid(LayerMain).name()));
    if (lm)
    {
        lm->update_unread_message_count();
    }
    
    LayerGroupList * lgl = dynamic_cast<LayerGroupList*>(SceneManager::get_layer_by_name(typeid(LayerGroupList).name()));
    if (lgl)
    {
        lgl->update_unread_message_count();
    }
    
    LayerFriendList * lfl = dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()));
    if (lfl)
    {
        lfl->update_unread_message_count();
    }
    newOfcPokerRoom* opr = dynamic_cast<newOfcPokerRoom*>(SceneManager::GetRoomLayer());
    if (opr)
    {
        OfcBackGroundLayer* bg = dynamic_cast<OfcBackGroundLayer*>(opr->getChildByName("BACKGROUND_LAYER_TAG"));
        if (bg)
        {
            bg->update_chat_message();
        }
    }

//    OfcBackGroundLayer * ofcbg = dynamic_cast<OfcBackGroundLayer*>(SceneManager::get_layer_by_name(typeid(OfcBackGroundLayer).name()));
}