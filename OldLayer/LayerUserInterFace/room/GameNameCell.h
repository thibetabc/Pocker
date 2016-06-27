#pragma once


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "msg.pb.h"
using namespace cocos2d;
using namespace cocos2d::ui;



class GameNameCell : public Layout {
public:
    CREATE_FUNC(GameNameCell);
    
    virtual bool init();
    
    void update_data();
    void update_data(msg::RoomType idx,std::string name = "", int room_count = 0);
    
    void set_selected(const bool is_select);
    
    void set_callback(const std::function<void(msg::RoomType)>& callback) { callback_ = callback;}
    
    void set_in_game(const bool in_game) { in_game_ = in_game;}
    
    bool is_valid();
    msg::RoomType get_room_type(){return room_type;}
    
private:

    ImageView* image_game_type_;
    ImageView* image_game_type_bg_;
    Text*      text_game_type_;
    
    std::function<void(msg::RoomType)> callback_;
    msg::RoomType room_type;
    bool in_game_;
};
