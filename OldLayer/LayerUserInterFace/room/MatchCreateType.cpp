//
//  MatchCreateType.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/12/17.
//
//

#include "MatchCreateType.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "GameTools.h"
#include "TipView.h"
#include "msg.pb.h"
#include "GameDataManager.h"
#include "utils.h"
#include "RoomLayer.h"
bool MatchCreateType::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto root = CSLoader::createNode("LayerMatchType.csb");
    this->addChild(root);
    auto btn_sit_and_go = (Button *)CSLoader::seekNodeByName(root, "btn_size_match");
    auto btn_time_match = (Button *)CSLoader::seekNodeByName(root, "btn_time_match");
    btn_sit_and_go->addClickEventListener(std::bind(&MatchCreateType::click_btn_sit_and_go, this));
    btn_time_match->addClickEventListener(std::bind(&MatchCreateType::click_btn_time_match, this));
    LayerColor *layer = LayerColor::create(Color4B::WHITE);
    layer->setZOrder(-1);
    layer->setOpacity(100);
    this->addChild(layer);
    this->setSwallowsTouches(true);
    
    EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
    
    auto listen = EventListenerTouchOneByOne::create();
    listen->onTouchBegan = CC_CALLBACK_2(MatchCreateType::onTouchBegan,this);
    listen->onTouchMoved = CC_CALLBACK_2(MatchCreateType::onTouchMoved,this);
    listen->onTouchEnded = CC_CALLBACK_2(MatchCreateType::onTouchEnded,this);
    listen->onTouchCancelled = CC_CALLBACK_2(MatchCreateType::onTouchCancelled,this);
    eventDispatcher->addEventListenerWithSceneGraphPriority(listen,this);
    return true;
}

void MatchCreateType::click_btn_sit_and_go()
{
    this->roomLayer_->create_sit_and_go();
    this->removeFromParent();
}

void MatchCreateType::click_btn_time_match()
{
    this->roomLayer_->create_time_match();
    this->removeFromParent();
}
bool MatchCreateType::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return true;
}
void MatchCreateType::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}
void MatchCreateType::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    this->removeFromParent();
}