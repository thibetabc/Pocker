//
//  LayerLogin.hpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/24.
//
//
#ifndef __pokerclient__LayerLogin__
#define __pokerclient__LayerLogin__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "LoginManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerLogin : public BaseLayer, public EditBoxDelegate {
    
public:
    CREATE_FUNC(LayerLogin);
    
    virtual bool init();
    
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

    
    void set_loginbtn_enable(bool enable,std::string s)
    {
        btn_login_->setEnabled(enable);
        Text *text = Text::create(s.c_str(),"",30);
        text->setPosition(Vec2(375,300));
        this->addChild(text);
        isSwallowTouch = enable;
    }
    void handle_connect_break(const std::shared_ptr<server_status_data>& data);
    
private:
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
    void keyBoardWillShow(const float duration, const float distance);
    void keyBoardWillHide(const float duration, const float distance);
    
private:
#pragma makr click_event
    void click_text_register(Ref * sender);
    void click_text_login_mode(Ref * sender);
    void click_text_password_forgot(Ref * sender);
    void click_btn_login(Ref * sender);
    void click_btn_web_login(Ref * sender);
    
#pragma mark UI
private:
    Button* btn_login_;
    Button* btn_weixin_login_;
    
    EditBox* tf_account_;
    EditBox* tf_password_;
    
    Text* text_register_;
    Text* text_login_by_phone_;
    Text* text_password_forgot_;
    
    Text* text_version_num_;
    
    ImageView* image_account_icon_;
    ImageView* image_password_icon_;
    ImageView* image_account_input_bg_;
    ImageView* image_password_input_bg_;
    //ImageView* image_webchat_login_;//微信登陆
    
    Node* root_;

private:
    bool isSwallowTouch;
    account_data::LOGIN_MODE login_mode_;
    
    std::string username_;
    std::string phonenum_;
};

#endif /* defined(__pokerclient__LayerLogin__) */
