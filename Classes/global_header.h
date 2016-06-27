//
//  global_header.h
//  pokerclient
//
//  Created by duanmt on 15/5/4.
//
//

//#define DEBUG

#ifndef pokerclient_global_header_h
#define pokerclient_global_header_h

#define DESIGN_SIZE Size(750.0,1334.0)
#define DESIGN_WIDTH 750.0
#define DESIGN_HEIGHT 1334.0
#define TAG_SETTING_LAYER 90001
#define WINSIZE Director::getInstance()->getWinSize()

#define ANDRIOD_KEY_BACK_CLICK auto listener = EventListenerKeyboard::create();                            \
listener->onKeyReleased =[&](EventKeyboard::KeyCode keyCode, Event* event)                                 \
{                                                                                                          \
    if(keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE); \
    {                                                                                                      \
        this->removeFromParentAndCleanup(true);                                                            \
    }                                                                                                      \
};                                                                                                         \
Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

#include "msg.pb.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "ShaderSprite.h"
#include "events.h"
#include "cmd_data.h"

const cocos2d::Color4B COLOR_TOP = cocos2d::Color4B(51, 85, 115, 255);


const int GroupCellHeight = 232;

const std::string gFemaleIcon = "login_res/usersetting/btn_female.png";
const std::string gMaleIcon = "login_res/usersetting/btn_male.png";
const int TITLE_MAX_LENGHT = 15;

#endif
