#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"
#include "GameLayer.h"
#include "SceneManager.h"
#include "Gametools.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class PokerRoom;

class PlayerActionNode : public PokerChildLayer
{
public:
    
    CREATE_FUNC(PlayerActionNode);
    virtual bool init();
    
    void set_is_tournament(bool is_tournament);
    void set_dismiss_callback(const std::function<void()>& callback) { dismiss_callback_ = callback;}
    void set_giveup_callback(const std::function<void()>& callback) { giveup_callback_ = callback;}
    
    void click_standup(Ref* sender);
    void click_quit(Ref* sender);
    void quit(cmd_data_pointer data);
    void click_setting(Ref* sender);
    void click_remove();
    virtual void onExit();
    virtual bool onTouchBegan(Touch *touch, Event *unused_event){dismiss_callback_();;this->removeFromParent();return true;}
    /** Callback function for touch moved.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchMoved(Touch *touch, Event *unused_event){};
    /** Callback function for touch ended.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchEnded(Touch *touch, Event *unused_event){};

private:
    std::function<void()> dismiss_callback_;
    std::function<void()> giveup_callback_;

    Button* image_standup_;
    Button* image_quit_;
    Button* image_setting_;
    
    cocos2d::Point point_image_1;
    cocos2d::Point point_image_2;
    cocos2d::Point point_image_3;
};