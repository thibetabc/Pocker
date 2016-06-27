#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class LanguageLayer : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    CREATE_FUNC(LanguageLayer);
    virtual bool init();
    
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(DESIGN_WIDTH, 102);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
private:
    void click_btn_back(Ref * sender);
    std::vector<std::pair<std::string, std::pair<std::string, std::string> > > languages_;
    
    TableView* tableview_;
    
    Text* text_title_;
    Button* btn_back_;
};