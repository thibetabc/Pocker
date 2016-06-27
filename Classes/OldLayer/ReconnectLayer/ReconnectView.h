//
//  ReconnectView.h
//  pokerclient
//
//  Created by ronaldz on 15/12/25.
//
//

#ifndef __pokerclient__ReconnectView__
#define __pokerclient__ReconnectView__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"
#include "MultiEditBox.h"
#include "utils.h"

USING_NS_CC;
using namespace cocos2d::ui;

class ReconnectView:public Layer{
public:
    
    CREATE_FUNC(ReconnectView);
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
private:
    cocos2d::Sprite* sprite_reconnect_;
    cocos2d::Animation* sprite_animation_;
    
    cocos2d::ui::Text* text_;

};

#endif /* defined(__pokerclient__ReconnectView__) */
