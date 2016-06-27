//
//  ConnectingView.cpp
//  pokerclient
//  网络转圈连接中
//  Created by HuangFeng on 16/5/18.
//
//

#include "ConnectingView.h"

ConnectingView::ConnectingView() : connectingPosition(Vec2(Director::getInstance()->getVisibleSize()/2))
{
    
}

ConnectingView::~ConnectingView()
{
    
}

bool ConnectingView::init()
{
    if(!Layer::init())
    {
        return false;
    }
    auto loadingBgImg = Sprite::create("common/loading_bg.png");
    loadingBgImg->setPosition(connectingPosition);
    auto rotate0 = RotateBy::create(2.0f, 360.0f);
    loadingBgImg->runAction(RepeatForever::create(rotate0));
    addChild(loadingBgImg);
    
    auto loadingImg = Sprite::create("common/loading_action.png");
    loadingImg->setPosition(connectingPosition);
    auto rotate = RotateBy::create(2.0f, 360.0f);
    loadingImg->runAction(RepeatForever::create(rotate));
    addChild(loadingImg);
    return true;
}


ConnectingView *ConnectingView::create(Vec2 position)
{
    auto layer = new(std::nothrow) ConnectingView();
    if(layer)
    {
        layer->connectingPosition = position;
        layer->init();
        layer->autorelease();
    }
    return layer;
}

