//  创建房间
#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class room_data;
class Property;
class RoomTitle;

class RoomCreateLayer : public BaseLayer , public TableViewDataSource, public TableViewDelegate
{
public:
    
public:
    
    CREATE_FUNC(RoomCreateLayer);
    virtual bool init();
    
    void update_data(cmd_data_pointer);
    void update_room_type_list(cmd_data_pointer);
    
    void click_text_save(Ref* sender);
    
    void click_image_left(Ref* sender);
    void click_image_right(Ref* sender);

    
    void click_item(msg::RoomType idx);
    
    
    void touchListView(Ref* sender, ListView::EventType event);
    virtual void onEnterTransitionDidFinish();
    
    void touch_image(Ref* sender, Touch* touch, ui::Widget::TouchEventType event);//
    void touchListView222(Ref* sender, Touch* touch, ui::Widget::TouchEventType event);
    
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(DESIGN_WIDTH, 100);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};

private:
    void register_events();
    void handle_create_room_info(cmd_data_pointer data);
private:
    Point began_location_;
    Point tableview_touch_location_;
    Text* text_show_all_room_;
    bool move_;
    
    std::vector<std::shared_ptr<room_data> > room_datas_;
    
    msg::RoomType select_game_id_;
    
    events::event_dispatch dispatch_;

    ListView* listview_;
    TableView* tableview_;
    
    EditBox* tf_room_name_;
    
    RoomTitle* room_title_;
    
    std::shared_ptr<msg::RoomProp> propertys_;
};