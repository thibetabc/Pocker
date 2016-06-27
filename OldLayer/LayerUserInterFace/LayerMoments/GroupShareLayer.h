//
//  GroupShareLayer.h
//  pokerclient
//
//  Created by admin on 15-8-26.
//
//

#ifndef __pokerclient__GroupShareLayer__
#define __pokerclient__GroupShareLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "RecordLayer.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;



class GroupShareLayer : public BaseLayer, public EditBoxDelegate
{
public:
    
    CREATE_FUNC(GroupShareLayer);
    virtual bool init();
    TextField* tf_reward;
    
    /*void set_btn_publish_callback(const std::function<void()>& callback) {
        btn_publish_callback_ = callback;
    }
    void set_btn_cancel_callback(const std::function<void()>& callback) {
        btn_cancel_callback_ = callback;
    }
    void set_btn_hc_callback(const std::function<void()>& callback) {
        image_check_callback_ = callback;
    }*/
    
    void click_btn_publish(Ref* sender);
    void click_btn_cancel(Ref* sender);
    void click_btn_add_pic(Ref* sender);
    void click_image_visible(Ref* sender);
    void click_image_need_point(Ref* sender);
    void click_image_reward(Ref* sender);
    void click_image_handcard_select(Ref* sender);
    void click_image_public_card(Ref* sender);
    
    void handle_send_success_community(cmd_data_pointer);
    void handle_recv_success_community(cmd_data_pointer);
    void handle_upload_succeed(cmd_data_pointer);
    void handle_select_handcard(cmd_data_pointer data);
    void handle_who_scene(cmd_data_pointer data);
    void handle_public_hd(cmd_data_pointer data);
    events::event_dispatch dispatch_;
    
    const int get_type() { return type_; }
    void set_type(const int type);
    
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
private:
    /*std::function<void()> btn_publish_callback_;
    std::function<void()> btn_cancel_callback_;
    std::function<void()> btn_add_pic_callback_;
//    cocos2d::ui::MultiEditBox* m_pTextContent;
    std::function<void()> image_check_callback_;*/
    
    Button* btn_add_pic;
    Text* scene_;
    Text* public_;
    Layer *m_pLayer;
    
    
    ImageView* image_check_handcard_bg_;
    ImageView* image_check_publiccard_bg_;
    ImageView* image_check_visible_bg_;
    ImageView* image_check_need_point_bg_;
    ImageView* image_check_reward_bg_;
    
    ImageView* image_check_handcard_1_;
    ImageView* image_check_publiccard_1_;
    ImageView* image_check_visible_1_;
    ImageView* image_check_need_point_1_;
    ImageView* image_check_reward_1_;
    
    Text* text_check_handcard_;
    Text* text_check_publicard_;
    Text* text_check_visible_;
    Text* text_check_need_point_;
    Text* text_check_reward_;
    
    Text* text_check_publicard_note_;
    Text* text_check_visible_note_;
    Text* text_check_need_point_note_;
    Text* text_check_reward_note_;
    
    Sprite* sprite_check_handcard_;
    
    Text* text_title_;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    MultiEditBox* tf_descrip_;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    EditBox* tf_descrip_;
#endif
    
    msg::Processor_200_PublishMoment processor;
    msg::Moment* Moment;
    msg::MomentInfo * MomentInfo;
    msg::NormalMomentInfo * NormalMomentInfo;
    void after_choose_pic(const std::string& filename);
    int picname_number;
    int type_;
};

class SelectWhoScene : public Layer{
public:
    CREATE_FUNC(SelectWhoScene);
    virtual bool init();
    
    void click_image_all(Ref* sender);
    void click_image_fri(Ref* sender);
    void click_image_self(Ref* sender);
    void click_image_cancel(Ref* sender);
    
    
    
    
    virtual void onEnterTransitionDidFinish();
    

    
private:
    Point origin_friend;
    Node* root_friend;
    
    uint8_t use_type_;
    
    events::event_dispatch dispatch_;
};

class PublicHD : public Layer{
public:
    CREATE_FUNC(PublicHD);
    virtual bool init();
    
    void click_image_yes(Ref* sender);
    void click_image_no(Ref* sender);
    void click_image_cancel(Ref* sender);
    
    
    
    
    virtual void onEnterTransitionDidFinish();
    
    
    
private:
    Point origin_friend;
    Node* root_friend;
    
    uint8_t use_type_;
    
    events::event_dispatch dispatch_;
};


#endif /* defined(__pokerclient__GroupShareLayer__) */
