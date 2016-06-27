//
//  GroupFriendCell.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/9/1.
//
//

#include "GroupFriendCell.h"
#include "GameDataManager.h"
#include "utils.h"

bool GroupFriendCell::init()
{
    if(!TableViewCell::init())
    {
        return false;
    }
    
    auto root = CSLoader::createNode("Node_square_date.csb");
    this->addChild(root);
    
    text_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_nickname"));
    text_publish_content = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_publish_content"));
    text_content_type = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_content_type"));
    
    text_need_points = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_need_points"));
    text_need_points->setString(tools::local_string("need_point", "所需积分"));
    
    text_reward_points = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_reward_points"));
    text_reward_points->setString(tools::local_string("reward_point", "悬赏积分"));
    
    text_review = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_review"));
    image_review = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_review"));
    text_review->setString(tools::local_string("content","评论"));
    
    text_like = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_like"));
    image_like = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_like"));
    text_like->setString(tools::local_string("zan","分享"));
    
    text_content_time = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_content_time"));
    
    text_publish_content->setTextAreaSize(Size(900, 150));
    
    LayerColor *bg = LayerColor::create(Color4B(166, 166, 166, 100), 960, 250 - 2);
    bg->setAnchorPoint(Point::ZERO);
    bg->setPosition(Point::ZERO);
    root->addChild(bg, -1);
    for(auto a:root->getChildren())
    {
        cocos2d::log("%s",a->getName().c_str());
    }
    
  
    return true;
}

void GroupFriendCell::update_property(::msg::Moment data)
{
    char c[20];
    //sprintf(c, "%d",data.userid());
    this->text_nickname_->setString(std::string(c));
    this->text_nickname_->setColor(Color3B::BLACK);
    //this->text_publish_content->setString(data.momentinfo().normalmomentinfo().wordcontent());
//    this->text_publish_content->setHighlighted(true);
//    this->text_publish_content->setTextHorizontalAlignment(TextHAlignment::RIGHT);
//    this->text_publish_content->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    this->text_publish_content->setColor(Color3B::BLACK);
    //this->text_content_type = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_content_type"));
    this->text_need_points->setString(tools::local_string("2_points_minimum", "需要积分 2"));
    this->text_reward_points->setString(tools::local_string("3_points_award", "悬赏积分 3"));
    //this->text_review = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_review"));
    //this->image_review = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "image_review"));
    //this->text_like = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_like"));
    //this->image_like = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "image_like"));
    //this->text_content_time = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_content_time"));

//    image_icon_->loadTexture(group_data->get_pic());
//    text_group_name_->setString(group_data->get_name());
//    text_group_remark_->setString(group_data->get_note());
//    
//    uint32_t unread_message_count = GDM->get_unread_message_count(group_data->get_id());
//    const std::string submit_time = GDM->get_last_message_submit_time(group_data->get_id());
//    
//    
//    std::string text;
//    if(!submit_time.empty()) {
//        double time = tools::stod(submit_time);
//        time /= 1000;
//        text = tools::time_display(time);
//    }
//    
//    text_time_->setString(text);
//    image_red_ball_->setVisible(unread_message_count != 0);
//    text_unread_message_count_->setVisible(unread_message_count != 0);
//    text_unread_message_count_->setString(tools::to_string(unread_message_count));
}