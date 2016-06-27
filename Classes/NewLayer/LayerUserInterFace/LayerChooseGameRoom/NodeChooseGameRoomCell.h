//
//  NodeChooseGameRoomCell.hpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/28.
//
//
#ifndef __pokerclient__NodeChooseGameRoomCell__
#define __pokerclient__NodeChooseGameRoomCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "ShaderSprite.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeChooseGameRoomCell : public TableViewCell {
public:
    CREATE_FUNC(NodeChooseGameRoomCell);
    virtual bool init();
    
    void update_data(msg::RoomInfo room_data);
    msg::RoomInfo get_data(){return room_data_;}
    
    void handle_download_events(cmd_data_pointer);
    void update_ui();
    
    msg::RoomInfo room_data_;
    
    events::event_dispatch dispatch_;
private:
    ShaderSprite* sprite_icon_;
    
    ImageView* image_choosegameroom_cell_bg_;
    ImageView* image_group_icon_;
    ImageView* image_choosegameroom_icon_;
    Text* text_room_status_1_;
    Text* text_room_status_2_;
    Text* text_room_name_;
    Text* text_room_note_1_;
    Text* text_room_note_2_;
    Text* text_room_note_3_;
    
    Node* root_;
};
#endif /* defined(__pokerclient__NodeChooseGameRoomCell__) */
