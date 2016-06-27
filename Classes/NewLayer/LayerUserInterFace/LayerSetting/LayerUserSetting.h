//
//  LayerUserSetting.hpp
//  pokerclient
//
//  Created by zhongyu on 16/3/31.
//
//

#ifndef LayerUserSetting_hpp
#define LayerUserSetting_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "events.h"
#include "cmd_def.h"
#include "ShaderSprite.h"
#include "ImagePickerBridge.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerUserSetting : public BaseLayer
{
public:
    CREATE_FUNC(LayerUserSetting);
    virtual bool init();
#pragma mark ui
private:
    std::string picName_;
    msg::Sex Sex_;
    
    Node* root_;
    ImageView* panel_other_function_;
    bool is_choose_photo_ = false;
#pragma mark setting_layer_ui
    Button* btn_back_;
    Button* btn_buy_diamond_;
    Button* btn_charge_stamina_;
    Button* btn_bind_phone_;
    Button* btn_change_sex_;
    Button* btn_nickname_change_;
    Button* btn_change_password_;
    Button* btn_enter_record_layer_;
    Button* btn_game_setting_;
    Button* btn_female_;
    Button* btn_male_;
    
    
    Text* text_title_;
    Text* text_user_account_;
    Text* text_name_tip_;
    Text* text_name_;
    EditBox* tf_user_name_;
    Text* text_diamond_tip_;
    Text* text_diamond_;
    Text* text_stamina_tip_;
    Text* text_stamina_;
    Text* text_phone_tip_;
    Text* text_bind_phone_;
    Text* text_user_sex_;
    
    ImageView* image_nick_name_bg_;
    Text* text_score_title_;
    Text* text_score_game_type_;
    Text* text_score_time_;
    Text* text_score_first_;
    Text* text_score_second_;
    Text* text_score_third_;
    Text* text_score_first_num_;
    Text* text_score_second_num_;
    Text* text_score_third_num_;
    
#pragma mark other_function_ui
    Button* btn_other_function_;
    Text* btn_user_response_;
    Text* btn_exit_game_;
    Text* btn_enter_share_layer_;
    Layout* other_func_layer_;
#pragma mark choose_photo_ui
    Node* node_people_image_;
#pragma mark other_abriables
    ImagePickerBridge* picker_;
    ShaderSprite* sprite_icon_;
    void after_choose_pic(const std::string& filename);
    void refreshPhoneNumber(const std::string& PhoneNumber);
    std::string user_nickname_;
    std::string change_picname_;
    bool is_tournament_;
#pragma mark click_events
public:
    void click_btn_back(Ref* sender);
    void click_btn_buy_diamond(Ref* sender);
    void click_btn_charge_stamina(Ref* sender);
    void click_btn_bind_phone(Ref* sender);
    void click_btn_enter_record_layer(Ref* sender);
    void click_btn_enter_share_layer(Ref*sender);
    void click_btn_game_setting(Ref* sender);
    void click_btn_user_response(Ref* sender);
    void click_btn_exit_game(Ref* sender);
    void click_btn_change_password(Ref* sender);
    void click_btn_other_function(Ref* sender);
    void click_btn_change_sex(Ref* sender);
    void click_btn_nickname_change(Ref* sender);

    void addUserPhoto();
    cocos2d::Vec2 GetErrorPoint();
    void is_show_other_func_part(bool show);
    
#pragma mark touch
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
#pragma mark GDM refresh
public:
    void setPhoneNumber(const std::string& phone_number);
    void set_game_type();
private:
    void refreshUserInfo(std::string nickName,std::string picName,int diamond,int hit,std::string phoneNumber,msg::Sex sex);
protected:
    void RefreshLayer();
    void register_events();
    void handle_download_event(cmd_data_pointer data);
    void handle_upload_event(cmd_data_pointer data);
    void handle_get_user_setting(cmd_data_pointer data);
    void handle_change_user_nickname(cmd_data_pointer  data);
    void handle_get_score(cmd_data_pointer  data);
};
extern ImagePickerBridge* __createImagePicker(const ImagePickerType type);
#endif /* LayerUserSetting_hpp */
