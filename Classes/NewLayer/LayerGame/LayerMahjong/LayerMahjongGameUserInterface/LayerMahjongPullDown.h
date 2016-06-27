//
//  LayerMahjongPullDown.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/13.
//
//

#ifndef LayerMahjongPullDown_hpp
#define LayerMahjongPullDown_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"

#include "GameLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class MahjongRoom;

class LayerMahjongPullDown : public MahjongChildLayer {
public:
    static LayerMahjongPullDown* create(bool is_ower);
    virtual bool init(bool is_ower);
    
private:
    Node* root_;
    
    ImageView* image_stand_up_;
    ImageView* image_leave_;
    ImageView* image_setting_;
    ImageView* image_pause_;
    
    ImageView* image_ofc_pull_bg_;
    
    Text* text_stand_up_;
    Text* text_leave_;
    Text* text_setting_;
    Text* text_pause_;
public:
#pragma mark event;
    void click_stand_up(Ref* sender);
    void click_leave(Ref* sender);
    void click_setting(Ref* sender);
    void click_pause(Ref* sender);
    bool onTouchBegan(Touch* touch, Event* event)
    {
        return true;
    };
    void onTouchMoved(Touch* touch, Event* event){};
    void onTouchEnded(Touch* touch, Event* event);
    
};

#endif /* LayerMahjongPullDown_hpp */
