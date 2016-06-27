//
//  LayerFriendSearch.h
//  pokerclient
//
//  Created by ll on 15/8/21.
//
//

#ifndef __pokerclient__LayerFriendSearch__
#define __pokerclient__LayerFriendSearch__

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

class LayerFriendSearch : public BaseLayer, public TableViewDataSource, public TableViewDelegate, public EditBoxDelegate
{
public:
    CREATE_FUNC(LayerFriendSearch);
    virtual bool init();
    void update_table_view(cmd_data_pointer);
    
    void handle_create_group_finished(cmd_data_pointer);
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(DESIGN_WIDTH, 121);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
#pragma mark editbox delegate --
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
private:
    void handle_add_user_send_succeess(cmd_data_pointer);
    
    events::event_dispatch dispatch_;
    
    std::vector<msg::UserDetailInfo> friends_;
    
    void text_cancel_click(Ref* sender);
    
    TableView* tableview_;
    EditBox* tf_keyword_;
    
    ImageView* image_search_icon_;
};



#endif /* defined(__pokerclient__LayerFriendSearch__) */
