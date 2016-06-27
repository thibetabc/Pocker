//
//  PublicHandcard.h
//  pokerclient
//
//  Created by admin on 15-9-7.
//
//

#ifndef __pokerclient__PublishHandcard__
#define __pokerclient__PublishHandcard__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
//#include "CCTableView.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class PublishHandcard : public BaseLayer{
public:

    CREATE_FUNC(PublishHandcard);
    virtual bool init();
    
    void set_btn_publish_callback(const std::function<void()>& callback) {
        btn_publish_callback_ = callback;
    }
    void set_btn_cancel_callback(const std::function<void()>& callback) {
        btn_cancel_callback_ = callback;
    }
    
    void click_btn_publish(Ref* sender);
    void click_btn_cancel(Ref* sender);
    void click_btn_add_pic(Ref* sender);
    
    void click_image_visible(Ref* sender);
    void click_image_need_point(Ref* sender);
    void click_image_pay_point(Ref* sender);
    void click_image_add_handcard(Ref* sender);
    void click_image_public_hc(Ref* sender);
    
    events::event_dispatch dispatch_;
    
    
private:
    std::function<void()> btn_publish_callback_;
    std::function<void()> btn_cancel_callback_;
    std::function<void()> btn_add_pic_callback_;
    
    

};

#endif /* defined(__pokerclient__PublicHandcard__) */
