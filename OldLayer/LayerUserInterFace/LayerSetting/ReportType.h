//
//  ReportType.h
//  pokerclient
//
//  Created by admin on 15-9-1.
//
//

#ifndef __pokerclient__ReportType__
#define __pokerclient__ReportType__

#include "cocos2d.h"
#include "BaseLayer.h"
#include "ui/CocosGUI.h"
#include "events.h"
#include "cocostudio/CocoStudio.h"


using namespace cocos2d;
using namespace cocos2d::ui;

class ReportType : public BaseLayer
{
public:
    CREATE_FUNC(ReportType);
    virtual bool init();
    
    void click_type_1(Ref* sender);
    void click_type_2(Ref* sender);
    void click_type_3(Ref* sender);
    void click_type_4(Ref* sender);
    
    void click_cancel(Ref* sender);
    void click_next(Ref* sender);
    
    void set_report_type_(std::string descrip){descrip_ = descrip;}
    std::string get_report_type_(){return descrip_;}
private:
    Text* text_title;
    Button* btn_back;
    Text* text_next;
    
    Text* text_type_1;
    Text* text_type_2;
    Text* text_type_3;
    Text* text_type_4;
    
    Sprite* sprite_select_1;
    Sprite* sprite_select_2;
    Sprite* sprite_select_3;
    Sprite* sprite_select_4;
    
    std::string descrip_;
};

#endif /* defined(__pokerclient__ReportType__) */
