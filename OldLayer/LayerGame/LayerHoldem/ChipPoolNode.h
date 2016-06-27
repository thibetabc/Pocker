#pragma once


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include  "Room.h"
#include "msg.pb.h"
#include "net_data_recv.h"
#include "GameLayer.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class data_recv_receive_player_action;

class ChipPoolNode : public PokerChildNode
{
    
public:
    CREATE_FUNC(ChipPoolNode);
    virtual bool init();
    
    const uint32_t get_amount();
    
    void click_image_min(Ref* sender);
    void click_image_two_third(Ref* sender);
    void click_image_half_pool(Ref* sender);
    void click_image_full_pool(Ref* sender);
    void click_image_allin(Ref* sender);
    
    void click_btn_sub_chip(Ref* sender);
    void click_btn_add_chip(Ref* sender);
    void click_slider(Ref* sender, Slider::EventType);
    void clickTouchEventListener(Ref *sender, Widget::TouchEventType controlEvent);
    
    void update_data(const msg::PokerActionRequest& data);

    
    virtual void setVisible(bool vis);

    
private:
    
    float barLength;
    float slider_text_Y;
    
    Button* button_half_pool;
    Button* button_two_third_pool;
    Button* button_full_pool;
    Button* button_all_in;
    
    Text* text_chip_number_;
    Text* text_half_pool_;
    Text* text_two_third_;
    Text* text_full_pool_;
    
    Text* text_half_pool_amount;
    Text* text_two_third_pool_amount;
    Text* text_full_pool_amount;
    Text* text_all_in_amount;
    
    Text* slider_text;
    Slider* slider_chip_;
    Scale9Sprite * sliderRender;
    Sprite* chip_add_sub_bg;
  
    //msg::ActionRequest player_action_info_;
    msg::PokerActionRequest player_action_info_;
    
    std::function<void()> hide_action_btn_callback_;
    
    uint32_t amount_;
    uint32_t getBetSizeByPot(float APotRatio);
    uint32_t getAllinSize();
    void update_Chip_Amount();
    CC_SYNTHESIZE(PokerRoomLayer*, pokerRoomLayer, PokerRoomLayer);
};