//
//  PokerCardSprite.h
//  pokerclient
//
//  Created by ll on 15/9/25.
//
//

#ifndef __pokerclient__PokerCardSprite__
#define __pokerclient__PokerCardSprite__

#define PCS PokerCardSprite::getInstance()

#include "cocos2d.h"

USING_NS_CC;

class PokerCardSprite
{
public:
    static PokerCardSprite* getInstance() {
        if(!__PokerCardSprite__) {
            __PokerCardSprite__ = new PokerCardSprite();
        }
        return __PokerCardSprite__;
    }
    
    static void destroy() {
        delete __PokerCardSprite__;
        __PokerCardSprite__ = nullptr;
    }
    
    static Sprite * createCardSprite(const uint32_t & CardID);
    static void setCardSprite(const uint32_t & CardID,Sprite* ResultCard);
private:
    static PokerCardSprite* __PokerCardSprite__;
    PokerCardSprite();
    PokerCardSprite(const PokerCardSprite&);
    PokerCardSprite operator=(const PokerCardSprite&);

};





#endif /* defined(__pokerclient__PokerCardSprite__) */
