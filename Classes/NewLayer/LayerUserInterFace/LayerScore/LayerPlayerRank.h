//
//  LayerPlayerRank.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/21.
//
//

#ifndef LayerPlayerRank_hpp
#define LayerPlayerRank_hpp
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "ShaderSprite.h"
#include "NodeSelectGameType.h"
#include "LayerChooseGameType.h"
#define RANK_LIST_CELL_WIDTH 678
#define RANK_LIST_CELL_HEIGHT 234

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerPlayerRank:public BaseLayer,public TableViewDataSource,public TableViewDelegate {
public:
    static LayerPlayerRank* create(std::string groupName,msg::RoomType roomType,int groupId,bool istournament);
    virtual bool init(std::string groupName,msg::RoomType roomType,int groupId,bool istournament);
private:
    Node* root_;
    Button* btn_back_;
    Text* text_title_;
    ListView* list_rank_;
    TableView* tableview_;
    Node* node_select_btn_;
    NodeSelectGameType* node_select_game_;
private:
    std::string group_name_;
    msg::RoomType room_type_;
    int group_id_;
    bool istournament_;
    std::vector<msg::PlayerScore> data_source_;
public:
    void click_btn_back(Ref* sender);
    void click_node_select_game();
    void update_tableview_data(msg::RoomType type,bool istournament);
    void handle_get_rank(cmd_data_pointer data);
    void register_events();
#pragma mark tableview_datascource
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx){return Size(RANK_LIST_CELL_WIDTH,RANK_LIST_CELL_HEIGHT);};
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
};

#endif /* LayerPlayerRank_hpp */
