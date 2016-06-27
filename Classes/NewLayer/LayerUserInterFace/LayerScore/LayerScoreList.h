//
//  LayerSelfTrackRecord.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/11.
//
//

#ifndef LayerSelfTrackRecord_hpp
#define LayerSelfTrackRecord_hpp

#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LayerChooseGameType.h"
#include "NodeScoreCell.h"
#include "NodeCircleChoose.h"
#include "events.h"
#include "msg.pb.h"
#include "NodeSelectGameType.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerScoreList:public BaseLayer,public TableViewDataSource, public TableViewDelegate {
public:
    static LayerScoreList* create(bool isGroupMatch=false,int groupId=0,std::string groupName="");
    virtual bool init(bool isGroupMatch,int groupId,std::string groupName);
private:
    
    msg::RoomType current_room_type_;
    std::string group_name_;
    bool is_current_romm_tournament_;
    bool is_group_match_;
    int group_id_;
    bool is_in_selection_status_;
    
    Node* root_;
    Layout* panel_score_group_;
    Node* node_select_type_;
    NodeSelectGameType* select_game_;
    
    Button* btn_back_;
    Text* text_title_;
    Button* btn_player_rank_;
    
    Text* text_whole_score_;
    Text* text_hour_score_;
    Text* text_times_score_;
    Text* text_whole_score_num_;
    Text* text_hour_score_num_;
    Text* text_times_score_num_;
    
    Text* text_choose_type_;
    Text* text_choose_type_second_;
    
    bool isChipChoose_;
    bool isGroupChoose_;
    bool isWeekChoose_;
    bool isMonthChoose_;
    bool isAllChoose_;
    
    Node* node_week_btn_;
    Node* node_month_btn_;
    Text* text_current_week_;
    Text* text_current_month_;
    Node* node_all_;
    Text* text_all_;
    
    TableView* tableview_;
    ListView* listview_;
    
    NodeCircleChoose* choose_month_;
    NodeCircleChoose* choose_week_;
    NodeCircleChoose* choose_all_;
    
    std::vector<msg::HistoryScore> blind_cell_datas_;
    std::vector<msg::HistoryScore> blind_cell_datas_copy_;
    uint32_t groupid_;
    long startTime_ = 0;
    long endtime_ = 0;
    float allscore = 0;
    float allscore_time = 0;
    float allscore_hands = 0;
    float alltime = 0;
    float allhands = 0;
    std::string choose_group_name_="";
    std::string choose_chip_num="";
    void send_get_history_score();
public:
    void click_btn_back(Ref* sender);
    void click_btn_choose_gametype();
    void send_request();
    void select_chip_num(std::string chipNum);
    void select_group_name(std::string groupId,std::string groupName);
    void click_select_week(std::string choose);
    void click_select_month(std::string choose);
    void click_select_all(std::string choose);
    void click_text_select_type(Ref* sender);
    void click_text_select_type_second(Ref* sender);
    void click_btn_player_rank(Ref* sender);
public:
    void update_tableview_data(msg::RoomType room_type,bool istournament);
    std::function<void(msg::RoomType,bool)> choose_room_type_callfunc_;
    void set_choose_call_func(std::function<void(msg::RoomType,bool)> callfunc) {choose_room_type_callfunc_ = callfunc;}
    void register_events();
    void handle_get_user_match(cmd_data_pointer data);
    void getChipChoosedData(std::string chipNum);
    void getGroupChoosedData(std::string groupName);
    void reset_score();
    void refresh_score_data();
    std::string getFormatText(std::string str);
#pragma mark tableview datasource --
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
};



#endif /* LayerSelfTrackRecord_hpp */
