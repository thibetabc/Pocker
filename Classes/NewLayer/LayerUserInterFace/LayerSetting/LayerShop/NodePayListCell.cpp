//
//  NodePayListCell.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/26.
//
//
#include "utils.h"
#include "NodePayListCell.h"
bool NodePayListCell::init()
{
    if (!Widget::init())
    {
        return false;
    }
    
    root_ = CSLoader::createNode("NodePayListCell.csb");
    this->addChild(root_);
    
    text_diamond_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_diamond_num"));
    
    text_should_pay_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_should_pay"));
    
    text_title_first_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title_first"));
    text_title_first_->setString(tools::local_string("diamond", "钻石"));
    text_title_second_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title_second"));
    text_title_second_->setString(tools::local_string("payable", "应付"));
    
    panel_pay_cell_ = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "panel_pay_cell"));
    
    this->setContentSize(panel_pay_cell_->getContentSize());
    
    return true;
}

void NodePayListCell::update_data(std::pair<std::string,std::string> &data)
{
    text_diamond_num_->setString(data.first);
    text_should_pay_->setString(data.second);
}