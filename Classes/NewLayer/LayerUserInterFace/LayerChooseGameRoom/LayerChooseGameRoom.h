//
//  LayerChooseGameRoom.h
//  pokerclient
//
//  Created by lvzhangle on 16/3/28.
//
//

#ifndef __pokerclient__LayerChooseGameRoom__
#define __pokerclient__LayerChooseGameRoom__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"

#include "GameDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerChooseGameRoom : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    static LayerChooseGameRoom* create(msg::RoomType room_type, bool is_tournament, const msg::GroupInfo& groupinfo);
    static LayerChooseGameRoom* create(msg::RoomType room_type, bool is_tournament);
    void set_groupinfo(const msg::GroupInfo& groupinfo);
    virtual bool init(msg::RoomType room_type, bool is_tournament);
    
    void send_get_room_info();
protected:
    void click_btn_back(Ref * sender);
    void click_btn_create_room(Ref * sender);
    
    void RefreshLayer();
    void register_events();
    void handle_get_room_info(cmd_data_pointer data);
private:
    msg::GroupInfo select_groupinfo_;
    bool is_tournament_;
    msg::RoomType room_type_;
    
    std::vector<msg::RoomInfo> room_info_datas_;
#pragma mark tableview datasource --
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};

    
#pragma mark UI
private:
    Text* text_title_;
    
    Button* btn_back_;
    
    Button* btn_create_room_;
    
    ImageView* image_layer_choosegameroom_bg_;
    
    ImageView* image_choosegameroom_search_bg_;
    
    ImageView* image_choosegameroom_search_icon_;
    
    //EditBox* tf_choosegameroom_search_;
    
    TextField* tf_choosegameroom_search_;
    
    ListView* listview_;
    
    TableView* tableview_;
    
    Node* root_;
};


#endif /* defined(__pokerclient__LayerChooseGameRoom__) */
