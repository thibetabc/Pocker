//
//  NodeCreateTourRewardInfoCell.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/5/26.
//
//

#include "NodeCreateTourRewardInfoCell.h"
#include "utils.h"

NodeCreateTourRewardInfoCell* NodeCreateTourRewardInfoCell::create(uint32_t idx, msg::PrizeLevel prizeLevel, bool isCreate)
{
    NodeCreateTourRewardInfoCell* result = new NodeCreateTourRewardInfoCell;
    if(result->init(idx, prizeLevel, isCreate)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

bool NodeCreateTourRewardInfoCell::init(uint32_t idx, msg::PrizeLevel prizeLevel, bool isCreate)
{
    if(!Widget::init()) {
        return false;
    }
    
    cell_idx_ = idx;
    prize_level_ = prizeLevel;
    is_create_ = isCreate;
    
    root_ = CSLoader::createNode("NodeTournamentRewardInfoCell.csb");
    this->addChild(root_);
    
    text_reward_info_rank_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_reward_info_rank"));
    
    text_reward_info_reward_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_reward_info_reward"));
    
    text_reward_info_other_reward_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_reward_info_other_reward"));
    
    if (prizeLevel.minrank() == prizeLevel.maxrank())
    {
        text_reward_info_rank_->setString(std::string("第") + tools::to_string(prizeLevel.minrank()) + std::string("名"));
    }else
    {
        text_reward_info_rank_->setString(std::string("第") + tools::to_string(prizeLevel.minrank()) + std::string("-") + tools::to_string(prizeLevel.maxrank()) + std::string("名"));
    }
    
    text_reward_info_reward_->setString(tools::to_string(prizeLevel.prizepercent()));
    
    if (prizeLevel.otherprize() == "")
    {
        text_reward_info_other_reward_->setString("-----");
    }else
    {
        text_reward_info_other_reward_->setString(prizeLevel.otherprize());
    }


    this->setContentSize(cocos2d::Size(630, 80));
    
    this->setTouchEnabled(true);
    
    this->addClickEventListener(std::bind(&NodeCreateTourRewardInfoCell::click_cell, this, std::placeholders::_1));
    
    return true;
}

void NodeCreateTourRewardInfoCell::click_cell(Ref * sender)
{
    if (is_create_ && reward_cell_callback_)
    {
        reward_cell_callback_(cell_idx_);
    }
}