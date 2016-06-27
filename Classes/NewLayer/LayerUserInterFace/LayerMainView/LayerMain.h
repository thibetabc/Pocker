//
//  LayerMain.hpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/25.
//
//

#ifndef __pokerclient__LayerMain__
#define __pokerclient__LayerMain__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "msg.pb.h"

#include "GameDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerMain : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:

    LayerMain();
    CREATE_FUNC(LayerMain);
    virtual bool init();
    void select_choose_room_type(msg::RoomType room_type, bool is_tournament);
    void handle_android_back();
    void send_get_room_info();
    
    void update_unread_message_count();
    void set_luanguage_info();//设置语言
protected:
    void register_events();
    void handle_get_room_info(cmd_data_pointer data);
    void handle_receive_request_list(cmd_data_pointer data);
    void handle_join(cmd_data_pointer data);
    
    Sprite* getMaskedSprite(cocos2d::Rect rect);
    void onTouchEventSprite(Ref*, Touch*, Widget::TouchEventType);
    Sprite *showdetail;
private:
    
    void click_btn_setting(Ref * sender);
    void click_btn_user(Ref * sender);
    void click_btn_bar_left(Ref * sender);
    void click_btn_bar_right(Ref * sender);
    void click_btn_center(Ref * sender);
    
    void click_select_button(uint32_t idx);
    void click_select_cell(uint32_t idx);
    
    void display_layer_animation(bool isOpen);

private:
    std::vector<msg::RoomInfo> main_room_page_datas_;
    
private:
    bool RoomPageOnTouchBegan(Touch * touch, Event* event);
    void RoomPageOnTouchMoved(Touch * touch, Event* event);
    void RoomPageOnTouchEnded(Touch * touch, Event* event);
    void RoomPageOnTouchCancelled(Touch * touch, Event* event);
    
    void RoomPageTouchEndedFunc(Touch * touch, Event* event);
    
    void RoomPageOnScroll(Ref * sender);
    
#pragma mark tableview datasource --
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
protected:
    void RefreshLayer();
#pragma mark UI
private:
    Text* text_title_;
    
    Button* btn_user_;
    ImageView* image_user_message_icon_;
    Text* text_user_message_count_;
    Button* btn_setting_;
    
    Button* btn_bar_left_;
    Button* btn_bar_right_;
    ImageView* image_group_message_icon_;
    Text* text_group_message_count_;
    
    Button* btn_center_;
    
    Text* text_bar_left_;
    Text* text_bar_right_;
    
    ListView* listview_;
    
    TableView* tableview_;
    
    Node* root_;
    long long exitTime_;
};

#endif /* defined(__pokerclient__LayerMain__) */
