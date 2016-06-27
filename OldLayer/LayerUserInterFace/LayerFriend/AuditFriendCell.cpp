//
//  AuditFriendCell.cpp
//  pokerclient
//
//  Created by ll on 15/8/20.
//
//

#include "AuditFriendCell.h"
#include "ProcessorManager.h"
#include "GameDataManager.h"
#include "utils.h"
#include "ShaderSprite.h"
#include "FileManager.h"
#include "cmd_def.h"

bool AuditFriendCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeAuditFriend.csb");
    this->addChild(root);
    
    sprite_icon_ = nullptr;
    root_ = root;
    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon"));
    
    image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg"));
    text_player_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_player_nick_name"));
    text_player_remark_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_player_remark"));
    btn_confirm_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_confirm"));
    text_has_confirm_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_has_confirm"));
    text_has_confirm_->setVisible(false);
    
    btn_confirm_->addClickEventListener(std::bind(&AuditFriendCell::btn_click_confirm, this, std::placeholders::_1));
    
    return true;
}

void AuditFriendCell::update_property(const msg::Invitation& request_data)
{
    invitation_data_.CopyFrom(request_data);
    update_ui();
}

void AuditFriendCell::update_ui()
{
    if(!invitation_data_.request().userdetailinfo().picname().empty())
    {
        if(GDM->is_file_exist(invitation_data_.request().userdetailinfo().picname())) {
            image_icon_->setOpacity(0);
            image_icon_->loadTexture(invitation_data_.request().userdetailinfo().picname());
            
            if (sprite_icon_ && sprite_icon_->getParent()) {
                sprite_icon_->removeFromParent();
            }
            sprite_icon_ = ShaderSprite::create(invitation_data_.request().userdetailinfo().picname(), "login_res/friend_res/node_friend_icon_mask.png");
            sprite_icon_->setScale(image_icon_->getContentSize().width / sprite_icon_->getContentSize().width,
                                   image_icon_->getContentSize().height / sprite_icon_->getContentSize().height);
            sprite_icon_->setPosition(image_icon_->getPosition());
            root_->addChild(sprite_icon_, image_icon_->getZOrder());
            //this->addChild(sprite_icon_, image_icon_->getZOrder());
        }
        else {
            register_download_events();
            
            FM.download_file(invitation_data_.request().userdetailinfo().picname());
        }
    }
    
    text_player_name_->setString(invitation_data_.request().userdetailinfo().nickname());
    
    if (invitation_data_.request().type() == msg::RequestType::ADDFRIEND)
    {
        text_player_remark_ ->setString(tools::local_string("request_to_be_friend", "请求添加你为好友"));
    }else if (invitation_data_.request().type() == msg::RequestType::ENTERGROUP){
        std::string mark = tools::local_string("applying_group", "申请加入  ") + invitation_data_.request().groupinfo().groupname() + tools::local_string("group", "群");
        text_player_remark_ ->setString(mark);
    }else if (invitation_data_.request().type() == msg::RequestType::INVITETOGROUP){
        std::string mark = invitation_data_.request().userdetailinfo().nickname() + tools::local_string("inviting_you_to_join", "  邀请你加入  ") + invitation_data_.request().groupinfo().groupname() + tools::local_string("group", "群");
    }
    if (invitation_data_.status() == msg::InvitationStatus::Waiting){
        btn_confirm_->setVisible(true);
        btn_confirm_->setEnabled(true);
        btn_confirm_->setTitleText(tools::local_string("operate", "同意"));
        
        
        text_has_confirm_->setVisible(false);
    }else {
        btn_confirm_->setEnabled(false);
        btn_confirm_->setVisible(false);
        std::string s = tools::local_string("rejected", "已拒绝");
        if(invitation_data_.status() == msg::InvitationStatus::Accepted)
        {
            s = tools::local_string("agreed", "已同意");
        }
        text_has_confirm_->setString(s);
            
        text_has_confirm_->setVisible(true);
    }
}

void AuditFriendCell::register_download_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, AuditFriendCell::handle_download_events));
}
void AuditFriendCell::handle_download_events(cmd_data_pointer)
{
    update_ui();
}

void AuditFriendCell::btn_click_confirm(Ref * sender)
{
    auto processor = PM->DealRequest_up(invitation_data_.request().id(), true);
    net_manager::shared_instence().send_data(processor,REQ_DEAL_REQUEST);
}

void AuditFriendCell::set_is_touch(bool is_touch)
{
    std::string image_bg_name;
    if (is_touch)
    {
        image_bg_name = "login_res/friend_res/image_bg_select.png";
        image_bg_->loadTexture(image_bg_name);
    }else {
        image_bg_name = "login_res/friend_res/image_bg.png";
        image_bg_->loadTexture(image_bg_name);
    }
}