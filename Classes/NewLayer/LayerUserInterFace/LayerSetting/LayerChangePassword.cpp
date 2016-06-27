//
//  LayerChangePassword.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/6.
//
//

#include "LayerChangePassword.h"
#include "utils.h"
#include "TipView.h"
#include "GameTools.h"
#include "ProcessorManager.h"
#include "NET_CMD.h"
#include "GameDataManager.h"

bool LayerChangePassword::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    auto layer = LayerColor::create(Color4B(143, 143, 143, 100));
    this->addChild(layer);
    
    root_=CSLoader::createNode("LayerChangePassword.csb");
    this->addChild(root_);
    root_->setAnchorPoint(Point(0.5,0.5));
    root_->setPosition(Director::getInstance()->getWinSize().width/2, Director::getInstance()->getWinSize().height/2);
    
    btn_confirm_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_confirm"));
    btn_confirm_->addClickEventListener(std::bind(&LayerChangePassword::click_btn_confirm,this,std::placeholders::_1));
    btn_confirm_->setTitleText(tools::local_string("confirm_change_password","确定"));
    
    
    auto tf_password = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_password"));
    tf_password_ = GameTools::createEditBoxFromTextField(root_, tf_password);
    tf_password_->setPlaceHolder(tools::local_string("input_password", "请填写密码").c_str());
    tf_password_->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    
    auto tf_password_confirm = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_password_confirm"));
    tf_password_confirm_ = GameTools::createEditBoxFromTextField(root_, tf_password_confirm);
    tf_password_confirm_->setPlaceHolder(tools::local_string("passowrd_confirm_empty", "请再次输入密码！").c_str());
    tf_password_confirm_->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerChangePassword::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerChangePassword::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerChangePassword::onTouchEnded, this);
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
#pragma mark click events
void LayerChangePassword::click_btn_confirm(cocos2d::Ref *sender)
{
    ClearError();
    std::string password=tf_password_->getText();
    std::string passwordConfirm=tf_password_confirm_->getText();
    if (password.empty())
    {
        ShowError(tools::local_string("password_empty", "密码不能为空！"));
        return;
    }
    if (passwordConfirm.empty())
    {
        ShowError(tools::local_string("passowrd_confirm_empty", "请再次输入密码！"));
        return;
    }
    if(password!=passwordConfirm)
    {
        ShowError(tools::local_string("passowrd_not_same", "密码不一致，请重新输入！"));
        return;
    }
    send_password(password);
}
#pragma mark touch events
bool LayerChangePassword::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void LayerChangePassword::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
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

void LayerChangePassword::send_password(const std::string& password)
{
    msg::UserLoginInfo info;
    info.set_password(password);
    auto processor = PM->changeUserSetting_up(info);
    send_data(processor, REQ_CHANGE_USER_SETTING);
}

void LayerChangePassword::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void LayerChangePassword::handle_change_user_setting(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()) {
        case msg::Status::SUCCESS:
        {
            TipView::showAlertView(tools::local_string("change_password_successed","密码修改成功"));
            removeFromParent();
            break;
        }
        case msg::Status::FAILED:
            ShowError(processor.message());
            break;
        default:
            break;
    }
}

void LayerChangePassword::register_events()
{
    dispatch_.register_event(REQ_CHANGE_USER_SETTING,
                             BIND_FUNC(this, LayerChangePassword::handle_change_user_setting));
}
cocos2d::Vec2 LayerChangePassword::GetErrorPoint()
{
    Size winSize = Director::getInstance()->getWinSize();
    return Vec2(winSize.width/2,winSize.height-150);
}