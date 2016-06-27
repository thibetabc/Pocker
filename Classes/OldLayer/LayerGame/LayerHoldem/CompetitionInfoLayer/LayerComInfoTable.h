//
//  LayerComInfoTable.h
//  pokerclient
//
//  Created by ll on 15/11/30.
//
//

#ifndef __pokerclient__LayerComInfoTable__
#define __pokerclient__LayerComInfoTable__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include "utils.h"
#include "msg.pb.h"
#include "Room.h"
#include "NodeComInfoTableCell.h"
#include "GameLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerComInfoTable : public PokerChildLayer, public TableViewDataSource, public TableViewDelegate {
public:
    
    CREATE_FUNC(LayerComInfoTable);
    virtual bool init();
    
public:
    
    void update_data(std::shared_ptr<msg::TourmentTableInfo> table_info);
    void set_roomid(uint32_t roomID);
    
private:
    void click_btn_go_observer(Ref* sender);
    TableView* tableview_;
    
#pragma mark tableview datasource --
    virtual cocos2d::Size cellSizeForTable(TableView *table) {
        return cocos2d::Size(561, 68);
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
    Button* btn_go_observer_;
    
    uint32_t room_id_;
    uint32_t table_id_;
    
    std::weak_ptr<msg::TourmentTableInfo> table_info_;
    std::vector<std::shared_ptr<NodeComInfoTableCellData>> table_cell_datas_;
};


#endif /* defined(__pokerclient__LayerComInfoTable__) */
