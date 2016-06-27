//
//  LayerComInfoReward.cpp
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#include "LayerComInfoReward.h"

bool LayerComInfoReward::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerComInfoRewardPortrait.csb");
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

void LayerComInfoReward::update_data(const msg::Processor_530_PokerGetTourmentStatus_DOWN& tourment_status)
{
    reward_cell_datas_.clear();
    
    for (int i = 0; i < tourment_status.tournamentinfo().tournamentprize().level().size(); i++)
    {
        int32_t total_rewards = tourment_status.prizepool();
        int32_t prize_percent = tourment_status.tournamentinfo().tournamentprize().level(i).prizepercent();
        std::shared_ptr<NodeRewardCellData> reward_data(new NodeRewardCellData);
        reward_data->reward_rank = tools::to_string(i+1);
        reward_data->reward_prize = tools::to_string((total_rewards * prize_percent / 100));
        reward_data->reward_other_prize = "";
        
        reward_cell_datas_.push_back(reward_data);
    }
    for (int i = 0; i < tourment_status.tournamentinfo().tournamentprize().level().size(); i++)
    {
        int32_t min_rank = tourment_status.tournamentinfo().tournamentprize().level(i).minrank();
        int32_t max_rank = tourment_status.tournamentinfo().tournamentprize().level(i).maxrank();
        std::string other_prize = tourment_status.tournamentinfo().tournamentprize().level(i).otherprize();

        if (min_rank >= 1 && max_rank >= 1 &&
            min_rank <= max_rank &&
            min_rank <= reward_cell_datas_.size() &&
            max_rank <= reward_cell_datas_.size())
        {
            for (int j = min_rank-1; j <= max_rank-1; j++)
            {
                reward_cell_datas_[j]->reward_other_prize += reward_cell_datas_[j]->reward_other_prize == "" ? "" : "、";
                reward_cell_datas_[j]->reward_other_prize += other_prize;
            }
        }
    }
    
    tableview_->reloadData();
}

#pragma mark tableview datasource --
TableViewCell* LayerComInfoReward::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto& data = reward_cell_datas_.at(idx);
    
    NodeRewardCell* cell = dynamic_cast<NodeRewardCell*>(table->dequeueCell());
    if (!cell){
        cell = NodeRewardCell::create();
    }
    cell->update_data(data);
    
    return cell;
}

ssize_t LayerComInfoReward::numberOfCellsInTableView(TableView *table)
{
    return reward_cell_datas_.size();
}

#pragma mark tableview delegate --
void LayerComInfoReward::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

void LayerComInfoReward::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    
}

void LayerComInfoReward::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
}