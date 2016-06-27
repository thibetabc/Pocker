//
//  LayerQuickAccess.hpp
//  pokerclient
//
//  Created by zhongyu on 16/3/30.
//
//

#ifndef LayerQuickAccess_hpp
#define LayerQuickAccess_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerQuickAccess : public BaseLayer
{
public:
    CREATE_FUNC(LayerQuickAccess);
    virtual bool init();
    
#pragma mark ui
private:
    Text* text_title_;
    Button* btn_back_;
    Text* text_roomtip_;
    EditBox* tf_room_input_;
    Button* btn_enter_room_;
    Button* btn_create_room_;
    Node* root_;
    
#pragma mark button_click_events
public:
    void click_btn_enter_room(Ref* sender);
    void click_btn_create_room(Ref* sender);
    void click_btn_back(Ref* sender);
    
};
#endif /* LayerQuickAccess_hpp */
