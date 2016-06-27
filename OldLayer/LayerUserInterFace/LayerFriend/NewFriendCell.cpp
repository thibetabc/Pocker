//
//  NewFriendCell.cpp
//  pokerclient
//
//  Created by ll on 15/8/20.
//
//

#include "NewFriendCell.h"
#include "GameDataManager.h"
#include "utils.h"

bool NewFriendCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeFriendBaseCell.csb");
    this->addChild(root);
    
    image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg"));
    text_player_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_base_name"));
    
    text_player_name_->setString(tools::local_string("add_friend", "添加好友"));
    
    auto image_red_ball = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_red_ball"));
    auto text_unread_message_count = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_unread_message_count"));
    
    image_red_ball->setVisible(false);
    text_unread_message_count->setVisible(false);
    
    auto image_icon = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon_bg"));
    image_icon->loadTexture("login_res/friend_res/image_friend_icon_add.png");
    
    auto image_icon_2 = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon"));
    image_icon_2->setVisible(false);
    
    return true;
}

void NewFriendCell::update_property(const msg::UserDetailInfo& friend_data)
{
    set_friend_data(friend_data);
}

void NewFriendCell::set_is_touch(bool is_touch)
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