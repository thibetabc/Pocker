//
//  ChatMemberCell.cpp
//  pokerclient
//
//  Created by ll on 15/9/23.
//
//

#include "ChatMemberCell.h"
#include "utils.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "cmd_def.h"
#include "LayerFriendDetails.h"

bool ChatMemberCell::init()
{
    if (!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeGroupMember.csb");
    this->addChild(root);
    sprite_icon_ = nullptr;
    root_ = root;
    image_head_pic_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_head_pic"));
    text_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_name"));
    btn_delete_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_delete"));
    
    btn_delete_->setZOrder(10);
    btn_delete_->addClickEventListener(std::bind(&ChatMemberCell::click_btn_delete, this, std::placeholders::_1));
    btn_delete_->setSwallowTouches(false);
    
    image_head_pic_->addTouchEventListener(std::bind(&ChatMemberCell::touch_image_head_pic, this, std::placeholders::_1, std::placeholders::_2));
    image_head_pic_->setSwallowTouches(false);
    return true;
}

void ChatMemberCell::click_btn_delete(Ref* sender)
{
    GDM->send_kick_from_group(group_data_->groupid(), user_.userid());
}
bool isTouchMove = false;
void ChatMemberCell::touch_image_head_pic(Ref* sender, Widget::TouchEventType event)
{
    //群主自己，不能踢自己
    if (cell_type == ChatMemberCell::CELLTYPE::GROUP)
    {
        
        switch (event) {
            case Widget::TouchEventType::BEGAN: {
                is_touch_time_ = 0.0;
                positionX = ((ImageView *)sender)->getParent()->getParent()->getPositionX();
                this->schedule(std::bind(&ChatMemberCell::touch_header_image, this, std::placeholders::_1), 0.1, "click_delete_timer");
                break;
            }
            case Widget::TouchEventType::MOVED: {
                
                isTouchMove = true;
                break;
            }
            case Widget::TouchEventType::ENDED: {
                this->unschedule("click_delete_timer");
                if (is_touch_time_ > 0.9){
                    if(group_data_->groupownerid() != GDM->get_user_id()){
                        is_touch_time_ = 0.0;
                        return;
                    }
                    if(callback) {
                        callback(true);
                    }
                    
                    btn_delete_->setVisible(true);
                    is_touch_time_ = 0.0;
                }
                else if (!btn_delete_->isVisible())
                {
                    if (std::abs(image_head_pic_->getTouchEndPosition().x - image_head_pic_->getTouchBeganPosition().x) < 5)
                        isTouchMove = false;
                    if (isTouchMove)
                    {
                        isTouchMove = false;
                        break;
                    }
                    
                    LayerFriendDetails* glc = LayerFriendDetails::create();
                    
                    
                    glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
                    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent()->getParent()->getParent());
                    parent->addChild(glc, 2);
                    glc->update_property(friend_data_);
//                    if(GDM->get_user_data()->get_id() == GDM->get_select_friend_id()){
//                        glc->update_type(4, false);
//                    }else
//                    {
//                        if (GDM->get_my_friends().find(new_friend_data->get_userid()) != GDM->get_my_friends().end())
//                        {
//                            glc->update_type(3, true);
//                        }else{
//                            glc->update_type(1, true);
//                        }
//                    }

                    
                }else if (is_touch_time_ < 0.9) {
                    if(btn_delete_->isVisible()) {
                        btn_delete_->setVisible(false);
                        return;
                    }
                }
                is_touch_time_ = 0.0;
                
                break;
            }
            case Widget::TouchEventType::CANCELED: {
                break;
            }
            default: {
                break;
            }
        }
    }else if (cell_type == ChatMemberCell::CELLTYPE::FRIEND){
        switch (event) {
            case Widget::TouchEventType::BEGAN: {
                break;
            }
            case Widget::TouchEventType::MOVED: {
                break;
            }
            case Widget::TouchEventType::ENDED: {
                    LayerFriendDetails* glc = LayerFriendDetails::create();
                    glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
                    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent()->getParent()->getParent()->getParent());
                    parent->addChild(glc, 2);
                    glc->update_property(friend_data_);

//                if(GDM->get_user_id() == GDM->get_select_friend_id()){
//                    glc->update_type(4, true);
//                }else
//                {
//                    if (GDM->get_my_friends().find(friend_data_->get_userid()) != GDM->get_my_friends().end())
//                    {
//                        glc->update_type(3, true);
//                    }else{
//                        glc->update_type(1, true);
//                    }
//                    
//                }
                
                break;
            }
            case Widget::TouchEventType::CANCELED: {
                break;
            }
            default: {
                break;
            }
        }
    }
}

void ChatMemberCell::touch_header_image(float dt)
{
    is_touch_time_ += 0.1;
    if (is_touch_time_ > 0.9){
        if(group_data_->groupownerid() != GDM->get_user_id()){
            is_touch_time_ = 0.0;
            return;
        }
        if(callback) {
            callback(true);
        }
        btn_delete_->setVisible(true);
    }
    cocos2d::log("is_touch_time : %f", is_touch_time_);
}

void ChatMemberCell::update_ui(const msg::UserDetailInfo& user)
{
    cell_type = ChatMemberCell::CELLTYPE::GROUP;
    user_ = user;
    
    btn_delete_->setVisible(false);
    
    if(GDM->is_file_exist(user_.picname())) {
        image_head_pic_->setOpacity(0);
        image_head_pic_->loadTexture(user_.picname());
        
        if (sprite_icon_ && sprite_icon_->getParent()) {
            sprite_icon_->removeFromParent();
        }
        sprite_icon_ = ShaderSprite::create(user_.picname(), "image/head_mask.png");
        sprite_icon_->setScale(image_head_pic_->getContentSize().width / sprite_icon_->getContentSize().width,
                               image_head_pic_->getContentSize().height / sprite_icon_->getContentSize().height);
        sprite_icon_->setPosition(image_head_pic_->getPosition());
        root_->addChild(sprite_icon_, image_head_pic_->getZOrder());
    }
    else {
        register_download_events();
        
        FM.download_file(user_.picname());
    }
    text_name_->setString(user_.nickname());
}

void ChatMemberCell::update_friend_ui(const msg::UserDetailInfo& set_friend_data)
{
    cell_type = ChatMemberCell::CELLTYPE::FRIEND;
    friend_data_ = set_friend_data;
    btn_delete_->setVisible(false);

    btn_delete_->setVisible(false);
    
    if(GDM->is_file_exist(friend_data_.picname())) {
        image_head_pic_->setOpacity(0);
        image_head_pic_->loadTexture(friend_data_.picname());
        
        if (sprite_icon_ && sprite_icon_->getParent()) {
            sprite_icon_->removeFromParent();
        }
        sprite_icon_ = ShaderSprite::create(friend_data_.picname(), "image/head_mask.png");
        sprite_icon_->setScale(image_head_pic_->getContentSize().width / sprite_icon_->getContentSize().width,
                               image_head_pic_->getContentSize().height / sprite_icon_->getContentSize().height);
        sprite_icon_->setPosition(image_head_pic_->getPosition());
        root_->addChild(sprite_icon_, image_head_pic_->getZOrder());
    }
    else {
        register_download_events();
        
        FM.download_file(friend_data_.picname());
    }
    text_name_->setString(friend_data_.nickname());
}

void ChatMemberCell::register_download_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, ChatMemberCell::handle_download_events));
}

void ChatMemberCell::handle_download_events(cmd_data_pointer)
{
    if (cell_type == ChatMemberCell::CELLTYPE::GROUP){
        update_ui(user_);
    }else if (cell_type == ChatMemberCell::CELLTYPE::FRIEND){
        update_ui(friend_data_);
    }
    

}