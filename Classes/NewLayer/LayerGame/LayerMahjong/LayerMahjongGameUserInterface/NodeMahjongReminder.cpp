//
//  NodeMahjongReminder.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/12.
//
//

#include "NodeMahjongReminder.h"
#include "NodeMahjong.h"
#include "utils.h"
#include "MahjongCommon.h"

bool NodeMahjongReminder::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeMahjongReminder.csb");
    this->addChild(root_);
    
    node_card_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_,"node_card"));
    
    text_fan_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_,"text_fan"));
    text_fan_desc_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_,"text_fan_desc"));
    text_zhang_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_,"text_zhang"));
    text_zhang_desc_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_,"text_zhang_desc"));
    text_zhang_desc_->setString("张");
    text_fan_desc_->setString("番");
    this->setContentSize(Size(136, 101));
    
    return true;
}

void NodeMahjongReminder::set_data(const msg::MahjongTing &ting)
{
    auto card_node = createCard(CLASS_CARD::SOUTH_HAND, ting.card()); //手牌
    node_card_->addChild(card_node);
    text_fan_->setString(tools::to_string(ting.score()));
    text_zhang_->setString(tools::to_string(ting.number()));
}