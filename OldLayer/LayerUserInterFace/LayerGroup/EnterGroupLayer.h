//  群列表？

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class EnterGroupLayer : public LayerPop
{
public:
    
    CREATE_FUNC(EnterGroupLayer);
    virtual bool init();
    
    void click_btn_confirm(Ref* sender);
    void click_btn_cancel(Ref* sender);
    void set_dismiss_callback(const std::function<void()>& callback) { callback_ = callback;}
    void onExit();
private:
    Text* text_group_name_edit_;
    EditBox* tf_password_;
    msg::GroupInfo* group_data_;
    std::function<void()> callback_;
};