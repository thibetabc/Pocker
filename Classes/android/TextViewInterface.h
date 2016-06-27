#pragma once

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <string>
#include "cocos2d.h"

class TextViewInterface {
public:
    TextViewInterface();
    ~TextViewInterface();
    void setString(const std::string& text);
    void setPosition(const cocos2d::Point& pos);
    void setContentSize(const cocos2d::Size& size);
    void remove();
protected:
    int control_id;
};

#endif