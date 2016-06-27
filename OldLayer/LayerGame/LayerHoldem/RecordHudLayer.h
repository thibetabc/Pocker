#pragma once


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include "ChipNode.h"
#include "Room.h"
#include "GameLayer.h"
#include "LayerManager.h"
class PokerRoom;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class RecordHudLayer : public PokerChildLayer
{
public:
    CREATE_FUNC(RecordHudLayer);
    virtual bool init();
    
    void click_btn_back(Ref* sender);
    void click_btn_play(Ref* sender);
    void click_btn_pre(Ref* sender);
    void click_btn_next(Ref* sender);
    
    void set_dismiss_callback(const std::function<void()>& callback) { dismiss_callback_ = callback;}
    void set_clear_callback(const std::function<void()>& callback) { clear_callback_ = callback;}
    void Refresh();
    void handle_play_history_finished();
    void onExit();
private:
    Button* btn_back_;
    Button* btn_play_;
    Button* btn_pre_;
    Button* btn_next_;
    
    std::vector<ChipNode*> table_pot_chips_re;
    
    Text* text_step_;
    
    bool auto_play_;
    
    std::function<void()> dismiss_callback_;
    std::function<void()> clear_callback_;
    
    events::event_dispatch dispatch_;
};