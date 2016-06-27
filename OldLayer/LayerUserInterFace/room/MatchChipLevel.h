//
//  MatchChipLevel.hpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/26.
//
//

#ifndef MatchChipLevel_hpp
#define MatchChipLevel_hpp

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "global_header.h"
#include "msg.pb.h"
#include "BaseLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;
using namespace cocos2d;
class MatchChipLevel :  public TableViewDelegate,public Layer, public TableViewDataSource
{
public:
    CREATE_FUNC(MatchChipLevel);
    static MatchChipLevel* create(bool isCreate)
    {
        m_bIsCreate = isCreate;
        return create();
    }
    bool init();
    void click_btn_add(Ref *sender);
    void set_isCreate(bool isCreate){this->m_bIsCreate = isCreate;}
    
    bool touchBegan(Touch* touch, Event* event);
    void touchMoved(Touch* touch, Event* event);
    void touchEnded(Touch* touch, Event* event);
    void touchCancel(Touch* touch, Event* event);
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
    msg::TournamentInfo *m_pTournamentInfo;
    static bool m_bIsCreate;
};

#endif /* MatchChipLevel_hpp */
