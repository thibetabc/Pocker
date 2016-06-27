//
//  LayerFriendDetails.h
//  pokerclient
//
//  Created by ll on 15/8/23.
//
//

#ifndef __pokerclient__LayerFriendDetails__
#define __pokerclient__LayerFriendDetails__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "events.h"
#include "ShaderSprite.h"
#include "msg.pb.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerFriendDetails : public BaseLayer, public EditBoxDelegate
{
public:
    CREATE_FUNC(LayerFriendDetails);
    virtual bool init();
    void onExit();
    void update_data(cmd_data_pointer = nullptr);
    void update_type(uint32_t details_type, bool is_need_permission = false);
    
    void update_ui(cmd_data_pointer = nullptr);
    void update_property(const msg::UserDetailInfo& friend_data);
    void update_request(const msg::Invitation& request_data){invitation_data_.CopyFrom(request_data);}
    const msg::Invitation& get_invitation_data() {return invitation_data_;}
    
    void set_credit_visible(bool isVisible);
    
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
private:
    void handle_add_user_send_succeess(cmd_data_pointer);
    void handle_delete_friend_succeess(cmd_data_pointer);
    void handle_deal_success(cmd_data_pointer);
    void handle_get_user_permission(cmd_data_pointer);
    
    void clickback(Ref * sender);
    
    void btn_click_agree(Ref * sender);
    void btn_click_disagree(Ref * sender);
    void btn_click_go_chat(Ref * sender);
    void btn_click_delete_friend(Ref * sender);
    void btn_click_request_add(Ref * sender);
    void btn_click_shield(Ref * sender);// 屏蔽朋友圈
    void btn_click_not_shield(Ref * sender);// 解除朋友圈屏蔽
    void btn_click_menu(Ref * sender);
    void btn_click_submit(Ref * sender);// 设置群成员信用值
    
    void btn_click_on(Ref * sender);//设置玩家信用值开
    void btn_click_off(Ref * sender);//设置玩家信用值关
    
    ImageView* image_icon_;
    ShaderSprite* sprite_icon_;
    Text* text_nickname_;
    Text* text_account_name_;
    events::event_dispatch dispatch_;
    Node* root_;
    cocos2d::ui::Text *text_title;
    cocos2d::ui::Text *text_account;
    
    Button * btn_1_;
    Button * btn_2_;
    Button * btn_menu;
    Button * btn_shield;
    bool is_shield_show;
    bool is_shield;
    
    // 玩家信用值信息
    Button * btn_on;
    Button * btn_off;
    Label * label_playercredit;
    EditBox * editbox_playercredit;
    Button * btn_submitcredit;
    bool need_credit;
    uint32_t player_credit;
    Label * label_describe;// 信用值开关说明

    std::function<void()> back_callback_;
    msg::GroupInfo* group_data_;
    msg::UserDetailInfo friend_;
    msg::Invitation invitation_data_;
    bool m_bOn;
};

#endif /* defined(__pokerclient__LayerFriendDetails__) */
