#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class RecordAnimationLayer : public BaseLayer
{
public:
    
    CREATE_FUNC(RecordAnimationLayer);
    virtual bool init();

    void record();
    void cancel();
    void giveup();
    
private:
    Sprite* sprite_microphone_;
    Sprite* sprite_cancel_;
    Sprite* sprite_record_;
    Text* text_note_;
    ImageView* image_red_;
};


