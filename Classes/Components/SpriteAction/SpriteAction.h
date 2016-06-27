//
//  SpriteAction.h
//  pokerclient
//
//  Created by admin on 15-7-15.
//
//

#ifndef __pokerclient__SpriteAction__
#define __pokerclient__SpriteAction__
#include "cocos2d.h"

typedef std::function<void ()> actionCallBack;

class SpriteAction {
    enum SPActionType{
        SCALE,
        BLINK
    };
    
public:
    

    
    static cocos2d::ActionInterval* getCardAction();
    static void runCardAction(cocos2d::Node* ptr,
                              cocos2d::Vec2 startPoint,
                              cocos2d::Vec2 endPoint,actionCallBack acb);
    
    
    static void runChipAction(cocos2d::Node* ptr,
                              cocos2d::Vec2 startPoint,
                              cocos2d::Vec2 endPoint,
                              uint32_t num,actionCallBack acb);
    
private:
    static cocos2d::ActionInterval* getScaleAction();
    static cocos2d::ActionInterval* getBlinkAction();
    
};

#endif /* defined(__pokerclient__SpriteAction__) */
