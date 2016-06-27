//
//  LayerGameChat.cpp
//  pokerclient
//
//  Created by zhongyu on 16/5/18.
//
//

#include "LayerGameChat.h"
#include "GameTools.h"
#include "utils.h"
#include "VoiceRecorderInterface.h"
#include "NET_CMD.h"

#include "GameDataManager.h"
#include "AudioManager.h"
#include "FileManager.h"
#include "ChatMessageManager.h"
#include "ProcessorManager.h"

#include "ChatMessageCell.h"
#include "RecordAnimationLayer.h"
#include "LayerEmotion.h"
#include "TipView.h"
#include "LayerChoosePhoto.h"
#include "LayerScoreList.h"
#include "LayerChooseGameRoom.h"
#include "LayerGroupSetting.h"
#include "LayerUserInfo.h"
#include "Toast.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "SoundPlayerInterface.h"
#endif

#define ANIMATION_LAYER_TAG 1

#define ZORDER_BG 0
#define ZORDER_TABLEVIEW 1
#define ZORDER_BUTTON 2
#define ZORDER_CHAT_BAR_BG 3
#define ZORDER_CHAT_INPUT_BG 4
#define ZORDER_CHAT_INPUT 5
#define ZORDER_CHAT_BUTTON 6
#define ZORDER_EMOTION 7
#define ZORDER_RECORD_ANIMATION 8
#define ZORDER_NEW_LAYER 9

LayerGameChat* LayerGameChat::createByGroup(const msg::GroupInfo & group_info)
{
    LayerGameChat* result = new LayerGameChat;
    if(result->init()) {
        result->autorelease();
        result->update_chat_status_by_group(group_info);
        return result;
    }
    return nullptr;
}
LayerGameChat* LayerGameChat::createByGroup(const uint32_t group_id)
{
    LayerGameChat* result = new LayerGameChat;
    if(result->init()) {
        result->autorelease();
        result->update_chat_status_by_group(group_id);
        return result;
    }
    return nullptr;
}
LayerGameChat* LayerGameChat::createByGroup()
{
    LayerGameChat* result = new LayerGameChat;
    if(result->init()) {
        result->autorelease();
        result->update_chat_status_by_group();
        return result;
    }
    return nullptr;
}
bool LayerGameChat::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerOfcChat.csb");
    this->addChild(root_);
    
    group_id_ = 0;
    
    image_chat_bar_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_chat_bar_bg"));
    image_chat_bar_bg_->setLocalZOrder(ZORDER_CHAT_BAR_BG);
    
    image_ofc_chat_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_ofc_chat_bg"));
//    image_ofc_chat_bg_->setLocalZOrder(ZORDER_CHAT_BAR_BG);

    
    btn_voice_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_voice"));
    btn_voice_->addClickEventListener(std::bind(&LayerGameChat::click_btn_voice, this, std::placeholders::_1));
    btn_voice_->setLocalZOrder(ZORDER_CHAT_BUTTON);
    
    btn_keyboard_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_keyboard"));
    btn_keyboard_->addClickEventListener(std::bind(&LayerGameChat::click_btn_keyboard, this, std::placeholders::_1));
    btn_keyboard_->setLocalZOrder(ZORDER_CHAT_BUTTON);
    
    btn_add_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_add"));
    btn_add_->addClickEventListener(std::bind(&LayerGameChat::click_btn_add, this, std::placeholders::_1));
    btn_add_->setLocalZOrder(ZORDER_CHAT_BUTTON);
    
    btn_chat_emotion_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_chat_emotion"));
    btn_chat_emotion_->addClickEventListener(std::bind(&LayerGameChat::click_btn_chat_emotion, this, std::placeholders::_1));
    btn_chat_emotion_->setLocalZOrder(ZORDER_CHAT_BUTTON);
    
    
    btn_record_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_record"));
    btn_record_->addTouchCallback(std::bind(&LayerGameChat::touch_btn_record, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    btn_record_->setLocalZOrder(ZORDER_CHAT_BUTTON);
    
    image_input_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_input_bg"));
    image_input_bg_->setLocalZOrder(ZORDER_CHAT_INPUT_BG);
    
    tf_input_= dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_input"));
    
    editbox_input_ = GameTools::createEditBoxFromTextField(this, tf_input_);
    editbox_input_->setReturnType(EditBox::KeyboardReturnType::SEND);
    editbox_input_->setInputMode(EditBox::InputMode::SINGLE_LINE);
    editbox_input_->setDelegate(this);
    editbox_input_->setMaxLength(250);
    //editbox_input_->setPlaceholderFontColor(cocos2d::Color3B(151, 151, 151));
    editbox_input_->setFontColor(cocos2d::Color3B(255, 255, 255));
    editbox_input_->addClickEventListener([this](Ref*){
        isTouchLayerout_ = false;
    });
    editbox_input_->setLocalZOrder(ZORDER_CHAT_INPUT);
    tf_input_->removeFromParentAndCleanup(true);
    
    text_chat_status_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_chat_status"));
    text_chat_status_->setVisible(false);
    text_chat_status_->setLocalZOrder(ZORDER_NEW_LAYER);
    text_chat_status_->setString(tools::local_string("clud_mute","禁言"));
    
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "ListView_chat"));
    tableview_ = TableView::create(this, listview_->getContentSize());
    tableview_->setAnchorPoint(listview_->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    
    tableview_->setPosition(listview_->getPosition());
    tableview_->setDelegate(this);
    
    //    auto layoutView = Layout::create();
    //    layoutView->setContentSize(tableview_->getContentSize());
    //    layoutView->setTouchEnabled(true);
    //    layoutView->setAnchorPoint(tableview_->getAnchorPoint());
    //    layoutView->setSwallowTouches(true);
    //    layoutView->setPosition(tableview_->getPosition());
    //    layoutView->setClippingEnabled(true);
    //    this->addChild(layoutView, ZORDER_TABLEVIEW);
    //    layoutView->addChild(tableview_);
    
    this->addChild(tableview_, ZORDER_TABLEVIEW);
    listview_->removeFromParent();
    
    
    //    tableview_->touchBeganCallback_ = std::bind(&LayerChat::ChatViewOnTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
    //    tableview_->touchMovedCallback_ = std::bind(&LayerChat::ChatViewOnTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    //    tableview_->touchEndedCallback_ = std::bind(&LayerChat::ChatViewOnTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
    //    tableview_->touchCancelledCallback_ = std::bind(&LayerChat::ChatViewOnTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerGameChat::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerGameChat::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerGameChat::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->setSwallowTouches(false);
    layout->addClickEventListener([this](Ref*){
        isTouchLayerout_ = true;
        GameTools::editBoxLoseInputFocus();
    });
    
    this->addChild(layout, 10);
    
    node_about_textfileds_.insert(std::make_pair(btn_voice_, btn_voice_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_chat_emotion_, btn_chat_emotion_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(editbox_input_, editbox_input_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(image_input_bg_, image_input_bg_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(tableview_, tableview_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_keyboard_, btn_keyboard_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_record_, btn_record_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_add_, btn_add_->getPosition()));
    
    click_btn_keyboard(nullptr);
    GameTools::editBoxLoseInputFocus();
    
    //没有获取到信息之前功能键无效
    has_info_ = false;
    
    register_events();
    
    return true;
}

void LayerGameChat::set_is_allow_chat(bool isAllowChat)
{
//    isAllowChat = true;
    if (isAllowChat)
    {
        btn_keyboard_->setVisible(false);
        btn_voice_->setVisible(true);
        
        btn_record_->setVisible(false);
        
        image_input_bg_->setVisible(true);
        editbox_input_->setVisible(true);
        
        btn_chat_emotion_->setVisible(true);
        btn_add_->setVisible(true);
        
        text_chat_status_->setVisible(false);
    }else
    {
        btn_keyboard_->setVisible(false);
        btn_voice_->setVisible(false);
        
        btn_record_->setVisible(false);
        
        editbox_input_->setVisible(false);
        image_input_bg_->setVisible(false);
        
        btn_chat_emotion_->setVisible(false);
        btn_add_->setVisible(false);
        
        text_chat_status_->setVisible(true);
    }
}

void LayerGameChat::update_chat_status_by_group()
{
    chat_message_datas_.clear();
}

void LayerGameChat::update_chat_status_by_group(const msg::GroupInfo & group_info)
{
    update_chat_status_by_group();
    group_info_ = group_info;
    group_id_ = group_info_.groupid();
//    refresh_group_info_view();
    set_is_allow_chat(group_info_.allowchat());
    update_chat_message_datas();
    send_get_group_info();
//    send_get_room_info();
}
void LayerGameChat::update_chat_status_by_group(const uint32_t group_id)
{
    update_chat_status_by_group();
    group_id_ = group_id;
    msg::GroupInfo nullgroup;
    nullgroup.set_groupid(group_id_);
    group_info_.CopyFrom(nullgroup);
    set_is_allow_chat(false);
    update_chat_message_datas();
    send_get_group_info();
//    send_get_room_info();
}

void LayerGameChat::update_chat_message_datas()
{
    CM->clear_unreal_message_by(group_id_, ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    std::vector<msg::ChatMessage> vec_message;
    const msg::ChatMessages* chatmessages = CM->get_chats(group_id_, ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    if (chatmessages)
    {
        for (auto messageIter : chatmessages->message())
        {
            vec_message.push_back(messageIter);
        }
    }
    update_chat_message_datas(vec_message);
}
void LayerGameChat::update_chat_message_datas(const std::vector<msg::ChatMessage> & chat_message_datas)
{
    chat_message_datas_ = chat_message_datas;
    refresh_tableview();
}

void LayerGameChat::send_chatmessage_text(std::string chat_text)
{
    msg::ChatMessage sendMessage;
    sendMessage.mutable_messageinfo()->set_type(msg::MessageType::Text);
    sendMessage.mutable_messageinfo()->mutable_textinfo()->set_text(chat_text);
    sendMessage.set_groupid(group_id_);
    CM->send_chat_message(sendMessage);
}

void LayerGameChat::send_chatmessage_picture(std::string pic_file_name)
{
    msg::ChatMessage sendMessage;
    sendMessage.mutable_messageinfo()->set_type(msg::MessageType::Picture);
    sendMessage.mutable_messageinfo()->mutable_picinfo()->set_picname(pic_file_name);
    sendMessage.set_groupid(group_id_);
    CM->send_chat_message(sendMessage);
}

void LayerGameChat::send_chatmessage_voice(std::string voice_file_name, int voice_time)
{
    msg::ChatMessage sendMessage;
    
    sendMessage.mutable_messageinfo()->set_type(msg::MessageType::Voice);
    sendMessage.mutable_messageinfo()->mutable_voiceinfo()->set_voicename(voice_file_name);
    sendMessage.mutable_messageinfo()->mutable_voiceinfo()->set_voicetime(voice_time);
    sendMessage.set_groupid(group_id_);
    CM->send_chat_message(sendMessage);
}
bool LayerGameChat::is_display_time(uint32_t idx)
{
    if (idx == 0)
    {
        return true;
    }
    
    bool is_display_time = false;
    double current_time = 0.0;
    std::string submit_time = chat_message_datas_[0].subtime();
    if(!submit_time.empty()) {
        current_time = tools::stod(submit_time);
    }
    
    for (int i = 0; i < idx; i++)
    {
        submit_time = chat_message_datas_[i].subtime();
        if(!submit_time.empty()) {
            double iter_time = tools::stod(submit_time);
            if (iter_time - current_time - (10 * 60 * 1000) > 0)
            {
                //cocos2d::log("CurrentTime > IterTime idx:%u", idx);
                current_time = iter_time;
            }
        }
    }
    
    double idx_time;
    submit_time = chat_message_datas_[idx].subtime();
    if(!submit_time.empty()) {
        double idx_time = tools::stod(submit_time);
        //cocos2d::log("idx_time %s", tools::time_display(idx_time / 1000).c_str());
        if (idx_time - current_time - (10 * 60 * 1000) > 0)
        {
            is_display_time = true;
        }
    }
    
    
    cocos2d::log("***IsDisplayTime %s, return %s  ***", tools::time_display(idx_time / 1000).c_str(), is_display_time ? "true" : "false");
    return is_display_time;
}


void LayerGameChat::RefreshLayer()
{
    send_get_group_info();
//    send_get_room_info();

}

#pragma mark click_event
void LayerGameChat::click_btn_keyboard(Ref * sender)
{
    btn_keyboard_->setVisible(false);
    btn_voice_->setVisible(true);
    btn_record_->setVisible(false);
    //tf_chat_->setPositionY(m_pEditBoxPosition.y);
    editbox_input_->setVisible(true);
}

void LayerGameChat::touch_btn_record(Ref* sender, Touch* touch, Widget::TouchEventType event)
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
                addChild(record_animation_layer_, ZORDER_RECORD_ANIMATION, ANIMATION_LAYER_TAG);
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
                send_chatmessage_voice(record_filename_, record_time);
                
                //                ChatMessage->mutable_userdetailinfo()->set_userid(GDM->get_user_id());
                //                ChatMessage->mutable_userdetailinfo()->set_nickname(GDM->get_user_data()->get_nickname());
                //                ChatMessage->mutable_userdetailinfo()->set_picname(GDM->get_user_data()->get_head_pic_name());
                //                ChatMessage->mutable_userdetailinfo()->set_sex(GDM->get_user_data()->get_sex());
                //
                //                ChatMessage->mutable_messageinfo()->mutable_voiceinfo()->set_voicename(record_filename_);
                //                ChatMessage->mutable_messageinfo()->mutable_voiceinfo()->set_voicetime(record_time);
                //                ChatMessage->mutable_messageinfo()->set_type(msg::MessageType::Voice);
                //                if (GDM->get_chat_status() == 1){
                //                    ChatMessage->set_groupid(GDM->get_select_group_id());
                //                }else if (GDM->get_chat_status() == 2){
                //                    ChatMessage->set_receiveuserid(GDM->get_select_friend_id());
                //                }
                //                GDM->send_chat_message(*ChatMessage);
                
                
            }
            else {
                Toast::ToastShow(tools::local_string("record_time_too_shop", "录音时间太短"));
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

void LayerGameChat::click_btn_voice(Ref * sender)
{
    btn_record_->setTitleText(tools::local_string("press_voice", "按住  说话"));
    btn_voice_->setVisible(false);
    btn_keyboard_->setVisible(true);
    btn_record_->setVisible(true);
    //    m_pEditBoxPosition = tf_chat_->getPosition();
    //    tf_chat_->setPositionY(-100);
    editbox_input_->setVisible(false);
}

void LayerGameChat::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}

void LayerGameChat::click_btn_chat_emotion(Ref * sender)
{
    editbox_input_->setVisible(false);
    LayerEmotion* el = LayerEmotion::create();
    el->set_dismiss_callback(std::bind(&LayerGameChat::remove_emotion_layer, this));
    el->set_voice_callback([this](){
        for (auto& iter : node_about_textfileds_) {
            if(iter.first != tableview_) {
                iter.first->setVisible(true);
            }
        }
        this->keyBoardWillHide(0.2f, 0);
        this->click_btn_voice(nullptr);
        
    });
    //el->set_chat_callback(std::bind(&LayerChat::show_keyboard, this));
    el->set_chat_callback([=](){
        this->remove_emotion_layer();
        this->show_keyboard();
    });
    el->set_emoition_callback(std::bind(&LayerGameChat::send_chatmessage_picture, this, std::placeholders::_1));
    
    this->addChild(el, ZORDER_EMOTION);
    
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != tableview_) {
            iter.first->setVisible(false);
        }
    }
    
    
    float dis = 425;
    //    auto& items = listview_->getItems();
    //    if(!items.empty()) {
    //        auto back = items.back();
    //        Point p = back->getPosition() + node_about_textfileds_.at(tableview_);
    //        if(p.y < dis) {
    //            Point p1 = node_about_textfileds_.at(listview_) + Point(0, dis - p.y + editbox_input_->getContentSize().height + 20);
    //            tableview_->stopAllActions();
    //            tableview_->runAction(MoveTo::create(0.2f, p1));
    //        }
    //    }
    
    tableview_->stopAllActions();
//    tableview_->runAction(MoveTo::create(0.2f, cocos2d::Point(tableview_->getPositionX(), tableview_->getPositionY() + dis)));
}

void LayerGameChat::click_btn_add(Ref * sender)
{
    editbox_input_->setVisible(false);
    LayerChoosePhoto* as = LayerChoosePhoto::create();
    as->setIsChat(true);
    as->set_choose_photo_callback(std::bind(&LayerGameChat::send_chatmessage_picture, this, std::placeholders::_1));
    as->set_dismiss_callback([this](){
        editbox_input_->setVisible(true);
        GameTools::editBoxLoseInputFocus();
    });
    this->addChild(as, ZORDER_NEW_LAYER);
}

#pragma mark tableview datasource --
TableViewCell* LayerGameChat::tableCellAtIndex(TableView *table, ssize_t idx)
{
    ChatMessageCell * cell = dynamic_cast<ChatMessageCell*>(table->dequeueCell());
    if (!cell)
    {
        cell = ChatMessageCell::create();
    }
    cell->update_data(chat_message_datas_[idx], tableview_->getContentSize().width, is_display_time(idx));
    
    return cell;
}

Size LayerGameChat::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    msg::ChatMessage chat_data = chat_message_datas_[idx];
    
    float cellOffSet = idx == 0 ? MESSAGE_CELL_OFFSET : MESSAGE_CELL_OFFSET * 2;
    
    switch (chat_data.messageinfo().type()) {
        case msg::MessageType::Text:
        {
            std::string strMessage = chat_data.messageinfo().textinfo().text();
            
            RichText* richtext_message_ = RichText::create();
            
            richtext_message_->ignoreAnchorPointForPosition(true);
            
            if (!strMessage.empty())
            {
                std::u16string utf16String;
                StringUtils::UTF8ToUTF16(strMessage, utf16String);
                
                int intStrCur = 0;
                
                while (utf16String.length() - intStrCur > 125)
                {
                    std::u16string u16StrText;
                    
                    u16StrText = utf16String.substr(intStrCur, 125);
                    
                    std::string strText;
                    StringUtils::UTF16ToUTF8(u16StrText, strText);
                    
                    RichElementText * richEleText = RichElementText::create(0, cocos2d::Color3B::WHITE, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                    
                    richtext_message_->pushBackElement(richEleText);
                    
                    intStrCur += 125;
                }
                std::u16string u16StrText;
                
                u16StrText = utf16String.substr(intStrCur, utf16String.length());
                
                std::string strText;
                StringUtils::UTF16ToUTF8(u16StrText, strText);
                
                RichElementText * richEleText = RichElementText::create(0, cocos2d::Color3B::WHITE, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                
                richtext_message_->pushBackElement(richEleText);
            }
            
            
            ImageView* image_chatmessage_cell_bg_ = ImageView::create("login_res/chat_res/image_chatmessage_cell_bg.png");
            
            image_chatmessage_cell_bg_->setScale9Enabled(true);
            image_chatmessage_cell_bg_->setCapInsets(cocos2d::Rect(35, 30, 418, 30));
            
            richtext_message_->ignoreContentAdaptWithSize(true);
            richtext_message_->formatText();
            
            if (richtext_message_->getContentSize().width < MESSAGE_TEXT_BG_WIDTH_MAX - MESSAGE_TEXT_BG_OFFSET_LEFT - MESSAGE_TEXT_BG_OFFSET_RIGHT)
            {
                
            }else
            {
                richtext_message_->ignoreContentAdaptWithSize(false);
                richtext_message_->setContentSize(cocos2d::Size(MESSAGE_TEXT_BG_WIDTH_MAX - MESSAGE_TEXT_BG_OFFSET_LEFT - MESSAGE_TEXT_BG_OFFSET_RIGHT, 0));
                
                richtext_message_->formatText();
            }
            
            cocos2d::Size sizeRichText = richtext_message_->getVirtualRendererSize();
            
            cocos2d::Size sizeImageBg = sizeRichText;
            sizeImageBg.width = std::max(sizeRichText.width + MESSAGE_TEXT_BG_OFFSET_LEFT + MESSAGE_TEXT_BG_OFFSET_RIGHT, (float)MESSAGE_TEXT_BG_WIDTH_MIN);
            sizeImageBg.height = std::max(sizeRichText.height +  MESSAGE_TEXT_BG_OFFSET_TOP +  MESSAGE_TEXT_BG_OFFSET_BUTTOM, (float)MESSAGE_TEXT_BG_HEIGHT_MIN);
            
            Text* text_nick_name_ = Text::create(chat_data.userdetailinfo().nickname(), "", MESSAGE_NICK_NAME_FONT_SIZE);
            
            sizeImageBg.height = sizeImageBg.height + text_nick_name_->getContentSize().height + 5;
            
            //cocos2d::log("LayerChat::tableCellSizeForIndex %f, %f", 750.0, sizeImageBg.height);
            
            return cocos2d::Size(750, sizeImageBg.height + cellOffSet);
            
            break;
        }
        case msg::MessageType::Voice:
        {
            ImageView* image_voice_icon_ = ImageView::create();
            
            image_voice_icon_->loadTexture("login_res/chat_res/image_voice_icon_self.png");
            
            cocos2d::Size sizeImageMessageBg;
            sizeImageMessageBg.height = image_voice_icon_->getContentSize().height + MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM;
            
            Text* text_nick_name_ = Text::create(chat_data.userdetailinfo().nickname(), "", MESSAGE_NICK_NAME_FONT_SIZE);
            
            sizeImageMessageBg.height = sizeImageMessageBg.height + text_nick_name_->getContentSize().height + 5;
            
            cocos2d::log("LayerChat::tableCellSizeForIndex %f, %f", 750.0, sizeImageMessageBg.height);
            
            return cocos2d::Size(750, sizeImageMessageBg.height + cellOffSet);
            
            break;
        }
        case msg::MessageType::GroupNotice:
        {
            std::string strMessage = chat_data.messageinfo().groupnoticeinfo().text();
            
            RichText* richtext_message_ = RichText::create();
            
            richtext_message_->ignoreAnchorPointForPosition(true);
            
            if (!strMessage.empty())
            {
                std::u16string utf16String;
                StringUtils::UTF8ToUTF16(strMessage, utf16String);
                
                int intStrCur = 0;
                
                while (utf16String.length() - intStrCur > 125)
                {
                    std::u16string u16StrText;
                    
                    u16StrText = utf16String.substr(intStrCur, 125);
                    
                    std::string strText;
                    StringUtils::UTF16ToUTF8(u16StrText, strText);
                    
                    RichElementText * richEleText = RichElementText::create(0, cocos2d::Color3B::WHITE, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                    
                    richtext_message_->pushBackElement(richEleText);
                    
                    intStrCur += 125;
                }
                std::u16string u16StrText;
                
                u16StrText = utf16String.substr(intStrCur, utf16String.length());
                
                std::string strText;
                StringUtils::UTF16ToUTF8(u16StrText, strText);
                
                RichElementText * richEleText = RichElementText::create(0, cocos2d::Color3B::WHITE, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                
                richtext_message_->pushBackElement(richEleText);
            }
            
            
            ImageView* image_chatmessage_cell_bg_ = ImageView::create("login_res/chat_res/image_chatmessage_cell_bg.png");
            
            image_chatmessage_cell_bg_->setScale9Enabled(true);
            image_chatmessage_cell_bg_->setCapInsets(cocos2d::Rect(60, 80, 415, 30));
            image_chatmessage_cell_bg_->setEnabled(false);
            
            richtext_message_->ignoreContentAdaptWithSize(true);
            richtext_message_->formatText();
            
            if (richtext_message_->getContentSize().width < MESSAGE_BG_WIDTH_MAX - MESSAGE_BG_OFFSET_LEFT - MESSAGE_BG_OFFSET_RIGHT)
            {
                
            }else
            {
                richtext_message_->ignoreContentAdaptWithSize(false);
                richtext_message_->setContentSize(cocos2d::Size(MESSAGE_BG_WIDTH_MAX - MESSAGE_BG_OFFSET_LEFT - MESSAGE_BG_OFFSET_RIGHT, 0));
                
                richtext_message_->formatText();
            }
            
            cocos2d::Size sizeRichText = richtext_message_->getVirtualRendererSize();
            
            
            sizeRichText.height = std::max(sizeRichText.height+MESSAGE_BG_OFFSET_TOP + MESSAGE_BG_OFFSET_BUTTOM, (float)MESSAGE_BG_HEIGHT_MIN);
            
            std::string strTime;
            std::string submit_time = chat_data.subtime();
            if(!submit_time.empty()) {
                double dbTime = tools::stod(submit_time);
                dbTime /= 1000;
                strTime = tools::time_display(dbTime);
            }
            
            
            Text* text_message_time = Text::create(strTime, "", MESSAGE_MESSAGE_TIME_FONT_SIZE);
            
            sizeRichText.height = sizeRichText.height + text_message_time->getContentSize().height + 5;
            
            cocos2d::log("LayerChat::tableCellSizeForIndex %f, %f", 750.0, sizeRichText.height);
            
            return cocos2d::Size(750, sizeRichText.height);
            
            break;
        }
        case msg::MessageType::Picture:
        {
            std::string strTime;
            std::string submit_time = chat_data.subtime();
            if(!submit_time.empty()) {
                double dbTime = tools::stod(submit_time);
                dbTime /= 1000;
                strTime = tools::time_display(dbTime);
            }
            
            Text* text_message_time = Text::create(strTime, "", MESSAGE_MESSAGE_TIME_FONT_SIZE);
            
            auto vecImageFile = tools::split(chat_data.messageinfo().picinfo().picname(), "#");
            if (vecImageFile.size() > 0)
            {
                if (vecImageFile.size() == 1)
                {
                    std::string path = FileUtils::getInstance()->fullPathForFilename("emotion/" + vecImageFile[0]);
                    
                    Size s(120, 120);
                    if (path.empty())
                    {
                        //                        ImageView* emotion;
                        //                        if(GDM->is_file_exist(vecImageFile[0])) {
                        ////                            emotion =  ImageView::create(vecImageFile[0]);
                        ////                            Size s(120, 120);
                        ////                            Size contentSize = emotion->getContentSize();
                        ////                            s.height = s.width * (contentSize.height/contentSize.width);
                        ////                            if(contentSize.width > s.width || contentSize.height > s.height) {
                        ////                                emotion->ignoreContentAdaptWithSize(false);
                        ////                                emotion->setContentSize(s);
                        ////                            }
                        //
                        //                        }
                        //                        else {
                        ////                            emotion = ImageView::create("msg_pic.png");
                        ////                            Size s(120, 120);
                        ////                            Size contentSize = emotion->getContentSize();
                        ////                            if(contentSize.width > s.width || contentSize.height > s.height) {
                        ////                                emotion->ignoreContentAdaptWithSize(false);
                        ////                                emotion->setContentSize(s);
                        ////                            }
                        //
                        //                        }
                        return cocos2d::Size(750, s.height + MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM + text_message_time->getContentSize().height + 5 + cellOffSet);
                    }else
                    {
                        
                        return cocos2d::Size(750, s.height + MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM + text_message_time->getContentSize().height + 5 + cellOffSet);
                    }
                    
                }else
                {
                    cocos2d::Size sizeEmotion = cocos2d::Size(120, 120);
                    
                    int emotionColumn = (MESSAGE_TEXT_BG_WIDTH_MAX - MESSAGE_TEXT_BG_OFFSET_LEFT - MESSAGE_TEXT_BG_OFFSET_RIGHT) / sizeEmotion.width;
                    
                    int emotionLines = vecImageFile.size() / emotionColumn;
                    
                    if (vecImageFile.size() % emotionColumn != 0)
                    {
                        emotionLines++;
                    }
                    
                    float messageHeight = sizeEmotion.height * emotionLines;
                    
                    return cocos2d::Size(750, messageHeight + MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM + text_message_time->getContentSize().height + 5 + cellOffSet);
                    
                }
            }else
            {
                return cocos2d::Size(750, MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM + text_message_time->getContentSize().height + 5 + cellOffSet);
            }
            
            
            break;
        }
        case msg::MessageType::CreateRoom:
        {
            
            ImageView* image_room_icon = ImageView::create();
            
            switch (chat_data.messageinfo().createroominfo().type()) {
                case msg::RoomType::NLHOLDEM:
                {
                    image_room_icon->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                    break;
                }
                case msg::RoomType::OFC3:
                {
                    image_room_icon->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_ofc3.png");
                    break;
                }
                case msg::RoomType::OFC4:
                {
                    image_room_icon->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_ofc3.png");
                    break;
                }
                case msg::RoomType::PLO:
                {
                    image_room_icon->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                    break;
                }
                case msg::RoomType::PLOHILO:
                {
                    image_room_icon->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                    break;
                }
                default:
                {
                    break;
                }
            }
            
            
            cocos2d::Size sizeImageRoomIcon = image_room_icon->getContentSize();
            
            RichText* richtext_message_ = RichText::create();
            
            richtext_message_->ignoreAnchorPointForPosition(true);
            
            std::string strMessage = chat_data.messageinfo().createroominfo().roomname();
            
            strMessage += '\n';
            strMessage += '\n';
            strMessage += chat_data.messageinfo().createroominfo().text();
            
            cocos2d::Color3B colorText;
            colorText = cocos2d::Color3B(255, 255, 255);
            
            if (!strMessage.empty())
            {
                std::u16string utf16String;
                StringUtils::UTF8ToUTF16(strMessage, utf16String);
                
                int intStrCur = 0;
                
                while (utf16String.length() - intStrCur > 125)
                {
                    std::u16string u16StrText;
                    
                    u16StrText = utf16String.substr(intStrCur, 125);
                    
                    std::string strText;
                    StringUtils::UTF16ToUTF8(u16StrText, strText);
                    
                    RichElementText * richEleText = RichElementText::create(0, colorText, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                    
                    richtext_message_->pushBackElement(richEleText);
                    
                    intStrCur += 125;
                }
                std::u16string u16StrText;
                
                u16StrText = utf16String.substr(intStrCur, utf16String.length());
                
                std::string strText;
                StringUtils::UTF16ToUTF8(u16StrText, strText);
                
                RichElementText * richEleText = RichElementText::create(0, colorText, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                
                richtext_message_->pushBackElement(richEleText);
            }
            
            richtext_message_->ignoreContentAdaptWithSize(false);
            richtext_message_->setContentSize(cocos2d::Size(MESSAGE_BG_WIDTH_MAX - MESSAGE_BG_OFFSET_LEFT - MESSAGE_BG_OFFSET_RIGHT - sizeImageRoomIcon.width - 10, 0));
            
            richtext_message_->formatText();
            cocos2d::Size sizeRichText = richtext_message_->getVirtualRendererSize();
            
            cocos2d::Size sizeImageBg;
            
            sizeImageBg.height = std::max(sizeImageRoomIcon.height, sizeRichText.height) + MESSAGE_BG_OFFSET_TOP + MESSAGE_BG_OFFSET_BUTTOM;
            
            Text* text_nick_name_ = Text::create(chat_data.messageinfo().createroominfo().creater().nickname(), "", MESSAGE_NICK_NAME_FONT_SIZE);
            
            sizeImageBg.height = sizeImageBg.height + text_nick_name_->getContentSize().height + 5;
            
            
            return cocos2d::Size(750, sizeImageBg.height);
        }
    }
    
    return cocos2d::Size(750, 100);
}

ssize_t LayerGameChat::numberOfCellsInTableView(TableView *table)
{
    return chat_message_datas_.size();
}

#pragma mark tableview delegate --
void LayerGameChat::tableCellTouched(TableView* table, TableViewCell* cell)
{
    cocos2d::log("CellTouched Idx:%zd", cell->getIdx());
}

#pragma mark EditBox delegate
void LayerGameChat::editBoxEditingDidBegin(EditBox* editBox)
{
    
}

void LayerGameChat::editBoxEditingDidEnd(EditBox* editBox)
{
    
}
void LayerGameChat::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    isTouchLayerout_ = false;
}

#pragma mark OnTouchEvent
bool LayerGameChat::ChatViewOnTouchBegan(Touch * touch, Event* event)
{
    return true;
}

void LayerGameChat::ChatViewOnTouchMoved(Touch * touch, Event* event)
{
    
}

void LayerGameChat::ChatViewOnTouchEnded(Touch * touch, Event* event)
{
    
}

void LayerGameChat::ChatViewOnTouchCancelled(Touch * touch, Event* event)
{
    
}

void LayerGameChat::editBoxReturn(EditBox* editBox)
{
    if (!isTouchLayerout_)
    {
        std::string content = editbox_input_->getText();
        if(!content.empty()) {
            if (content.size() > 1000) {
                TipView::showAlertView(tools::local_string("too_many_info","输入内容过长，请重新输入"));
                return;
            }
            
            send_chatmessage_text(content);
            editbox_input_->setText("");
        }
    }
}

#pragma mark ViewFunc
void LayerGameChat::keyBoardWillShow(const float duration, const float distance)
{
    
    float dis = distance;
    
    if(SceneManager::get_landscape()) {
        dis *= 2;
        dis += 20;
    }
    
    //    auto& items = tableview_->getItems();
    //    if(!items.empty()) {
    //        auto back = items.back();
    //        Point p = back->convertToWorldSpace(Point::ZERO);
    //        if(p.y < dis + editbox_input_->getContentSize().height) {
    //            Point p1 = listview_->getPosition() + Point(0, dis - p.y + editbox_input_->getContentSize().height + back->getContentSize().height/2);
    //            tableview_->stopAllActions();
    //            tableview_->runAction(MoveTo::create(duration, p1));
    //        }
    //    }
    
    for (auto& iter : node_about_textfileds_) {
        //        if(iter.first != tableview_) {
        //            Point p = iter.first->getPosition() + Point(0, dis);
        //            iter.first->stopAllActions();
        //            iter.first->runAction(MoveTo::create(duration, p));
        //        }
        Point p = iter.first->getPosition() + Point(0, dis);
        iter.first->stopAllActions();
        iter.first->runAction(MoveTo::create(duration, p));
        
    }
}
void LayerGameChat::keyBoardWillHide(const float duration, const float distance)
{
    float dis = distance;
    if (dis < 0.0f) dis = 0.0f;
    
    for (auto& iter : node_about_textfileds_) {
        //        if(iter.first != tableview_) {
        //            iter.first->stopAllActions();
        //            iter.first->runAction(MoveTo::create(duration, iter.second));
        //        }
        iter.first->stopAllActions();
        iter.first->runAction(MoveTo::create(duration, iter.second));
    }
}

void LayerGameChat::show_keyboard()
{
    editbox_input_->setVisible(true);
    GameTools::editBoxLoseInputFocus();
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != tableview_) {
            iter.first->setVisible(true);
        }
    }
    click_btn_keyboard(btn_keyboard_);
    
    editbox_input_->touchDownAction(editbox_input_, cocos2d::ui::Widget::TouchEventType::ENDED);
    
}

void LayerGameChat::remove_emotion_layer()
{
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != tableview_) {
            iter.first->setVisible(true);
        }
    }
    
    click_btn_keyboard(btn_keyboard_);
    this->keyBoardWillHide(0.2f, 0);
    
}

void LayerGameChat::refresh_tableview()
{
    tableview_->reloadData();
    
    cocos2d::Size sizeContainer = tableview_->getContainer()->getContentSize();
    cocos2d::Size sizeTableView = tableview_->getViewSize();
    if (sizeContainer.height - sizeTableView.height > 1)
    {
        tableview_->setContentOffset(cocos2d::Point(0, 0));
    }
}

void LayerGameChat::register_events()
{
    dispatch_.register_event(REQ_GET_GROUP_INFO,
                             BIND_FUNC(this, LayerGameChat::handle_get_group_info));
    dispatch_.register_event(REQ_GET_ROOM_INFO,
                             BIND_FUNC(this, LayerGameChat::handle_get_room_info));
}

#pragma mark SEND
void LayerGameChat::send_get_room_info()
{
    if (group_id_ != 0)
    {
        auto processor = PM->GetRoomInfo_UP(group_id_);
        send_data(processor, REQ_GET_ROOM_INFO);
    }
}

void LayerGameChat::send_get_group_info()
{
    if (group_id_ != 0)
    {
        auto processor = PM->GetGroupInfo_UP(group_id_);
        send_data(processor, REQ_GET_GROUP_INFO);
    }
}

#pragma mark HANDLE
void LayerGameChat::handle_get_room_info(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    msg::Processor_150_GetRoomInfo_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS: {
            uint32_t uintRoomCount = processor.roominfo().size();
            break;
        }
        case msg::Status::FAILED: {
            //TipView::showAlertView(tools::local_string("get_room_list_failed", "获取房间列表失败"));
            break;
        }
        default:
            break;
    }
}
void LayerGameChat::handle_get_group_info(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    msg::Processor_145_GetGroupInfo_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS: {
            if (processor.groupinfo().groupid() == group_id_)
            {
                group_info_.CopyFrom(processor.groupinfo());
                set_is_allow_chat(group_info_.allowchat());
            }
            break;
        }
        case msg::Status::FAILED: {
            //TipView::showAlertView(processor.response().message());
            break;
        }
    }
}

#pragma mark OnExit
void LayerGameChat::onExit()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AM->stopAll();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    if(SoundPlayerInterface::is_playing()) {
        SoundPlayerInterface::stop_play();
    }
    
#endif
    BaseLayer::onExit();
}

void LayerGameChat::onTouchEnded(Touch* touch, Event* event)
{
    //将openGL坐标转化为node坐标
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    if ( image_ofc_chat_bg_->getPositionY()+image_ofc_chat_bg_->getBoundingBox().size.height/2 >= touchLocation.y )
    {
        
    }else
    {
        this->removeFromParent();
    }
}