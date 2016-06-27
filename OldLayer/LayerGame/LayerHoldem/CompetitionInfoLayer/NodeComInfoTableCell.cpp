//
//  NodeComInfoTableCell.cpp
//  pokerclient
//
//  Created by ll on 15/11/30.
//
//

#include "NodeComInfoTableCell.h"

bool NodeComInfoTableCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeComInfoTableCellPortrait.csb");
    this->addChild(root_);
    
    text_player_seat_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_seat_num"));
    text_player_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_nickname"));
    text_player_chips_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_chips"));
    
    text_player_seat_num_->setString("");
    text_player_nickname_->setString("");
    text_player_chips_->setString("");
    
    return true;
}

void NodeComInfoTableCell::update_data(const std::shared_ptr<NodeComInfoTableCellData> & table_cell_data)
{
    table_cell_data_ = table_cell_data;
    auto data = table_cell_data_.lock();
    
    if (data == nullptr)
    {
        cocos2d::log("Error : NodeComInfoTableCell data == nullptr");
        return;
    }
    
    text_player_seat_num_->setString(tools::to_string(data->player_seat_num));
    text_player_nickname_->setString(data->player_nickname);
    text_player_chips_->setString(tools::to_string(data->player_chips));
}