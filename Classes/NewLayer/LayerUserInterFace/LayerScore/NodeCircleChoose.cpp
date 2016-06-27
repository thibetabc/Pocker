//
//  NodeSelectedCircleButton.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/14.
//
//

#include "NodeCircleChoose.h"
bool NodeCircleChoose::init()
{
    if(!ImageView::init())
    {
        return false;
    }
    root_=CSLoader::createNode("NodeCircleChoose.csb");
    this->addChild(root_);
    
    this->setContentSize(root_->getContentSize());
    
    btn_choose_status_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_choose_status"));
    btn_choose_status_->setVisible(false);
    btn_choose_status_->setTouchEnabled(false);
    btn_choose_status_->setTitleText("");
    btn_choose_status_->addClickEventListener(std::bind(&NodeCircleChoose::click_btn_event, this,std::placeholders::_1));
    
    
    btn_not_choose_status_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_not_choose_status"));
    btn_not_choose_status_->setTitleText("");
    btn_not_choose_status_->setVisible(true);
    btn_not_choose_status_->setTouchEnabled(true);
    btn_not_choose_status_->addClickEventListener(std::bind(&NodeCircleChoose::click_btn_event, this,std::placeholders::_1));
    btn_not_choose_status_->setTouchEnabled(false);
    
    return true;
}

void NodeCircleChoose::click_btn_event(cocos2d::Ref *sender)
{
    if (btn_not_choose_status_->isVisible())
    {
        if (function_callback_) {
            btn_choose_status_->setVisible(true);
            btn_not_choose_status_->setVisible(false);
            btn_not_choose_status_->setTouchEnabled(false);
            function_callback_("choose");
        }
    }
}
void NodeCircleChoose::set_node_unchoose_status(bool isChoose)
{
    if(isChoose)
    {
        btn_choose_status_->setVisible(true);
        btn_not_choose_status_->setVisible(false);
        btn_not_choose_status_->setTouchEnabled(false);
    }
    else
    {
        if(btn_choose_status_->isVisible())
        {
            btn_choose_status_->setVisible(false);
            btn_choose_status_->setTouchEnabled(false);
            btn_not_choose_status_->setVisible(true);
            if (function_callback_) {
                btn_not_choose_status_->setTouchEnabled(true);
            }
            
        }
    }
    
}
