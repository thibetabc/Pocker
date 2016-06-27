//
//  PublicHandcard.cpp
//  pokerclient
//
//  Created by admin on 15-9-7.
//
//

#include "PublishHandcard.h"
#include "GameDataManager.h"
#include "utils.h"
#include "cmd_def.h"
//#include "GameTools.h"
#include "ActionSheet.h"
#include "TipView.h"



bool PublishHandcard::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    
    auto size = Director::getInstance()->getWinSize();
    
    
    auto root = CSLoader::createNode("LayerHandCards.csb");
    this->addChild(root);
    
    auto btn_publish = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_publish"));
    auto btn_cancel = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_cancel"));
//    auto btn_add_pic = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_add_pic"));

    btn_publish->setTitleText(tools::local_string("publish", "发布"));
    btn_cancel->setTitleText(tools::local_string("cancel", "取消"));
    
    auto image_add_handcard = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_add_handcard"));
    auto image_public_hc = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_public_hc"));
    auto image_check = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check"));
    auto image_need_point = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_need_point"));
    auto image_pay_point = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_pay_point"));
    image_add_handcard->addClickEventListener(std::bind(&PublishHandcard::click_image_add_handcard, this, std::placeholders::_1));
    image_public_hc->addClickEventListener(std::bind(&PublishHandcard::click_image_public_hc, this, std::placeholders::_1));
    image_check->addClickEventListener(std::bind(&PublishHandcard::click_image_visible, this, std::placeholders::_1));
    image_need_point->addClickEventListener(std::bind(&PublishHandcard::click_image_need_point, this, std::placeholders::_1));
    image_pay_point->addClickEventListener(std::bind(&PublishHandcard::click_image_pay_point, this, std::placeholders::_1));
    
    auto text_add_handcard = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_add_handcard"));
    auto text_public_hc = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_public_hc"));
    auto text_check = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_check"));
    auto text_need_point = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_need_point"));
    auto text_pay_point = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_pay_point"));
    text_add_handcard->setString(tools::local_string("publish", "添加手牌"));
    text_public_hc->setString(tools::local_string("publish", "公布手牌"));
    text_check->setString(tools::local_string("publish", "谁能查看"));
    text_need_point->setString(tools::local_string("publish", "所需积分"));
    text_pay_point->setString(tools::local_string("publish", "悬赏积分"));

    
    auto text_public_hc_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_public_hc_"));
    auto text_check_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_check_"));
    auto text_need_point_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_need_point_"));
    auto text_pay_point_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_pay_point_"));
    text_public_hc_->setString(tools::local_string("publish", "YES"));
    text_check_->setString(tools::local_string("publish", "ALL"));
    text_need_point_->setString(tools::local_string("publish", "0"));
    text_pay_point_->setString(tools::local_string("publish", "0"));

    
    auto text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_content"));
    text_title->setString(tools::local_string("publish HnadCard", "发布手牌"));
    
    return true;
}



void PublishHandcard::click_btn_publish(Ref* sender){
    
    
}

void PublishHandcard::click_btn_cancel(Ref* sender){
    this->removeFromParent();
    if(btn_cancel_callback_) {
        btn_cancel_callback_();
    }
}
void PublishHandcard::click_btn_add_pic(Ref* sender){
    ActionSheet* AS = ActionSheet::create();
    BaseLayer* parent1 = static_cast<BaseLayer*>(this->getParent());
    parent1->addChild(AS);
    
    
    if(btn_add_pic_callback_) {
        btn_add_pic_callback_();
    }
    
    
    
}
void PublishHandcard::click_image_visible(Ref* sender){
    
}
void PublishHandcard::click_image_need_point(Ref* sender){
    
}
void PublishHandcard::click_image_pay_point(Ref* sender){
    
}
void PublishHandcard::click_image_add_handcard(Ref* sender)
{}
void PublishHandcard::click_image_public_hc(Ref* sender)
{}


