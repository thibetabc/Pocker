#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "events.h"
#include "msg.pb.h"

class RoomTitle;

using namespace cocos2d;
using namespace cocos2d::ui;

class UserPropertyLayer : public Layer {
public:
//    CREATE_FUNC(UserPropertyLayer);
    ~UserPropertyLayer();
    static UserPropertyLayer* create(const bool in_game);
    virtual bool init(const bool in_game);
  
    void click_text_save(Ref* sender);
    void click_image_left(Ref* sender);
    void click_image_right(Ref* sender);
    void click_btn_back(Ref* sender);
    
    void set_click_item(msg::RoomType game_idx);
    
    
    void set_btn_close_visible(const bool visible) {
        btn_close_->setVisible(visible);
    }
    
    void update_data(cmd_data_pointer data = nullptr);
    
    void refresh_data();
    
    void click_item(msg::RoomType game_idx);
    
    void touch_image(Ref* sender, Touch* touch, Widget::TouchEventType event);//
private:
    void register_events();
    msg::UserGameSetting* setting;
private:
    //
    Point began_location_;
    bool move_;
    
    ListView* listview_property_;
    Text* text_save_;
    Button* btn_back_;
    ImageView* image_left_;
    ImageView* image_right_;
    
    Button* btn_close_;
    
    msg::RoomType select_game_index_;
    msg::UserGameSettings gamesettings;
    RoomTitle* room_title_;
    
    bool in_game_;
    
    events::event_dispatch dispatch_;
};