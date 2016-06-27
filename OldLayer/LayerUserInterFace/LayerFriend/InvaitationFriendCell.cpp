//
//  InvaitationFriendCell.cpp
//  pokerclient
//
//  Created by ll on 15/8/20.
//
//

#include "InvaitationFriendCell.h"
#include "InvitationManager.h"
#include "GameDataManager.h"
#include "utils.h"
#include "ShaderSprite.h"
#include "events.h"
#include "FileManager.h"
#include "cmd_def.h"

bool InvaitationFriendCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeFriendBaseCell.csb");
    this->addChild(root);
    
    image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg"));
    text_player_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_base_name"));
    image_red_ball_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_red_ball"));
    text_unread_message_count_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_unread_message_count"));
    auto image_icon = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon_bg"));
    image_icon->loadTexture("login_res/friend_res/image_friend_icon_inviatation.png");
    
    auto image_icon_2 = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon"));
    image_icon_2->setVisible(false);
    
    text_player_name_->setString(tools::local_string("Invitation", "邀请函"));
    dispatch_.register_event(EVENT_CLEAR_UNREAD_INVAITATION, BIND_FUNC(this, InvaitationFriendCell::update_ui));
    
    return true;
}

void InvaitationFriendCell::update_property(const msg::UserDetailInfo& friend_data)
{
    set_friend_data(friend_data);
    update_ui();
}

void InvaitationFriendCell::update_ui(cmd_data_pointer)
{
    uint32_t unread_message_count = IM->get_unread_invitation_count();
    image_red_ball_->setVisible(unread_message_count != 0);
    text_unread_message_count_->setVisible(unread_message_count != 0);
    text_unread_message_count_->setString(tools::to_string(unread_message_count));
}

void InvaitationFriendCell::set_is_touch(bool is_touch)
{
    std::string image_bg_name;
    if (is_touch)
    {
        image_bg_name = "login_res/friend_res/image_friend_cell_bg_select.png";
        image_bg_->loadTexture(image_bg_name);
    }else {
        image_bg_name = "login_res/friend_res/image_friend_cell_bg.png";
        image_bg_->loadTexture(image_bg_name);
    }
}