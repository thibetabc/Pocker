//
//  MahjongCommon.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/6/16.
//
//

#ifndef MahjongCommon_hpp
#define MahjongCommon_hpp

#include <stdio.h>
#include "global_header.h"

using namespace cocos2d;
using namespace cocos2d::ui;
//牌背景
enum CLASS_CARD
{
    //东
    EAST_DI, //底牌
    EAST_HAND, //手牌
    EAST_HAND_PENG,//手牌-碰-位置
    EAST_HAND_PENG_POSITION,//手牌-碰-位置
    EAST_HAND_AN_GANG, //手牌-暗杠
    EAST_OUT, //打出去的牌
    EAST_DOUBLE_OUT, //打出去的牌
    
    //南
    SOUTH_DI, //底牌
    SOUTH_HAND, //手牌
    SOUTH_INVISIBLE_HAND, //手牌
    SOUTH_HAND_PENG,//手牌-碰-位置
    SOUTH_HAND_PENG_POSITION,//手牌-碰-位置
    SOUTH_HAND_AN_GANG, //手牌-暗杠
    SOUTH_OUT, //打出去的牌
    SOUTH_DOUBLE_OUT, //打出去的牌
    
    //西
    WEST_DI, //底牌
    WEST_HAND, //手牌
    WEST_HAND_PENG,//手牌-碰-位置
    WEST_HAND_PENG_POSITION,//手牌-碰-位置
    WEST_HAND_AN_GANG, //手牌-暗杠
    WEST_OUT, //打出去的牌
    WEST_DOUBLE_OUT, //打出去的牌
    
    //北-自己
    NORTH_DI, //底牌
    NORTH_HAND, //手牌
    NORTH_HAND_PENG,//手牌-碰-位置
    NORTH_HAND_PENG_POSITION,//手牌-碰-位置
    NORTH_HAND_AN_GANG, //手牌-暗杠
    NORTH_OUT, //打出去的牌
    NORTH_DOUBLE_OUT, //打出去的牌
};

class MahjongCard: public Button{
public:
    CREATE_FUNC(MahjongCard);
    
    static MahjongCard* create(const std::string& normalImage,
                          const std::string& selectedImage = "",
                          const std::string& disableImage = "",
                          TextureResType texType = TextureResType::LOCAL);
    void SetSelected(bool value);
    bool GetSelected() {return selected_;}
    void SetEnabled(bool value);
    void set_ting( bool ting ){ is_ting_ = ting ;}
    bool get_ting(){return is_ting_;}
    msg::MahjongCard GetCardNumber() {return cardnumber_;}
    void SetCardNumber(msg::MahjongCard value) {cardnumber_ = value;}
    void SetHasMark(bool value);
private:
    bool selected_ = false;
    bool has_mark = false;
    bool is_ting_ = false;
    msg::MahjongCard cardnumber_;
};

MahjongCard* createCard(CLASS_CARD dir, msg::MahjongCard cardIndex); //底牌类型
std::string getCardName(ssize_t cardIdx);//麻将名字
msg::MahjongSuit GetSuit(msg::MahjongCard card);
std::string get_wintype(msg::MahjongScoreChange score_change);

const int VERT_OUT_WIDTH = 32;
const int VERT_OUT_HEIGHT = 40;
const int HORZ_OUT_WIDTH = 43;
const int HORZ_OUT_HEIGHT = 26;

const int VERT_DOUBLE_OUT_WIDTH = 34;
const int VERT_DOUBLE_OUT_HEIGHT = 47;
const int HORZ_DOUBLE_OUT_WIDTH = 43;
const int HORZ_DOUBLE_OUT_HEIGHT = 34;

const int HAND_NUMBER_WIDTH = 40;
const int HAND_NUMBER_HEIGHT = 55;
const int OUT_NUMBER_WIDTH = 24;
const int OUT_NUMBER_HEIGHT = 33;
const int HAND_NUMBER_WIDTH_OFFSET = 6;
const int HAND_NUMBER_HEIGHT_OFFSET = 19;
const int VERT_OUT_NUMBER_WIDTH_OFFSET = 4;
const int VERT_OUT_NUMBER_HEIGHT_OFFSET = 5;
const int HORZ_OUT_NUMBER_WIDTH_OFFSET = 3;
const int HORZ_OUT_NUMBER_HEIGHT_OFFSET = 0;
const int VERT_DOUBLE_OUT_NUMBER_WIDTH_OFFSET = 4;
const int VERT_DOUBLE_OUT_NUMBER_HEIGHT_OFFSET = 4;
const int HORZ_DOUBLE_OUT_NUMBER_WIDTH_OFFSET = 4;
const int HORZ_DOUBLE_OUT_NUMBER_HEIGHT_OFFSET = 0;

const int SOUTH_HAND_WIDTH = 53;
const int SOUTH_HAND_HEIGHT = 81;
const int EAST_HAND_WIDTH = 29;
const int EAST_HAND_HEIGHT = 59;
const int WEST_HAND_WIDTH = 29;
const int WEST_HAND_HEIGHT = 59;
const int NORTH_HAND_WIDTH = 46;
const int NORTH_HAND_HEIGHT = 70;

const int CHANGE_SELECT_CARD_POSITION_Y = 20;
const int CARD_HAND_SPACE = 5; //手牌和刚抓的牌间隔
//子副牌数量
const int MAX_FU_SHUN = 3; //顺子
const int MAX_FU_PENG = 3; //碰
const int MAX_FU_GANG = 4; //杠
const int MAX_FU_SPACE = 16; //副牌间隔
//无效position
const int INVAID_TAG_POSITION = -1;

#endif /* MahjongCommon_hpp */
