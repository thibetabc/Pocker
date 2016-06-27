//
//  MatchJoinList.hpp
//  pokerclient
//
//  Created by qqronaldz on 15/12/2.
//
//

#ifndef MatchJoinList_hpp
#define MatchJoinList_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "global_header.h"
#include "msg.pb.h"
#include "cmd_data.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;
using namespace cocos2d;

class MatchJoinList :  public cocos2d::extension::TableViewDelegate,public Layer, public cocos2d::extension::TableViewDataSource
{
public:
    CREATE_FUNC(MatchJoinList);
    virtual bool init();
    void click_join();
    void refresh(cmd_data_pointer data);
    void set_room_id(uint32_t room_id);
    void send_get_jion_list(cmd_data_pointer data);
    void click_cancel_match();
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(DESIGN_WIDTH, 105);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
private:
    Text *text1;
    bool hasJoin;
    uint32_t room_id;
    TableView *tableview;
    Button *btn_join;
    Button *btn_cancel_match;
    events::event_dispatch dispatch_;
    
};

#endif /* MatchJoinList_hpp */
