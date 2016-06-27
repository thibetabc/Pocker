//
//  LayerUserMatchInfo.h
//  pokerclient
//
//  Created by ll on 15/11/27.
//
//

#ifndef __pokerclient__LayerUserMatchInfo__
#define __pokerclient__LayerUserMatchInfo__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "cmd_def.h"

#include "msg.pb.h"


class RoomTitle;

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerUserMatchInfo : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    
    CREATE_FUNC(LayerUserMatchInfo);
    virtual bool init();
    
    bool set_is_group_match(bool is_group_match, uint32_t group_id);
    
private:
    void click_checkbox_game_status(Ref * sender);
    void click_btn_back(Ref * sender);
    void click_text_user_rank(Ref * sender);
    void click_room_item(msg::RoomType game_idx);
    void click_label(std::string label_text);
    
    
    bool touchBegan(Touch* touch, Event* event);
    void touchMoved(Touch* touch, Event* event);
    void touchEnded(Touch* touch, Event* event);
    void touchCancel(Touch* touch, Event* event);
    
    void refresh_data();
    
    events::event_dispatch dispatch_;
    void register_events();
    
    void update_room_type_list(cmd_data_pointer);
    void handle_receive_get_history_score(cmd_data_pointer);
private:
    msg::RoomType select_game_idx_;
    std::vector<std::string> check_labels_;
    std::vector<std::shared_ptr<msg::HistoryScore> > user_score_datas_;
    
    std::shared_ptr<msg::Processor_170_GetHistoryScore_Down> historyscore_;
    
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(DESIGN_WIDTH, 188);
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
    ListView* listview_label_;
    RoomTitle* room_title_;
    
    bool is_group_match_;
    uint32_t group_id_;
    
    Text* text_mark_name_1_;
    Text* text_mark_name_2_;
    Text* text_mark_name_3_;
    Text* text_mark_num_1_;
    Text* text_mark_num_2_;
    Text* text_mark_num_3_;
    
    Text* text_title_;
    Text* text_user_rank_;
    
    uint32_t refresh_times_;
    uint32_t last_cell_idx_;
};

#endif /* defined(__pokerclient__LayerUserMatchInfo__) */
