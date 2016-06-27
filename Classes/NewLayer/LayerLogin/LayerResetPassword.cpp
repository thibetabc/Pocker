//
//  LayerResetPassword.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/5/19.
//
//

#include "LayerResetPassword.h"
#include "GameDataManager.h"
#include "ProcessorManager.h"
#include "utils.h"
#include "GameTools.h"
#include "NET_CMD.h"

bool LayerResetPassword::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerResetPassword.csb");
    this->addChild(root_);
    
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerResetPassword::click_btn_back, this, std::placeholders::_1));
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("forgot_password", " 忘记密码"));
    
    btn_verification_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_verification"));
    btn_verification_->addClickEventListener(std::bind(&LayerResetPassword::click_btn_verification, this, std::placeholders::_1));
    btn_verification_->setTitleText(tools::local_string("veri_code", "验证码"));
    btn_verification_->setEnabled(false);
    
    btn_confirm_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_confirm"));
    btn_confirm_->addClickEventListener(std::bind(&LayerResetPassword::click_btn_confirm, this, std::placeholders::_1));
    btn_confirm_->setTitleText(tools::local_string("confirm","确定"));
    btn_confirm_->setEnabled(false);
    
    image_account_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_account_bg"));
    image_password_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_password_bg"));
    image_password_confirm_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_password_confirm_bg"));
    image_verification_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_verification_bg"));
    
    auto tf_account =  dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_account"));
    tf_account_ = GameTools::createEditBoxFromTextField(this, tf_account);
    tf_account_->setFontColor(Color3B(255, 255, 255));
    tf_account_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_account_->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    tf_account_->setPlaceHolder(tools::local_string("input_account_name", "请填写账号").c_str());
    tf_account_->setDelegate(this);
    
    auto tf_password = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_password"));
    tf_password_ = GameTools::createEditBoxFromTextField(this, tf_password);
    tf_password_->setFontColor(Color3B(255, 255, 255));
    tf_password_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_password_->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    tf_password_->setPlaceHolder(tools::local_string("input_password", "请填写密码").c_str());
    tf_password_->setDelegate(this);
    
    auto tf_password_confirm = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_password_confirm"));
    tf_password_confirm_ = GameTools::createEditBoxFromTextField(this, tf_password_confirm);
    tf_password_confirm_->setFontColor(Color3B(255, 255, 255));
    tf_password_confirm_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_password_confirm_->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    tf_password_confirm_->setPlaceHolder(tools::local_string("input_password_again", "请再次填写密码").c_str());
    tf_password_confirm_->setDelegate(this);
    
    
    auto tf_verification = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_verification"));
    tf_verification_ = GameTools::createEditBoxFromTextField(this, tf_verification);
    tf_verification_->setFontColor(Color3B(255, 255, 255));
    tf_verification_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_verification_->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    tf_verification_->setPlaceHolder(tools::local_string("veri_code", "验证码").c_str());
    tf_verification_->setDelegate(this);
    
    
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
    this->addChild(layout,10);
    return true;
}

#pragma mark click_event
void LayerResetPassword::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}

void LayerResetPassword::click_btn_verification(Ref * sender)
{
    ClearError();
    std::string strAccount = tf_account_->getText();

    if (!strAccount.empty())
    {
        auto processor = PM->send_verification_by_user(strAccount);
        send_data(processor, REQ_SEND_VERIFICATION_BY_USER, false);
        current_verification_time_ = 60.0;
        root_->schedule(std::bind(&LayerResetPassword::schedule_btn_verification, this, std::placeholders::_1), 1, "TAG_BTN_VERIFICATION");
    }else
    {
        ShowError(tools::local_string("input_account_name","请填写账号"));
    }
}

void LayerResetPassword::click_btn_confirm(Ref * sender)
{
    ClearError();
    std::string account = tf_account_->getText();
    std::string password = tf_password_->getText();
    std::string password_confirm = tf_password_confirm_->getText();
    std::string verification = tf_verification_->getText();
    
    if(account.empty()) {
        ShowError(tools::local_string("username_empty", "用户名不能为空"));
        return;
    }
    if(password.empty()) {
        ShowError(tools::local_string("password_empty", "密码不能为空！"));
        return;
    }
    if(password_confirm.empty()) {
        ShowError(tools::local_string("passowrd_confirm_empty", "请再次输入密码！"));
        return;
    }
    if(password != password_confirm) {
        ShowError(tools::local_string("passowrd_not_same", "密码不一致，请重新输入！"));
        return;
    }
    
    if (verification.empty())
    {
        ShowError(tools::local_string("input_vertify","请输入验证码"));
        return;
    }
    
    auto processor = PM->reset_password(account, password, verification);
    send_data(processor, REQ_RESET_PASSWORD, false);
}

#pragma mark EditBox delegate
void LayerResetPassword::editBoxEditingDidBegin(EditBox* editBox)
{
    
}

void LayerResetPassword::editBoxEditingDidEnd(EditBox* editBox)
{
    
}

void LayerResetPassword::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    std::string account = tf_account_->getText();
    std::string password = tf_password_->getText();
    std::string password_confirm = tf_password_confirm_->getText();
    std::string verification = tf_verification_->getText();
    
    if (strlen(account.c_str()) > 15)
    {
        ShowError(tools::local_string("account_too_long","帐号过长，建议15个字符以内"));
    }else if (strlen(password.c_str()) > 15 || strlen(password_confirm.c_str()) > 15)
    {
        ShowError(tools::local_string("passwords_too_long","密码过长，建议15个字符以内"));
    }else
    {
        ClearError();
    }
    
    if (!account.empty() && !password.empty() && !password_confirm.empty() && !verification.empty())
    {
        if (current_verification_time_ <= 0.0)
        {
            btn_verification_->setTitleText(tools::local_string("get_verify_code","获取验证码"));
            btn_verification_->setEnabled(true);
        }
        btn_confirm_->setEnabled(true);
    }else if (!account.empty() &&
              (password.empty() || password_confirm.empty() || verification.empty()))
    {
        if (current_verification_time_ <= 0.0)
        {
            btn_verification_->setTitleText(tools::local_string("get_verify_code","获取验证码"));
            btn_verification_->setEnabled(true);
        }
        btn_confirm_->setEnabled(false);
    }else
    {
        btn_verification_->setEnabled(false);
        btn_confirm_->setEnabled(false);
    }

}

void LayerResetPassword::editBoxReturn(EditBox* editBox)
{
    
}

void LayerResetPassword::register_events()
{
    dispatch_.register_event(REQ_RESET_PASSWORD, BIND_FUNC(this, LayerResetPassword::handle_reset_password));
    dispatch_.register_event(REQ_SEND_VERIFICATION_BY_USER,
                             BIND_FUNC(this, LayerResetPassword::handle_send_verification));
}

void LayerResetPassword::handle_send_verification(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()) {
        case msg::Status::SUCCESS: {
            break;
        }
        case msg::Status::FAILED:
            current_verification_time_ = 0;
            root_->unschedule("TAG_BTN_SEND_VERIFIVATION");
            this->editBoxTextChanged(nullptr, "");
            ShowError(processor.message());
            break;
        default:
            break;
    }
}

void LayerResetPassword::handle_reset_password(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->response_down(rec);
    if(processor.status() == msg::Status::SUCCESS) {
        LM->login(tf_account_->getText(), tf_password_->getText(), account_data::LOGIN_MODE::USERNAME);
    }
    else {
        current_verification_time_ = 0;
        root_->unschedule("TAG_BTN_SEND_VERIFIVATION");
        this->editBoxTextChanged(nullptr, "");
        ShowError(processor.message());
    }
    
}

#pragma mark shcedule
void LayerResetPassword::schedule_btn_verification(float dt)
{
    if (current_verification_time_ > 0)
    {
        if (btn_verification_->isEnabled())
        {
            btn_verification_->setEnabled(false);
        }
        
        current_verification_time_ -= 1.0;
        
        btn_verification_->setTitleText(tools::to_string(current_verification_time_));
    }else
    {
        root_->unschedule("TAG_BTN_VERIFICATION");
        this->editBoxTextChanged(nullptr, "");
    }
}