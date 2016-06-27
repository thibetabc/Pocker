//
//  LayerComInfoTable.cpp
//  pokerclient
//
//  Created by ll on 15/11/30.
//
//

#include "LayerComInfoTable.h"
#include "PokerDataManager.h"
#include "GameDataManager.h"

bool LayerComInfoTable::init()
{
    if(!Layer::init()) {
        return false;
    }
    root_ = CSLoader::createNode("LayerComInfoTablePortrait.csb");
    this->addChild(root_);
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    root_->addChild(tableview_);
    
    listview->removeFromParent();

    btn_go_observer_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_go_observer"));
    btn_go_observer_->addClickEventListener(std::bind(&LayerComInfoTable::click_btn_go_observer, this, std::placeholders::_1));
    
    return true;
}

void LayerComInfoTable::click_btn_go_observer(Ref* sender)
{
    auto table_info_data = table_info_.lock();
    if (table_info_data == nullptr)
    {
        cocos2d::log("Error : LayerComInfoTable::click_btn_go_observer data == nullptr");
        return;
    }
    
    //uint32_t tableID = table_info_data->tableid();
    PDM->send_poker_tourment_change_table(room_id_, table_id_);
}

void LayerComInfoTable::set_roomid(uint32_t roomID)
{
    room_id_ = roomID;
}

void LayerComInfoTable::update_data(std::shared_ptr<msg::TourmentTableInfo> table_info)
{
    table_info_ = table_info;
    
    auto table_info_data = table_info_.lock();
    
    table_id_ = table_info_data->tableid();
    
    table_cell_datas_.clear();
    
    for (int i = 0; i < table_info_data->player().size(); i++)
    {
        int32_t player_seat_num = table_info_data->player(i).sitnum();
        std::string player_nickname = table_info_data->player(i).userinfo().nickname();
        int32_t player_chips = table_info_data->player(i).chips();
        
        std::shared_ptr<NodeComInfoTableCellData> table_cell_data(new NodeComInfoTableCellData);
        table_cell_data->player_seat_num = player_seat_num;
        table_cell_data->player_nickname = player_nickname;
        table_cell_data->player_chips = player_chips;
        
        table_cell_datas_.push_back(table_cell_data);
    }

    auto user = static_pointer_cast<user_texas>(GetRoom()->get_user(GDM->get_user_id()));
    if(user) {
        if(user->get_status() == msg::SittingStatus::OBSERVER ||
           user->get_status() == msg::SittingStatus::LEAVE) {
            btn_go_observer_->setVisible(true);
        }else
        {
            btn_go_observer_->setVisible(false);
        }
    }else
    {
        btn_go_observer_->setVisible(false);
    }

    
    tableview_->reloadData();
}

#pragma mark tableview datasource --
TableViewCell* LayerComInfoTable::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto& data = table_cell_datas_.at(idx);
    
    NodeComInfoTableCell* cell = dynamic_cast<NodeComInfoTableCell*>(table->dequeueCell());
    if (!cell){
        cell = NodeComInfoTableCell::create();
    }
    cell->update_data(data);
    
    return cell;
}

ssize_t LayerComInfoTable::numberOfCellsInTableView(TableView *table)
{
    return table_cell_datas_.size();
}

#pragma mark tableview delegate --
void LayerComInfoTable::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

void LayerComInfoTable::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    
}

void LayerComInfoTable::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
}