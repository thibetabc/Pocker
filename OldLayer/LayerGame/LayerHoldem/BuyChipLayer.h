#pragma once


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "Room.h"
#include "GameLayer.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class BuyChipLayer : public PokerChildLayer
{
public:
    
    CREATE_FUNC(BuyChipLayer);
    virtual bool init();
    
    void click_btn_cancel(Ref* sender);
    void click_btn_confirm(Ref* sender);
    void slider_slider_chip(Ref* sender, Slider::EventType);
    
    void click_btn_add(Ref* sender);
    void click_btn_sub(Ref* sender);
    void Refresh();
#pragma mark touch delegate
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    /** Callback function for touch moved.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    /** Callback function for touch ended.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
protected:
    void register_events();
private:
    Text* text_max_number_;
    Text* text_min_number_;
    Text* text_buy_number_;
    
    Slider* slider_chip_;
    msg::GroupPermission permission_;
    int amount_;
    void handle_get_permission(cmd_data_pointer data);
};