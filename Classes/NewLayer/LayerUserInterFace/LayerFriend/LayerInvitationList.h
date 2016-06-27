//
//  LayerInvitationList.hpp
//  pokerclient
//  申请好友/手机联系人列表
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef LayerInvitationList_hpp
#define LayerInvitationList_hpp

#include <stdio.h>
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerInvitationList : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    CREATE_FUNC(LayerInvitationList);
    virtual bool init();
    void refresh_deal_data();
protected:
    void register_events();
    void RefreshLayer();
    void handle_deal_request(cmd_data_pointer data);
private:
    #pragma mark - tableviewDatasource
    virtual Size cellSizeForTable(TableView *table)
    {
        return Size(DESIGN_WIDTH, GroupCellHeight);
    };
    TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView *table);
    
    #pragma mark - tableviewDelegate
    void tableCellTouched(TableView* table, TableViewCell* cell);
    void tableCellHighlight(TableView* table, TableViewCell* cell);
    void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    void sortByTime();
    
private:
    bool result;
    uint32_t operation_request_id;
    TableView *mTableview;
    
    //请求列表消息数据
    std::vector<msg::Invitation> mInvitations;
    Size sizeOffset;
    
};
#endif /* LayerInvitationList_hpp */
