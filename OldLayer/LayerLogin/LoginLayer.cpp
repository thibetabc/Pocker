//
//  LoginLayer.cpp
//  pokerclient
//
//  Created by duanmt on 15/4/30.
//
//

#include "LoginLayer.h"
#include "TipView.h"
#include "utils.h"
#include "LayerRegister.h"
#include "SceneManager.h"
#include "GameTools.h"
#include "LoginManager.h"
#include "WaitingView.h"

using namespace cocos2d;
using namespace cocos2d::ui;

bool LoginLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    if(isSwallowTouch)
    {
        this->setTouchEnabled(true);
        this->setSwallowsTouches(true);
        this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    }
    this->set_show_dir(DIRECTION_NONE);
    
    auto root = CSLoader::createNode("LayerLogin.csb");
    //root->setScale(640.0/750.0, 1136.0/1334.0);
    this->addChild(root);
    
    Button* btn_register = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_register"));
    btn_login = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_login"));
    Text* text_forgot_password = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_forgot_password"));
    
    CCLOGERROR("createEditBoxFromTextFieldcreateEditBoxFromTextFieldcreateEditBoxFromTextField");
    auto tf_cellphone = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_cellphone"));

    tf_username_ = GameTools::createEditBoxFromTextField(this, tf_cellphone);
    tf_username_->setPlaceHolder(tools::local_string("input_account", "请填写好牌账号").c_str());
    tf_username_->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    
    auto tf_password = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_password"));
    tf_password_ = GameTools::createEditBoxFromTextField(this, tf_password);
    tf_password_->setPlaceHolder(tools::local_string("input_password", "请填写密码").c_str());
    tf_password_->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    tf_password_->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    
    btn_register->addClickEventListener(std::bind(&LoginLayer::clickRegister, this, std::placeholders::_1));
    btn_register->setTitleText(tools::local_string("register", "注册"));
    
    btn_login->setTitleText(tools::local_string("login", "登录"));
    btn_login->addClickEventListener(std::bind(&LoginLayer::clickLogin, this, std::placeholders::_1));
    
    text_forgot_password->setString(tools::local_string("forgot_password", " 忘记密码"));
    text_forgot_password->addClickEventListener(std::bind(&LoginLayer::clickForgotPassword, this, std::placeholders::_1));
    text_forgot_password->setVisible(false);
    
    auto account = LM->get_default_account();
    
    if(account) {
        cocos2d::log("account:%s, %s", account->get_username().c_str(), account->get_password().c_str());
        tf_username_->setText(account->get_username().c_str());
        tf_password_->setText(account->get_password().c_str());
    }
    else {
        cocos2d::log("account is nil");
    }
    
    //    NotificationCenter::getInstance()->postNotification("for_android");
    
    //对手机返回键的监听
    auto listener = EventListenerKeyboard::create();
    //和回调函数绑定
    listener->onKeyReleased =[&](EventKeyboard::KeyCode keyCode, Event* event)
    {
        if(keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE);
        {
            Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
        }
    };
    //添加到事件分发器中
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    return true;
}

void LoginLayer::clickRegister(Ref* sender)
{
    LayerRegister* lr = LayerRegister::create();
    lr->set_dismiss_callback(std::bind(&LoginLayer::dismiss_child, this));
    this->addChild(lr);
}
void LoginLayer::clickLogin(Ref* sender)
{
    
    cocos2d::log("username, password");
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    std::string username = tf_username_->getText();
    std::string password = tf_password_->getText();
    
    
    for (int i = 0; i < strlen(username.c_str()); i++) {
        cocos2d::log("%d",username.c_str()[i]);
        if (48 <= username.c_str()[i] && (username.c_str()[i] <= 57)) {
            cocos2d::log("数字");
        }else if ((97 <= username.c_str()[i]) && (username.c_str()[i] <= 122))
        {
            cocos2d::log("xiao");
        }else if((65 <= username.c_str()[i]) && (username.c_str()[i] <= 90))
        {
            cocos2d::log("da");
        }else
        {
            TipView::showAlertView("账号只能包含数字和字母");
            return;
        }
    }
    
    if(username.empty()) {
        TipView::showAlertView(tools::local_string("username_empty", "用户名不能为空"));
        return;
    }
    if(password.empty()) {
        TipView::showAlertView(tools::local_string("password_empty", "密码不能为空！"));
        return;
    }
    cocos2d::log("username, password:%lu, %lu, %s, %s", username.length(), password.length(), username.c_str(), password.c_str());
    LM->login(username, password);
    
#endif
}
void LoginLayer::clickCheckLogin(Ref* sender)
{
    LM->check_version();
}
void LoginLayer::clickForgotPassword(Ref *sender)
{
}

void LoginLayer::dismiss_child()
{
}

void LoginLayer::onEnter()
{
    BaseLayer::onEnter();
}

void LoginLayer::onExit()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    log("BaseLayer::onExit();");
    NotificationCenter::getInstance()->removeAllObservers(this);
#endif
    LayerPop::onExit();
}
