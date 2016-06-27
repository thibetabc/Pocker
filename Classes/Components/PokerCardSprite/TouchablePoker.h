//
//  TouchablePoker.hpp
//  pokerclient
//
//  Created by NaughyDog on 16/1/21.
//
//

#ifndef TouchablePoker_hpp
#define TouchablePoker_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;


class TouchablePoker : public Sprite
{
    Sprite * icon;
    bool isSelected;
    bool isFlipped;
    std::string pokerFileName;
    bool isTouchPointInside(cocos2d::Point &point);
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
public:
    static TouchablePoker* create(const std::string& filename);
    bool initWithFile(const std::string& filename);
    void setIsSelected(bool b);
    bool getIsSelected(){return isSelected;}
    void flip();
};
#endif /* TouchablePoker_hpp */
