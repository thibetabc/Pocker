//
//  LayerChargeForOther.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/27.
//
//

#include "LayerChargeForOther.h"
#include "LayerFriendList.h"
#include "GameDataManager.h"
#include "TipView.h"
#include "utils.h"
LayerChargeForOther* LayerChargeForOther::create(bool is_charge_other)
{
    LayerChargeForOther* charge = new LayerChargeForOther;
    if (charge->init(is_charge_other))
    {
        charge->autorelease();
        return charge;
    }
    return nullptr;
}

bool LayerChargeForOther::init(bool is_charge_other)
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    auto layer = LayerColor::create(Color4B(143, 143, 143, 100));
    this->addChild(layer);
    
    root_ = CSLoader::createNode("LayerChargeForOther.csb");
    this->addChild(root_);
    
    btn_cancel_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_cancel"));
    btn_cancel_->addClickEventListener(std::bind(&LayerChargeForOther::click_cancle, this,std::placeholders::_1));
    btn_cancel_->setTitleText(tools::local_string("cancel","取消"));
    
    btn_charge_self_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_charge_self"));
    btn_charge_other_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_charge_other"));
    
    panel_charge_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "panel_charge_bg"));
    
    if (!is_charge_other)
    {
        btn_charge_self_->setTitleText(tools::local_string("charge_for_self","为自己充值"));
        btn_charge_self_->addClickEventListener(std::bind(&LayerChargeForOther::click_charge_self, this,std::placeholders::_1));
        btn_charge_other_->setTitleText(tools::local_string("charge_for_others","为他人充值"));
        btn_charge_other_->addClickEventListener(std::bind(&LayerChargeForOther::click_charge_other, this,std::placeholders::_1));
    }
    else
    {
        btn_charge_other_->setTitleText(tools::local_string("charge_for_self","为自己充值"));
        btn_charge_other_->addClickEventListener(std::bind(&LayerChargeForOther::click_charge_self, this,std::placeholders::_1));
        btn_charge_self_->setVisible(false);
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerChargeForOther::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerChargeForOther::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerChargeForOther::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void LayerChargeForOther::click_cancle(cocos2d::Ref *sender)
{
    this->removeFromParent();
}

void LayerChargeForOther::click_charge_self(cocos2d::Ref *sender)
{
    if (callback_)
    {
        callback_(GDM->get_user_id());
    }
}

void LayerChargeForOther::click_charge_other(cocos2d::Ref *sender)
{
    auto layer = LayerFriendList::create(LayerFriendList::Type::SELECT_SINGLE);
    layer->set_select_callback(std::bind(&LayerChargeForOther::handle_get_friend_id, this, std::placeholders::_1));
    this->addChild(layer,0,"LayerFriendList");
}

void LayerChargeForOther::handle_get_friend_id(std::vector<msg::UserDetailInfo> userDatailInfo)
{
    if (callback_)
    {
        if (userDatailInfo.size()==0)
        {
            callback_(0);
        }
        else
        {
            callback_(userDatailInfo.at(0).userid());
        }
    }
}

void LayerChargeForOther::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    if (panel_charge_bg_->getBoundingBox().containsPoint(touchLocation))
    {
        
    }
    else
    {
        this->removeFromParent();
    }
}





