//
//  LayerMahjongMenu.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/7.
//
//

#ifndef LayerMahjongMenu_hpp
#define LayerMahjongMenu_hpp

#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameLayer.h"
#include "LayerGameChat.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerMahjongMenu : public MahjongChildLayer
{
public:
    CREATE_FUNC(LayerMahjongMenu);
    virtual bool init();

    void handle_receive_chat(cmd_data_pointer);
    void refresh_chat_btn();
private:
    Node* root_;
    
    ImageView* image_mahjong_menu_bg_;
    Button* btn_share_0_;
    Button* btn_chat_0_;
    Button* btn_match_0_;
    Button* btn_record_last_hand_0_;
    
    ImageView* image_red_;
    Text* text_unread_message_count_;
    
    LayerGameChat* gcl;
public:
#pragma mark click_event
    void click_btn_share(Ref* sender);
    void click_btn_chat(Ref* sender);
    void click_btn_match(Ref* sender);
    void click_btn_record_last_hand(Ref* sender);
#pragma mark touch
    bool onTouchBegan(Touch* touch, Event* event){ return true; }
    void onTouchMoved(Touch* touch, Event* event){};
    void onTouchEnded(Touch* touch, Event* event);
};
#endif /* LayerMahjongMenu_hpp */
