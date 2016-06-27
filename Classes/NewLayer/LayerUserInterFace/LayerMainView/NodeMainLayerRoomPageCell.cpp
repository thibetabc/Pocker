//
//  NodeMainLayerRoomPageCell.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/27.
//
//

#include "NodeMainLayerRoomPageCell.h"

bool NodeMainLayerRoomPageCell::init()
{
    if(!Widget::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeMainLayerRoomPageCell.csb");
    this->addChild(root_);
    
    text_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_1"));
    text_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_2"));
    
    this->setContentSize(cocos2d::Size(750, 58));
    
    return true;
}

void NodeMainLayerRoomPageCell::update_data(std::pair<std::string , std::string> room_page_cell_data)
{
    room_page_cell_data_ = room_page_cell_data;
    
    text_1_->setString(room_page_cell_data_.first);
    text_2_->setString(room_page_cell_data_.second);
}