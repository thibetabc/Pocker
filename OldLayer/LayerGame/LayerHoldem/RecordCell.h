#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class RecordObject;

class RecordCell : public TableViewCell {
public:
    static Size size();
    
    CREATE_FUNC(RecordCell);
    
    const uint32_t get_hand_id() { return hand_id_;}
    const std::shared_ptr<msg::GameRecord>& get_gameRecord() { return gr_cell_; }
    
    virtual bool init();
    
    void update_data(const std::shared_ptr<msg::GameRecord>& obj);
    
    void select(const bool s);
    
    void click_replay();
    void play_record(cmd_data_pointer);
    
private:
    Node* sprite_card_bg_;
    Text* text_nickname_;
    Text* text_amount_;
    ImageView* image_bg;
    Button *btn_replay;
    uint32_t hand_id_;
    std::shared_ptr<msg::GameRecord> gr_cell_;
    events::event_dispatch dispatch_;
//    msg::GameRecord* gr_cell_;
};