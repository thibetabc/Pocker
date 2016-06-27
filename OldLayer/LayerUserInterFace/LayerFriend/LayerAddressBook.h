//
//  LayerAddressBook.h
//  pokerclient
//
//  Created by ll on 15/8/18.
//
//

#ifndef __pokerclient__LayerAddressBook__
#define __pokerclient__LayerAddressBook__


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

class LayerAddressBook : public BaseLayer, public TableViewDataSource, public TableViewDelegate, public EditBoxDelegate
{
public:
    
    CREATE_FUNC(LayerAddressBook);
    virtual bool init();
    void onExit();
    void update_table_view(cmd_data_pointer data);
    void set_is_charge (bool b,std::string id){mIsCharge = b;mProductID = id;}
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
#pragma mark editbox delegate --
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
    
private:
    
    std::vector<msg::UserDetailInfo> friends_;
    
    EditBox* tf_keyword_;
    
    ImageView* image_search_icon_;
    
        void clickback(Ref * sender);
    std::function<void()> back_callback_;
    bool mIsCharge = false;
    std::string mProductID = "";
};



#endif /* defined(__pokerclient__LayerAddressBook__) */

