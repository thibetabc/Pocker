//
//  LayerOfcPullDown.hpp
//  pokerclient
//
//  Created by zhongyu on 16/5/19.
//
//

#ifndef LayerOfcPullDown_hpp
#define LayerOfcPullDown_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"

#include "GameLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class OFCRoom;

class LayerOfcPullDown : public OFCChildLayer {
public:
    CREATE_FUNC(LayerOfcPullDown);
    virtual bool init();

private:
    Node* root_;
    
    ImageView* image_stand_up_;
    ImageView* image_leave_;
    ImageView* image_setting_;
    
    ImageView* image_ofc_pull_bg_;
    
    Text* text_stand_up_;
    Text* text_leave_;
    Text* text_setting_;
public:
#pragma mark event;
    void click_stand_up(Ref* sender);
    void click_leave(Ref* sender);
    void click_setting(Ref* sender);
    bool onTouchBegan(Touch* touch, Event* event)
    {
        return true;
    };
    void onTouchMoved(Touch* touch, Event* event){};
    void onTouchEnded(Touch* touch, Event* event);

};

#endif /* LayerOfcPullDown_hpp */
