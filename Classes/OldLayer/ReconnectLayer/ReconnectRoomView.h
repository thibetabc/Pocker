//
//  ReconnectRoomView.h
//  pokerclient
//
//  Created by ronaldz on 15/12/25.
//
//

#ifndef __pokerclient__ReconnectRoomView__
#define __pokerclient__ReconnectRoomView__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"
#include "MultiEditBox.h"

USING_NS_CC;
using namespace cocos2d::ui;

class ReconnectRoomView:public Layer{
public:
    //static ReconnectLayer* showReconnectLayer(const std::string& message);
    CREATE_FUNC(ReconnectRoomView);
    virtual bool init();
    
    void set_reconnect_callback(const std::function<void()>& callback) { m_reconnectcallback = callback;}
    void set_cancel_callback(const std::function<void()>& callback) { m_cancelcallback = callback;}
    
    void click_btn_reconnect(Ref* sender);
    void click_btn_cancel(Ref* sender);
    
    void update_note(const std::string& note);
    void showbutton(bool isshow);
    
private:
    Button* m_btncancel;
    Button* m_btnreconnect;
    Label * m_note;
    Layout* m_layout;
    
    std::function<void()> m_reconnectcallback;
    std::function<void()> m_cancelcallback;
};

#endif /* defined(__pokerclient__ReconnectRoomView__) */
