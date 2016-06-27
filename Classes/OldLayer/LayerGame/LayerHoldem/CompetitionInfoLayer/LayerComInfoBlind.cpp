//
//  LayerComInfoBlind.cpp
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#include "LayerComInfoBlind.h"

bool LayerComInfoBlind::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerComInfoBlindPortrait.csb");
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
    
    return true;
}

void LayerComInfoBlind::update_data(const msg::Processor_530_PokerGetTourmentStatus_DOWN& tourment_status)
{
    
    current_level_ = tourment_status.currentlevel();
    blind_cell_datas_.clear();
    for (int i = 0; i < tourment_status.tournamentinfo().tournamentlevel().level().size(); i++)
    {
        int32_t current_small_blind = tourment_status.tournamentinfo().tournamentlevel().level(i).smallblind();
        int32_t current_big_blind = tourment_status.tournamentinfo().tournamentlevel().level(i).bigblind();
        int32_t current_ante = tourment_status.tournamentinfo().tournamentlevel().level(i).ante();
        int32_t current_last_time = tourment_status.tournamentinfo().tournamentlevel().level(i).lasttime();
        int32_t current_rest_time = tourment_status.tournamentinfo().tournamentlevel().level(i).resttime();
        
        std::shared_ptr<NodeBlindCellData> blind_data(new NodeBlindCellData);
        blind_data->blind_rank = tools::to_string(i+1);
        blind_data->blind_blind = tools::to_string(current_small_blind) + std::string("/") + tools::to_string(current_big_blind);
        blind_data->blind_ante = tools::to_string(current_ante);
        blind_data->blind_lasttime = tools::to_string(current_last_time);
        blind_data->blind_resttime = tools::to_string(current_rest_time);
        
        blind_cell_datas_.push_back(blind_data);
    }
    
    tableview_->reloadData();
}

#pragma mark tableview datasource --
TableViewCell* LayerComInfoBlind::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto& data = blind_cell_datas_.at(idx);

    NodeBlindCell* cell = dynamic_cast<NodeBlindCell*>(table->dequeueCell());
    if (!cell){
        cell = NodeBlindCell::create();
    }
    cell->update_data(data);
    
    if (idx == current_level_ - 1)
        cell->set_is_select(true);
    else
        cell->set_is_select(false);
    
    return cell;
}

ssize_t LayerComInfoBlind::numberOfCellsInTableView(TableView *table)
{
    return blind_cell_datas_.size();
}

#pragma mark tableview delegate --
void LayerComInfoBlind::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void LayerComInfoBlind::tableCellHighlight(TableView* table, TableViewCell* cell)
{

}

void LayerComInfoBlind::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

}