//
//  MahjongCommon.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/6/16.
//
//

#include "MahjongCommon.h"
#include "NodeCommon.h"
#include "utils.h"

std::string getCardName(ssize_t cardIdx)
{
    return StringUtils::format("%ld.png",cardIdx);
}

MahjongCard* createCard(CLASS_CARD dir, msg::MahjongCard cardIndex){
    std::string buttonBg = "mj_02.png";
    bool isAddCard = true;
    float buttonScale = 1.0f;
    float cardScale = 1.0f;
    float cardRotation = 0.0f;
    float buttonRotation = 0.0f;
    float cardPositionX = 0;
    float cardPositionY = 0;
    switch(dir)
    {
            //西
        case CLASS_CARD::WEST_DI://底牌
            isAddCard = false;
            buttonBg = "mj_19.png";
            break;
        case CLASS_CARD::WEST_HAND: //手牌
            isAddCard = false;
            buttonBg = "mj_15.png";
            break;
            
            //北
        case CLASS_CARD::NORTH_DI://底牌
            isAddCard = false;
            buttonBg = "mj_06.png";
            break;
        case CLASS_CARD::NORTH_HAND: //手牌
            isAddCard = false;
            buttonBg = "mj_03.png";
            break;
            
            //东
        case CLASS_CARD::EAST_DI: //底牌
            isAddCard = false;
            buttonBg = "mj_19.png";
            break;
        case CLASS_CARD::EAST_HAND: //手牌
            isAddCard = false;
            buttonBg = "mj_20.png";
            break;
            
            //南-自己
        case CLASS_CARD::SOUTH_DI: //底牌
            isAddCard = false;
            buttonBg = "mj_06.png";
            break;
        case CLASS_CARD::SOUTH_HAND: //手牌
            buttonBg = "mj_02.png";
            cardScale = 0.75f;
            cardPositionX = HAND_NUMBER_WIDTH_OFFSET + HAND_NUMBER_WIDTH / 2;
            cardPositionY = HAND_NUMBER_HEIGHT_OFFSET + HAND_NUMBER_HEIGHT / 2;
            break;
        case CLASS_CARD::SOUTH_INVISIBLE_HAND:
            isAddCard = false;
            buttonBg = "mj-da.png";
            break;
        case CLASS_CARD::WEST_HAND_AN_GANG: //手牌-暗杠
            isAddCard = false;
            buttonBg = "mj_18.png";
            break;
        case CLASS_CARD::NORTH_HAND_AN_GANG: //手牌-暗杠
            isAddCard = false;
            buttonBg = "mj-xiao.png";
            break;
        case CLASS_CARD::EAST_HAND_AN_GANG: //手牌-暗杠
            isAddCard = false;
            buttonBg = "mj_18.png";
            break;
        case CLASS_CARD::SOUTH_HAND_AN_GANG: //手牌-暗杠
            isAddCard = false;
            buttonBg = "mj-xiao.png";
            break;
            //碰-杠
            
        case CLASS_CARD::SOUTH_OUT: //打出去的牌
        case CLASS_CARD::SOUTH_HAND_PENG:
        case CLASS_CARD::EAST_HAND_PENG_POSITION://手牌-碰-位置
            buttonBg = "mj_10.png";
            cardScale = 0.45f;
            cardPositionX = VERT_OUT_NUMBER_WIDTH_OFFSET + OUT_NUMBER_WIDTH / 2;
            cardPositionY = VERT_OUT_NUMBER_HEIGHT_OFFSET + OUT_NUMBER_HEIGHT / 2;
            break;
            
        case CLASS_CARD::WEST_OUT: //打出去的牌
        case CLASS_CARD::WEST_HAND_PENG:
        case CLASS_CARD::SOUTH_HAND_PENG_POSITION://手牌-碰-位置
            buttonBg = "mj_16.png";
            cardScale = 0.45f;
            cardRotation = 90.0f;
            cardPositionX = HORZ_OUT_NUMBER_WIDTH_OFFSET + OUT_NUMBER_HEIGHT / 2;
            cardPositionY = HORZ_OUT_NUMBER_HEIGHT_OFFSET + OUT_NUMBER_WIDTH / 2;
            break;
            
        case CLASS_CARD::NORTH_OUT: //打出去的牌
        case CLASS_CARD::NORTH_HAND_PENG:
        case CLASS_CARD::WEST_HAND_PENG_POSITION://手牌-碰-位置
            buttonBg = "mj_10.png";
            cardScale = 0.45f;
            cardPositionX = VERT_OUT_NUMBER_WIDTH_OFFSET + OUT_NUMBER_WIDTH / 2;
            cardPositionY = VERT_OUT_NUMBER_HEIGHT_OFFSET + OUT_NUMBER_HEIGHT / 2;
            cardRotation = 180.0f;
            break;
            
        case CLASS_CARD::EAST_OUT: //打出去的牌
        case CLASS_CARD::EAST_HAND_PENG:
        case CLASS_CARD::NORTH_HAND_PENG_POSITION://手牌-碰-位置
            buttonBg = "mj_16.png";
            cardScale = 0.45f;
            cardRotation = -90.0f;
            cardPositionX = HORZ_OUT_NUMBER_WIDTH_OFFSET + OUT_NUMBER_HEIGHT / 2;
            cardPositionY = HORZ_OUT_NUMBER_HEIGHT_OFFSET + OUT_NUMBER_WIDTH / 2;
            break;
            
        case CLASS_CARD::SOUTH_DOUBLE_OUT: //打出去的牌
            buttonBg = "mj_07.png";
            cardScale = 0.45f;
            cardPositionX = VERT_DOUBLE_OUT_NUMBER_WIDTH_OFFSET + OUT_NUMBER_WIDTH / 2;
            cardPositionY = VERT_DOUBLE_OUT_NUMBER_HEIGHT_OFFSET + OUT_NUMBER_HEIGHT / 2;
            break;
            
        case CLASS_CARD::WEST_DOUBLE_OUT: //打出去的牌
            buttonBg = "mj_17.png";
            cardScale = 0.45f;
            cardRotation = 90.0f;
            cardPositionX = HORZ_DOUBLE_OUT_NUMBER_WIDTH_OFFSET + OUT_NUMBER_HEIGHT / 2;
            cardPositionY = HORZ_DOUBLE_OUT_NUMBER_HEIGHT_OFFSET + OUT_NUMBER_WIDTH / 2;
            break;
            
        case CLASS_CARD::NORTH_DOUBLE_OUT: //打出去的牌
            buttonBg = "mj_07.png";
            cardScale = 0.45f;
            cardPositionX = VERT_DOUBLE_OUT_NUMBER_WIDTH_OFFSET + OUT_NUMBER_WIDTH / 2;
            cardPositionY = VERT_DOUBLE_OUT_NUMBER_HEIGHT_OFFSET + OUT_NUMBER_HEIGHT / 2;
            cardRotation = 180.0f;
            break;
            
        case CLASS_CARD::EAST_DOUBLE_OUT: //打出去的牌
            buttonBg = "mj_17.png";
            cardScale = 0.45f;
            cardRotation = -90.0f;
            cardPositionX = HORZ_DOUBLE_OUT_NUMBER_WIDTH_OFFSET + OUT_NUMBER_HEIGHT / 2;
            cardPositionY = HORZ_DOUBLE_OUT_NUMBER_HEIGHT_OFFSET + OUT_NUMBER_WIDTH / 2;
            break;
        default:
            break;
            
    }
    //buttonBg
    auto button = MahjongCard::create(buttonBg, buttonBg, buttonBg);
    //card
    if(isAddCard)
    {
        std::string cardName = getCardName(cardIndex);
        auto spriteCard = Sprite::create(cardName);
        button->addChild(spriteCard);
        AlignNode(spriteCard, TOPLEFT, cardPositionX, cardPositionY, MIDDLE);
        spriteCard->setScale(cardScale);
        spriteCard->setRotation(cardRotation);
    }
    
    button->setScale(buttonScale);
    button->setRotation(buttonRotation);
    button->SetCardNumber(cardIndex);
    return button;
}

void MahjongCard::SetEnabled(bool value)
{
    setColor(value ? Color3B::WHITE : Color3B::GRAY);
}

void MahjongCard::SetSelected(bool value)
{
    if (selected_ != value){
        selected_ = value;
        if(selected_)
            setPositionY(getPositionY() + CHANGE_SELECT_CARD_POSITION_Y);
        else
            setPositionY(getPositionY() - CHANGE_SELECT_CARD_POSITION_Y);
    }
}

void MahjongCard::SetHasMark(bool value)
{
    if (has_mark != value){
        has_mark = value;
        if(has_mark)
        {
            auto sprite = getChildByName("last_card_action");
            if(!sprite)
            {
                Sprite* sprite = Sprite::create("image_coordinates.png");
                auto moveBy = MoveBy::create(0.8f, Vec2(0, 20));
                auto moveBack = moveBy->reverse();
                auto action = RepeatForever::create(Sequence::createWithTwoActions(moveBy, moveBack));
                sprite->runAction(action);
                sprite->setOpacity(125);
                sprite->setName("last_card_action");
                addChild(sprite);
                AlignNode(sprite, MIDDLE, 0, getContentSize().height / 2 + 8);
            }
        }
        else
        {
            auto sprite = getChildByName("last_card_action");
            if(sprite)
            {
                sprite->removeFromParent();
            }
        }
    }
}

MahjongCard* MahjongCard::create(const std::string &normalImage,
                                 const std::string& selectedImage ,
                                 const std::string& disableImage,
                                 TextureResType texType)
{
    MahjongCard *btn = new (std::nothrow) MahjongCard;
    if (btn && btn->init(normalImage,selectedImage,disableImage,texType))
    {
        btn->autorelease();
        return btn;
    }
    CC_SAFE_DELETE(btn);
    return nullptr;
}

msg::MahjongSuit GetSuit(msg::MahjongCard card)
{
    if (card >= msg::MahjongCard::TIAO1 && card <= msg::MahjongCard::TIAO9)
        return msg::MahjongSuit::TIAO;
    else if (card >= msg::MahjongCard::BING1 && card <= msg::MahjongCard::BING9)
        return msg::MahjongSuit::BING;
    else if (card >= msg::MahjongCard::WAN1 && card <= msg::MahjongCard::WAN9)
        return msg::MahjongSuit::WAN;
    else if (card >= msg::MahjongCard::EAST && card <= msg::MahjongCard::BAI)
        return msg::MahjongSuit::FENG;
    else
        return msg::MahjongSuit::OTHER;
}

std::string get_wintype(msg::MahjongScoreChange score_change)
{
    std::unordered_map<int,int> num_map;
    int num = 0;
    for (int i = 0; i < score_change.type_size() ;i++)
    {
        num = 1;
        for (int j = i + 1 ; j <score_change.type_size(); j++ )
        {
            if ( score_change.type(i) == score_change.type(j) )
            {
                num = num + 1;
            }
        }
        auto data = num_map.find(score_change.type(i));
        if ( data == num_map.end() )
        {
            num_map.insert(std::make_pair(score_change.type(i), num));
        }
    }
    
    std::string win_type = "";
    std::unordered_map<int,int> num_map_1;
    for (int i = 0;i < score_change.type_size() ; i++)
    {
        if ( num_map_1.find(score_change.type(i)) == num_map_1.end() )
        {
            auto data = num_map.find(score_change.type(i));
            if (win_type != "")
                win_type += ",";
            if ( data->second == 1 )
            {
                win_type = win_type + tools::local_string("mahjong_win_type" + tools::to_string(score_change.type(i) - 1));
            }
            else
            {
                win_type = win_type + tools::to_string(data->second) + tools::local_string("mahjong_win_type" + tools::to_string(score_change.type(i) - 1));
            }
            num_map_1.insert(std::make_pair(score_change.type(i), i));
        }
        
    }
    return win_type;
}