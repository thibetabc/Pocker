//
//  LayerWelcome.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/6.
//
//

#include "LayerWelcome.h"

bool LayerWelcome::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    ImageView* imageWelcomeBg = ImageView::create("Default/image_welcome_bg.png");
    
    cocos2d::Size sizeWin = Director::getInstance()->getWinSize();
    
    imageWelcomeBg->setContentSize(sizeWin);
    
    imageWelcomeBg->setPosition(cocos2d::Point(sizeWin.width * imageWelcomeBg->getAnchorPoint().x, sizeWin.height * imageWelcomeBg->getAnchorPoint().y));
    
    this->addChild(imageWelcomeBg);
    
    return true;
}