#include "GameChatLayer.h"
#include "GameTools.h"
#include "EmotionLayer.h"
#include "GameDataManager.h"
#include "ChatMessageCell.h"
#include "GameChatInputLayer.h"
#include "cmd_def.h"
#include "FileManager.h"
#include "VoiceRecorderInterface.h"
#include "TipView.h"
#include "RecordAnimationLayer.h"
#include "AudioManager.h"
#include "ActionSheet.h"
#include "utils.h"
#include "user_data.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "SoundPlayerInterface.h"
#endif

#define ANIMATION_LAYER_TAG 1

bool GameChatLayer::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    auto layout = Layout::create();
    layout->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(layout, -1);
    layout->setTouchEnabled(true);
    layout->addClickEventListener([=](Ref *){
        this->removeFromParent();
        set_isShow_chat(false);
    });
    
    auto root = CSLoader::createNode("LayerGameChatII.csb");
    this->addChild(root);
    
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    btn_record_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_record"));
    btn_emtion_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_emotion"));
    btn_add_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_add"));
    text_unread_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_unread_num"));
    bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "bg"));
    btn_keyboard_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_keyboard"));
    
    btn_tf_bg_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_tf_bg"));
    btn_tf_bg_->addClickEventListener(std::bind(&GameChatLayer::click_image_tf_bg, this, std::placeholders::_1));
    btn_tf_bg_->addTouchCallback(std::bind(&GameChatLayer::touch_btn_tf_bg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    btn_record_->addClickEventListener(std::bind(&GameChatLayer::click_btn_record, this, std::placeholders::_1));
    btn_emtion_->addClickEventListener(std::bind(&GameChatLayer::click_btn_emtion, this, std::placeholders::_1));
    btn_add_->addClickEventListener(std::bind(&GameChatLayer::click_btn_add, this, std::placeholders::_1));
    btn_keyboard_->addClickEventListener([=](Ref*){
        btn_keyboard_->setVisible(false);
        btn_record_->setVisible(true);
        btn_tf_bg_->setTitleText("");
    });
    btn_keyboard_->setVisible(false);
    
    is_not_touch_ = true;
    
    listview_->addTouchCallback(std::bind(&GameChatLayer::touch_listview, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    refresh_chat_message(REFRESH_MODE::REFRESH_OLD_MESSAGE);
    
    register_event();
    set_isShow_chat(true);
    return true;
}

void GameChatLayer::touch_listview(Ref* sender, Touch* touch, Widget::TouchEventType event)
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

void GameChatLayer::click_btn_record(Ref* sender)
{
    btn_tf_bg_->setTitleText(tools::local_string("press_voice", "按住  说话"));
    btn_record_->setVisible(false);
    btn_keyboard_->setVisible(true);
}
void GameChatLayer::click_btn_emtion(Ref* sender)
{
    EmotionLayer* el = EmotionLayer::create();
    el->setScale(bg_->getContentSize().width / DESIGN_WIDTH);
    el->setAnchorPoint(Point(0, 0));
    el->setPosition(bg_->getPosition().x, 0);
    
    
    el->set_chat_callback([this](){
        GameChatInputLayer* gcil = GameChatInputLayer::create();
        this->addChild(gcil);
    });
    el->set_voice_callback([this](){
        this->click_btn_record(nullptr);
    });
    
    this->addChild(el);
}
void GameChatLayer::click_btn_add(Ref* sender)
{
    ActionSheet* as = ActionSheet::create();
    as->set_after_choose_pic_callback(std::bind(&GameChatLayer::after_choose_pic, this, std::placeholders::_1));
    this->addChild(as);
}

void GameChatLayer::after_choose_pic(const std::string& filename)
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

void GameChatLayer::click_image_tf_bg(Ref* sender)
{
    //    if(btn_keyboard_->isVisible()) return;
    GameChatInputLayer* gcil = GameChatInputLayer::create();
    this->addChild(gcil);
}

void GameChatLayer::touch_btn_tf_bg(Ref* sender, Touch* touch, Widget::TouchEventType event)
{
    if(btn_record_->isVisible()) {
        return;
    }
    
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
            btn_tf_bg_->setTitleText(tools::local_string("release_send", "松开  发送"));
            RecordAnimationLayer* record_animation_layer_ = (RecordAnimationLayer*)getChildByTag(ANIMATION_LAYER_TAG);
            if(!record_animation_layer_) {
                record_animation_layer_ = RecordAnimationLayer::create();
                this->addChild(record_animation_layer_, 0, ANIMATION_LAYER_TAG);
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
            if(record_time != 0) {
                const std::string upload_filename = record_filename_ + ".amr";
                FM.upload_file(upload_filename);
                // by lvzhangle
                msg::ChatMessage * ChatMessage = new msg::ChatMessage();
                
                ChatMessage->mutable_userdetailinfo()->set_userid(GDM->get_user_id());
                
                ChatMessage->mutable_messageinfo()->mutable_voiceinfo()->set_voicename(record_filename_);
                ChatMessage->mutable_messageinfo()->mutable_voiceinfo()->set_voicetime(record_time);
                ChatMessage->mutable_messageinfo()->set_type(msg::MessageType::Voice);
                ChatMessage->set_groupid(groupid);
                
                GDM->send_chat_message(*ChatMessage);
                
                //GDM->send_chat_message(msg::MessageType::Voice, GDM->get_select_group_id(), record_filename_, record_time);
            }else {
                TipView::showAlertView(tools::local_string("record_time_too_shop", "录音时间太短"));
            }
            record_filename_ = "";
            btn_tf_bg_->setTitleText(tools::local_string("press_voice", "按住  说话"));
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
            btn_tf_bg_->setTitleText(tools::local_string("press_voice", "按住  说话"));
            AM->end_recording();
            break;
    }
    
}

void GameChatLayer::reload_data(cmd_data_pointer)
{
    //    GDM->clear_unreal_message_by_group_id(GDM->get_select_group_id());
    //    chats_ = GDM->get_current_group_chats();
    //
    //    listview_->removeAllItems();
    //    for (auto& iter : chats_) {
    //        auto cell = ChatMessageCell::create();
    //        cell->listview = listview_;
    //        cell->update_data(iter, listview_->getContentSize().width, true);
    //        cell->listview = listview_;
    //        if (is_not_touch_)
    //        {
    //            cell->set_not_touch();
    //        }
    //        listview_->pushBackCustomItem(cell);
    //    }
    //    listview_->refreshView();
    //    listview_->jumpToBottom();
    refresh_chat_message(REFRESH_MODE::RECEIVE_NEW_MESSAGE);
}

void GameChatLayer::refresh_chat_message(REFRESH_MODE set_refresh_mode)
{
    int add_message_count = 10; //刷新旧信息时的条数
    
    GDM->clear_unreal_message_by_group_id(groupid);
    chats_ = GDM->get_group_chats(groupid);
    
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

void GameChatLayer::register_event()
{
    dispatch_.register_event(EVENT_HANDLE_REFRESH_CHAT_RECORD,
                             BIND_FUNC(this, GameChatLayer::reload_data));
}

void GameChatLayer::onExit()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AM->stopAll();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    SoundPlayerInterface::stop_play();
#endif
    Layer::onExit();
}