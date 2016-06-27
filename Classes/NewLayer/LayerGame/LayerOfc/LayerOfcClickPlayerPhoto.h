//
//  LayerOfcClickPlayerPhoto.hpp
//  pokerclient
//
//  Created by zhongyu on 16/5/19.
//
//

#ifndef LayerOfcClickPlayerPhoto_hpp
#define LayerOfcClickPlayerPhoto_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "ShaderSprite.h"
#include "NET_CMD.h"
#include "cmd_def.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerOfcClickPlayerPhoto : public BaseLayer, public EditBoxDelegate {
public:
    CREATE_FUNC(LayerOfcClickPlayerPhoto);
    virtual bool init();
    
private:
    
#pragma mark ui
    Node* root_;
    
    Button* btn_voice_play_;
    Button* btn_tick_player_;
    Text* text_user_name_;
    Text* text_user_id_;
    Text* text_tool_title_;
    Text* text_tool_desc_;
    
    Button* btn_male_;
    Button* btn_female_;
    
    Layout* layout_credit_value_;
    Text* title_credit_value_;
    TextField* textfield_credit_value_;
    EditBox* editbox_credit_value_;
    
    ImageView* image_ofc_click_other_bg_;
    
    Node* Node_user_photo_;
    ShaderSprite* sprite_icon_;
    ImageView* image_tool_bg_1_;
    ImageView* image_tool_bg_2_;
    ImageView* image_tool_bg_3_;
    ImageView* image_tool_bg_4_;
    ImageView* image_tool_bg_5_;
    ImageView* image_tool_bg_6_;
    ImageView* image_tool_bg_7_;
    ImageView* image_tool_bg_8_;
    ImageView* image_tool_bg_9_;
    ImageView* image_tool_bg_10_;
#pragma mark value
    msg::UserDetailInfo user_info_;
    uint32_t group_id_;
    uint32_t room_id_;
    uint32_t owner_id_;
    bool is_ofc_;
    std::vector<std::string> tool_bg_name = {"image_tool_bg_1","image_tool_bg_2","image_tool_bg_3","image_tool_bg_4","image_tool_bg_5","image_tool_bg_6","image_tool_bg_7","image_tool_bg_8","image_tool_bg_9","image_tool_bg_10"};
    std::string tool_name_ = "image_ofc_tool_";
    std::map<ImageView*,msg::Game_Tool_Type> tools_;
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
public:
    void click_btn_voice_play(Ref* sender);
    void click_btn_tick_player(Ref* sender);
    void click_image_tool(msg::Game_Tool_Type type);
    
    bool onTouchBegan(Touch* touch, Event* event)
    {
        return true;
    };
    void onTouchMoved(Touch* touch, Event* event){};
    void onTouchEnded(Touch* touch, Event* event);
    
    void set_player_info(const msg::UserDetailInfo info, uint32_t group_id, uint32_t owner_id, uint32_t room_id,bool is_ofc = false);
    void addUserPhoto();
    
    
    void register_events();
    void handle_download_event(cmd_data_pointer data);
    void handle_get_permission(cmd_data_pointer data);
    void handle_change_permission(cmd_data_pointer data);
};






#endif /* LayerOfcClickPlayerPhoto_hpp */
