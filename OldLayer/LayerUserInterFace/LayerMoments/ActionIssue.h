//
//  ActionIssue.h
//  pokerclient
//
//  Created by admin on 15-8-19.
//
//

#ifndef __pokerclient__ActionIssue__
#define __pokerclient__ActionIssue__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;



class ActionIssue : public LayerPop {
public:
    CREATE_FUNC(ActionIssue);
    virtual bool init();
    
    void click_image_content(Ref* sender);
    void click_image_cancel(Ref* sender);
    void click_image_report(Ref* sender);
    void click_image_brand(Ref* sender);
    void click_image_issue_event(Ref* sender);
    void finishChoosePic(std::string filename);
    virtual void onEnterTransitionDidFinish();
    
private:
    Point origin_friend;
    Node* root_friend;
    
    events::event_dispatch dispatch_;
};

#endif /* defined(__pokerclient__ActionIssue__) */
