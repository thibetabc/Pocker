#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class InputLayer : public Layer
{
public:
    
    CREATE_FUNC(InputLayer);
    virtual bool init();
    
    
    void click_btn_confirm(Ref* sender);
    void click_btn_cancel(Ref* sender);
    
    void set_btn_confirm_callback(const std::function<void(std::string&)>& callback) { callback_ = callback;}
    void set_btn_cancel_callback(const std::function<void()>& callback) { close_callback_ = callback;}
private:
    EditBox* tf_input_;
    std::function<void()> close_callback_;
    std::function<void(std::string&)> callback_;
};