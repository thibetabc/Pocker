//
//  LayerSearchUser.hpp
//  pokerclient
//  搜索好友
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef LayerSearchUser_hpp
#define LayerSearchUser_hpp
#include <stdio.h>
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerSearchUser : public BaseLayer, public TableViewDataSource, public TableViewDelegate, public EditBoxDelegate
{
public:
    CREATE_FUNC(LayerSearchUser);
    virtual bool init();
protected:
    void handle_search_user(cmd_data_pointer data);
    void register_events();
    
private:
    void sendMsg(const std::string nick);
    LayerSearchUser();
private:
    #pragma mark - tableviewDatasource
    virtual Size cellSizeForTable(TableView *table)
    {
        return Size(DESIGN_WIDTH, GroupCellHeight);
    };
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    #pragma mark - tableviewDelegate
    void tableCellTouched(TableView* table, TableViewCell* cell);
    void tableCellHighlight(TableView* table, TableViewCell* cell);
    void tableCellUnhighlight(TableView* table, TableViewCell* cell);
#pragma mark editbox delegate --
    void editBoxEditingDidBegin(EditBox* editBox);
    void editBoxEditingDidEnd(EditBox* editBox);
    void editBoxTextChanged(EditBox* editBox, const std::string& text);
    void editBoxReturn(EditBox* editBox);
    
private:
    TableView *tableview_;
    EditBox* tf_keyword_;
    Node *inputIcon;
    std::vector<msg::UserDetailInfo> mUserDetailInfo;
    bool isSearch;
};

#endif /* LayerSearchUser_hpp */

