#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"

class room_data;

class RoomTitle;

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class RoomLayer : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    
    CREATE_FUNC(RoomLayer);
    virtual bool init();
    
    void click_btn_back(Ref* sender);
    
    void click_btn_create(Ref* sender);
    
    void create_sit_and_go();
    void create_time_match();
    
    void click_image_left(Ref* sender);
    void click_image_right(Ref* sender);
    
    void click_checkbox_game_status(Ref* sender);
    void click_check_display_offical_room(Ref * sender);
    
    void click_room_item(msg::RoomType game_idx);
    
    void touch_listview_name(Ref*, ListView::EventType event);
    
    void touch_image(Ref* sender, Touch* touch, Widget::TouchEventType event);
    
    void send_update_data();
    void update_data(cmd_data_pointer);
    void update_room_type_list(cmd_data_pointer);
    void handle_create_room_info(cmd_data_pointer);
    void handle_get_score(cmd_data_pointer);
    void handle_create_room_fail(cmd_data_pointer);
    void join_list(cmd_data_pointer);
    void scroll_ended();
    
    
    bool touchBegan(Touch* touch, Event* event);
    void touchMoved(Touch* touch, Event* event);
    void touchEnded(Touch* touch, Event* event);
    void touchCancel(Touch* touch, Event* event);
    
    void update_data_all_room();
    
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(DESIGN_WIDTH, 151);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
public:
    void SetTableSize();
private:
    void register_events();
    
private:
    Button* btn_back_;
    Button *btn_sit_and_go_;
    Button *btn_time_match_;
    CheckBox* checkbox_game_status_;
    TableView* tableview_;
    RoomTitle* room_title_;
    std::vector<msg::RoomInfo> room_datas_;
    events::event_dispatch dispatch_;
    msg::RoomType select_game_idx_;
    
    Point began_location_;
    Point tableview_touch_location_;
    Text* text_show_all_room_;
    Text* text_show_all_record_;
    bool move_;
    bool has_init_room_type_;
    ssize_t top_cell_idx_;
    time_t time_;
    bool m_bCanTouch;
    uint32_t room_id_;
    
    ImageView* image_display_offical_room_;
    Text* text_display_offical_room_;
    CheckBox* check_display_offical_room_;
};