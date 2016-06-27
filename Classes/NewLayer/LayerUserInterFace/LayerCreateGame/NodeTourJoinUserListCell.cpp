//
//  NodeTourJoinUserListCell.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/5/26.
//
//

#include "NodeTourJoinUserListCell.h"
#include "utils.h"

NodeTourJoinUserListCell* NodeTourJoinUserListCell::create(msg::UserDetailInfo userInfo)
{
    NodeTourJoinUserListCell* result = new NodeTourJoinUserListCell;
    if(result->init()) {
        result->autorelease();
        result->update_data(userInfo);
        return result;
    }
    return nullptr;
}

bool NodeTourJoinUserListCell::init()
{
    if(!Widget::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeTourJoinUserListCell.csb");
    this->addChild(root_);
    
    text_join_user_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_join_user_nickname"));
    
    this->setContentSize(cocos2d::Size(630, 80));
    
    return true;
}

void NodeTourJoinUserListCell::update_data(msg::UserDetailInfo userInfo)
{
    user_info_ = userInfo;
    text_join_user_nickname_->setString(userInfo.nickname());
}
