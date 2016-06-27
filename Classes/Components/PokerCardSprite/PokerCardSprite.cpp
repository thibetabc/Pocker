//
//  PokerCardSprite.cpp
//  pokerclient
//
//  Created by ll on 15/9/25.
//
//

#include "PokerCardSprite.h"
#include "GameDataManager.h"
#include "utils.h"


PokerCardSprite* PokerCardSprite::__PokerCardSprite__ = nullptr;

PokerCardSprite::PokerCardSprite()
{
    
}

Sprite* PokerCardSprite::createCardSprite(const uint32_t & CardID)
{
    if (CardID < 53)
    {
        if (CardID != 0)
        {
            Sprite * ResultCard = Sprite::create("card/poker_card_front.png");
            Point DesignPosition = Vec2(ResultCard->getContentSize().width/2, ResultCard->getContentSize().height/3 * 1);
            Point NumberPosition = Vec2(ResultCard->getContentSize().width/2, ResultCard->getContentSize().height/4 * 3);
            
            uint32_t CardSuit = 0;
            int nCardNum = 0;
            nCardNum = (CardID + 1) % 13;
            if (nCardNum < 2)
            {
                nCardNum += 13;
            }
            CardSuit = CardID/13;
            if (CardID % 13 > 0)
            {
                CardSuit ++;
            }
            std::string pStrDesign = "";
            std::string resultstr = "";
            switch (CardSuit)
            {
                case 1:
                {
                    if(GDM->get_is_newcard()){
                        pStrDesign = "card/new_poker/mei.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "-2.png";
                    }
                    else{
                        pStrDesign ="card/new_poker/mei_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "-1_10.png";
                    }
                }
                    break;
                    
                case 2:
                {
                    if(GDM->get_is_newcard()){
                        pStrDesign = "card/new_poker/fang.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "-3.png";
                    }
                    else{
                        pStrDesign ="card/new_poker/fang_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "_10.png";
                    }

                }
                    break;
                case 3:
                {
                    if(GDM->get_is_newcard()){
                        pStrDesign = "card/new_poker/hong_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "_10.png";
                    }
                    else{
                        pStrDesign ="card/new_poker/hong_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "_10.png";
                    }

                }
                    break;
                case 4:
                {
                    if(GDM->get_is_newcard()){
                        pStrDesign = "card/new_poker/hei_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "-1_10.png";
                    }
                    else{
                        pStrDesign ="card/new_poker/hei_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "-1_10.png";
                    }

                }
                    break;
                default:
                    break;
            }
            Sprite * design = Sprite::create(pStrDesign.c_str());
            design->setPosition(DesignPosition);
            ResultCard->addChild(design);
            Sprite * pSpriteNum = Sprite::create(resultstr);
            pSpriteNum->setPosition(NumberPosition);
            ResultCard->addChild(pSpriteNum);
            return ResultCard;
            
        }
        else
        {
            Sprite * ResultCard = Sprite::create("card/poker_card_black.png");
            return ResultCard;
        }
        
        
    }
    
    cocos2d::log("error : PokerCardSprite CardID is %u", CardID);
    return nullptr;
}
void PokerCardSprite::setCardSprite(const uint32_t & CardID,Sprite* ResultCard)
{
    if (CardID < 53)
    {
        if (CardID != 0)
        {
            ResultCard->setTexture("card/poker_card_front.png");
            Point DesignPosition = Vec2(ResultCard->getContentSize().width/2, ResultCard->getContentSize().height/3 * 1);
            Point NumberPosition = Vec2(ResultCard->getContentSize().width/2, ResultCard->getContentSize().height/4 * 3);
            
            uint32_t CardSuit = 0;
            int nCardNum = 0;
            nCardNum = (CardID + 1) % 13;
            if (nCardNum < 2)
            {
                nCardNum += 13;
            }
            CardSuit = CardID/13;
            if (CardID % 13 > 0)
            {
                CardSuit ++;
            }
            std::string pStrDesign = "";
            std::string resultstr = "";
            switch (CardSuit)
            {
                case 1:
                {
                    if(GDM->get_is_newcard()){
                        pStrDesign = "card/new_poker/mei.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "-2.png";
                    }
                    else{
                        pStrDesign ="card/new_poker/mei_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "-1_10.png";
                    }
                }
                    break;
                    
                case 2:
                {
                    if(GDM->get_is_newcard()){
                        pStrDesign = "card/new_poker/fang.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "-3.png";
                    }
                    else{
                        pStrDesign ="card/new_poker/fang_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "_10.png";
                    }
                    
                }
                    break;
                case 3:
                {
                    if(GDM->get_is_newcard()){
                        pStrDesign = "card/new_poker/hong_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "_10.png";
                    }
                    else{
                        pStrDesign ="card/new_poker/hong_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "_10.png";
                    }
                    
                }
                    break;
                case 4:
                {
                    if(GDM->get_is_newcard()){
                        pStrDesign = "card/new_poker/hei_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "-1_10.png";
                    }
                    else{
                        pStrDesign ="card/new_poker/hei_27.png";
                        resultstr = "card/new_poker/" + tools::to_string(nCardNum+42) + "-1_10.png";
                    }
                    
                }
                    break;
                default:
                    break;
            }
            Sprite * design = Sprite::create(pStrDesign.c_str());
            design->setPosition(DesignPosition);
            ResultCard->addChild(design);
            Sprite * pSpriteNum = Sprite::create(resultstr);
            pSpriteNum->setPosition(NumberPosition);
            ResultCard->addChild(pSpriteNum);
            
        }
        else
        {
            ResultCard->removeAllChildren();
            ResultCard->setTexture("card/poker_card_black.png");
        }
        
        
    }
    
}

