//
//  LayerBindPhone.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/6.
//
//

#ifndef LayerBindPhone_hpp
#define LayerBindPhone_hpp

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
class LayerBindPhone:public BaseLayer,public EditBoxDelegate{
public:
    static LayerBindPhone* create(const bool is_bind_phone);
    virtual bool init(const bool is_bind_phone);
    
    std::function<void(const std::string&)> set_phone_number_callback;
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
protected:
    void register_events();
#pragma mark ui
private:
    Node* root_;
    EditBox* tf_bind_phone_;
    EditBox* tf_verification_;
    
    Button* btn_verification_;
    Button* btn_bind_phone_;
    float current_send_verification_time_ = 0;
    void send_bind_phone(std::string phone_number, std::string verification);
    void send_get_verification(std::string phone_number);
    void handle_bind_phone(cmd_data_pointer data);
    void handle_send_verification(cmd_data_pointer data);
    cocos2d::Vec2 GetErrorPoint();
#pragma mark click_events
public:
    void click_btn_verification(Ref* sender);
    void click_btn_bind_phone(Ref* sender);
    void schedule_btn_send_verification(float dt);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
};
#endif /* LayerBindPhone_hpp */
