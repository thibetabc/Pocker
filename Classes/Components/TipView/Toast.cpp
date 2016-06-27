//
//  Toast.cpp
//  pokerclient
//
//  Created by HuangFeng on 16/5/13.
//
//

#include "Toast.h"
using namespace ui;

#define TOP_ZODER 99999

void Toast::ToastShow(std::string txt, float time, float rotation)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto text = Text::create(txt, "", 24);
    text->setColor(Color3B::WHITE);
    auto layout = Layout::create();
    layout->ignoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layout->setBackGroundImage("common/toast_frame.9.png");
    layout->setBackGroundImageScale9Enabled(true);
    layout->setBackGroundImageCapInsets(Rect(15, 15, 12, 12));
    const float MIN_WIDTH = 300.0f;
    Size textSize = text->getContentSize();
    if (textSize.width < MIN_WIDTH)
    {
        layout->setContentSize(Size(MIN_WIDTH, textSize.height + 40));
    }
    else
    {
        layout->setContentSize(Size(text->getContentSize().width + 60, textSize.height + 40));
    }
    text->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
    layout->addChild(text);
    EaseSineInOut *moveTo1 = nullptr;
    MoveTo *moveTo2 = nullptr;
    if (rotation > 89.9f && rotation < 90.1f)
    {
        layout->setRotation(rotation);
        layout->setPosition(Vec2(0, visibleSize.height / 2));
        moveTo1 = EaseSineInOut::create(MoveTo::create(0.2f, Vec2(visibleSize.width / 6.0f, visibleSize.height / 2.0f)));
        moveTo2 = MoveTo::create(0.1f, Vec2(visibleSize.width / 6.1f, visibleSize.height / 2.0f));
    }
    else if (rotation < -89.9f && rotation > -90.1f)
    {
        layout->setRotation(rotation);
        layout->setPosition(Vec2(visibleSize.width, visibleSize.height / 2.0f));
        moveTo1 = EaseSineInOut::create(MoveTo::create(0.2f, Vec2(visibleSize.width / 1.6f, visibleSize.height / 2.0f)));
        moveTo2 = MoveTo::create(0.1f, Vec2(visibleSize.width / 1.5f, visibleSize.height / 2.0f));
    }
    else
    {
        layout->setPosition(Vec2(visibleSize.width / 2, 0));
        moveTo1 = EaseSineInOut::create(MoveTo::create(0.2f, Vec2(visibleSize.width / 2, visibleSize.height / 6.0f)));
        moveTo2 = MoveTo::create(0.1f, Vec2(visibleSize.width / 2, visibleSize.height / 6.1f));
    }
    auto delay = DelayTime::create(time);
    auto fadeout = FadeOut::create(0.2f);
    auto callback = CallFunc::create([=](){layout->removeFromParent(); });
    layout->runAction(Sequence::create(moveTo1, moveTo2, delay, fadeout, callback, NULL));
    
    
    auto scene = Director::getInstance()->getRunningScene();
    scene->addChild(layout, TOP_ZODER);
    
}
