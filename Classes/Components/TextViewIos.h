#pragma once

#include "CustomTextViewBridge.h"

class TextViewIos : public CustomTextViewBridge {
public:
    TextViewIos(const cocos2d::Size& contentSize);
    ~TextViewIos();
    virtual void setString(const std::string& text);
    
    UITextView* textView_;
    
    virtual void setPosition(const cocos2d::Point& pos);
    
    virtual void onExit();
};
