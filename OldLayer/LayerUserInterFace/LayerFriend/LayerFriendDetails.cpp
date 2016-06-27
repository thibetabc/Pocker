//
//  LayerFriendDetails.cpp
//  pokerclient
//
//  Created by ll on 15/8/23.
//
//

#include "LayerFriendDetails.h"
#include "SceneManager.h"
#include "utils.h"
#include "GameDataManager.h"
#include "utils.h"
#include "FileManager.h"
#include "cmd_def.h"
#include "TipView.h"
#include "LayerChatOld.h"
#include "string"
#include "ProcessorManager.h"

using namespace std;


bool LayerFriendDetails::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    sprite_icon_ = nullptr;
    
    auto root = CSLoader::createNode("LayerFriendDetails.csb");
    this->addChild(root);
    
    root_ = root;
    
    text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    text_title->setString(tools::local_string("player_details", "玩家信息"));
    
    text_account = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_account"));
    text_account->setString(tools::local_string("account", "账号"));
    
    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon"));
    image_icon_->setVisible(false);
    image_icon_->setScale(1.5);
    
    text_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_nickname"));
    text_account_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_account_name"));
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->setTitleText("    " + tools::local_string("back", "返回"));
    btn_back->addClickEventListener(std::bind(&LayerFriendDetails::clickback, this, std::placeholders::_1));
    
    btn_1_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_1"));
    btn_2_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_2"));
    
    //屏蔽玩家
    btn_menu = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_group_menu"));
    btn_shield = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_shield"));
    
    // 朋友圈屏蔽功能
    btn_menu->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_menu, this, std::placeholders::_1));
    
    btn_shield->setVisible(false);
    is_shield_show = false;
    is_shield = false;
    
    std::vector<uint32_t>& tempvector = GDM->get_defriends();
    for (int i = 0; i < tempvector.size(); i++)
    {
        if (friend_.userid() == tempvector[i])
        {
            btn_shield->setTitleText("解除屏蔽朋友圈");
            is_shield = true;
        }
    }
    
    // 设置群成员信用值
    label_describe = Label::createWithSystemFont("有限的信用值: 是:ON  否:OFF", "Thonburi", 36);
    label_describe->setTextColor(Color4B::BLACK);
    label_describe->setAnchorPoint(Vec2(0, 0.5));
    btn_on = Button::create("image/open.png");
    btn_off = Button::create("image/close.png");
    label_playercredit = Label::createWithSystemFont("玩家信用值：", "Thonburi", 36);
    label_playercredit->setTextColor(Color4B::BLACK);
    editbox_playercredit = EditBox::create(Size(140,label_playercredit->getContentSize().height), Scale9Sprite::create("shurukuang.png"));
    editbox_playercredit->setFontColor(Color3B::BLACK);
    editbox_playercredit->setText("0");
    editbox_playercredit->setDelegate(this);
    editbox_playercredit->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_playercredit->setInputMode(EditBox::InputMode::NUMERIC);
    btn_submitcredit = Button::create("image/button.png","image/button_press.png");
    btn_submitcredit->setTitleText("提交");
    btn_submitcredit->setTitleColor(Color3B::WHITE);
    btn_submitcredit->setTitleFontSize(28);
    
    btn_on->setPosition(Vec2(140, 800));
    btn_off->setPosition(Vec2(140, 800));
    label_playercredit->setPosition(Vec2(140+btn_on->getContentSize().width/2+label_playercredit->getContentSize().width/2, 800));
    editbox_playercredit->setPosition(Vec2(label_playercredit->getPosition().x+label_playercredit->getContentSize().width/2
                                           +editbox_playercredit->getContentSize().width/2,800));
    btn_submitcredit->setPosition(Vec2(editbox_playercredit->getPosition().x+editbox_playercredit->getContentSize().width/2+btn_submitcredit->getContentSize().width/2 + 10, 800));
    label_describe->setPosition(Vec2(btn_on->getPosition().x-60, 860));
    
    root->addChild(label_describe);
    root->addChild(btn_on);
    root->addChild(btn_off);
    root->addChild(label_playercredit);
    root->addChild(editbox_playercredit);
    root->addChild(btn_submitcredit);
    
    btn_on->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_on, this, std::placeholders::_1));
    btn_off->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_off, this, std::placeholders::_1));
    btn_submitcredit->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_submit, this, std::placeholders::_1));
    
    //玩家信用值默认不可见
    set_credit_visible(false);
    
    //    auto image_account = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_account"));
    //
    //    image_setting->addClickEventListener(std::bind(&SettingLayer::click_image_setting, this, std::placeholders::_1));
    //
    //    image_account->addClickEventListener(std::bind(&SettingLayer::click_image_account, this, std::placeholders::_1));
    //
    
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, LayerFriendDetails::update_ui));
    dispatch_.register_event(EVENT_REQ_GET_USER_PERMISSION,
                             BIND_FUNC(this, LayerFriendDetails::handle_get_user_permission));
    
    return true;
}

void LayerFriendDetails::update_ui(cmd_data_pointer)
{
    
    
    //text_nickname_->setString(friend_->get_nick_name());
    //text_account_name_->setString(tools::to_string(friend_->get_userid()));
    if(!friend_.picname().empty())
    {
        if(GDM->is_file_exist(friend_.picname())) {
            image_icon_->loadTexture(friend_.picname());
            if (sprite_icon_ && sprite_icon_->getParent()) {
                sprite_icon_->removeFromParent();
            }
            sprite_icon_ = ShaderSprite::create(friend_.picname(), "image/head_mask.png");
            sprite_icon_->setPosition(image_icon_->getPosition());
            sprite_icon_->setScale(image_icon_->getContentSize().width / sprite_icon_->getContentSize().width,
                                   image_icon_->getContentSize().height / sprite_icon_->getContentSize().height);
            root_->addChild(sprite_icon_, image_icon_->getZOrder());
        }
        else {
            FM.download_file(friend_.picname());
        }
        
        
        text_nickname_->setString(friend_.nickname());
        text_account_name_->setString(tools::to_string(friend_.userid()));
    }
}

void LayerFriendDetails::update_property(const msg::UserDetailInfo& friend_data){
    friend_.CopyFrom(friend_data);
    update_ui();
}

void LayerFriendDetails::update_data(cmd_data_pointer)
{
    
    
}

void LayerFriendDetails::update_type(uint32_t details_type, bool is_need_permission)
{
    if (is_need_permission)
    {
        auto processor = PM->GetUserPermission_up(group_data_->groupid(), friend_.userid());
        net_manager::shared_instence().send_data(processor, REQ_GET_PERMISSION);
    }
    //1 添加好友
    if (details_type == 1)
    {
        btn_1_->setVisible(true);
        btn_2_->setVisible(false);
        btn_1_->setTitleText(tools::local_string("request_add_friend", "请求添加好友"));
        btn_1_->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_request_add, this, std::placeholders::_1));
        
    }else if (details_type == 2){ // 2 通过验证 拒绝
        btn_1_->setVisible(true);
        btn_2_->setVisible(true);
        btn_1_->setTitleText(tools::local_string("agree_friend_request", "通过验证"));
        btn_2_->setTitleText(tools::local_string("disagree_friend_request", "拒绝"));
        btn_1_->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_agree, this, std::placeholders::_1));
        btn_2_->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_disagree, this, std::placeholders::_1));
        dispatch_.register_event(EVENT_HANDLE_REQ_DEAL_REQUEST_SUCCESS,
                                 BIND_FUNC(this, LayerFriendDetails::handle_deal_success));
    }else if (details_type == 3){ // 3 发送消息 删除好友
        btn_1_->setVisible(true);
        btn_2_->setVisible(true);
        btn_1_->setTitleText(tools::local_string("send_friend_message", "发送消息"));
        btn_2_->setTitleText(tools::local_string("delete_friend", "删除好友"));
        btn_1_->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_go_chat, this, std::placeholders::_1));
        btn_2_->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_delete_friend, this, std::placeholders::_1));
        dispatch_.register_event(EVENT_HANDLE_REQ_DELETE_FRIEND_SUCCESS,
                                 BIND_FUNC(this, LayerFriendDetails::handle_delete_friend_succeess));
        
    }
    else if (details_type == 4){  // 4 查看自己信息
        btn_1_->setVisible(false);
        btn_2_->setVisible(false);
    }
    
    if (false == is_shield){
        btn_shield->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_shield, this, std::placeholders::_1));
    }
    else{
        btn_shield->addClickEventListener(std::bind(&LayerFriendDetails::btn_click_not_shield, this, std::placeholders::_1));
    }
}

void LayerFriendDetails::clickback(Ref * sender)
{
    removeFromParent();
}

void LayerFriendDetails::onExit()
{
    if(back_callback_) {
        back_callback_();
    }
    LayerPop::onExit();
}

void LayerFriendDetails::btn_click_agree(Ref * sender)
{
    auto processor = PM->DealRequest_up(invitation_data_.request().id(), true);
    net_manager::shared_instence().send_data(processor,REQ_DEAL_REQUEST);
}
void LayerFriendDetails::btn_click_disagree(Ref * sender)
{
    auto processor = PM->DealRequest_up(invitation_data_.request().id(), false);
    net_manager::shared_instence().send_data(processor,REQ_DEAL_REQUEST);
}
void LayerFriendDetails::btn_click_go_chat(Ref * sender)
{
    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
    
    LayerChatOld* glc = LayerChatOld::create();
    glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
    
    //Director::getInstance()->getRunningScene()->addChild(glc);
    parent->addChild(glc, 1);
    
    removeFromParent();
}
void LayerFriendDetails::btn_click_delete_friend(Ref * sender)
{
    auto processor = PM->DeleteFriend_up(friend_.userid());
    net_manager::shared_instence().send_data(processor,REQ_DELETE_FRIEND);
}
void LayerFriendDetails::btn_click_request_add(Ref * sender)
{
    msg::Request request;
    request.set_receiverid(friend_.userid());
    request.set_type(msg::RequestType::ADDFRIEND);
    request.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    auto processor = PM->SendRequest_up(request);
    net_manager::shared_instence().send_data(processor,REQ_ADD_FRIEND);
}

void LayerFriendDetails::handle_add_user_send_succeess(cmd_data_pointer)
{
    removeFromParent();
}

void LayerFriendDetails::handle_delete_friend_succeess(cmd_data_pointer)
{
    removeFromParent();
}

void LayerFriendDetails::handle_deal_success(cmd_data_pointer)
{
    removeFromParent();
}

void LayerFriendDetails::handle_get_user_permission(cmd_data_pointer)
{
//        for (int i = 0; i < user_permission->permission().size(); i++)
//        {
//            if (user_permission->permission(i).groupid() == group_data_->groupid())
//            {
//                need_credit = user_permission->permission(i).needcredit();
//                player_credit = user_permission->permission(i).credit();
//                break;
//            }
//        }
    
    
    if(group_data_->groupownerid() == GDM->get_user_id() &&
       friend_.userid() != GDM->get_user_id()){
        if(GDM->m_isbyreachingfriend){
            set_credit_visible(false);
        }
        else{
            set_credit_visible(true);
        }
    }
    if(group_data_->groupownerid() != GDM->get_user_id() &&
       friend_.userid() == GDM->get_user_id()){
        if(need_credit == true){
            std::string strcredit = tools::to_string(player_credit);
            std::string strlabel = "信用值：";
            strlabel = strlabel+strcredit;
            Label * label = Label::createWithSystemFont(strlabel, "Thonburi", 36);
            label->setPosition(Vec2(Director::getInstance()->getWinSize().width/2, 800));
            label->setTextColor(Color4B::BLACK);
            this->addChild(label);
        }
        else{
            std::string strcredit = "无限";
            std::string strlabel = "信用值：";
            strlabel = strlabel+strcredit;
            Label * label = Label::createWithSystemFont(strlabel, "Thonburi", 36);
            label->setPosition(Vec2(Director::getInstance()->getWinSize().width/2, 800));
            label->setTextColor(Color4B::BLACK);
            this->addChild(label);
        }
    }
}

void LayerFriendDetails::btn_click_menu(Ref * sender)
{
    if(friend_.userid() == GDM->get_user_id()){
        btn_shield->setVisible(false);
    }
    else{
        if(false == is_shield_show){
            btn_shield->setVisible(true);
            is_shield_show = true;
        }
        else{
            btn_shield->setVisible(false);
            is_shield_show = false;
        }
        
    }
}

void LayerFriendDetails::btn_click_shield(Ref * sender)
{
    auto processor = PM->BlockUser_up(friend_.userid(), true);
    net_manager::shared_instence().send_data(processor,REQ_SHIELD_FRIEND);
    btn_shield->setTitleText("解除屏蔽朋友圈");
    btn_shield->setVisible(false);
    is_shield_show = false;
    is_shield = true;
    GDM->circletype = GameDataManager::TYPE_FRIEND;
}

void LayerFriendDetails::btn_click_not_shield(Ref * sender)
{
    auto processor = PM->BlockUser_up(friend_.userid(), false);
    net_manager::shared_instence().send_data(processor, REQ_SHIELD_FRIEND);
    btn_shield->setTitleText("屏蔽朋友圈");
    btn_shield->setVisible(false);
    is_shield_show = false;
    is_shield = false;
    GDM->circletype = GameDataManager::TYPE_FRIEND;
}

// 设置群成员信用值
void LayerFriendDetails::btn_click_submit(Ref * sender)
{
    string strcredit = editbox_playercredit->getText();
    uint32_t credit = atoi(strcredit.c_str());
    auto processor = PM->GetUserPermission_up(group_data_->groupid(), friend_.userid());
    net_manager::shared_instence().send_data(processor, REQ_GET_PERMISSION);
}

void LayerFriendDetails::set_credit_visible(bool isVisible)
{
    
    if(isVisible){
        if(need_credit){
            label_describe->setVisible(true);
            btn_on->setVisible(true);
            btn_off->setVisible(false);
            label_playercredit->setVisible(true);
            editbox_playercredit->setVisible(true);
            std::string strcredit = tools::to_string(player_credit);
            editbox_playercredit->setText(strcredit.c_str());
            btn_submitcredit->setVisible(true);
            //            btn_submitcredit->setEnabled(true);
        }
        else{
            label_describe->setVisible(true);
            btn_on->setVisible(false);
            btn_off->setVisible(true);
            label_playercredit->setVisible(false);
            editbox_playercredit->setVisible(false);
            btn_submitcredit->setVisible(true);
            //            btn_submitcredit->setEnabled(false);
        }
    }
    else{
        label_describe->setVisible(false);
        btn_on->setVisible(false);
        btn_off->setVisible(false);
        label_playercredit->setVisible(false);
        editbox_playercredit->setVisible(false);
        btn_submitcredit->setVisible(false);
    }
}

void LayerFriendDetails::btn_click_on(Ref * sender)
{
    btn_on->setVisible(false);
    btn_off->setVisible(true);
    //    btn_submitcredit->setEnabled(false);
    m_bOn = false;
    label_playercredit->setVisible(false);
    editbox_playercredit->setVisible(false);
    //    uint32_t groupid = GDM->get_select_group_id();
    //    uint32_t userid = GDM->get_select_friend()->get_userid();
    //    bool needcredit = false;
    //    GDM->send_change_needcredit(groupid, userid, needcredit);
}

void LayerFriendDetails::btn_click_off(Ref * sender)
{
    btn_off->setVisible(false);
    btn_on->setVisible(true);
    //    btn_submitcredit->setEnabled(true);
    m_bOn = true;
    label_playercredit->setVisible(true);
    editbox_playercredit->setVisible(true);
    //    uint32_t groupid = GDM->get_select_group_id();
    //    uint32_t userid = GDM->get_select_friend()->get_userid();
    //    bool needcredit = true;
    //    GDM->send_change_needcredit(groupid, userid, needcredit);
}

void LayerFriendDetails::editBoxEditingDidBegin(EditBox* editBox)
{
    
}

void LayerFriendDetails::editBoxEditingDidEnd(EditBox* editBox)
{
    
}

void LayerFriendDetails::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    std::string s = editBox->getText();
    for (auto i : s)
    {
        if (i < '0' || i > '9')
        {
            //存在非法字符
            TipView::showAlertView("存在非法字符，请输入数字");
            editBox->setText("");
        }
    }
    
}

void LayerFriendDetails::editBoxReturn(EditBox *editBox)
{
    
}
