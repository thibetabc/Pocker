#pragma once

#include "CustomTextViewBridge.h"

class TextViewInterface;

class TextViewAndroid : public CustomTextViewBridge {
public:
    TextViewAndroid(const cocos2d::Size& contentSize);
    ~TextViewAndroid();
    virtual void setString(const std::string& text);
    
    
    virtual void setPosition(const cocos2d::Point& pos);
    
    virtual void onExit();
    
protected:
    TextViewInterface* _systemControl;
};
