//
//  LayerChatOld.cpp
//  pokerclient
//  进入特定群之后的聊天界面(游戏内聊天室界面见GameChatLayer)
//
//  Created by ll on 15/9/8.
//
//

#include "LayerChatOld.h"
#include "GameTools.h"
#include "ChatMessageCell.h"
#include "cmd_def.h"
#include "utils.h"
#include "VoiceRecorderInterface.h"
#include "TipView.h"
#include "ActionSheet.h"
#include "AudioManager.h"
#include "ProcessorManager.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "SceneManager.h"
#include "LayerManager.h"

#include "RoomLayer.h"
#include "ChatSettingLayer.h"
#include "RecordAnimationLayer.h"
#include "EmotionLayer.h"
#include "LayerUserMatchInfo.h"

#define ANIMATION_LAYER_TAG 1

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "SoundPlayerInterface.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_EditBoxPool_send(JNIEnv* env, jobject thiz, int idx)
    {
//        if(LayerChatOld::__glc__) {
//            LayerChatOld::__glc__->editBoxReturn(nullptr);
//        }
//        cocos2d::log("Java_org_cocos2dx_cpp_AppEditBoxPool_send");
        
    }
}
#endif
bool LayerChatOld::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    chat_status_ = 1;
    m_bIsTouchedLayout = false;
    auto& win_size = Director::getInstance()->getWinSize();
    
    auto root = CSLoader::createNode("LayerChat.csb");
    this->addChild(root);
    
    LayerColor* top = LayerColor::create(COLOR_TOP, win_size.width, 128);
    top->setAnchorPoint(Vec2(0, 1));
    top->setPosition(0, DESIGN_HEIGHT - top->getContentSize().height);
    root->addChild(top, 1);
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    btn_member_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_member"));
    
    auto tf_chat = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_chat"));
    
    tf_chat_ = GameTools::createEditBoxFromTextField(this, tf_chat);
    tf_chat_->setReturnType(EditBox::KeyboardReturnType::SEND);
    tf_chat_->setInputMode(EditBox::InputMode::SINGLE_LINE);
    tf_chat_->setDelegate(this);
    tf_chat_->setMaxLength(250);
    tf_chat_->addClickEventListener([this](Ref*){
        this->m_bIsTouchedLayout = false;
    });
    m_pEditBoxPosition = tf_chat_->getPosition();
    tf_chat->removeFromParentAndCleanup(true);
    /*
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    tf_chat_->setKeyBoardWillShowCallback(std::bind(&LayerChatOld::keyBoardWillShow, this, std::placeholders::_1, std::placeholders::_2));
    tf_chat_->setKeyBoardWillHideCallback(std::bind(&LayerChatOld::keyBoardWillHide, this, std::placeholders::_1, std::placeholders::_2));
#endif*/
    
    text_room_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_room"));
    btn_keyboard_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_keyboard"));
    btn_keyboard_->addClickEventListener(std::bind(&LayerChatOld::click_btn_keyboard, this, std::placeholders::_1));
    
    btn_record_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_record"));
    
    //    btn_record_->addTouchEventListener(std::bind(&LayerChatOld::touch_btn_record, this, std::placeholders::_1, std::placeholders::_2));
    btn_record_->addTouchCallback(std::bind(&LayerChatOld::touch_btn_record, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    btn_voice_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_voice"));
    btn_voice_->addClickEventListener(std::bind(&LayerChatOld::click_btn_voice, this, std::placeholders::_1));
    
    btn_room_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_room"));
    
    btn_group_match_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_group_match"));
    
    btn_emotion_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_emotion"));
    btn_emotion_->addClickEventListener(std::bind(&LayerChatOld::click_btn_emotion, this, std::placeholders::_1));
    
    room_number_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "room_number"));
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    listview_ = listview;
    
    listview_->setZOrder(-2);
    
    auto image_tf_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_tf_bg"));
    btn_add_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_add"));
    btn_add_->addClickEventListener(std::bind(&LayerChatOld::click_btn_add, this, std::placeholders::_1));
    
    btn_send_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_send"));
    btn_send_->addClickEventListener(std::bind(&LayerChatOld::click_btn_send, this, std::placeholders::_1));
    btn_send_->setVisible(false);
    
    node_about_textfileds_.insert(std::make_pair(btn_voice_, btn_voice_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_emotion_, btn_emotion_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(tf_chat_, tf_chat_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(image_tf_bg, image_tf_bg->getPosition()));
    node_about_textfileds_.insert(std::make_pair(listview_, listview_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_keyboard_, btn_keyboard_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_record_, btn_record_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_add_, btn_add_->getPosition()));
//    node_about_textfileds_.insert(std::make_pair(btn_send_, btn_send_->getPosition()));
    
    
    btn_back_->addClickEventListener(std::bind(&LayerChatOld::click_btn_back, this, std::placeholders::_1));

    btn_group_match_->setZOrder(2);
    btn_back_->setZOrder(2);
    text_title_->setZOrder(2);
    btn_room_->setZOrder(2);
    room_number_->setZOrder(2);
    btn_member_->setZOrder(2);
    
    root_ = root;

    if (chat_status_ == 1){
        init_group();
    }else if (chat_status_ == 2){
        init_friend();
    }
    
    update_data();
    
    listview_->addTouchCallback(std::bind(&LayerChatOld::touch_listview, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    refresh_chat_message(REFRESH_MODE::REFRESH_OLD_MESSAGE);
    
    click_btn_keyboard(btn_keyboard_);
    auto& size = Director::getInstance()->getWinSize();
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->setSwallowTouches(false);
    layout->addClickEventListener([this](Ref*){
        this->m_bIsTouchedLayout = true;
        GameTools::editBoxLoseInputFocus();
    });
    
    GameTools::editBoxLoseInputFocus();
    this->addChild(layout, 10);
    
    this->setContentSize(win_size);
    
    return true;
}

void LayerChatOld::touch_listview(Ref* sender, Touch* touch, Widget::TouchEventType event)
{
    float needRefreshMoveHeight = 200; //下拉距离 越大需要下拉的距离越远
    
    switch (event) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN: {
            
            
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::MOVED: {
            
            auto innercontainer = listview_->getInnerContainer();
            
            cocos2d::Point posInnerContainer = innercontainer->getPosition();
            cocos2d::Size sizeInnerContainer = innercontainer->getContentSize();
            
            cocos2d::log("touch_listview: innercontainer P:%f, %f, S:%f, %f", innercontainer->getPosition().x, innercontainer->getPosition().y, innercontainer->getContentSize().width, innercontainer->getContentSize().height);
            
            if (posInnerContainer.y < 0)
            {
                float needMoveHeight = sizeInnerContainer.height - listview_->getContentSize().height;
                
                cocos2d::log("touch_listview: needMoveHeight %f", needMoveHeight);
                
                if (needMoveHeight * -1 >= posInnerContainer.y + needRefreshMoveHeight)
                {
                    can_refresh_list_ = true;
                }else
                {
                    can_refresh_list_ = false;
                }
            }else
            {
                can_refresh_list_ = false;
            }

            
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::ENDED: {
            
            if (can_refresh_list_)
            {
                cocos2d::log("touch_listview: TouchEventType::ENDED can Refresh");
                refresh_chat_message(REFRESH_MODE::REFRESH_OLD_MESSAGE);
            }
            can_refresh_list_ = false;
            
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::CANCELED: {
            
            if (can_refresh_list_)
            {
                cocos2d::log("touch_listview: TouchEventType::CANCELED can Refresh");
                refresh_chat_message(REFRESH_MODE::REFRESH_OLD_MESSAGE);
            }
            
            can_refresh_list_ = false;
            
            break;
        }
    }
}


void LayerChatOld::register_events_group()
{
    dispatch_.register_event(EVENT_HANDLE_REFRESH_CHAT_RECORD,
                             BIND_FUNC(this, LayerChatOld::reload_data));
    dispatch_.register_event(EVENT_HANDLE_OUT_MEMBER_FINISHED,
                             BIND_FUNC(this, LayerChatOld::remove_from_parent));
    //dispatch_.register_event(EVENT_HANDLE_ROOM_LIST_FINISHED,
     //                        BIND_FUNC(this, LayerChatOld::update_data));
    dispatch_.register_event(EVENT_HANDLE_EDIT_GROUP_INFO_FINISHED,
                             BIND_FUNC(this, LayerChatOld::update_data));
    dispatch_.register_event(EVENT_HANDLE_CLEAR_EDIT_CHAT,
                             BIND_FUNC(this, LayerChatOld::handle_send_msg_clear_editBox));
}

void LayerChatOld::register_events_friend()
{
    
    dispatch_.register_event(EVENT_HANDLE_REFRESH_CHAT_RECORD,
                             BIND_FUNC(this, LayerChatOld::reload_data));
    dispatch_.register_event(EVENT_HANDLE_OUT_MEMBER_FINISHED,
                             BIND_FUNC(this, LayerChatOld::remove_from_parent));
    dispatch_.register_event(EVENT_HANDLE_CLEAR_EDIT_CHAT,
                             BIND_FUNC(this, LayerChatOld::handle_send_msg_clear_editBox));
}
void LayerChatOld::init_group()
{
    btn_room_->addClickEventListener(std::bind(&LayerChatOld::click_btn_room, this, std::placeholders::_1));
    btn_member_->addClickEventListener(std::bind(&LayerChatOld::click_btn_member, this, std::placeholders::_1));
    btn_group_match_->addClickEventListener(std::bind(&LayerChatOld::click_btn_group_match, this, std::placeholders::_1));
    register_events_group();
    GDM->send_get_room_info(group_data_->groupid());
}
void LayerChatOld::init_friend()
{
    register_events_friend();
    
    text_room_->setVisible(false);
    room_number_->setVisible(false);
    btn_room_->setVisible(false);
    btn_group_match_->setVisible(false);
    //btn_member_->setVisible(false);
    btn_member_->addClickEventListener(std::bind(&LayerChatOld::click_btn_member, this, std::placeholders::_1));
}

void LayerChatOld::update_data(cmd_data_pointer data)
{
//    if (chat_status_ == 1)
//    {
//        auto& group_data = GDM->get_select_group();
//        std::string title = group_data->get_name() + "(" + tools::to_string(group_data->get_player_counts()) + ")";
//        text_title_->setString(title);
//        
//        int32_t room_size = GDM->get_select_group_all_room_size();
//        text_room_->setString(tools::to_string(room_size));
//    }else if (chat_status_ == 2){
//        auto& is_friend_data = GDM->get_select_friend();
//        std::string title = is_friend_data->get_nick_name();
//        
//        text_title_->setString(title);
//        
//    }

}


void LayerChatOld::click_btn_back(Ref* sender)
{
    removeFromParent();
}
void LayerChatOld::click_btn_member(Ref* sender)
{
    auto gsl = ChatSettingLayer::create();
    if (chat_status_ == 1){
        gsl->set_clear_record_callback([this](){
            this->reload_data(nullptr);
        });
        gsl->set_setting_layer_status(ChatSettingLayer::settingstatus::GROUP);
        auto processor = PM->GetGroupSetting_up(group_data_->groupid());
        net_manager::shared_instence().send_data(processor, REQ_GET_GROUP_SETTING);
    }else if (chat_status_ == 2){
        gsl->set_setting_layer_status(ChatSettingLayer::settingstatus::FRIEND);
    }
    gsl->set_dismiss_callback([this](){
    });
//    this->getParent()->addChild(gsl, 1);
    this->addChild(gsl,1);
}

void LayerChatOld::click_btn_room(Ref* sender)
{
    RoomLayer* rl = RoomLayer::create();
    this->addChild(rl, 1);
    rl->set_dismiss_callback([this](){
    });
}

void LayerChatOld::click_btn_group_match(Ref* sender)
{
    LayerUserMatchInfo* lumi = LayerUserMatchInfo::create();
    lumi->set_is_group_match(true, group_data_->groupid());
    this->addChild(lumi, 1);
    
}

void LayerChatOld::touch_btn_record(Ref* sender, Touch* touch, Widget::TouchEventType event)
{
    unsigned long time = tools::date();
    uint32_t user_id = GDM->get_user_id();
    
    int record_time = 0;
    switch (event) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN: {
            AM->begin_recording();
            
            touch_record_begin_position_ = touch->getLocation();
            record_filename_ = tools::to_string(user_id) + "_" + tools::to_string(time);
            log("filename1....:%s", record_filename_.c_str());
            FM.get_recorder()->beginRecordByFileName(record_filename_);
            btn_record_->setTitleText(tools::local_string("release_send", "松开  发送"));
            RecordAnimationLayer* record_animation_layer_ = (RecordAnimationLayer*)getChildByTag(ANIMATION_LAYER_TAG);
            if(!record_animation_layer_) {
                record_animation_layer_ = RecordAnimationLayer::create();
                addChild(record_animation_layer_, 0, ANIMATION_LAYER_TAG);
                record_animation_layer_->record();
            }
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::MOVED: {
            Point p = touch->getLocation();
            if(!btn_record_->isHighlighted()) {
                RecordAnimationLayer* record_animation_layer_ = (RecordAnimationLayer*)getChildByTag(ANIMATION_LAYER_TAG);
                if(record_animation_layer_) {
                    float sub = p.y - touch_record_begin_position_.y;
                    log("sub:%f", sub);
                    if(fabsf(p.y - touch_record_begin_position_.y) < 100) {
                        record_animation_layer_->cancel();
                    }
                    else {
                        record_animation_layer_->giveup();
                    }
                }
            }
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::ENDED: {
            
            record_time = FM.get_recorder()->stopRecord();
            if(record_time!=0) {
                const std::string upload_filename = record_filename_ + ".amr";
                FM.upload_file(upload_filename);
                // by lvzhangle
                msg::ChatMessage * ChatMessage = new msg::ChatMessage();
                
                ChatMessage->mutable_userdetailinfo()->set_userid(GDM->get_user_id());
                ChatMessage->mutable_messageinfo()->mutable_voiceinfo()->set_voicename(record_filename_);
                ChatMessage->mutable_messageinfo()->mutable_voiceinfo()->set_voicetime(record_time);
                ChatMessage->mutable_messageinfo()->set_type(msg::MessageType::Voice);
                if (chat_status_ == 1){
                    ChatMessage->set_groupid(group_id_);
                }else if (chat_status_ == 2){
                    ChatMessage->set_receiveuserid(friend_id_);
                }
                GDM->send_chat_message(*ChatMessage);

                
                //GDM->send_chat_message(msg::MessageType::Voice, GDM->get_select_group()->get_id(), record_filename_, record_time);
            }
            else {
                TipView::showAlertView(tools::local_string("record_time_too_shop", "录音时间太短"));
            }
            record_filename_ = "";
            btn_record_->setTitleText(tools::local_string("press_voice", "按住  说话"));
            
            RecordAnimationLayer* record_animation_layer_ = (RecordAnimationLayer*)getChildByTag(ANIMATION_LAYER_TAG);
            if(record_animation_layer_)
                record_animation_layer_->removeFromParent();
            AM->end_recording();
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            RecordAnimationLayer* record_animation_layer_ = (RecordAnimationLayer*)getChildByTag(ANIMATION_LAYER_TAG);
            if(record_animation_layer_)
                record_animation_layer_->removeFromParent();
            btn_record_->setTitleText(tools::local_string("press_voice", "按住  说话"));
            AM->end_recording();
            break;
    }
}

void LayerChatOld::click_btn_emotion(Ref* sender)
{
    //    tableview_->runAction(MoveBy::create(0.1f, Point(0, 500)));
    tf_chat_->setVisible(false);
    EmotionLayer* el = EmotionLayer::create();
    el->set_dismiss_callback(std::bind(&LayerChatOld::remove_emotion_layer, this));
    //    el->set_dismiss_callback(std::bind(&LayerChatOld::keyBoardWillHide, this, std::placeholders::_1, std::placeholders::_2));
    el->set_voice_callback([this](){
        for (auto& iter : node_about_textfileds_) {
            if(iter.first != listview_) {
                iter.first->setVisible(true);
            }
        }
        this->keyBoardWillHide(0.2f, 0);
        this->click_btn_voice(nullptr);
        
    });
    //el->set_chat_callback(std::bind(&LayerChatOld::show_keyboard, this));
    el->set_chat_callback([=](){
        this->remove_emotion_layer();
        this->show_keyboard();
    });
    this->addChild(el);
    
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != listview_) {
            iter.first->setVisible(false);
        }
    }
    
    //    this->keyBoardWillShow(0.2f, 425);
    
    float dis = 425;
    auto& items = listview_->getItems();
    if(!items.empty()) {
        auto back = items.back();
        Point p = back->getPosition() + node_about_textfileds_.at(listview_);
        if(p.y < dis) {
            Point p1 = node_about_textfileds_.at(listview_) + Point(0, dis - p.y + tf_chat_->getContentSize().height + 20);
            listview_->stopAllActions();
            listview_->runAction(MoveTo::create(0.2f, p1));
        }
    }
}

void LayerChatOld::click_btn_voice(Ref* sender)
{
    btn_record_->setTitleText(tools::local_string("press_voice", "按住  说话"));
    btn_voice_->setVisible(false);
    btn_keyboard_->setVisible(true);
    btn_record_->setVisible(true);
    m_pEditBoxPosition = tf_chat_->getPosition();
    tf_chat_->setPositionY(-100);
}

void LayerChatOld::click_btn_keyboard(Ref* sender)
{
    btn_keyboard_->setVisible(false);
    btn_voice_->setVisible(true);
    btn_record_->setVisible(false);
    tf_chat_->setPositionY(m_pEditBoxPosition.y);
//    GameTools::editBoxLoseInputFocus();
}

void LayerChatOld::click_btn_add(Ref* sender)
{
    tf_chat_->setVisible(false);
    ActionSheet* as = ActionSheet::create();
    as->setIsChat(true);
    as->set_dismiss_callback([this](){
        tf_chat_->setVisible(true);
        GameTools::editBoxLoseInputFocus();
    });
    as->set_after_choose_pic_callback(std::bind(&LayerChatOld::after_choose_pic, this, std::placeholders::_1));
    this->addChild(as);
}

void LayerChatOld::after_choose_pic(const std::string& filename)
{
    FM.upload_file(filename);
    
    msg::ChatMessage * ChatMessage = new msg::ChatMessage();
    
    ChatMessage->mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    
    ChatMessage->mutable_messageinfo()->mutable_picinfo()->set_picname(filename);
    ChatMessage->mutable_messageinfo()->set_type(msg::MessageType::Picture);
    //            if (GDM->get_chat_status() == 1){
    //                ChatMessage->set_groupid(GDM->get_select_group_id());
    //            }else if (GDM->get_chat_status() == 2){
    //                ChatMessage->set_receiveuserid(GDM->get_select_friend_id());
    //            }
    GDM->send_chat_message(*ChatMessage);
    
    //            if (GDM->get_chat_status() == 1)
    //                GDM->send_chat_message(msg::MessageType::Picture, GDM->get_select_group()->get_id(), pic_name);
    //            else if (GDM->get_chat_status() == 2)
    //                GDM->send_chat_message(msg::MessageType::Picture, GDM->get_select_friend_id(), pic_name);
}

void LayerChatOld::click_btn_send(Ref* sneder)
{
    btn_send_->setVisible(false);
    btn_emotion_->setEnabled(true);
    btn_emotion_->setVisible(true);
    
    btn_add_->setEnabled(true);
    btn_add_->setVisible(true);
    std::string content = tf_chat_->getText();
    if(!content.empty()) {
        if (content.size() > 300) {
            TipView::showAlertView("输入内容过长，请重新输入");
            return;
        }
        
        // by lvzhangle
        msg::ChatMessage * ChatMessage = new msg::ChatMessage();
        
        ChatMessage->mutable_userdetailinfo()->set_userid(GDM->get_user_id());
        
        ChatMessage->mutable_messageinfo()->mutable_textinfo()->set_text(content);
        ChatMessage->mutable_messageinfo()->set_type(msg::MessageType::Text);
        if (chat_status_ == 1){
            ChatMessage->set_groupid(group_id_);
        }else if (chat_status_ == 2){
            ChatMessage->set_receiveuserid(friend_id_);
        }
        
        
        GDM->send_chat_message(*ChatMessage);
        ChatMessage->Clear();
        delete ChatMessage;
    }
}

#pragma mark private
void LayerChatOld::register_events()
{
    dispatch_.register_event(EVENT_HANDLE_REFRESH_CHAT_RECORD,
                             BIND_FUNC(this, LayerChatOld::reload_data));
    dispatch_.register_event(EVENT_HANDLE_OUT_MEMBER_FINISHED,
                             BIND_FUNC(this, LayerChatOld::remove_from_parent));
    dispatch_.register_event(EVENT_HANDLE_EDIT_GROUP_INFO_FINISHED,
                             BIND_FUNC(this, LayerChatOld::update_data));
    dispatch_.register_event(EVENT_HANDLE_CLEAR_EDIT_CHAT,
                             BIND_FUNC(this, LayerChatOld::handle_send_msg_clear_editBox));
}

void LayerChatOld::remove_from_parent(cmd_data_pointer)
{
    this->removeFromParent();
}

void LayerChatOld::reload_data(cmd_data_pointer)
{
//    if (chat_status_ == 1){
//       bool is_top_layer = LayerPool::shared_instence().is_layer_chat_on_top();
//        if (is_top_layer)
//            GDM->clear_unreal_message_by_group_id(GDM->get_select_group()->get_id());
//        chats_ = GDM->get_current_group_chats();
//    }else if (chat_status_ == 2){
//        cocos2d::log("LayerChatOld : EVENT_HANDLE_REFRESH_FRIEND_CHAT_RECORD reload_data");
//        GDM->clear_friend_unread_message_by_user_id(GDM->get_select_friend()->get_userid());
//        chats_ = GDM->get_current_friend_chats();
//    }

//    listview_->removeAllItems();
//    for (auto& iter : chats_) {
//        auto cell = ChatMessageCell::create();
//        cell->listview = listview_;
//        cell->update_data(iter, listview_->getContentSize().width);
//        log("cellsize:%f, %f", cell->getContentSize().width, cell->getContentSize().height);
//        listview_->pushBackCustomItem(cell);
//        
//        cell->listview = listview_;
//        if (is_not_touch_)
//        {
//            cell->set_not_touch();
//        }
//    }
//    listview_->refreshView();
//    listview_->jumpToBottom();
    
    refresh_chat_message(REFRESH_MODE::RECEIVE_NEW_MESSAGE);
}

void LayerChatOld::refresh_chat_message(REFRESH_MODE set_refresh_mode)
{
    int add_message_count = 10; //刷新旧信息时的条数
    
    if (chat_status_ == 1)
    {
        bool is_top_layer = LayerPool::shared_instence().is_layer_chat_on_top();
        if (is_top_layer)
            GDM->clear_unreal_message_by_group_id(group_data_->groupid());
        chats_ = GDM->get_group_chats(group_data_->groupid());
    }else if (chat_status_ == 2)
    {
        GDM->clear_friend_unread_message_by_user_id(friend_id_);
        chats_ = GDM->get_friend_chats(friend_id_);
    }
    
    if (chats_.size() == 0) //清空了聊天记录的情况下
    {
        listview_->removeAllItems();
        listview_->refreshView();
        listview_->jumpToBottom();
        
        return;
    }

    for (auto chat_iter = listview_->getItems().begin(); chat_iter != listview_->getItems().end(); chat_iter++)
    {
        ChatMessageCell* cell = (ChatMessageCell*)(*chat_iter);
        if (cell->get_message()->changed()){
            cell->update_data(cell->get_message(), listview_->getContentSize().width);
            cell->get_message()->set_changed(false);
        }
    }
    
    switch (set_refresh_mode) {
        case REFRESH_MODE::REFRESH_OLD_MESSAGE : {
            if (listview_->getItems().size() == 0)
            {
                int chat_iter_count = 0;
                listview_->removeAllItems();
                for (auto chat_iter = chats_.rbegin(); chat_iter != chats_.rend(); chat_iter++) {
                    auto cell = ChatMessageCell::create();
                    cell->listview = listview_;
                    cell->update_data(*chat_iter, listview_->getContentSize().width);
                    
                    listview_->insertCustomItem(cell, 0);
                    
                    if (is_not_touch_)
                    {
                        cell->set_not_touch();
                    }
                    chat_iter_count++;
                    if (chat_iter_count >= add_message_count)
                        break;
                }
                listview_->refreshView();
                listview_->jumpToBottom();
            }else
            {
                double db_first_time;
                ChatMessageCell* cell = (ChatMessageCell*)listview_->getItems().front();
                std::string str_first_time = cell->get_message()->subtime();
                if (!str_first_time.empty())
                {
                    db_first_time = tools::stod(str_first_time);
                }
                
                int chat_iter_count = 0;
                for (auto chat_iter = chats_.rbegin(); chat_iter != chats_.rend(); chat_iter++)
                {
                    double db_iter_time;
                    std::string str_iter_time = (*chat_iter)->subtime();
                    if (!str_iter_time.empty())
                    {
                        db_iter_time = tools::stod(str_iter_time);
                    }
                    if (db_iter_time < db_first_time)
                    {
                        auto cell = ChatMessageCell::create();
                        cell->listview = listview_;
                        cell->update_data(*chat_iter, listview_->getContentSize().width);
                        log("cellsize:%f, %f", cell->getContentSize().width, cell->getContentSize().height);
                        listview_->insertCustomItem(cell, 0);
                        listview_->refreshView();
                        
                        if (is_not_touch_)
                        {
                            cell->set_not_touch();
                        }
                        
                        chat_iter_count++;
                    }
                    if (chat_iter_count >= add_message_count)
                        break;
                }
            }
            
            break;
        }
        case REFRESH_MODE::RECEIVE_NEW_MESSAGE : {
            if (listview_->getItems().size() != 0)
            {
                double db_last_time;
                ChatMessageCell* cell = (ChatMessageCell*)listview_->getItems().back();
                std::string str_last_time = cell->get_message()->subtime();
                if (!str_last_time.empty())
                {
                    db_last_time = tools::stod(str_last_time);
                }
                
                for (auto chat_iter = chats_.begin(); chat_iter != chats_.end(); chat_iter++)
                {
                    double db_iter_time;
                    std::string str_iter_time = (*chat_iter)->subtime();
                    if (!str_iter_time.empty())
                    {
                        db_iter_time = tools::stod(str_iter_time);
                    }
                    
                    if (db_iter_time > db_last_time)
                    {
                        auto cell = ChatMessageCell::create();
                        cell->listview = listview_;
                        cell->update_data(*chat_iter, listview_->getContentSize().width);
                        log("cellsize:%f, %f", cell->getContentSize().width, cell->getContentSize().height);
                        listview_->pushBackCustomItem(cell);
                        
                        if (is_not_touch_)
                        {
                            cell->set_not_touch();
                        }
                    }
                }
                
                listview_->refreshView();
                listview_->jumpToBottom();
            }else
            {
                for (auto& iter : chats_) {
                    auto cell = ChatMessageCell::create();
                    cell->listview = listview_;
                    cell->update_data(iter, listview_->getContentSize().width);
                    log("cellsize:%f, %f", cell->getContentSize().width, cell->getContentSize().height);
                    listview_->pushBackCustomItem(cell);
                    
                    if (is_not_touch_)
                    {
                        cell->set_not_touch();
                    }
                }
                listview_->refreshView();
                listview_->jumpToBottom();
            }


            break;
        }
        default:{
            break;
        }
    }
}

#pragma mark tableview datasource --
Size LayerChatOld::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    auto iter = cell_sizes_.find(idx);
    if(iter != cell_sizes_.end()) {
        return iter->second;
    }
    return Size(DESIGN_WIDTH, 140);
};

TableViewCell* LayerChatOld::tableCellAtIndex(TableView *table, ssize_t idx)
{
    //    ChatMessageCell* cell = dynamic_cast<ChatMessageCell*>(table->dequeueCell());
    //    if(!cell) {
    //        cell = ChatMessageCell::create();
    //    }
    //    auto& data = chats_.at(idx);
    //    cell->update_data(data);
    //
    //    cell_sizes_[idx] = cell->getSize();
    //
    //    return cell;
}


ssize_t LayerChatOld::numberOfCellsInTableView(TableView *table)
{
    //    return chats_.size();
}


#pragma mark tableview delegate --
void LayerChatOld::tableCellTouched(TableView* table, TableViewCell* cell)
{
    log("LayerChatOld::tableCellTouched");
}


#pragma mark EditBox delegate
void LayerChatOld::editBoxEditingDidBegin(EditBox* editBox)
{
}
void LayerChatOld::editBoxEditingDidEnd(EditBox* editBox)
{
    isSize = false;
    
}
void LayerChatOld::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    if (text.size() != 0) {
//        btn_send_->setVisible(true);
//        
//        btn_emotion_->setEnabled(false);
//        btn_emotion_->setVisible(false);
//        
//        btn_add_->setEnabled(false);
//        btn_add_->setVisible(false);
//    } else {
//        btn_send_->setVisible(false);
//        
//        btn_emotion_->setEnabled(true);
//        btn_emotion_->setVisible(true);
//        
//        btn_add_->setEnabled(true);
//        btn_add_->setVisible(true);
//    }
//#endif
    if (text.size() >= 280 && !isSize) {
        isSize = true;
        m_bIsTouchedLayout = true;
        TipView::showAlertView(tools::local_string("chat_alert", "输入内容已达到最多，继续输入将无法发送"));
    }

}
void LayerChatOld::editBoxReturn(EditBox* editBox)
{
    if(!m_bIsTouchedLayout)
    {
        click_btn_send(nullptr);
    }
}

void LayerChatOld::handle_send_msg_clear_editBox(cmd_data_pointer)
{
    tf_chat_->setText("");
}

void LayerChatOld::keyBoardWillShow(const float duration, const float distance)
{
//    btn_send_->setVisible(true);
    cocos2d::log("LayerChatOld::keyBoardWillShow");
    float dis = distance;
    //    if (dis < 0.0f) dis = 0.0f;
    
    //    log("distance:%f", distance);
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    //    dis *= glview->getScaleY();
    
    //    auto& size = Director::getInstance()->getWinSize();
    //    float scale = glview->getDesignResolutionSize().height/glview->getScreenSize().height;
    //    dis *= scale;
    if(SceneManager::get_landscape()) {
        dis *= 2;
        dis += 20;
    }
    
    
    log("dis:%f", dis);
    
    auto& items = listview_->getItems();
    if(!items.empty()) {
        auto back = items.back();
        Point p = back->convertToWorldSpace(Point::ZERO);
        if(p.y < dis + tf_chat_->getContentSize().height) {
            listview_->setBackGroundColor(Color3B::ORANGE);
            Point p1 = listview_->getPosition() + Point(0, dis - p.y + tf_chat_->getContentSize().height + back->getContentSize().height/2);
            listview_->stopAllActions();
            listview_->runAction(MoveTo::create(duration, p1));
        }
    }
    
    
    
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != listview_) {
            Point p = iter.first->getPosition() + Point(0, dis);
            iter.first->stopAllActions();
            iter.first->runAction(MoveTo::create(duration, p));
        }
    }
    
    
}
void LayerChatOld::keyBoardWillHide(const float duration, const float distance)
{
    btn_send_->setVisible(false);
    cocos2d::log("LayerChatOld::keyBoardWillHide");
    float dis = distance;
    if (dis < 0.0f) dis = 0.0f;
    //    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    //    dis *= glview->getScaleY();
    
    for (auto& iter : node_about_textfileds_) {
        iter.first->stopAllActions();
        iter.first->runAction(MoveTo::create(duration, iter.second));
    }
    //    auto iter = node_about_textfileds_.find(listview_);
    //    if(iter != node_about_textfileds_.end()) {
    //        listview_->runAction(MoveTo::create(duration, Point(0, iter->second)));
    //    }
}

void LayerChatOld::show_keyboard()
{
    tf_chat_->setVisible(true);
    GameTools::editBoxLoseInputFocus();
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != listview_) {
            iter.first->setVisible(true);
        }
    }
    click_btn_keyboard(btn_keyboard_);
    
    tf_chat_->touchDownAction(tf_chat_, cocos2d::ui::Widget::TouchEventType::ENDED);
    
}

void LayerChatOld::remove_emotion_layer()
{
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != listview_) {
            iter.first->setVisible(true);
        }
    }
    
    this->keyBoardWillHide(0.2f, 0);
    click_btn_keyboard(btn_keyboard_);
}

//void LayerChatOld::setScale(float scale)
//{
//    BaseLayer::setScale(scale);
//    listview_->refreshView();
//}

void LayerChatOld::onExit()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AM->stopAll();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    SoundPlayerInterface::stop_play();
#endif
    if (chat_status_ == 1)
    {
        GDM->GameDataManager::send_search_group(msg::GroupSearchType::SEARCHMINE, ""); //by liujia
    }else if (chat_status_ == 2){
        
    }
    LayerPop::onExit();
}

//void LayerChatOld::delete_LayerChatOld()
//{
//    if (__glc__)
//    {
//        __glc__ = nullptr;
//    }
//}