//
//  NodeMainLayerRoomPage.h
//  pokerclient
//
//  Created by lvzhangle on 16/3/26.
//
//

#ifndef __pokerclient__NodeMainLayerRoomPage__
#define __pokerclient__NodeMainLayerRoomPage__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeMainLayerRoomPage : public TableViewCell {
public:
    CREATE_FUNC(NodeMainLayerRoomPage);
    virtual bool init();
    
    enum ROOMPAGE_ROTATION_TYPE{
        LEFT,
        RIGHT
    };
    
    void setNodeMainLayerRoomPageIndex(uint32_t idx);
    void setNodeMainLayerRoomRotation(ROOMPAGE_ROTATION_TYPE rotation_type, float rotation);
    void update_data(std::string string_data) { text_top_title_->setString(string_data); }
    
    void update_data(msg::RoomInfo room_data);
    
    void set_click_btn_callback(std::function<void(const int &)> click_btn_callback) {click_btn_callback_ = click_btn_callback;}
    void set_click_cell_callback(std::function<void(const int &)> click_cell_callback) {click_cell_callback_ = click_cell_callback;}
    
private:
    void display_node_animation(bool isOpen);
    
    void click_btn_other(Ref * sender);
    void click_cell_bg(Ref * sender);
    
    bool cellOnTouchBegan(Touch * touch, Event* event);
    void cellOnTouchMoved(Touch * touch, Event* event);
    void cellOnTouchEnded(Touch * touch, Event* event);
    void cellOnTouchCancelled(Touch * touch, Event* event);
    
    std::function<void(const int &)> click_btn_callback_;
    std::function<void(const int &)> click_cell_callback_;
    
    std::vector<std::pair<std::string, std::string>> room_page_cell_datas_;
    
    msg::RoomInfo room_data_;
    
    uint32_t intRoomPageIdx_;
    
    Point pos_touch_record_;
    bool is_click_cell_;
    
#pragma mark UI
private:
    ImageView* image_mainpage_top_;
    ImageView* image_mainpage_bg_;
    ListView* listview_;
    Button* btn_other_;
    Text* text_top_title_;
    Text* text_title_;
    ImageView* image_mainpage_icon_1_;
    ImageView* image_mainpage_icon_2_;
    
    Node* root_;
};
#endif /* defined(__pokerclient__NodeMainLayerRoomPage__) */
