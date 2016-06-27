#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "CustomTextViewBridge.h"
#include "SceneManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerLicense : public BaseLayer
{
public:
    CREATE_FUNC(LayerLicense);
    virtual bool init();
    void load_text();
    
    
    Text* text_title_;
    Button* btn_back_;

    CustomTextViewBridge* textView_;
};

extern CustomTextViewBridge* __createTextView(const cocos2d::Size& size);
