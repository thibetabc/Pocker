//
//  SpriteAction.cpp
//  pokerclient
//
//  Created by admin on 15-7-15.
//
//

#include "SpriteAction.h"
#include "GameTools.h"
USING_NS_CC;


cocos2d::ActionInterval* SpriteAction::getCardAction(){
    return getBlinkAction();
}


void SpriteAction::runCardAction(cocos2d::Node* ptr,
                          cocos2d::Vec2 startPoint,
                          cocos2d::Vec2 endPoint,actionCallBack acb){
    
    
    Sprite* card = Sprite::create("res/card/pokerback.png");
    card->setPosition(startPoint);
    MoveTo* move = MoveTo::create(0.2, endPoint);
    CallFunc* calf =  CallFunc::create([ = ](){
        card -> removeFromParent();
        acb();
    });
    Sequence* seq = Sequence::create(move, calf, NULL);
    ptr -> addChild(card);
    card -> runAction(seq);
    
 
    
}


void SpriteAction::runChipAction(cocos2d::Node* ptr,
                                 cocos2d::Vec2 startPoint,
                                 cocos2d::Vec2 endPoint,
                                 uint32_t amount,actionCallBack acb){

    Sprite* chips = Sprite::create(GameTools::get_chip_image_name(amount));
    chips->setPosition(startPoint);
    MoveTo* move = MoveTo::create(0.2, endPoint);
    CallFunc* calf =  CallFunc::create([ = ](){
        chips -> removeFromParent();
        acb();
    });
    Sequence* seq = Sequence::create(move, calf, NULL);
    ptr -> addChild(chips);
    chips -> runAction(seq);
}



cocos2d::ActionInterval* SpriteAction::getScaleAction(){
    
    ScaleTo* scaletob = ScaleTo::create(0.2, 1.5);
    ScaleTo* scaletos = ScaleTo::create(0.2, 1);
    Sequence* sequence = Sequence::create(scaletob, scaletos, NULL);
    
    return sequence;
}


cocos2d::ActionInterval* SpriteAction::getBlinkAction(){
    Blink* blink = Blink::create(0.9, 3);
    return blink;
}





