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
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "GameChatLayer.h"
#include "MatchScroeNode.h"
#include "GameLayer.h"
using namespace cocos2d;
using namespace cocos2d::ui;

class OFCRoom;

class OfcBackGroundLayer : public OFCChildLayer
{
public:
    OfcBackGroundLayer(){LayerPool::shared_instence().pop_layer(this);}
    CREATE_FUNC(OfcBackGroundLayer);
    virtual bool init();
    
    void update_record_status();
    void close(){};
    
    void update_game_status(const msg::GameStatus game_status);
    
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
    
    void register_events();
    void click_item(Ref* sender);
    void onExit()
    {
        Layer::onExit();
    }
private:
    
    events::event_dispatch dispatch_;
    
private:
    MatchScroeNode* msn = NULL;
    GameChatLayer* gcl = NULL;
    Node* root_;
    
    Button* btn_funcitem_;
    Button* btn_standup_;
    Button* btn_quit_;
    Button* btn_setting_;
    Button* btn_record_last_hand_;
    Button* btn_match_score_;
    Button* btn_voice_;
    Button* btn_chat_;
    Button* btn_record_back_;
    Button* btn_share_; // 分享
    Button* btn_pause_;
    
    Text* text_standup_;
    Text* text_quit_;
    Text* text_setting_;
    
    Text* text_record_last_hand_;
    Text* text_match_score_;
    Text* text_voice_;
    Text* text_chat_;
    
    ImageView* image_red_;
    Text* text_unread_message_count_;
    
    Vec2 point_btn_standup_;
    Vec2 point_btn_quit_;
    Vec2 point_btn_setting_;
    
    Vec2 point_text_standup_;
    Vec2 point_text_quit_;
    Vec2 point_text_setting_;
    
    bool btn_funcitem_isShow_;
private:
    std::string record_filename_;
};

#endif /* defined(__pokerclient__OfcBackGroundLayer__) */
