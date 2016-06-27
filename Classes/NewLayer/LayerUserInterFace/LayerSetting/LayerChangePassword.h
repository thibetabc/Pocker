//
//  LayerChangePassword.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/6.
//
//

#ifndef LayerChangePassword_hpp
#define LayerChangePassword_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "events.h"
#include "msg.pb.h"
#include "cmd_def.h"
using namespace cocos2d;
using namespace cocos2d::ui;
class LayerChangePassword:public BaseLayer
{
public:
    CREATE_FUNC(LayerChangePassword);
    virtual bool init();
protected:
    void register_events();
#pragma mark ui
private:
    Node* root_;
    Button* btn_confirm_;
    
    EditBox* tf_password_;
    EditBox* tf_password_confirm_;
    void send_password(const std::string& password);
    void handle_change_user_setting(cmd_data_pointer data);
    cocos2d::Vec2 GetErrorPoint();
#pragma mark click event
public:
    void click_btn_confirm(Ref* sender);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
};


#endif /* LayerChangePassword_hpp */
