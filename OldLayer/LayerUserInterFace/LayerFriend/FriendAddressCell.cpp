//
//  FriendAddressCell.cpp
//  pokerclient
//
//  Created by ll on 15/8/27.
//
//

#include "FriendAddressCell.h"
#include "GameDataManager.h"
#include "utils.h"
#include "ShaderSprite.h"
#include "events.h"
#include "FileManager.h"
#include "cmd_def.h"

bool FriendAddressCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeFriendBaseCell.csb");
    this->addChild(root);
    
    image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg"));
    text_player_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_base_name"));
    auto image_red_ball = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_red_ball"));
    auto text_unread_message_count = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_unread_message_count"));
    auto image_icon = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon_bg"));
    image_icon->loadTexture("login_res/friend_res/image_friend_icon_address.png");
    auto image_icon_2 = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon"));
    image_icon_2->setVisible(false);
    
    image_red_ball->setVisible(false);
    text_unread_message_count->setVisible(false);
    
    text_player_name_->setString(tools::local_string("Address_book", "通讯录"));
    
    return true;
}

void FriendAddressCell::update_property(const msg::UserDetailInfo& friend_data)
{
    set_friend_data(friend_data);
    //text_player_name_->setString(friend_data->get_nick_name());
    //image_icon_->loadTexture(group_data->get_pic());
    
    //uint32_t unread_message_count = GDM->get_friend_unread_invaitation_count();
    //const std::string submit_time = GDM->get_last_message_submit_time(group_data->get_id());
    //uint32_t unread_message_count = 3;//GDM->get_unread_message_count(group_data->get_id());
    
    
    //std::string text;
    //if(!submit_time.empty()) {
    //    double time = tools::stod(submit_time);
    //    time /= 1000;
    //    text = tools::time_display(time);
    //}
    //text_time_->setString(text);
    //image_red_ball_->setVisible(unread_message_count != 0);
    //text_unread_message_count_->setVisible(unread_message_count != 0);
    //text_unread_message_count_->setString(tools::to_string(unread_message_count));
}

void FriendAddressCell::set_is_touch(bool is_touch)
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