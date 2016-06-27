//
//  ActionIssue.cpp
//  pokerclient
//
//  Created by admin on 15-8-19.
//
//

#include "ActionIssue.h"
#include "utils.h"
#include "cmd_def.h"
#include "GroupFriendSquare.h"
#include "GroupShareLayer.h"
#include "TipView.h"

bool ActionIssue::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto layerColor = LayerColor::create(Color4B(100, 100, 100, 80));
    this->addChild(layerColor, -1);
    
    auto size_ = Director::getInstance()->getWinSize();
    size_ = Size(std::min(size_.width, size_.height), std::max(size_.width, size_.height));
    
    auto layout_= ui::Layout::create();
    layout_->setContentSize(size_);
    layout_->setTouchEnabled(true);
    layout_->setSwallowTouches(true);
    layout_->setAnchorPoint(Point::ZERO);
    layout_->addClickEventListener([=](Ref* sender){
        ActionIssue::click_image_cancel(sender);
    });
    this->addChild(layout_, -9);
    
    origin_friend = Point(0, -189);
    
    root_friend = CSLoader::createNode("LayerFriendIssue.csb");
    root_friend->setPosition(origin_friend);
    this->addChild(root_friend);
    
    auto image_content= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_content"));
    auto image_issue_event= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_issue_event"));
    auto image_brand= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_brand"));
     auto image_report= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_report"));
    auto image_cancel = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_cancel"));
    
    cocos2d::log("ActionIssue : iamge_content %f", image_content->getContentSize().height);
    
    image_brand->setTouchEnabled(true);
    image_content->addClickEventListener(std::bind(&ActionIssue::click_image_content, this, std::placeholders::_1));
    image_cancel->addClickEventListener(std::bind(&ActionIssue::click_image_cancel, this, std::placeholders::_1));
    image_issue_event->addTouchCallback(std::bind(&ActionIssue::click_image_issue_event, this, std::placeholders::_1));
    image_brand->addClickEventListener(std::bind(&ActionIssue::click_image_brand, this, std::placeholders::_1));
    image_report->addClickEventListener(std::bind(&ActionIssue::click_image_report, this, std::placeholders::_1));
    
    auto text_content= dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_content"));
    auto text_cancel = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_cancel"));
    auto text_issue_event = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_issue_event"));
    auto text_brand = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_brand"));
    auto text_report= dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_report"));
    
    text_content->setString(tools::local_string("综合内容", "综合内容"));
    text_cancel->setString(tools::local_string("取消", "取消"));
    text_issue_event->setString(tools::local_string("发布活动", "发布活动"));
    text_brand->setString(tools::local_string("手牌分享", "手牌分享"));
    text_report->setString(tools::local_string("现场报告", "取消"));
    
    this->removeChild(text_report);
    this->removeChild(text_issue_event);
    this->removeChild(image_report);
    this->removeChild(image_issue_event);
    
    image_issue_event->setVisible(false);
    text_issue_event->setVisible(false);
    image_report->setVisible(false);
    text_report->setVisible(false);
    
    image_brand->setPositionY(image_brand->getPositionY()-image_brand->getContentSize().height);
    text_brand->setPositionY(text_brand->getPositionY()-image_brand->getContentSize().height);
    //硬patch， 先屏蔽掉其他两项未实现内容。 
    
    //auto& size = Director::getInstance()->getWinSize();
    
    /*image_content->setContentSize(Size(size.width, image_content->getContentSize().height));
    image_cancel->setContentSize(Size(size.width, image_cancel->getContentSize().height));
    
    image_content->setPosition(Point(size.width/2, image_content->getPosition().y));
    image_cancel->setPosition(Point(size.width/2, image_cancel->getPosition().y));
    
    text_content->setPosition(Point(size.width/2, text_content->getPosition().y));
    text_cancel->setPosition(Point(size.width/2, text_cancel->getPosition().y));*/
    
    //register_events();
    
    return true;
}

void ActionIssue::click_image_content(Ref* sender)//zongheneirong
{
    GroupShareLayer *event = GroupShareLayer::create();
    event->set_type(1);
    this->getParent()->addChild(event);
    this->removeFromParentAndCleanup(true);
}

void ActionIssue::click_image_cancel(Ref* sender)
{
    MoveTo* move = MoveTo::create(0.2f, origin_friend);
    Sequence* seq = Sequence::create(move, CallFunc::create([this]() {
        this->removeFromParentAndCleanup(true);
    }), NULL);
    
    root_friend->runAction(seq);
}
void ActionIssue::click_image_report(Ref* sender)
{
    TipView::showAlertView("此功能暂未开通");
}
void ActionIssue::click_image_brand(Ref* sender){//shoupai
    GroupShareLayer *event = GroupShareLayer::create();
    event->set_type(2);
    this->getParent()->addChild(event);
    this->removeFromParentAndCleanup(true);
}
void ActionIssue::click_image_issue_event(Ref* sender){
    TipView::showAlertView("此功能暂未开通");
    
}

void ActionIssue::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    root_friend->runAction(MoveTo::create(0.2f, Point::ZERO));
}



