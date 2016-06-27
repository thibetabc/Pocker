#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "MatchScroeNode.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class MatchScroeCell : public Widget {
public:
    CREATE_FUNC(MatchScroeCell);
    
    virtual bool init();
    
    void update_data(const std::shared_ptr<MatchScroeNode::MATCH_OBJECT>& obj);

private:
    Text* text_nickname_;        //昵称
    Text* text_buy_chip_amount_; //买入数量
    Text* text_win_chip_amount_; //盈利数量  +  得分
    Text* text_play_time;      
};