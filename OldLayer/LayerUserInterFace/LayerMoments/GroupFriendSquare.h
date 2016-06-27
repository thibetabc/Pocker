
//
//  GroupFriendSquare.h
//  pokerclient
//
//  Created by admin on 15-8-24.
//
//



#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
//#include "BaseLayer.h"
#include "events.h"


using namespace cocos2d;
using namespace cocos2d::ui;



class GroupFriendSquare : public Layer {
public:
    CREATE_FUNC(GroupFriendSquare);
    virtual bool init();
    
    void chilk_image_like(Ref* sender);
    void click_image_review(Ref* sender);
//    void click_image_report(Ref* sender);
//    void click_image__brand(Ref* sender);
//    void click_image_issue_event(Ref* sender);
    
    
    
    
    virtual void onEnterTransitionDidFinish();
    
    //std::shared_ptr<ImagePickerBridge> picker_;
    
    //void register_events();
    
    //void set_use_type(const uint8_t use_type) { use_type_ = use_type;}
    // void change_head_pic(cmd_data_pointer);
    
private:
    Point origin_friend;
    Node* root_friend;
    
    uint8_t use_type_;
    
    events::event_dispatch dispatch_;
    //--郭
    //TableView* tableview_;
};
