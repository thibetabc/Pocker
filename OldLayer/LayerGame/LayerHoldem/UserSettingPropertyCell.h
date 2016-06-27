#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class UserSettingPropertyCell : public Widget {
public:
    CREATE_FUNC(UserSettingPropertyCell);
    
    virtual bool init();
    
    void update_data(const std::vector<std::shared_ptr<msg::Property> >& properties);
    
    void click_checkbox(Ref* sender, CheckBox::EventType);
    
private:
    std::vector<std::shared_ptr<msg::Property>> properties_;
    std::vector<Node*> roots_;
};
