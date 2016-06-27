#pragma once


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "msg.pb.h"
#include "cocos-ext.h"


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class room_data;

class RoomInfoCell : public TableViewCell {
public:
    CREATE_FUNC(RoomInfoCell);
    
    virtual bool init();
    
    void update_data(const msg::RoomInfo& data);
    
    const msg::RoomInfo& get_room_data() { return data_;}
    
    void set_is_touch(bool is_touch = true);
    
    void share(Ref* sender);
    
private:
    ImageView* image_bg_;
    ImageView* image_icon_;
    Text* text_name_;
    Text* text_group_name_;
    Text* text_status_;
    Text* text_player_number_;
    Text* text_share_;
    
    
    msg::RoomInfo data_;
};
