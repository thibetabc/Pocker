//
//  LayerMahjongOnTheWater.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#ifndef LayerMahjongOnTheWater_hpp
#define LayerMahjongOnTheWater_hpp

#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "cmd_data.h"
#include "GameLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerMahjongOnTheWater : public MahjongChildLayer, public TableViewDelegate, public TableViewDataSource
{
public:
    CREATE_FUNC(LayerMahjongOnTheWater);
    virtual bool init();
    
private:
#pragma mark ui
    Node* root_;
    ImageView* image_title_;
    Button* btn_close_;
    Text* text_des_;
    Text* text_score_;
    ListView* list_mahjong_;
    TableView* tableview_;
#pragma mark value
    msg::MahjongPlayerInfo player_info_;
public:
#pragma mark click_event
    void click_btn_close(Ref* sender)
    {
        this->removeFromParent();
    }
#pragma mark func
    void set_player_info(msg::MahjongPlayerInfo &player_info);
#pragma mark tableview_datascource
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx){return Size(456,40);};
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
};

#endif /* LayerMahjongOnTheWater_hpp */
