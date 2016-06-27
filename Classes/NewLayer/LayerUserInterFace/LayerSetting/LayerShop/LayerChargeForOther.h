//
//  LayerChargeForOther.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/27.
//
//

#ifndef LayerChargeForOther_hpp
#define LayerChargeForOther_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerChargeForOther : public BaseLayer {
public:
    static LayerChargeForOther* create(bool is_charge_other);
    virtual bool init(bool is_charge_other);
    
private:
    Node* root_;
    Button* btn_charge_self_;
    Button* btn_charge_other_;
    Button* btn_cancel_;
    std::function<void(int)> callback_;
    ImageView* panel_charge_bg_;
    
public:
    void click_charge_self(Ref* sender);
    void click_charge_other(Ref* sender);
    void click_cancle(Ref* sender);
    void set_callback(std::function<void(int)> callback){ callback_ = callback; };
    void handle_get_friend_id(std::vector<msg::UserDetailInfo> userDatailInfo);
    
#pragma mark touch
    bool onTouchBegan(Touch* touch, Event* event){ return true; };
    void onTouchMoved(Touch* touch, Event* event){};
    void onTouchEnded(Touch* touch, Event* event);
    
};

#endif /* LayerChargeForOther_hpp */
