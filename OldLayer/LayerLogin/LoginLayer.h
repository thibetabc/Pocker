//
//  LoginLayer.h
//  pokerclient
//
//  Created by duanmt on 15/4/30.
//
//

#ifndef __pokerclient__LoginLayer__
#define __pokerclient__LoginLayer__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LoginLayer : public BaseLayer {
    
public:
    CREATE_FUNC(LoginLayer);
    
    virtual bool init();
    
    
    void clickRegister(Ref* sender);
    void clickLogin(Ref* sender);
    void clickCheckLogin(Ref *sender);
    void clickForgotPassword(Ref *sender);
    
    void dismiss_child();
    
    void set_loginbtn_enable(bool enable,std::string s)
    {
        btn_login->setEnabled(enable);
        Text *text = Text::create(s.c_str(),"",30);
        text->setPosition(Vec2(375,300));
        this->addChild(text);
        isSwallowTouch = enable;
    }
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event){return true;}
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
    cocos2d::ui::EditBox* tf_username_;
    cocos2d::ui::EditBox* tf_password_;
    Button* btn_login;
    bool isSwallowTouch;
};

#endif /* defined(__pokerclient__LoginLayer__) */
