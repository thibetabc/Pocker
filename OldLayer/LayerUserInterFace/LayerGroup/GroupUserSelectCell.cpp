//
//  GroupUserSelectCell.cpp
//  pokerclient
//
//  Created by ll on 15/9/9.
//
//

#include "GroupUserSelectCell.h"
#include "GameDataManager.h"
#include "utils.h"
#include "ShaderSprite.h"
#include "FileManager.h"
#include "cmd_def.h"

bool GroupUserSelectCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeGroupUserSelect.csb");
    this->addChild(root);
    
    sprite_icon_ = nullptr;
    root_ = root;
    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon"));
    
    image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg"));
    
    text_player_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_player_name"));
    
    sprite_select_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_select"));
    
    return true;
}


void GroupUserSelectCell::update_property(const msg::UserDetailInfo& friend_data)
{
    set_friend_data(friend_data);
    
    text_player_name_->setString(friend_data.nickname());

    update_ui();
}

void GroupUserSelectCell::update_ui()
{
    if(!friend_data_.has_picname())
    {
        if(GDM->is_file_exist(friend_data_.picname())) {
            image_icon_->setOpacity(0);
            image_icon_->loadTexture(friend_data_.picname());
            
            if (sprite_icon_ && sprite_icon_->getParent()) {
                sprite_icon_->removeFromParent();
            }
            sprite_icon_ = ShaderSprite::create(friend_data_.picname(), "image/head_mask.png");
            sprite_icon_->setScale(image_icon_->getContentSize().width / sprite_icon_->getContentSize().width,
                                   image_icon_->getContentSize().height / sprite_icon_->getContentSize().height);
            sprite_icon_->setPosition(image_icon_->getPosition());
            root_->addChild(sprite_icon_, image_icon_->getZOrder());
            //this->addChild(sprite_icon_, image_icon_->getZOrder());
        }
        else {
            register_download_events();
            
            FM.download_file(friend_data_.picname());
        }
    }
}

void GroupUserSelectCell::register_download_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, GroupUserSelectCell::handle_download_events));
}
void GroupUserSelectCell::handle_download_events(cmd_data_pointer)
{
    update_ui();
}

void GroupUserSelectCell::set_select(const uint8_t status)
{
    is_select_ = status;
    if (status == GroupUserSelectCell::select_status::ISTRUE){
        sprite_select_->setTexture("login_res/friend_res/select_11.png");
    }else if (status == GroupUserSelectCell::select_status::ISFALSE){
        sprite_select_->setTexture("login_res/friend_res/select_17.png");
    }else if (status == GroupUserSelectCell::select_status::ISFORBID){
        sprite_select_->setTexture("login_res/friend_res/select_05.png");
    }
}

void GroupUserSelectCell::set_is_touch(bool is_touch)
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
