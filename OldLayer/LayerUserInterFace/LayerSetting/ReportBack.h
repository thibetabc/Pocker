//
//  ReportBack.h
//  pokerclient
//
//  Created by admin on 15-9-1.
//
//

#ifndef __pokerclient__ReportBack__
#define __pokerclient__ReportBack__

#include "cocos2d.h"
#include "BaseLayer.h"
#include "cocostudio/CocoStudio.h"
#include "MultiEditBox.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class ReportBack : public BaseLayer{
public:
    CREATE_FUNC(ReportBack);
    virtual bool init();
    
    void click_report(Ref* sender);
    void click_cancel(Ref* sender);
    void click_add_pic(Ref* sender);
    
    void handle_report_back_success(cmd_data_pointer);
    
    void set_report_type(std::string descrip){descrip_ = descrip;}
    std::string get_report_type(){return descrip_;}
    
    
private:
    std::string descrip_;
    Text* text_title;
    Button* btn_back;
    Text* text_report;
    
    Sprite* sprite_add_pic;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    MultiEditBox* tf_descrip_;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    EditBox* tf_descrip_;
#endif
    
    events::event_dispatch dispatch_;
};

#endif /* defined(__pokerclient__ReportBack__) */
