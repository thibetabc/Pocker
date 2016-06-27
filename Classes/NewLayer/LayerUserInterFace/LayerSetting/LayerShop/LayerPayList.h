//
//  LayerPayList.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/26.
//
//

#ifndef LayerPayList_hpp
#define LayerPayList_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerPayList : public BaseLayer {
public:
    CREATE_FUNC(LayerPayList);
    virtual bool init();
    
private:
    
#pragma amrk ui
    Node* root_;
    Text* text_title_;
    ListView* listview_pay_;
    ImageView* image_pay_list_bg_;
    
public:
    void update_data();
#pragma mark touch
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
};

#endif /* LayerPayList_hpp */
