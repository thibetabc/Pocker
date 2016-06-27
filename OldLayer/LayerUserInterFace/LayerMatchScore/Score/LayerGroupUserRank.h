//
//  LayerGroupUserRank.h
//  pokerclient
//
//  Created by ll on 16/2/17.
//
//

#ifndef __pokerclient__LayerGroupUserRank__
#define __pokerclient__LayerGroupUserRank__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "cmd_def.h"

#include "msg.pb.h"
#include "NodeScoreCellOld.h"


class RoomTitle;

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerGroupUserRank : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    
    CREATE_FUNC(LayerGroupUserRank);
    virtual bool init();
    
    void set_group_user_rank_status(uint32_t group_id, msg::RoomType current_room_type, bool is_tournament);
    
private:
    void click_checkbox_game_status(Ref* sender);
    void click_btn_back(Ref* sender);
    void click_room_item(msg::RoomType game_idx);
    
    bool touchBegan(Touch* touch, Event* event);
    void touchMoved(Touch* touch, Event* event);
    void touchEnded(Touch* touch, Event* event);
    void touchCancel(Touch* touch, Event* event);
    
    void refresh_data();
    
    events::event_dispatch dispatch_;
    void register_events( );
    
    void update_room_type_list(cmd_data_pointer);
    void handle_get_group_user_rank(cmd_data_pointer);
private:
    msg::RoomType select_game_idx_;
    std::vector<std::shared_ptr<MATCH_OBJECT> > match_objects_;
    
    std::shared_ptr<msg::Processor_171_PokerGetGroupUserRank_DOWN> group_user_rank_;
    
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(DESIGN_WIDTH, 135);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
private:
    Node* root_;
    
    CheckBox* checkbox_game_status_;
    TableView* tableview_;
    RoomTitle* room_title_;
    
    uint32_t group_id_;
    
    Text* text_title_;
};

#endif /* defined(__pokerclient__LayerGroupUserRank__) */
