//
//  ReconnectView.cpp
//  pokerclient
//
//  Created by ronaldz on 15/12/25.
//
//

#include "ReconnectView.h"


bool ReconnectView::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    auto& win_size = Director::getInstance()->getWinSize();
    
    text_ = cocos2d::ui::Text::create("连接中...", "Thonburi", 38);
    text_->setAnchorPoint(Vec2(0, 0));
    text_->setPosition(Point(80, win_size.height-120));
    text_->setTextColor(Color4B::YELLOW);
    this->addChild(text_, 100);
    
    return true;
}

void ReconnectView::onEnter()
{
    Layer::onEnter();
    auto& win_size = Director::getInstance()->getWinSize();
    sprite_reconnect_ = Sprite::create("waitingview/round_1.png");
    
    sprite_reconnect_->setPosition(Point(sprite_reconnect_->getContentSize().width, win_size.height-100));
    this->addChild(sprite_reconnect_, 10);
    
    sprite_animation_ = Animation::create();
    for (int i = 1; i < 11; ++i) {
        std::string filename = "waitingview/round_" + tools::to_string(i) + ".png";
        sprite_animation_->addSpriteFrameWithFile(filename);
    }
    sprite_animation_->setDelayPerUnit(0.1f);
    Animate* animate = Animate::create(sprite_animation_);
    
    sprite_reconnect_->runAction(RepeatForever::create(animate));
    
}

void ReconnectView::onExit()
{
    Layer::onExit();
}