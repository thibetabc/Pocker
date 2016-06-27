//
//  LayerFriendListOld.h
//  pokerclient
//
//  Created by ll on 15/8/27.
//
//

#ifndef __pokerclient__LayerFriendListOld__
#define __pokerclient__LayerFriendListOld__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerFriendListOld : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    
    CREATE_FUNC(LayerFriendListOld);
    virtual bool init();
    void update_table_view(cmd_data_pointer data);
private:
    
    TableView* tableview_;
    events::event_dispatch dispatch_;
    
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(DESIGN_WIDTH, 130);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
private:
    
    std::vector<msg::UserDetailInfo> friends_;
    
    Sprite* sprite_search_icon_;
    bool m_bCanSearch ;
};

#endif /* defined(__pokerclient__LayerFriendListOld__) */
