//
//  LayerSearchGroup.h
//  pokerclient
//  搜索俱乐部 界面
//
//  Created by duanmt on 15/5/13.
//
//

#ifndef __pokerclient__LayerSearchGroup__
#define __pokerclient__LayerSearchGroup__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class group_data;

class LayerSearchGroup : public BaseLayer, public TableViewDataSource, public TableViewDelegate, public EditBoxDelegate
{
public:
    
    CREATE_FUNC(LayerSearchGroup);
    virtual bool init();
    
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
    
protected:
    void register_events();
    void handle_search_group(cmd_data_pointer data);
    void handle_add_group(cmd_data_pointer data);
private:
    void sendSearchMsg(msg::GroupSearchType type, const std::string);
private:
    
    std::vector<msg::GroupInfo> groups_;
    
    void text_cancel_click(Ref* sender);
    
    TableView* tableview_;
    EditBox* tf_keyword_;
    
    ImageView* image_search_icon_;
};

#endif /* defined(__pokerclient__LayerSearchGroup__) */
