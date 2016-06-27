//
//  LayerGroupList.h
//  pokerclient
//
//  Created by duanmt on 15/5/3.
//
//

#ifndef __pokerclient__LayerGroupList__
#define __pokerclient__LayerGroupList__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class group_data;

class LayerGroupList : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    
    enum GROUPLAYERTYPE
    {
        GROUPLAYER = 0,
        GAMESELECTGROUP = 1
    };
    
    CREATE_FUNC(LayerGroupList);
    virtual bool init();
    
    void update_unread_message_count();

    void set_btn_add_callback(const std::function<void()>& callback) {
        btn_add_callback_ = callback;
    }
    void set_btn_search_callback(const std::function<void()>& callback) {
        btn_search_callback_ = callback;
    }
    
    void set_select_group_callback(const std::function<void(const msg::GroupInfo&)>& callback){
        select_group_callback_ = callback;
    }
    
    void set_group_layer_type(enum GROUPLAYERTYPE group_layer_type);
    
    void click_btn_add(Ref* sender);
    void click_btn_search(Ref* sender);
    void click_btn_menu(Ref* sender);
    void click_btn_back(Ref* sender);
    void requestGroupList();
    void RefreshMessage();
    
protected:
    void RefreshLayer();
    void register_events();
    void handle_search_group(cmd_data_pointer data);
private:
    std::function<void()> btn_add_callback_;
    std::function<void()> btn_search_callback_;
    std::function<void()> btn_menu_callback_;
    std::function<void(const msg::GroupInfo&)> select_group_callback_;
    
    
    enum GROUPLAYERTYPE group_layer_type_;
    
    bool is_select_group_;
    
    Node* root_;
    
    Text* text_title_;
    
    Button* btn_back_;
    Button* btn_group_search_;
    Button* btn_group_create_;
    
    TableView* tableview_;
    std::vector<msg::GroupInfo> groups_;
    Size sizeOffset;
    ssize_t countGroupList;
    
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(DESIGN_WIDTH, GroupCellHeight);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
//    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
    void touchend(Touch*, Event*);
    LayerGroupList();
    
    
};



#endif /* defined(__pokerclient__LayerGroupList__) */
