//
//  SelectHandcard.h
//  pokerclient
//
//  Created by admin on 15-9-7.
//
//

#ifndef __pokerclient__SelectHandcard__
#define __pokerclient__SelectHandcard__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class SelectHandcard : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    
    CREATE_FUNC(SelectHandcard);
    virtual bool init();
    
    void click_btn_back(Ref* sender);
    void click_btn_create(Ref* sender);
    
    
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
private:
    TableView* tableview_;
    
    
};

#endif /* defined(__pokerclient__SelectHandcard__) */
