//
//  GroupFriendSquare.cpp
//  pokerclient
//
//  Created by admin on 15-8-24.
//
//

#include "GroupFriendSquare.h"
#include "utils.h"
#include "ImagePickerBridge.h"
#include "cmd_def.h"

bool GroupFriendSquare::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto layerColor = LayerColor::create(Color4B(100, 100, 100, 80));
    this->addChild(layerColor, -1);
    
    auto layout = Layout::create();
    layout->setContentSize(this->getContentSize());
    layout->setTouchEnabled(true);
    this->addChild(layout);
    
    origin_friend = Point(0, -189);
    
    root_friend = CSLoader::createNode("Node_square_date.csb");
    root_friend->setPosition(origin_friend);
    this->addChild(root_friend);
    
    auto image_square_head_pic= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_square_head_pic"));
    auto image_like= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_like"));
    auto image_review= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_review"));
    auto text_nickname= dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_nickname"));
    auto text_publish_content= dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_publish_content"));
    auto text_content_type= dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_content_type"));
    auto text_need_points = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_need_points"));
    auto text_reward_points = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_reward_points"));
    auto text_review = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_review"));
    auto text_like = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_like"));
    auto text_content_time = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_content_time"));
    
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_friend, "ListView_"));
    

    
    image_like->addClickEventListener(std::bind(&GroupFriendSquare::chilk_image_like, this, std::placeholders::_1));
    image_review->addClickEventListener(std::bind(&GroupFriendSquare::click_image_review, this, std::placeholders::_1));
    

    
//    text_content->setString(tools::local_string("综合内容", "综合内容"));
//    text_cancel->setString(tools::local_string("取消", "取消"));
//    text_issue_event->setString(tools::local_string("发布活动", "发布活动"));
//    text_brand->setString(tools::local_string("猜手牌", "猜手牌"));
//    text_report->setString(tools::local_string("现场报告", "取消"));
    
    
    auto& size = Director::getInstance()->getWinSize();
    
    image_like->setContentSize(Size(size.width, image_like->getContentSize().height));
    image_review->setContentSize(Size(size.width, image_review->getContentSize().height));
    
//    image_content->setPosition(Point(size.width/2, image_content->getPosition().y));
//    image_cancel->setPosition(Point(size.width/2, image_cancel->getPosition().y));
//
//    text_content->setPosition(Point(size.width/2, text_content->getPosition().y));
//    text_cancel->setPosition(Point(size.width/2, text_cancel->getPosition().y));
    
    //register_events(); 
    
  
    
    
    return true;
}




void GroupFriendSquare::chilk_image_like(Ref* sender)
{
    
}

void GroupFriendSquare::click_image_review(Ref* sender)
{
   
    
}

void GroupFriendSquare::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    //root_friend->runAction(MoveTo::create(0.2f, Point::ZERO));

}
