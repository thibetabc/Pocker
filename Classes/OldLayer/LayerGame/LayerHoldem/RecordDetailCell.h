//
//  RecordDetailCell.hpp
//  pokerclient
//
//  Created by NaughtyDog on 16/3/11.
//
//

#ifndef RecordDetailCell_hpp
#define RecordDetailCell_hpp

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class RecordObject;

class RecordDetailCell : public TableViewCell {
public:
    static Size size();
    
    CREATE_FUNC(RecordDetailCell);
    
    const uint32_t get_hand_id() { return hand_id_;}
    const std::shared_ptr<msg::GameRecord>& get_gameRecord() { return gr_cell_; }
    
    virtual bool init();
    
    void update_data(msg::Processor_300_PokerGetHandHistory_DOWN& records, ssize_t idx,bool isDetail,float offset);
    
    void select(const bool s);

    
    
private:
    Node* root;
    Node* detailNode;
    Node* sprite_card_bg_;
    Text* text_nickname_;
    Text* text_amount_;
    Text* text_hand_type_;
    ImageView* image_bg;
    uint32_t hand_id_;
    std::shared_ptr<msg::GameRecord> gr_cell_;
    events::event_dispatch dispatch_;
    
};
#endif /* RecordDetailCell_hpp */
