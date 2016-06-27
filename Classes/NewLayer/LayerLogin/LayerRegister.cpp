//
//  LayerRegister.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/23.
//
//

#include "LayerRegister.h"
#include "GameDataManager.h"
#include "ProcessorManager.h"
#include "utils.h"
#include "GameTools.h"
#include "NET_CMD.h"
#include "LayerLicense.h"

bool LayerRegister::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerRegisterNew.csb");
    this->addChild(root_);
    
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerRegister::click_btn_back, this, std::placeholders::_1));
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("register", "注册"));
    
    text_license_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_license_1"));
    text_license_1_->setString(tools::local_string("license1", "点击上面的注册按钮，即表示你同意"));
    
    text_license_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_license_2"));
    text_license_2_->setString(tools::local_string("license2", "《好牌许可及服务协议》"));
    text_license_2_->addClickEventListener(std::bind(&LayerRegister::click_text_license_2, this, std::placeholders::_1));
    
    btn_verification_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_verification"));
    btn_verification_->addClickEventListener(std::bind(&LayerRegister::click_btn_verification, this, std::placeholders::_1));
    btn_verification_->setTitleText(tools::local_string("veri_code", "验证码"));
    btn_verification_->setEnabled(false);
    
    btn_confirm_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_confirm"));
    btn_confirm_->addClickEventListener(std::bind(&LayerRegister::click_btn_confirm, this, std::placeholders::_1));
    btn_confirm_->setTitleText(tools::local_string("confirm","确定"));
    btn_confirm_->setEnabled(false);
    
    image_account_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_account_bg"));
    image_password_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_password_bg"));
    image_password_confirm_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_password_confirm_bg"));
    image_bind_phone_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_bind_phone_bg"));
    image_referee_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_referee_bg"));
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
    
    auto tf_referee = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_referee"));
    tf_referee_ = GameTools::createEditBoxFromTextField(this, tf_referee);
    tf_referee_->setFontColor(Color3B(255, 255, 255));
    tf_referee_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_referee_->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    tf_referee_->setPlaceHolder(tools::local_string("referer_Optional_", "推荐人（非必填）").c_str());
    
    auto tf_bind_phone = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_bind_phone"));
    tf_bind_phone_ = GameTools::createEditBoxFromTextField(this, tf_bind_phone);
    tf_bind_phone_->setFontColor(Color3B(255, 255, 255));
    tf_bind_phone_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_bind_phone_->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    tf_bind_phone_->setPlaceHolder(tools::local_string("cell_Optional", "请输入手机号（非必填）").c_str());
    tf_bind_phone_->setDelegate(this);
    
    auto tf_verification = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_verification"));
    tf_verification_ = GameTools::createEditBoxFromTextField(this, tf_verification);
    tf_verification_->setFontColor(Color3B(255, 255, 255));
    tf_verification_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_verification_->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    tf_verification_->setPlaceHolder(tools::local_string("veri_code_choose","验证码（非必填）").c_str());
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
void LayerRegister::click_text_license_2(Ref * sender)
{
    LayerLicense* ll = LayerLicense::create();
    ll->set_dismiss_callback([this]() {
        
    });
    this->addChild(ll);
}

void LayerRegister::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}

void LayerRegister::click_btn_verification(Ref * sender)
{
    ClearError();
    std::string phone_number = tf_bind_phone_->getText();
    bool is_phone_number_vaild = true;
    
    for (int i = 0; i < strlen(phone_number.c_str()); i++) {
        cocos2d::log("%d",phone_number.c_str()[i]);
        if (48 <= phone_number.c_str()[i] && (phone_number.c_str()[i] <= 57)) {
            cocos2d::log("数字");
        }else{
            is_phone_number_vaild = false;
            break;
        }
    }
    
    if (strlen(phone_number.c_str()) < 5 || strlen(phone_number.c_str()) > 20)
    {
        is_phone_number_vaild = false;
    }
    
    if (is_phone_number_vaild)
    {
        auto processor = PM->SendVerification_up(phone_number);
        send_data(processor, REQ_SEND_VERIFICATION,false);
        current_verification_time_ = 60.0;
        root_->schedule(std::bind(&LayerRegister::schedule_btn_verification, this, std::placeholders::_1), 1, "TAG_BTN_VERIFICATION");
    }else
    {
        ShowError(tools::local_string("Invalid_cell","无效的电话号码"));
    }
}

void LayerRegister::click_btn_confirm(Ref * sender)
{
    ClearError();
    std::string account = tf_account_->getText();
    std::string password = tf_password_->getText();
    std::string password_confirm = tf_password_confirm_->getText();
    std::string referee = tf_referee_->getText();
    std::string bind_phone = tf_bind_phone_->getText();
    std::string verification = tf_verification_->getText();
    
    for (int i = 0; i < strlen(account.c_str()); i++) {
        if (48 <= account.c_str()[i] && (account.c_str()[i] <= 57)) {
            cocos2d::log("数字");
        }else if ((97 <= account.c_str()[i]) && (account.c_str()[i] <= 122))
        {
            
        }else if((65 <= account.c_str()[i]) && (account.c_str()[i] <= 90))
        {
            
        }else
        {
            ShowError(tools::local_string("account_format","账号只能包含数字和字母"));
            return;
        }
    }
    
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
    
    if (bind_phone.empty() && !verification.empty())
    {
        ShowError(tools::local_string("bind_cell","如需绑定手机号，请输入手机号"));
        return;
    }
    
    if (!bind_phone.empty() && verification.empty())
    {
        ShowError(tools::local_string("bind_cell_vertify","如需绑定手机号，请输入验证码"));
        return;
    }
    
    auto processor = PM->register_up(account, password, bind_phone, verification, referee);
    send_data(processor, REQ_REGISTER,false);
}

#pragma mark EditBox delegate
void LayerRegister::editBoxEditingDidBegin(EditBox* editBox)
{
    
}

void LayerRegister::editBoxEditingDidEnd(EditBox* editBox)
{
    
}

void LayerRegister::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    std::string account = tf_account_->getText();
    std::string password = tf_password_->getText();
    std::string password_confirm = tf_password_confirm_->getText();
    std::string bind_phone = tf_bind_phone_->getText();
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
    
    if (strlen(account.c_str()) == 0 || strlen(account.c_str()) > 15)
    {
        if (btn_confirm_->isEnabled())
        {
            btn_confirm_->setEnabled(false);
        }
    }else if (strlen(password.c_str()) == 0 ||
              strlen(password_confirm.c_str()) == 0 ||
              strlen(password.c_str()) > 15 ||
              strlen(password_confirm.c_str()) > 15)
    {
        if (btn_confirm_->isEnabled())
        {
            btn_confirm_->setEnabled(false);
        }
    }else if ((strlen(bind_phone.c_str()) == 0 && strlen(verification.c_str()) != 0) ||
              (strlen(bind_phone.c_str()) != 0 && strlen(verification.c_str()) == 0))
    {
        if (btn_confirm_->isEnabled())
        {
            btn_confirm_->setEnabled(false);
        }
    }else
    {
        if (!btn_confirm_->isEnabled())
        {
            btn_confirm_->setEnabled(true);
        }
    }
    
    if (strlen(bind_phone.c_str()) != 0 && current_verification_time_ <= 0.0)
    {
        if (!btn_verification_->isEnabled())
        {
            btn_verification_->setEnabled(true);
        }
        //btn_verification_->setTitleText("获取验证码");
        btn_verification_->setTitleText(tools::local_string("get_verify_code","获取验证码"));
    }else
    {
        if (btn_verification_->isEnabled())
        {
            btn_verification_->setEnabled(false);
        }
    }
}

void LayerRegister::editBoxReturn(EditBox* editBox)
{
    
}

void LayerRegister::register_events()
{
    dispatch_.register_event(REQ_REGISTER, BIND_FUNC(this, LayerRegister::handle_register));
    dispatch_.register_event(REQ_SEND_VERIFICATION,
                             BIND_FUNC(this, LayerRegister::handle_send_verification));
}

void LayerRegister::handle_send_verification(cmd_data_pointer data)
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
            current_verification_time_=0;
            root_->unschedule("TAG_BTN_SEND_VERIFIVATION");
            this->editBoxTextChanged(nullptr, "");
            ShowError(processor.message());
            break;
        default:
            break;
    }
}

void LayerRegister::handle_register(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->response_down(rec);
    if(processor.status() == msg::Status::SUCCESS) {
        LM->login(tf_account_->getText(), tf_password_->getText(), account_data::LOGIN_MODE::USERNAME);
    }
    else {
        current_verification_time_=0;
        root_->unschedule("TAG_BTN_SEND_VERIFIVATION");
        this->editBoxTextChanged(nullptr, "");
        ShowError(processor.message());
    }
    
}

#pragma mark shcedule
void LayerRegister::schedule_btn_verification(float dt)
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