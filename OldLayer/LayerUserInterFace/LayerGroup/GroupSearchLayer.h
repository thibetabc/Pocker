//
//  GroupSearchLayer.h
//  pokerclient
//  搜索群 界面
//
//  Created by duanmt on 15/5/13.
//
//

#ifndef __pokerclient__GroupSearchLayer__
#define __pokerclient__GroupSearchLayer__

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

class group_data;

class GroupSearchLayer : public BaseLayer, public TableViewDataSource, public TableViewDelegate, public EditBoxDelegate
{
public:
    
    CREATE_FUNC(GroupSearchLayer);
    virtual bool init();
    
    void update_table_view(cmd_data_pointer data);
    
    void handle_create_group_finished(cmd_data_pointer);
    virtual void onEnterTransitionDidFinish(); 
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(DESIGN_WIDTH, GroupCellHeight);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
#pragma mark editbox delegate --
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
private:
    events::event_dispatch dispatch_;
    
    std::vector<msg::GroupInfo*> groups_;
    
    void text_cancel_click(Ref* sender);
    uint32_t selectgroupid;
    TableView* tableview_;
    EditBox* tf_keyword_;
    
    ImageView* image_search_icon_;
};

#endif /* defined(__pokerclient__GroupSearchLayer__) */
