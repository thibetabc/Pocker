#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"


using namespace cocos2d;
using namespace cocos2d::ui;

class ChipNode : public Node {
public:
    CREATE_FUNC(ChipNode);
    virtual bool init();
    
    void update_data(const int amount, const bool pool = false);
    void setHorizontal();
    int _amount;
    
    Sprite* sprite_chip_;
    ImageView *bg_;
    Text* text_amount_;
};