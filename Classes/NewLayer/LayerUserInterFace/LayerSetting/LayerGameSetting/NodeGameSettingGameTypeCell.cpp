//
//  NodeGameSettingGameTypeCell.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/19.
//
//

#include "NodeGameSettingGameTypeCell.h"
bool NodeGameSettingGameTypeCell::init()
{
    if (!Node::init())
    {
        return false;
    }
    root_=CSLoader::createNode("NodeGameSettingGameTypeCell.csb");
    this->addChild(root_);
    
    Node_choose_btn_=dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "Node_choose_btn"));
    btn_choose_=NodeCircleChoose::create();
    Node_choose_btn_->addChild(btn_choose_);
    btn_choose_->setTouchEnabled(false);
    
    text_game_type_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_game_type"));
    text_game_type_->setString("");
    
    return true;
}
void NodeGameSettingGameTypeCell::set_btn_choose_status(bool visible)
{
    if (visible)
    {
        btn_choose_->set_node_unchoose_status(true);
        text_game_type_->setColor(Color3B(255, 255, 255));
    }
    else
    {
        btn_choose_->set_node_unchoose_status();
        text_game_type_->setColor(Color3B(76, 96, 115));
    }
}

void NodeGameSettingGameTypeCell::set_text_game_type(std::string str,msg::RoomType type)
{
    text_game_type_->setString(str);
    room_type_=type;
}

void NodeGameSettingGameTypeCell::set_bind_func(std::function<void(std::string)> bind_func)
{
    btn_choose_->set_click_callback(bind_func);
}