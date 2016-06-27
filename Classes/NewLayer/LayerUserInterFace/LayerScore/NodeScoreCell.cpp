//
//  NodeScoreCell.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/11.
//
//

#include "NodeScoreCell.h"
#include "utils.h"
NodeScoreCell* NodeScoreCell::create(bool is_group_match)
{
    NodeScoreCell* cell = new NodeScoreCell;
    if (cell->init(is_group_match))
    {
        cell->autorelease();
        return cell;
    }
    return nullptr;
}
bool NodeScoreCell::init(bool is_group_match)
{
    if (!TableViewCell::init())
    {
        return false;
    }
    root_=CSLoader::createNode("NodeScoreCell.csb");
    this->addChild(root_);
    is_group_match_ = is_group_match;
    text_chip_num_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_chip_num"));
    text_come_from_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_come_from"));
    text_earn_chip_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_earn_chip"));
    text_group_name_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_group_name"));
    text_room_name_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_name"));
    text_room_time_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_time"));
    text_time_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_time"));
    
    text_chip_num_->setTouchEnabled(true);
    text_chip_num_->addClickEventListener(std::bind(&NodeScoreCell::response_for_chip_callback, this,std::placeholders::_1));
    
    text_room_name_->setTouchEnabled(true);
    text_room_name_->addClickEventListener(std::bind(&NodeScoreCell::response_for_group_callback,this,std::placeholders::_1));
    
    text_chip_num_->setString("");
    text_come_from_->setString(tools::local_string("from","来自"));
    text_earn_chip_->setString("");
    text_group_name_->setString("");
    text_room_name_->setString("");
    text_room_time_->setString("");
    text_time_->setString("");
    
    image_chip_=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_chip"));
    image_time_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_time"));
    
    this->setContentSize(cocos2d::Size(100, 100));
    
    return true;
}

void NodeScoreCell::update_data(const msg::HistoryScore& reward_cell_data)
{
    reward_cell_data_.CopyFrom(reward_cell_data);
    if (is_group_match_)
    {
        text_chip_num_->setPosition(text_time_->getPosition());
        image_chip_->setPosition(image_time_->getPosition());
        text_time_->setVisible(false);
        image_time_->setVisible(false);
        text_earn_chip_->setVisible(false);        
    }
    if(reward_cell_data_.label_size()==1)
    {
        text_chip_num_->setString(reward_cell_data_.label(0));
        image_chip_->setVisible(true);
    }
    else
    {
        text_chip_num_->setString("");
        image_chip_->setVisible(false);
    }
   
    text_earn_chip_->setString(tools::to_string(reward_cell_data_.score()));
    text_group_name_->setString(reward_cell_data_.roomname());
    text_room_name_->setString(reward_cell_data_.groupname());
    double roomtime=tools::stod(reward_cell_data_.endtime());
    roomtime=roomtime/1000;
    text_room_time_->setString(tools::time_display(roomtime,tools::TIMEDISPLAYTYPE::ONLYDAY)+"  "+tools::time_display(roomtime,tools::TIMEDISPLAYTYPE::ONLYTIME));
    std::string timestr="";
    long time=reward_cell_data_.playtime()/(60*60*1000);
    if (time==0)
    {
        time=reward_cell_data_.playtime()/(60*1000);
        timestr=tools::to_string(time)+tools::local_string("minutes","分钟");
    }
    else
    {
        timestr=tools::to_string(time)+tools::local_string("hours","小时");
    }
    text_time_->setString(timestr);
    room_id_=reward_cell_data_.roomid();
}

int NodeScoreCell::getRoomId()
{
    return room_id_;
}

void NodeScoreCell::response_for_chip_callback(cocos2d::Ref *sender)
{
    if (func_click_chip_)
    {
        func_click_chip_(text_chip_num_->getString());
    }
}

void NodeScoreCell::response_for_group_callback(cocos2d::Ref *sender)
{
    if (func_click_groupname_) {
        func_click_groupname_(tools::to_string(room_id_),text_room_name_->getString());
    }
}

