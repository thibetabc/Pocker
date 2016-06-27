#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "events.h"
#include "ShaderSprite.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class UserInfoLayer : public BaseLayer {
public:
    CREATE_FUNC(UserInfoLayer);
    virtual bool init();
    void click_charge(Ref *sender);
    void click_btn_back(Ref* sender);
    void click_image_user_head(Ref* sender);
    void click_image_username(Ref* sender);
    void click_image_password(Ref* sender);
    void click_image_sex(Ref* sender);
    void click_image_game_setting(Ref* sender);
    void click_image_bind_phone(Ref * sender);
    
    void update_data(cmd_data_pointer data = nullptr);

    void register_events();
    
    void set_update_head_pic_callback(const std::function<void()>& callback) { update_head_pic_callback_ = callback;}
private:
    void handle_upload_succeed(cmd_data_pointer data);
    void after_choose_pic(const std::string& filename);
private:
    std::string m_StrPicName;
    Text* text_title_;
    Text* const_text_head_;
    Text* const_text_username_;
    Text* text_username_;
    Text* text_password_;
    Text* const_text_sex_;
    Text* text_sex_;
    
    ImageView* image_head_;
    ImageView* image_user_head_;
    
    Text* text_bind_phone_;
    Text* text_bind_phone_status_;
    ImageView* image_bind_phone_;
    ImageView* image_bind_phone_1_;
    
    events::event_dispatch dispatch_;
    
    std::function<void()> update_head_pic_callback_;
    
    cocos2d::ShaderSprite* sprite_icon_;
    
    Node* root_;
    msg::UserDetailInfo userinfo;
    bool m_bIsUploadSuccess;
};