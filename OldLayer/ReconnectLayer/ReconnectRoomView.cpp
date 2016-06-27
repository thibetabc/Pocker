//
//  ReconnectRoomView.cpp
//  pokerclient
//
//  Created by ronaldz on 15/12/25.
//
//

#include "ReconnectRoomView.h"

bool ReconnectRoomView::init()
{
    if(!Layer::init()){
        return false;
    }
    m_layout = Layout::create();
    m_layout->setSize(Size(544, 298));
    m_layout->setBackGroundImage("image/bg_reconnect.png");
    addChild(m_layout);
    m_layout->ignoreAnchorPointForPosition(false);
    m_layout->setAnchorPoint(Vec2(0.5, 0.5));
    m_layout->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    m_note = Label::createWithSystemFont("", "Thonburi", 30);
    m_btnreconnect = Button::create("image/btn_reconnect.png", "image/btn_pre_reconnect.png");
    m_btnreconnect->addClickEventListener(std::bind(&ReconnectRoomView::click_btn_reconnect, this, std::placeholders::_1));
    m_btnreconnect->setTitleText("重连");
    m_btnreconnect->setTitleFontSize(28);
    m_btncancel = Button::create("image/btn_reconnect.png","image/btn_pre_reconnect.png");
    m_btncancel->addClickEventListener(std::bind(&ReconnectRoomView::click_btn_cancel, this, std::placeholders::_1));
    m_btncancel->setTitleText("取消");
    m_btncancel->setTitleFontSize(28);
    m_layout->addChild(m_note);
    m_layout->addChild(m_btncancel);
    m_layout->addChild(m_btnreconnect);
    m_note->setPosition(Vec2(m_layout->getContentSize().width/2, m_layout->getContentSize().height*2/3));
    m_btncancel->setPosition(Vec2(m_layout->getContentSize().width/4, m_layout->getContentSize().height/3));
    m_btnreconnect->setPosition(Vec2(m_layout->getContentSize().width*3/4, m_layout->getContentSize().height/3));
    
    return true;
}

void ReconnectRoomView::click_btn_cancel(cocos2d::Ref *sender)
{
    if(m_cancelcallback)
    {
        m_cancelcallback();
    }
}

void ReconnectRoomView::click_btn_reconnect(cocos2d::Ref *sender)
{
    if(m_reconnectcallback)
    {
        m_reconnectcallback();
    }
}

void ReconnectRoomView::update_note(const std::string& note)
{
    m_note->setString(note);
}

void ReconnectRoomView::showbutton(bool isshow)
{
    if(isshow)
    {
        m_btncancel->setVisible(true);
        m_btnreconnect->setVisible(true);
    }else{
        m_btncancel->setVisible(false);
        m_btnreconnect->setVisible(false);
    }
}
