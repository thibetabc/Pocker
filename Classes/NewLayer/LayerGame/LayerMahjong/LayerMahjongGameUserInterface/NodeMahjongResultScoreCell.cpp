//
//  NodeMahjongResultScoreCell.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#include "NodeMahjongResultScoreCell.h"
#include "utils.h"
#include "MahjongCommon.h"
bool NodeMahjongResultScoreCell::init()
{
    if ( !TableViewCell::init() )
    {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeMahjongReslutScoreCell.csb");
    this->addChild(root_);
    
    text_status_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_status"));
    
    text_score_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score"));
    
    text_direction_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_director"));
    text_status_->setAnchorPoint(Vec2(0, 0.5));
    
    return true;
}

void NodeMahjongResultScoreCell::set_data(msg::MahjongScoreChange score_change)
{
    std::string win_type = get_wintype(score_change);
    text_status_->setString(win_type);
    text_score_->setString(tools::to_string(score_change.score()));
    switch (score_change.source()) {
        case msg::MahjongPosition::LEFT:
            text_direction_->setString(tools::local_string("left_pos","上家"));
            break;
        case msg::MahjongPosition::OPPOSITE:
            text_direction_->setString(tools::local_string("opposite_pos","对家"));
            break;
        case msg::MahjongPosition::RIGHT:
            text_direction_->setString(tools::local_string("right_pos","下家"));
            break;
        case msg::MahjongPosition::SELF:
            text_direction_->setString(tools::local_string("self_pos","三家"));
            break;
        default:
            break;
    }
}