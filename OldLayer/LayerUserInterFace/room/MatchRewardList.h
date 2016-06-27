//
//  MatchRewardList.hpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/30.
//
//

#ifndef MatchRewardList_hpp
#define MatchRewardList_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "global_header.h"
#include "msg.pb.h"
#include "cmd_def.h"
#include "cmd_data.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class MatchRewardList :  public TableViewDelegate,public Layer, public TableViewDataSource
{
public:
    CREATE_FUNC(MatchRewardList);
    static MatchRewardList *create(bool b)
    {
        m_bIsCreate = b;
        return create();
    }
    virtual bool init();
    void click_btn_add(Ref *sender);
    void refresh_list(cmd_data_pointer data);
    void set_isCreate(bool isCreate){this->m_bIsCreate = isCreate;}
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
    Node *root;
    msg::TournamentPrize *m_pTournamentPrize;
    static bool m_bIsCreate;
    
};

#endif /* MatchRewardList_hpp */
