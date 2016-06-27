//
//  LayerGroupTrackRecord.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/11.
//
//

#ifndef LayerGroupTrackRecord_hpp
#define LayerGroupTrackRecord_hpp

#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "ShaderSprite.h"
#define SCORE_LIST_CELL_WIDTH 678
#define SCORE_LIST_CELL_HEIGHT 234

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerScore:public BaseLayer,public TableViewDataSource,public TableViewDelegate {
public:
//    CREATE_FUNC(LayerScore);
    static LayerScore* create(int roomid);
    virtual bool init(int roomid);
    
private:
    Node* root_;
    ListView* liastview_room_track_record_second_;
    ListView* liastview_room_track_record_first_;
    
    TableView* tableView_first_;
    TableView* tableView_second_;
    
    ImageView* image_middle_bg_first_;
    
    Text* text_whole_num_;
    Text* text_max_pot_;
    Text* text_whole_buying_;
    Text* text_whole_number_;
    Text* text_max_pot_num_;
    Text* text_whole_buying_num_;
    
    Text* text_mvp_;
    Text* text_big_fish_;
    Text* text_max_buy_;
    Text* text_mvp_name_;
    Text* text_big_fish_name_;
    Text* text_max_buy_name_;
    
    Text* text_game_time_show_first_;
    Text* text_game_time_show_second_;
    
    Node* node_mvp_photo_;
    Node* node_big_fish_photo_;
    Node* node_max_buy_photo_;
    
    Text* text_title_;
    Button* btn_back_;
private:
    std::string mvp_name_;
    std::string big_fish_name_;
    std::string max_buy_name_;
    
    std::string mvp_picname;
    std::string big_fish_picname;
    std::string max_buy_picname;
    
    int whole_buy_in_;
    std::vector<msg::PlayerScore> score_lsit_data_;
    msg::PlayerScore mvp_info_;
    msg::PlayerScore big_fish_info_;
    msg::PlayerScore maxBuy_info_;
    ShaderSprite* mvp_sprite_;
    ShaderSprite* bigfish_sprite_;
    ShaderSprite* maxbuy_sprite_;
    msg::RoomType room_type_;
public:
    void click_btn_back(Ref* sender);
    void register_events();
    void handle_get_score_list_data(cmd_data_pointer data);
    void isFirstTableViewVisible(bool visible);
    void addFirstPartPlayerPhoto(std::string picname,ShaderSprite* sprite,Node* node);
    void handle_downloadPhoto(cmd_data_pointer data);
#pragma mark tableview_datascource
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx){return Size(SCORE_LIST_CELL_WIDTH,SCORE_LIST_CELL_HEIGHT);};
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
};

#endif /* LayerGroupTrackRecord_hpp */
