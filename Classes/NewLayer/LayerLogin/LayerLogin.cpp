//
//  LayerLogin.cpp
//  pokerclient
//
//  Created by qqronaldz on 16/3/24.
//
//

#include "LayerLogin.h"
#include "utils.h"
#include "LayerRegister.h"
#include "LayerResetPassword.h"
#include "SceneManager.h"
#include "GameDataManager.h"
#include "GameTools.h"

////判断平台加入微信登陆 加入OC混编
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#include "WebChatCCSwitchToOC.h"
//WebChatCCSwitchToOC* webChatCCSwitchToOC;
//#endif


bool LayerLogin::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    this->setSwallowsTouches(true);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->set_show_dir(DIRECTION_NONE);
    
    root_ = CSLoader::createNode("LayerLoginNew.csb");
    this->addChild(root_);

    username_ = "";
    phonenum_ = "";
    
    GDM->clear_session_key();
    
    btn_login_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_login"));
    btn_login_->addClickEventListener(std::bind(&LayerLogin::click_btn_login, this, std::placeholders::_1));
    btn_login_->setTitleText(tools::local_string("login", "登录"));
    
    text_register_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_register"));
    text_register_->setString(tools::local_string("register", "注册"));
    text_register_->addClickEventListener(std::bind(&LayerLogin::click_text_register, this, std::placeholders::_1));
    
    text_login_by_phone_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_login_by_phone"));
    
    text_login_by_phone_->setString(tools::local_string("switch_to_cellphone_login", "切换手机号登陆"));
    //text_login_by_phone_->setString(tools::local_string("forgot_password", " 忘记密码"));
    text_login_by_phone_->addClickEventListener(std::bind(&LayerLogin::click_text_login_mode, this, std::placeholders::_1));
    
    text_password_forgot_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_password_forgot"));
    text_password_forgot_->setString(tools::local_string("forgot_password", " 忘记密码"));
    text_password_forgot_->addClickEventListener(std::bind(&LayerLogin::click_text_password_forgot, this, std::placeholders::_1));
    
    text_version_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_version_num"));
    text_version_num_->setString(std::string("version:") + tools::to_string(net_manager::CLIENT_VERSION));
    
    image_account_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_account_icon"));
    image_password_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_password_icon"));
    image_account_input_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_account_input_bg"));
    image_password_input_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_password_input_bg"));
    
//    btn_weixin_login_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_weixin_login"));
//    btn_weixin_login_->addClickEventListener(std::bind(&LayerLogin::click_btn_web_login, this, std::placeholders::_1));
//    btn_weixin_login_->setVisible(false);
//    
//    auto text_weixin_login = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_weixin_login"));
//    text_weixin_login->setString(tools::local_string("text_weixin_login", "使用微信账号登录"));
//    text_weixin_login->setVisible(false);

    auto tf_account = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_account"));
    tf_account_ = GameTools::createEditBoxFromTextField(this, tf_account);
    tf_account_->setFontColor(Color3B(255, 255, 255));
    tf_account_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_account_->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    tf_account_->setPlaceHolder(tools::local_string("input_account", "请填写好牌账号").c_str());
    
    auto tf_password = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_password"));
    tf_password_ = GameTools::createEditBoxFromTextField(this, tf_password);
    tf_password_->setFontColor(Color3B(255, 255, 255));
    tf_password_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_password_->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    tf_password_->setPlaceHolder(tools::local_string("input_password", "请填写密码").c_str());

    auto account = LM->get_default_account();
    
    if(account) {
        cocos2d::log("account:%s, %s", account->get_username().c_str(), account->get_password().c_str());
        login_mode_ = account->get_login_mode();
        username_ = account->get_username();
        phonenum_ = account->get_phonenum();
        if (login_mode_ == account_data::LOGIN_MODE::USERNAME)
        {

            tf_account_->setText(username_.c_str());
            //text_login_by_phone__->setAnchorPoint(cocos2d::Vec2(1, 0.5));
            tf_account_->setPlaceHolder(tools::local_string("input_account", "请填写好牌账号").c_str());
            text_login_by_phone_->setString(tools::local_string("switch_to_cellphone_login", "切换手机号登陆"));
        }else if (login_mode_ == account_data::LOGIN_MODE::PHONENUM)
        {

            tf_account_->setText(phonenum_.c_str());
            tf_account_->setPlaceHolder(tools::local_string("input_phone_number", "请填写手机号").c_str());
            //text_login_by_phone__->setAnchorPoint(cocos2d::Vec2(1, 0.5));
            text_login_by_phone_->setString(tools::local_string("switch_to_account_login", "切换账号登陆"));
        }
        tf_password_->setText(account->get_password().c_str());
    }
    else {
        cocos2d::log("account is nil");
        login_mode_ = account_data::LOGIN_MODE::USERNAME;
        text_login_by_phone_->setString(tools::local_string("switch_to_cellphone_login", "切换手机号登陆"));
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
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->setSwallowTouches(false);
    layout->addClickEventListener([this](Ref*){
        //this->m_bIsTouchedLayout = true;
        GameTools::editBoxLoseInputFocus();
    });
    
    GameTools::editBoxLoseInputFocus();
    this->addChild(layout, 10);
    
    return true;
}

void LayerLogin::click_text_register(Ref * sender)
{
    
    LayerRegister* lrn = LayerRegister::create();
    //lr->set_dismiss_callback(std::bind(&LoginLayer::dismiss_child, this));
    this->addChild(lrn);
}

void LayerLogin::click_text_login_mode(Ref * sender)
{
    if (login_mode_ == account_data::LOGIN_MODE::USERNAME)
    {
        login_mode_ = account_data::LOGIN_MODE::PHONENUM;
        tf_account_->setText(phonenum_.c_str());
         tf_account_->setPlaceHolder(tools::local_string("input_phone_number", "请填写手机号").c_str());
        //text_login_by_phone__->setAnchorPoint(cocos2d::Vec2(1, 0.5));
        text_login_by_phone_->setString(tools::local_string("switch_to_account_login", "切换账号登陆"));
        
    }else if (login_mode_ == account_data::LOGIN_MODE::PHONENUM)
    {
        login_mode_ = account_data::LOGIN_MODE::USERNAME;
        tf_account_->setText(username_.c_str());
        tf_account_->setPlaceHolder(tools::local_string("input_account", "请填写好牌账号").c_str());
        //text_login_by_phone__->setAnchorPoint(cocos2d::Vec2(1, 0.5));
        text_login_by_phone_->setString(tools::local_string("switch_to_cellphone_login", "切换手机号登陆"));
    }
}

void LayerLogin::click_btn_web_login(Ref * sender)//微信登陆
{
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    webChatCCSwitchToOC=new WebChatCCSwitchToOC;
//    webChatCCSwitchToOC->send_auth_request();
//#endif
}

void LayerLogin::click_text_password_forgot(Ref * sender)
{
    LayerResetPassword * layerResetPasswd = LayerResetPassword::create();
    this->addChild(layerResetPasswd);
/*
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    [WXApiRequestHandler sendAuthRequestScope: kAuthScope
                                        State:kAuthState
                                       OpenID:kAuthOpenID
                             InViewController:self];
    #endif
 */
}

void LayerLogin::click_btn_login(Ref * sender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    std::string username = tf_account_->getText();
    std::string password = tf_password_->getText();
    
    ClearError();
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
            ShowError(tools::local_string("account_format","账号只能包含数字和字母"));
            return;
        }
    }
    
    if(username.empty()) {
        ShowError(tools::local_string("username_empty", "用户名不能为空"));
        return;
    }
    if(password.empty()) {
        ShowError(tools::local_string("password_empty", "密码不能为空！"));
        return;
    }
    cocos2d::log("username, password:%lu, %lu, %s, %s", username.length(), password.length(), username.c_str(), password.c_str());
    
    if (login_mode_ == account_data::LOGIN_MODE::USERNAME)
    {
        LM->login(username, password, account_data::LOGIN_MODE::USERNAME);
    }else if (login_mode_ == account_data::LOGIN_MODE::PHONENUM)
    {
        LM->login(username, password, account_data::LOGIN_MODE::PHONENUM);
    }

    
#endif
}


#pragma mark EditBox delegate
void LayerLogin::editBoxEditingDidBegin(EditBox* editBox)
{
}
void LayerLogin::editBoxEditingDidEnd(EditBox* editBox)
{
    //isSize = false;
    
}
void LayerLogin::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    std::string account = tf_account_->getText();
    
    if (login_mode_ == account_data::LOGIN_MODE::USERNAME)
    {
        username_ = account;
    }else if (login_mode_ == account_data::LOGIN_MODE::PHONENUM)
    {
        phonenum_ = account;
    }
//    //#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    //    if (text.size() != 0) {
//    //        btn_send_->setVisible(true);
//    //
//    //        btn_emotion_->setEnabled(false);
//    //        btn_emotion_->setVisible(false);
//    //
//    //        btn_add_->setEnabled(false);
//    //        btn_add_->setVisible(false);
//    //    } else {
//    //        btn_send_->setVisible(false);
//    //
//    //        btn_emotion_->setEnabled(true);
//    //        btn_emotion_->setVisible(true);
//    //
//    //        btn_add_->setEnabled(true);
//    //        btn_add_->setVisible(true);
//    //    }
//    //#endif
//    if (text.size() >= 280 && !isSize) {
//        isSize = true;
//        m_bIsTouchedLayout = true;
//    }
    
}
void LayerLogin::editBoxReturn(EditBox* editBox)
{
//    if(!m_bIsTouchedLayout)
//    {
//        click_btn_send(nullptr);
//    }
}

#pragma mark OnEnter
void LayerLogin::onEnter()
{
    BaseLayer::onEnter();
}

#pragma mark OnExit
void LayerLogin::onExit()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    log("BaseLayer::onExit();");
    NotificationCenter::getInstance()->removeAllObservers(this);
#endif
    BaseLayer::onExit();
}
void LayerLogin::handle_connect_break(const std::shared_ptr<server_status_data>& data)
{
}
