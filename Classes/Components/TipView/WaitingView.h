#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SceneManager.h"

class WaitingView : public BaseLayer {
public:
    
    CREATE_FUNC(WaitingView);
    virtual bool init();
    
    static void show();
    static void remove();
    
    virtual void onEnter();
    
    //void setText(const std::string& pos);
    
    void on_timer(float t);
    
private:
    cocos2d::Sprite* sprite_waiting_;
    cocos2d::Animation* sprite_animation_;
    
    cocos2d::ui::Text* text_;
    double send_time;
};