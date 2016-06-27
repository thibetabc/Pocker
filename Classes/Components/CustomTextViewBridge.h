#pragma once

#include "cocos2d.h"

class CustomTextViewBridge : public cocos2d::Node {

public:
    virtual void setString(const std::string& text) = 0;
    
};