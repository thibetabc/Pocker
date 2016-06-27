//
//  FriendCell.cpp
//  pokerclient
//
//  Created by ll on 15/8/18.
//
//

#include "FriendCell.h"
#include "GameDataManager.h"
#include "utils.h"
#include "ShaderSprite.h"
#include "FileManager.h"
#include "cmd_def.h"

bool FriendCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeFriendBaseCell.csb");
    this->addChild(root);
    
    sprite_icon_ = nullptr;
    root_ = root;
    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon"));
    image_icon_->loadTexture("login_res/friend_res/2-1_05.png");
    
    image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg"));
    text_player_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_base_name"));
    
    image_red_ball_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_red_ball"));
    text_unread_message_count_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_unread_message_count"));
    
    image_red_ball_->setZOrder(image_bg_->getZOrder()+10);
    text_unread_message_count_->setZOrder(image_bg_->getZOrder()+11);
    
    dispatch_.register_event(EVENT_CLEAR_FRIEND_UNREAD_MESSAGE,
                             BIND_FUNC(this, FriendCell::handle_download_events));
    
    return true;
}


void FriendCell::update_property(const msg::UserDetailInfo& friend_data)
{
    set_friend_data(friend_data);
    
    //image_icon_->loadTexture(group_data->get_pic());
    text_player_name_->setString(friend_data.nickname());
    //uint32_t unread_message_count = GDM->get_unread_message_count(group_data->get_id());
    //const std::string submit_time = GDM->get_last_message_submit_time(group_data->get_id());
    uint32_t unread_message_count = GDM->get_friend_unread_message_count(friend_data.userid());

    
    //std::string text;
    //if(!submit_time.empty()) {
    //    double time = tools::stod(submit_time);
    //    time /= 1000;
    //    text = tools::time_display(time);
    //}
    //text_time_->setString(text);
    image_red_ball_->setVisible(unread_message_count != 0);
    text_unread_message_count_->setVisible(unread_message_count != 0);
    text_unread_message_count_->setString(tools::to_string(unread_message_count));
    update_ui();
}

void FriendCell::update_ui()
{
    if(!friend_data_.picname().empty())
    {
        if(GDM->is_file_exist(friend_data_.picname())) {
            image_icon_->setOpacity(0);
            image_icon_->loadTexture(friend_data_.picname());
            
            if (sprite_icon_ && sprite_icon_->getParent()) {
                sprite_icon_->removeFromParent();
            }
            sprite_icon_ = ShaderSprite::create(friend_data_.picname(), "login_res/friend_res/node_friend_icon_mask.png");
            sprite_icon_->setScale(image_icon_->getContentSize().width * 1.6 / sprite_icon_->getContentSize().width,
                                   image_icon_->getContentSize().height * 1.6/ sprite_icon_->getContentSize().height);
            sprite_icon_->setPosition(image_icon_->getPosition());
            root_->addChild(sprite_icon_, image_icon_->getLocalZOrder());
        }
        else {
            register_download_events();
            
            FM.download_file(friend_data_.picname());
        }
    }
    uint32_t unread_message_count = GDM->get_friend_unread_message_count(friend_data_.userid());
    image_red_ball_->setVisible(unread_message_count != 0);
    text_unread_message_count_->setVisible(unread_message_count != 0);
    text_unread_message_count_->setString(tools::to_string(unread_message_count));
    //text_name_->setString(friend_data_->get_nickname());
}

void FriendCell::register_download_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, FriendCell::handle_download_events));
}
void FriendCell::handle_download_events(cmd_data_pointer)
{
    update_ui();
}

void FriendCell::set_is_touch(bool is_touch)
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