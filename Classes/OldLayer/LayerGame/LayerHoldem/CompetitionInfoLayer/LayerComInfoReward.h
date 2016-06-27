//
//  LayerComInfoReward.h
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#ifndef __pokerclient__LayerComInfoReward__
#define __pokerclient__LayerComInfoReward__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include "utils.h"

#include "msg.pb.h"

#include "NodeRewardCell.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerComInfoReward : public Layer, public TableViewDataSource, public TableViewDelegate {
public:
    
    CREATE_FUNC(LayerComInfoReward);
    virtual bool init();
    
public:
    void update_data(const msg::Processor_530_PokerGetTourmentStatus_DOWN& tourment_status);
private:
    TableView* tableview_;
    events::event_dispatch dispatch_;
    
#pragma mark tableview datasource --
    virtual cocos2d::Size cellSizeForTable(TableView *table) {
        return cocos2d::Size(691, 68);
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
    std::vector<std::shared_ptr<NodeRewardCellData> > reward_cell_datas_;
};


#endif /* defined(__pokerclient__LayerComInfoReward__) */
