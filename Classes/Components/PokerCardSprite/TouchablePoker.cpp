//
//  TouchablePoker.cpp
//  pokerclient
//
//  Created by NaughyDog on 16/1/21.
//
//

#include "TouchablePoker.h"
#include "PokerRoomLayer.h"

TouchablePoker* TouchablePoker::create(const std::string& filename)
{
    TouchablePoker *pRet = new(std::nothrow) TouchablePoker();
    if (pRet && pRet->initWithFile(filename))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool TouchablePoker::initWithFile(const std::string& filename)
{
    if (!Sprite::initWithFile("card/pokerback.png")) {
        return false;
    }
    
    pokerFileName = filename;
    isSelected = false;
    isFlipped = false;
    
    icon = Sprite::create("login_res/game_res/player_card_show.png");
    icon->setPosition(getContentSize().width*0.4f,getContentSize().height*0.3f);
    icon->setVisible(false);
    addChild(icon);
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(TouchablePoker::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TouchablePoker::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TouchablePoker::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(TouchablePoker::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}
void TouchablePoker::flip()
{
    if (isFlipped) {
        return;
    }
    
    runAction(Sequence::create(
                                     OrbitCamera::create(0.2f,1, 0, 0, 90, 0, 0)
                                     ,
                                     CallFunc::create([this](){
        this->setTexture(pokerFileName);
        
    }),
                                     OrbitCamera::create(0.2f,1, 0, -90, 90, 0, 0),
                                     
                                     
                                     NULL));
    
    isFlipped = true;
    
}
void TouchablePoker::setIsSelected(bool b)
{
    isSelected = b;
    icon->setVisible(isSelected);
}
bool TouchablePoker::isTouchPointInside(Point &point)
{
    Rect rect1 = getTextureRect();
    Size s = rect1.size;
    Rect rect2 = Rect(-s.width / 2, -s.height / 2, s.width, s.height);
    
    return rect2.containsPoint(point);
}
bool TouchablePoker::onTouchBegan(Touch *touch, Event *event)
{
    Point touchPoint = touch->getLocation();
    touchPoint = convertToNodeSpaceAR(touchPoint);
    bool b = isTouchPointInside(touchPoint);
    if (b) {
        isSelected = !isSelected;
        setIsSelected(isSelected);
        
        PokerRoomLayer * prl = dynamic_cast<PokerRoomLayer *>(getParent()->getParent());
        if(prl)
        {
            prl->update_show_card();
        }
        
        return true;
    }
    return false;
}

void TouchablePoker::onTouchMoved(Touch *touch, Event *event)
{
}
void TouchablePoker::onTouchEnded(Touch *touch, Event *event)
{
}
void TouchablePoker::onTouchCancelled(Touch *touch, Event *event)
{
}