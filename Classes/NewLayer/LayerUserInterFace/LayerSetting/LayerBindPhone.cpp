//
//  LayerBindPhone.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/6.
//
//

#include "LayerBindPhone.h"
#include "utils.h"
#include "GameTools.h"
#include "NET_CMD.h"
#include "ProcessorManager.h"
#include "GameDataManager.h"

#define TAG_BTN_SEND_VERIFIVATION 100

LayerBindPhone* LayerBindPhone::create(const bool is_bind_phone)
{
    LayerBindPhone* bindPhone=new LayerBindPhone;
    if (bindPhone->init(is_bind_phone))
    {
        bindPhone->autorelease();
        return bindPhone;
    }
    return nullptr;
}

bool LayerBindPhone::init(const bool is_bind_phone)
{
    if(!BaseLayer::init())
    {
        return false;
    }
    auto layer = LayerColor::create(Color4B(143, 143, 143, 100));
    this->addChild(layer);
    
    root_=CSLoader::createNode("LayerBindPhone.csb");
    this->addChild(root_);
    root_->setAnchorPoint(Point(0.5,0.5));
    root_->setPosition(Director::getInstance()->getWinSize().width/2, Director::getInstance()->getWinSize().height/2);
    
    btn_bind_phone_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_bind_phone"));
    btn_bind_phone_->addClickEventListener(std::bind(&LayerBindPhone::click_btn_bind_phone,this,std::placeholders::_1));
    btn_bind_phone_->setTitleText(tools::local_string("bind_phone","绑定"));
    btn_bind_phone_->setEnabled(false);
    btn_bind_phone_->setTitleColor(cocos2d::Color3B(175, 174, 177));
    
    btn_verification_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_verification"));
    btn_verification_->addClickEventListener(std::bind(&LayerBindPhone::click_btn_verification,this,std::placeholders::_1));
    btn_verification_->setTitleText(tools::local_string("bind_phone_verification","验证码"));
    btn_verification_->setEnabled(false);
    btn_verification_->setTitleColor(cocos2d::Color3B(175, 174, 177));
    
    auto tf_bind_phone = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_bind_phone"));
    tf_bind_phone_ = GameTools::createEditBoxFromTextField(root_, tf_bind_phone);
    tf_bind_phone_->setPlaceHolder(tools::local_string("Cell_number","请输入手机号码").c_str());
    tf_bind_phone_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    tf_bind_phone_->setDelegate(this);
    
    auto tf_verification = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_verification"));
    tf_verification_ = GameTools::createEditBoxFromTextField(root_, tf_verification);
    tf_verification_->setPlaceHolder(tools::local_string("verfy_code","验证码").c_str());
    tf_verification_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    tf_verification_->setDelegate(this);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerBindPhone::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerBindPhone::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerBindPhone::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
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
    this->addChild(layout, 10);
    
    return true;
}

#pragma mark TouchEvent
bool LayerBindPhone::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void LayerBindPhone::onTouchMoved(Touch* touch, Event* event)
{
    
}

void LayerBindPhone::onTouchEnded(Touch* touch, Event* event)
{
    //将openGL坐标转化为node坐标
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    if (root_->getBoundingBox().containsPoint(touchLocation))
    {
        
    }else
    {
        this->removeFromParent();
    }
    
}
void LayerBindPhone::click_btn_bind_phone(cocos2d::Ref *sender)
{
    ClearError();
    std::string phone_number = tf_bind_phone_->getText();
    std::string verification = tf_verification_->getText();
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
        send_bind_phone(phone_number, verification);
    }else
    {
        ShowError(tools::local_string("Invalid_cell","无效的电话号码"));
    }
}

void LayerBindPhone::send_bind_phone(std::string phone_number, std::string verification)
{
    auto processor = PM->BindPhone_up(phone_number, verification);
    send_data(processor, REQ_BIND_PHONE);
}

void LayerBindPhone::register_events()
{
    dispatch_.register_event(REQ_BIND_PHONE,
                             BIND_FUNC(this, LayerBindPhone::handle_bind_phone));
    dispatch_.register_event(REQ_SEND_VERIFICATION,
                             BIND_FUNC(this, LayerBindPhone::handle_send_verification));
}

void LayerBindPhone::handle_send_verification(cmd_data_pointer data)
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
            current_send_verification_time_=0;
            root_->unschedule("TAG_BTN_SEND_VERIFIVATION");
            this->editBoxTextChanged(nullptr, "");
            ShowError(processor.message());
            break;
        default:
            break;
    }
}

void LayerBindPhone::handle_bind_phone(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()) {
        case msg::Status::SUCCESS: {
            set_phone_number_callback(tf_bind_phone_->getText());
            removeFromParent();
            break;
        }
        case msg::Status::FAILED:
            current_send_verification_time_=0;
            root_->unschedule("TAG_BTN_SEND_VERIFIVATION");
            this->editBoxTextChanged(nullptr, "");
            ShowError(processor.message());
            break;
        default:
            break;
    }
}


void LayerBindPhone::click_btn_verification(cocos2d::Ref *sender)
{
    ClearError();
    std::string phoneNumber=tf_bind_phone_->getText();
    bool isPhoneNumberVaild=true;
    for (int i =0; i<strlen(phoneNumber.c_str()); i++)
    {
        if (phoneNumber.c_str()[i]<48 || phoneNumber.c_str()[i]>57)
        {
            isPhoneNumberVaild=false;
            break;
        }
        
    }
    
    if (strlen(phoneNumber.c_str())<5||strlen(phoneNumber.c_str())>20)
    {
        isPhoneNumberVaild=false;
    }
    
    if (isPhoneNumberVaild)
    {
        send_get_verification(phoneNumber);
        
        current_send_verification_time_ = 60.0;
        root_->schedule(std::bind(&LayerBindPhone::schedule_btn_send_verification, this, std::placeholders::_1), 1, "TAG_BTN_SEND_VERIFIVATION");
    }else
    {
        ShowError(tools::local_string("Invalid_cell","无效的电话号码"));
    }
}

void LayerBindPhone::send_get_verification(std::string phone_number)
{
    auto processor = PM->SendVerification_up(phone_number);
    send_data(processor, REQ_SEND_VERIFICATION);
}

void LayerBindPhone::schedule_btn_send_verification(float dt)
{
    if (btn_verification_)
    {
        if (current_send_verification_time_ > 0)
        {
            if (btn_verification_->isEnabled())
            {
                btn_verification_->setEnabled(false);
            }
            current_send_verification_time_ -= 1.0;
            btn_verification_->setTitleText(tools::to_string(current_send_verification_time_));
        }
        else
        {
            root_->unschedule("TAG_BTN_SEND_VERIFIVATION");
            
            this->editBoxTextChanged(nullptr, "");
        }
    }
}

#pragma mark edit delegate
void LayerBindPhone::editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox)
{
    
}

void LayerBindPhone::editBoxEditingDidEnd(cocos2d::ui::EditBox *editBox)
{
    
}

void LayerBindPhone::editBoxReturn(cocos2d::ui::EditBox *editBox)
{
    
}

void LayerBindPhone::editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string &text)
{
    std::string phoneNumber=tf_bind_phone_->getText();
    std::string verification=tf_verification_->getText();
    if (phoneNumber.empty())
    {
        btn_bind_phone_->setEnabled(false);
        btn_bind_phone_->setTitleColor(cocos2d::Color3B(175, 174, 177));
        
        btn_verification_->setEnabled(false);
        btn_verification_->setTitleColor(cocos2d::Color3B(175, 174, 177));
    }
    else
    {
        if (current_send_verification_time_ <= 0)
        {
            btn_verification_->setEnabled(true);
            btn_verification_->setTitleColor(cocos2d::Color3B(255, 255, 255));
            
            btn_verification_->setTitleText(tools::local_string("get_verify_code", "获取验证码"));
        }else
        {
            if (btn_verification_->isEnabled())
            {
                btn_verification_->setEnabled(false);
            }
            btn_verification_->setTitleColor(cocos2d::Color3B(175, 174, 177));
        }
        
        if (strlen(verification.c_str()) == 0)
        {
            btn_bind_phone_->setEnabled(false);
            btn_bind_phone_->setTitleColor(cocos2d::Color3B(175, 174, 177));
        }else
        {
            btn_bind_phone_->setEnabled(true);
            btn_bind_phone_->setTitleColor(cocos2d::Color3B(255, 255, 255));
        }
    }
    
}
cocos2d::Vec2 LayerBindPhone::GetErrorPoint()
{
    Size winSize = Director::getInstance()->getWinSize();
    return Vec2(winSize.width/2,winSize.height-150);
}
