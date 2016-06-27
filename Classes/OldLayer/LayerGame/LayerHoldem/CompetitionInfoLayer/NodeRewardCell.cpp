//
//  NodeRewardCell.cpp
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#include "NodeRewardCell.h"

bool NodeRewardCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeComInfoRewardCellPortrait.csb");
    this->addChild(root_);
    
    image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_cominfo_cell_bg"));
    
    text_rank_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_reward_rank"));
    text_prize_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_reward_reward"));;
    text_other_prize_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_reward_other_reward"));;
    
    text_rank_->setString("");
    text_prize_->setString("");
    text_other_prize_->setString("");
    
    return true;
}

void NodeRewardCell::update_data(const std::shared_ptr<NodeRewardCellData> & reward_cell_data)
{
    reward_cell_data_ = reward_cell_data;
    auto data = reward_cell_data_.lock();
    
    if (data == nullptr)
    {
        cocos2d::log("Error : NodeRewardCell data == nullptr");
        return;
    }
    
    text_rank_->setString(data->reward_rank);
    text_prize_->setString(data->reward_prize);
    text_other_prize_->setString(data->reward_other_prize);
}

void NodeRewardCell::set_is_select(bool is_touch)
{
    std::string image_bg_name;
    if (is_touch)
    {
        image_bg_name = "image/tournament_info_portrait_res/image_cominfo_cell_bg_select.png";
        
    }else {
        image_bg_name = "image/tournament_info_portrait_res/image_cominfo_cell_bg.png";
    }
    image_bg_->loadTexture(image_bg_name);
}