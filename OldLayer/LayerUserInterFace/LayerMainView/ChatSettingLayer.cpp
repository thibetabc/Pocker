//
//  ChatSettingLayer.cpp
//  pokerclient
//
//  Created by ll on 15/9/22.
//
//

#include "ChatSettingLayer.h"
#include "GameDataManager.h"
#include "utils.h"
#include "cmd_def.h"
#include "LayerEditName.h"
#include "InputLayer.h"
#include "TipView.h"
#include "net_manager.h"
#include "ChatMemberCell.h"
#include "LayerFriendDetails.h"
#include "LayerAddGroupUser.h"
#include "GroupPushSetting.h"
#include "FileManager.h"
#include "msg.pb.h"

bool ChatSettingLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    m_bIsUploadSuccess = true;
    
    root_ = CSLoader::createNode("LayerGroupSetting.csb");
    this->addChild(root_);
    
    check_receive_push_ = dynamic_cast<CheckBox *>(CSLoader::seekNodeByName(root_, "CheckBox_1"));
    check_receive_push_->addEventListener(std::bind(&ChatSettingLayer::click_check_receive_push, this, std::placeholders::_1));
    check_receive_push_->setVisible(false);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    
    //text_back_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_back"));
    //text_back_->setString(tools::local_string("back", "返回"));
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    text_group_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_group_name"));
    text_group_note_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_group_note"));
    
    image_group_name_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_group_name"));
    image_group_note_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_group_note"));
    
    image_group_name_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_group_name_1"));
    image_group_note_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_group_note_1"));
    image_add_user_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_add_user_1"));
    
    text_group_name_const_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_group_name_const"));
    text_group_name_const_->setString(tools::local_string("group_name", "群名称"));
    
    text_group_note_const_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_group_note_const"));
    text_group_note_const_->setString(tools::local_string("group_note", "群备注"));
    
    text_add_group_user_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_add_group_user"));
    text_add_group_user_->setString(tools::local_string("add_group_user", "邀请群成员"));
    image_add_group_user_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_add_group_user"));
    point_text_add_group_user_ = text_add_group_user_->getPosition();
    point_image_add_group_user_ = image_add_group_user_->getPosition();
    
    image_clear_chat_record_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_clear_chat_record"));
    text_clear_record_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_clear_record"));
    text_clear_record_->setString(tools::local_string("clear_record_text", "清空聊天记录"));
    
    btn_quit_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_quit"));
    
    
    //text_back_->addClickEventListener(std::bind(&ChatSettingLayer::btn_back_click, this, std::placeholders::_1));
    btn_back_->addClickEventListener(std::bind(&ChatSettingLayer::btn_back_click, this, std::placeholders::_1));
    
    image_clear_chat_record_->addClickEventListener(std::bind(&ChatSettingLayer::image_clear_chat_record_click, this, std::placeholders::_1));
    btn_quit_->addClickEventListener(std::bind(&ChatSettingLayer::btn_quit_click, this, std::placeholders::_1));
    
    text_group_push_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_group_push"));
    image_group_push_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_group_push"));
    //image_group_push_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_group_push_1"));
    image_group_push_->addClickEventListener(std::bind(&ChatSettingLayer::image_group_push_click, this,std::placeholders::_1));
    
    image_head_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon"));
    image_user_head_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon_bg"));
    
    image_groupuser_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_groupinfo_bg_1"));
    image_groupinfo_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_groupinfo_bg_2"));
    image_quit_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_quit"));
    
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    tableview_->setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    tableview_->setTouchEnabled(true);
    tableview_->setSwallowsTouches(true);
    
    root_->addChild(tableview_);
    
    listview->removeFromParent();
    
    return true;
}

void ChatSettingLayer::click_check_receive_push(Ref *r)
{
//    auto user_group_setting = GDM->get_user_group_setting();
//    
//    if (user_group_setting != nullptr && user_group_setting->setting().groupid() == group_data_.groupid())
//    {
//        msg::GroupSetting group_setting;
//        group_setting.CopyFrom(user_group_setting->setting());
//        
//        group_setting.set_receivepush(!((CheckBox *)r)->isSelected());
//        
//        GDM->send_change_group_setting(group_setting);
//    }
}

void ChatSettingLayer::set_setting_layer_status(uint8_t settingstatus)
{
    settinglayer_status_ = settingstatus;
    
    if (settinglayer_status_ == ChatSettingLayer::settingstatus::GROUP)
    {
        text_group_push_->setVisible(false);
        image_group_push_->setVisible(false);
        //image_group_push_1_->setVisible(false);
        
        image_group_name_->addClickEventListener(std::bind(&ChatSettingLayer::image_group_name_click, this, std::placeholders::_1));
        image_group_note_->addClickEventListener(std::bind(&ChatSettingLayer::image_group_note_click, this, std::placeholders::_1));
        image_add_group_user_->addClickEventListener(std::bind(&ChatSettingLayer::image_add_group_user_click, this, std::placeholders::_1));
        btn_quit_->addClickEventListener(std::bind(&ChatSettingLayer::btn_quit_click, this, std::placeholders::_1));
        
        std::string title;
        bool is_owner = group_data_.groupownerid() == GDM->get_user_id();
        if(is_owner) {
            title = tools::local_string("dissolve_group", "解散群");
            image_user_head_->addClickEventListener(std::bind(&ChatSettingLayer::image_click_change_pic, this, std::placeholders::_1));
        }
        else {
            title = tools::local_string("quit_group", "删除并退出");
        }
        btn_quit_->setTitleText(title);
        
        if (group_data_.type() == msg::GroupType::OFFICALGROUP || group_data_.type() == msg::GroupType::NEWBIEGROUP)
        {
            float setoff_y = image_add_group_user_->getPosition().y - image_clear_chat_record_->getPosition().y;
            
            image_clear_chat_record_->setPositionY(image_clear_chat_record_->getPosition().y + setoff_y);
            text_clear_record_->setPositionY(text_clear_record_->getPosition().y + setoff_y);
            image_group_push_->setPositionY(image_group_push_->getPosition().y + setoff_y);
            //image_group_push_1_->setPositionY(image_group_push_1_->getPosition().y + setoff_y);
            text_group_push_->setPositionY(text_group_push_->getPosition().y + setoff_y);
            check_receive_push_->setPositionY(check_receive_push_->getPosition().y + setoff_y);
            
            image_add_group_user_->setVisible(false);
            image_add_user_1_->setVisible(false);
            text_add_group_user_->setVisible(false);
        }
        
        
    }else if (settinglayer_status_ == ChatSettingLayer::settingstatus::FRIEND){
        
        text_group_name_->setVisible(false);
        text_group_note_->setVisible(false);
        image_group_name_->setVisible(false);
        image_group_note_->setVisible(false);
        image_group_name_1_->setVisible(false);
        image_group_note_1_->setVisible(false);
        image_add_user_1_->setVisible(false);
        text_group_name_const_->setVisible(false);
        text_group_note_const_->setVisible(false);
        text_add_group_user_->setVisible(false);
        image_add_group_user_->setVisible(false);
        
        text_group_push_->setVisible(false);
        image_group_push_->setVisible(false);
        check_receive_push_->setVisible(false);
        //image_group_push_1_->setVisible(false);
        
        //text_clear_record_->setPosition(text_group_name_const_->getPosition());
        //image_clear_chat_record_->setPosition(image_group_name_->getPosition());
        text_clear_record_->setPositionY(text_group_name_const_->getPosition().y + image_groupuser_bg_->getContentSize().height);
        image_clear_chat_record_->setPositionY(text_group_name_const_->getPosition().y + image_groupuser_bg_->getContentSize().height);
        
        btn_quit_->setVisible(false);
        
    }
    
    set_ui_data();
    
    register_events();
}

void ChatSettingLayer::set_ui_data()
{
    if (settinglayer_status_ == ChatSettingLayer::settingstatus::GROUP)
    {
        
        std::string title = group_data_.groupname() + "(" + tools::to_string(group_data_.groupusernum()) + ")";
        text_title_->setString(title);
        
        text_group_name_->setString(group_data_.groupname());
        text_group_note_->setString(group_data_.groupcomment());
        
        tableview_->reloadData();
        
        image_head_->loadTexture("image/image_avatar_bg.png");
        image_head_->setVisible(false);
        
        if (sprite_icon_ && sprite_icon_->getParent())
        {
            sprite_icon_->removeFromParent();
            sprite_icon_ = nullptr;
        }
        if (GDM->is_file_exist(group_data_.grouppic()))
        {
            sprite_icon_ = ShaderSprite::create(group_data_.grouppic(), "image/head_mask.png");
            sprite_icon_->setPosition(image_head_->getPosition());
            sprite_icon_->setScale(image_head_->getContentSize().width*1.7 / sprite_icon_->getContentSize().width, image_head_->getContentSize().height*1.7 / sprite_icon_->getContentSize().height);
            
            root_->addChild(sprite_icon_, image_head_->getZOrder());
            
        }else
        {
            FM.download_file(group_data_.grouppic());
            dispatch_.register_event(EVENT_DOWLOAD_FILE,
                                     BIND_FUNC(this, ChatSettingLayer::handle_update_ui));
        }
        
        
        
    }else if (settinglayer_status_ == ChatSettingLayer::settingstatus::FRIEND){
        auto select_friend = members_.at(0);
        
        std::string title = select_friend.nickname();
        text_title_->setString(title);
        
        tableview_->reloadData();
        tableview_->setVisible(false);
        image_groupuser_bg_->setVisible(false);
        image_groupinfo_bg_->setVisible(false);
        image_quit_->setVisible(false);
        
        image_head_->loadTexture("image/main_avatar_bg.png");
        image_head_->setVisible(false);
        
        if (sprite_icon_ && sprite_icon_->getParent())
        {
            sprite_icon_->removeFromParent();
            sprite_icon_ = nullptr;
        }
        if (GDM->is_file_exist(select_friend.picname()))
        {
            sprite_icon_ = ShaderSprite::create(select_friend.picname(), "image/head_mask.png");
            sprite_icon_->setPosition(image_head_->getPosition());
            sprite_icon_->setScale(image_head_->getContentSize().width*1.7 / sprite_icon_->getContentSize().width, image_head_->getContentSize().height*1.7 / sprite_icon_->getContentSize().height);
            
            root_->addChild(sprite_icon_);
        }else
        {
            FM.download_file(select_friend.picname());
            dispatch_.register_event(EVENT_DOWLOAD_FILE,
                                     BIND_FUNC(this, ChatSettingLayer::handle_update_ui));
        }
    }
    
}

#pragma mark register
void ChatSettingLayer::register_events(){
    if (settinglayer_status_ == ChatSettingLayer::settingstatus::GROUP)
    {
        dispatch_.register_event(EVENT_HANDLE_EDIT_GROUP_INFO_FINISHED,
                                 BIND_FUNC(this, ChatSettingLayer::handle_update_ui));
        dispatch_.register_event(EVENT_HANDLE_MEMBER_LIST_FINISHED,
                                 BIND_FUNC(this, ChatSettingLayer::handle_update_ui));
        dispatch_.register_event(EVENT_UPLOAD_FILE_SUCCESS,BIND_FUNC(this, ChatSettingLayer::handle_upload_succeed));
        dispatch_.register_event(EVENT_HANDLE_USER_GROUP_SETTING,BIND_FUNC(this, ChatSettingLayer::handle_receive_user_group_setting));
        
        
    }else if (settinglayer_status_ == ChatSettingLayer::settingstatus::FRIEND){
        
        
    }
}

#pragma mark btn_click
void ChatSettingLayer::btn_quit_click(Ref* sender)
{
    if (settinglayer_status_ == ChatSettingLayer::settingstatus::GROUP)
    {
        auto tip = TipView::showAlertView("确定要离开此群吗？");
        tip->show_cancel(true);
        tip->set_tipview_close_callback([this](){
            GDM->clear_chat_record(group_data_.groupid(), GameDataManager::CLEARTYPE::CLEARGROUPCHAT);
            GDM->send_kick_from_group(group_data_.groupid(), GDM->get_user_id());
            this->removeFromParent();
        });
        /*GDM->clear_chat_record(GDM->get_select_group()->get_id());
         GDM->send_kick_from_group(GDM->get_user_data()->get_id());
         this->removeFromParent();*/
    }else if (settinglayer_status_ == ChatSettingLayer::settingstatus::FRIEND){
        this->removeFromParent();
    }
    
}

void ChatSettingLayer::btn_back_click(Ref* sender)
{
    if(m_bIsUploadSuccess)
    {
        this->removeFromParent();
    }
    else
    {
        TipView * tip = TipView::create();
        tip->update_note("头像正在上传，是否继续返回？");
        tip->set_tipview_close_callback([=]{this->removeFromParent();});
        tip->show_cancel(true);
        this->addChild(tip);
    }
}

void ChatSettingLayer::image_group_name_click(Ref* sender)
{
    if(group_data_.groupownerid() != GDM->get_user_id())
        return;
    
    LayerEditName* len = LayerEditName::create(LayerEditName::EDIT_GROUP_NAME);
    this->addChild(len);
    //    InputLayer *il = InputLayer::create();
    //    this->addChild(il);
    //
    //    il->set_btn_confirm_callback(std::bind(&ChatSettingLayer::change_name, this, std::placeholders::_1));
}
void ChatSettingLayer::image_group_note_click(Ref* sender)
{
    if(group_data_.groupownerid() != GDM->get_user_id())
        return;
    
    
    LayerEditName* len = LayerEditName::create(LayerEditName::EDIT_GROUP_NOTE);
    this->addChild(len);
    //    InputLayer *il = InputLayer::create();
    //    this->addChild(il);
    //
    //    il->set_btn_confirm_callback(std::bind(&ChatSettingLayer::change_note, this, std::placeholders::_1));
}

void ChatSettingLayer::image_add_group_user_click(Ref * sender)
{
    LayerAddGroupUser* len =  LayerAddGroupUser::create();
    this->addChild(len);
}

void ChatSettingLayer::image_clear_chat_record_click(Ref* sender)
{
    if (settinglayer_status_ == ChatSettingLayer::settingstatus::GROUP)
    {
        auto tip = TipView::showAlertView(tools::local_string("clear_record", "确定要清空聊天记录？"));
        tip->show_cancel(true);
        tip->set_tipview_close_callback([this](){
            GDM->clear_chat_record(group_data_.groupid(), GameDataManager::CLEARTYPE::CLEARGROUPCHAT);
            if(clear_record_callback_) {
                clear_record_callback_();
            }
        });
    }else if (settinglayer_status_ == ChatSettingLayer::settingstatus::FRIEND){
        auto tip = TipView::showAlertView(tools::local_string("clear_record", "确定要清空聊天记录？"));
        tip->show_cancel(true);
        tip->set_tipview_close_callback([this](){
            for (int i = 0; i < members_.size(); i++)
            {
                GDM->clear_chat_record(members_[i].userid(), GameDataManager::CLEARTYPE::CLEARFRIENDCHAT);
            }
            if(clear_record_callback_) {
                clear_record_callback_();
            }
        });
    }
}

void ChatSettingLayer::image_group_push_click(Ref* sender)
{
    
    return;
    GroupPushSetting *pLayer = GroupPushSetting::create();
    pLayer->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
    this->addChild(pLayer);
}

void ChatSettingLayer::image_click_change_pic(Ref * sender)
{
    ActionSheet* as = ActionSheet::create();
    as->set_after_choose_pic_callback(std::bind(&ChatSettingLayer::after_choose_pic, this, std::placeholders::_1));
    this->addChild(as);
}

void ChatSettingLayer::after_choose_pic(const std::string& filename)
{
    msg::GroupInfo info;
    info.set_grouppic(filename);
    info.set_groupid(group_data_.groupid());
    GDM->send_edit_group(info);
    FM.upload_file(filename);
    m_StrPicName = filename;
    LayerColor *black = LayerColor::create(Color4B(100, 100, 100, 100), 167, 167);
    black->setAnchorPoint(image_user_head_->getAnchorPoint());
    black->ignoreAnchorPointForPosition(false);
    //    black->setAnchorPoint(add_pic->getAnchorPoint());
    black->setPosition(image_user_head_->getPosition());
    image_user_head_->getParent()->addChild(black,300,1001);
    LabelTTF *label = LabelTTF::create("正在上传","Arial", 18);
    label->setColor(cocos2d::Color3B::WHITE);
    label->setPosition(Point(black->getContentSize()/2));
    black->addChild(label);
    m_bIsUploadSuccess = false;
    
}
void ChatSettingLayer::handle_upload_succeed(cmd_data_pointer data)
{
    bool bFinished = true;
    bool bNoFaild = true;
    
    auto strVec = FM.m_pUploadedFiles;
    if (std::find(strVec.begin(), strVec.end(), m_StrPicName) == strVec.end())
    {
        bFinished = false;
    }
    
    
    auto strVec1 = FM.m_pUploadFaidFiles;
    if (std::find(strVec1.begin(), strVec1.end(), m_StrPicName) != strVec1.end())
    {
        bNoFaild = false;
        
    }
    
    if (bFinished && bNoFaild)
    {
        image_user_head_->getParent()->removeChildByTag(1001);
        m_bIsUploadSuccess = true;
    }
}

void ChatSettingLayer::handle_receive_user_group_setting(cmd_data_pointer data)
{
    text_group_push_->setVisible(true);
    image_group_push_->setVisible(true);
    check_receive_push_->setVisible(true);
    
//    auto user_group_setting = GDM->get_user_group_setting();
//    
//    if (user_group_setting != nullptr && user_group_setting->setting().groupid() == group_data_.groupid())
//    {
//        check_receive_push_->setSelected(!user_group_setting->setting().receivepush());
//    }
}

#pragma mark handle
void ChatSettingLayer::handle_update_ui(cmd_data_pointer)
{
    set_ui_data();
}

#pragma mark tableview datasource --
TableViewCell* ChatSettingLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    ChatMemberCell * cell = dynamic_cast<ChatMemberCell*>(table->dequeueCell());
    if (!cell)
    {
        cell = ChatMemberCell::create();
    }
    if (settinglayer_status_ == ChatSettingLayer::settingstatus::GROUP)
    {
        auto& data = members_.at(idx);
        cell->update_ui(data);
        
        return cell;
    }else if (settinglayer_status_ == ChatSettingLayer::settingstatus::FRIEND){
        auto& data = members_.at(idx);
        cell->update_ui(data);
        
        return cell;
    }
    
    return cell;
}
ssize_t ChatSettingLayer::numberOfCellsInTableView(TableView *table)
{
    if (settinglayer_status_ == ChatSettingLayer::settingstatus::GROUP)
    {
        return members_.size();
    }else if (settinglayer_status_ == ChatSettingLayer::settingstatus::FRIEND){
        return members_.size();
    }
    return 0;
}

#pragma mark tableview delegate --
void ChatSettingLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    GDM->m_isbyreachingfriend = false;
}
