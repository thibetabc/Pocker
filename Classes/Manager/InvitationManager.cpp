//
//  InvitationManager.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/13.
//
//

#include "InvitationManager.h"
#include "GameDataManager.h"
#include "file_utils.h"
#include "ProcessorManager.h"
#include "NET_CMD.h"
#include "net_data_recv.h"
#include "cocos2d.h"
#include "TipView.h"
#include "LayerInvitationList.h"
#include "LayerMain.h"
#include "LayerFriendList.h"

InvitationManager* InvitationManager::__Invitation_manager__ = nullptr;



InvitationManager::InvitationManager()
{
    register_events();
}

void InvitationManager::register_events()
{
    dispatch_.register_event(RECEIVE_REQUEST_LIST,
                             BIND_FUNC(this, InvitationManager::handle_receive_request_list));
}

//新的朋友消息数量
void InvitationManager::add_request(msg::Processor_123_ReceiveRequestList processor)
{
    //unread_invitation_count_ = 0;
    for (int i = 0; i < processor.request().size(); i++)
    {
        msg::Invitation newinvitation;
        newinvitation.mutable_request()->CopyFrom(processor.request(i));
        newinvitation.set_status(msg::InvitationStatus::Waiting);
        auto key = processor.request(i).id();
        if(invitations_.find(key) == invitations_.end())
        {
            invitations_[key] = newinvitation;
            unread_invitation_count_++;
        }
    }
    update_unread_invitation_count();
}

void InvitationManager::deal_request(uint32_t requestid, bool result)
{
    auto it = invitations_.find(requestid);
    if (it != invitations_.end()){
        it->second.set_status(result ? msg::InvitationStatus::Accepted : msg::InvitationStatus::Refused);
        it->second.set_sendingstatus(msg::MessageStatus::Sending);
    }
    
    save_invitation_record();
    LayerInvitationList* layer = dynamic_cast<LayerInvitationList*>(SceneManager::get_layer_by_name(typeid(LayerInvitationList).name()));
    if(layer)
    {
        layer->refresh_deal_data();
    }
}

const std::vector<msg::Invitation> InvitationManager::get_vec_invitations_()
{
    
    std::vector<msg::Invitation> result;
    for(auto& iter : invitations_) {
        result.push_back(iter.second);
    }
    
    std::sort(result.begin(), result.end(), [&](const msg::Invitation& data1, const msg::Invitation& data2) {
        return data1.status() > data2.status();
    });
    return result;
}

void InvitationManager::save_invitation_record()
{
    if(invitations_.empty()) {
        std::string user_cache = GDM->get_user_directory();
        file_utils::delete_directory(user_cache + "invitation_data_record.a");
    }
    
    std::shared_ptr<msg::InvitationsForSave> pb_invitation_datas(new msg::InvitationsForSave());
    int max_count = 50;
    std::list<msg::Invitation> invitations;
    for (auto iter = invitations_.rbegin(); iter != invitations_.rend(); ++iter)
    {
        invitations.push_back(iter->second);
    }
    invitations.sort([](const msg::Invitation& c1, const msg::Invitation& c2)->bool {
        return c1.request().id() < c2.request().id();
    });
    while (invitations.size() > max_count) {
        invitations.pop_front();
    }
    for (auto & it : invitations){
        auto data = pb_invitation_datas->add_invitations();
        data->CopyFrom(it);
    }
    if(invitations_.size() > 0) {
        std::string result = "";
        
        bool succ = pb_invitation_datas->SerializeToString(&result);
        if(succ) {
            std::string user_cache = GDM->get_user_directory();
            file_utils::write_file(user_cache + "invitation_data_record.a", result);
        }
    }
}

void InvitationManager::init_invitation_records()
{
    invitations_.clear();
    std::string file_path = GDM->get_user_directory() + "invitation_data_record.a";
    if(file_utils::file_exists(file_path)) {
        auto content = file_utils::read_file(file_path);
        msg::InvitationsForSave pb_invitation_datas;
        bool succ = pb_invitation_datas.ParseFromString(content);
        if(succ) {
            for (int j = 0; j < pb_invitation_datas.invitations().size(); j++)
            {
                auto pb_cb = pb_invitation_datas.invitations(j);
                msg::Invitation cd;
                cd.CopyFrom(pb_cb);
                if (invitations_.find(cd.request().id()) == invitations_.end())
                    invitations_.insert(std::make_pair(cd.request().id(), cd));
            }
        }
    }
}

void InvitationManager::clear_unread_invitation_count()
{
    unread_invitation_count_ = 0;
    save_invitation_record();
    update_unread_invitation_count();
}

void InvitationManager::handle_receive_request_list(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->ReceiveRequestList_down(rec);
    add_request(processor);
}

void InvitationManager::update_unread_invitation_count()
{
    LayerMain * lm = dynamic_cast<LayerMain*>(SceneManager::get_layer_by_name(typeid(LayerMain).name()));
    if (lm)
    {
        lm->update_unread_message_count();
    }

    LayerFriendList * lfl = dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()));
    if (lfl)
    {
        lfl->update_unread_invitation_count();
    }
}
