//
//  NodeBlindCell.cpp
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#include "NodeBlindCell.h"

bool NodeBlindCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }

    root_ = CSLoader::createNode("NodeComInfoBlindCellPortrait.csb");
    this->addChild(root_);
    
    image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_cominfo_cell_bg"));
    
    text_rank_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_blind_rank"));
    text_blind_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_blind_blind"));
    text_ante_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_blind_ante"));
    text_last_time_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_blind_lasttime"));
    text_rest_time_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_blind_resttime"));
    
    text_rank_->setString("");
    text_blind_->setString("");
    text_ante_->setString("");
    text_last_time_->setString("");
    text_rest_time_->setString("");
    
    return true;
}

void NodeBlindCell::update_data(const std::shared_ptr<NodeBlindCellData> & blind_cell_data)
{
    blind_cell_data_ = blind_cell_data;
    auto data = blind_cell_data_.lock();
    
    if (data == nullptr)
    {
        cocos2d::log("Error : NodeBlindCell data == nullptr");
        return;
    }
    
    text_rank_->setString(data->blind_rank);
    text_blind_->setString(data->blind_blind);
    text_ante_->setString(data->blind_ante);
    text_last_time_->setString(data->blind_lasttime);
    text_rest_time_->setString(data->blind_resttime);
}

void NodeBlindCell::set_is_select(bool is_touch)
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