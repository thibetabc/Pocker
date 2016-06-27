//
//  OfcBackGroundLayer.h
//  pokerclient
//
//  Created by ll on 15/7/27.
//
//

#ifndef __pokerclient__OfcBackGroundLayer__
#define __pokerclient__OfcBackGroundLayer__

#include <stdio.h>

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cmd_def.h"
#include "user_data.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"
#include "MatchScroeNode.h"
#include "GameLayer.h"
#include "LayerGameChat.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class OFCRoom;

class OfcBackGroundLayer : public OFCChildLayer
{
public:
    OfcBackGroundLayer();
//    CREATE_FUNC(OfcBackGroundLayer);
    static OfcBackGroundLayer* create(bool is_Fantasy = false);
    virtual bool init(bool is_Fantasy);
    
    void update_record_status();
    void close(){};
    
    void update_game_status(const msg::GameStatus game_status);
    void update_room_note(const std::string room_note);
    
    void click_btn_record_back(Ref * sender);
    void click_btn_funcitem(Ref * sender);
    void click_btn_standup(Ref * sender);
    void click_btn_quit(Ref * sender);
    void click_btn_setting(Ref * sender);
    void click_btn_match_score(Ref * sender);
    void click_btn_voice(Ref * sender, Widget::TouchEventType event);
    void click_btn_chat(Ref * sender);
    void click_btn_record_last_hand(Ref * sender);
    void click_btn_share(Ref * sender);
    void click_btn_pause(Ref * sender);
    
    void show_chat_layer();
    void handle_download_events(cmd_data_pointer);
    void handle_receive_chat(cmd_data_pointer);
    
    void update_chat_message();
    
    void update_menu_status(bool is_fantasy);
    
    void register_events();
    void click_item(Ref* sender);
    void onExit()
    {
        BaseLayer::onExit();
    }
    void setPlayVoiceCallback(const std::function<void(uint32_t userId, int duration)> &callback)
    {
        mFunctionPlayVoiceCallback = callback;
    }
    
protected:
    bool IsIndepedent() {return false;}
private:
    std::function<void(uint32_t userId, int duration)> mFunctionPlayVoiceCallback;
    MatchScroeNode* msn = NULL;
    LayerGameChat* gcl = NULL;
    Node* root_;
    
    Button* btn_funcitem_;
    Button* btn_record_last_hand_;
    Button* btn_match_score_;
    Button* btn_voice_;
    Button* btn_chat_;
    Button* btn_record_back_;
    Button* btn_share_; // 分享
    Button* btn_pause_;
    
    Button* btn_funcitem_0_;
    Button* btn_share_0_;
    Button* btn_record_last_hand_0_;
    Button* btn_match_score_0_;
    Button* btn_voice_0_;
    Button* btn_chat_0_;
    Button* btn_record_back_0_;
    
    
    
//    Button* btn_funcitem_0_;
////    Button* btn_standup_;
////    Button* btn_quit_;
////    Button* btn_setting_;
//    Button* btn_record_last_hand_0_;
//    Button* btn_match_score_0_;
//    Button* btn_voice_0_;
//    Button* btn_chat_0_;
//    Button* btn_record_back_0_;
//    Button* btn_share_0_; // 分享
//    Button* btn_pause_0_;
    
    Text* text_standup_;
    Text* text_quit_;
    Text* text_setting_;
    
    Text* text_record_last_hand_;
    Text* text_match_score_;
    Text* text_voice_;
    Text* text_chat_;
    
    Text* text_table_note_;
    
    ImageView* image_red_;
    Text* text_unread_message_count_;
    
    ImageView* image_red_0_;
    Text* text_unread_message_count_0_;
    
    Vec2 point_btn_standup_;
    Vec2 point_btn_quit_;
    Vec2 point_btn_setting_;
    
    Vec2 point_text_standup_;
    Vec2 point_text_quit_;
    Vec2 point_text_setting_;
    
    bool btn_funcitem_isShow_;
private:
    std::string record_filename_;
    bool is_fantasy_;
};

#endif /* defined(__pokerclient__OfcBackGroundLayer__) */
