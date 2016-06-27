//
//  OfcBackGroundLayer.cpp
//  pokerclient
//
//  Created by ll on 15/7/27.
//
//

#include "OfcBackGroundLayer.h"

#include "MatchScroeNode.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "Room.h"
#include "RecordLayer.h"
#include "cmd_def.h"
#include "GameDataManager.h"
#include "UserSettingPropertyCell.h"
#include "LayerGameSetting.h"
#include "FileManager.h"
#include "VoiceRecorderInterface.h"
#include "TipView.h"
#include "AudioManager.h"
#include "RecordAnimationLayer.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "LayerGameChat.h"
#include "LayerOfcPullDown.h"
#include "ChatMessageManager.h"
// 引入相关的头文件
#include "LayerShare.h"
#include "Toast.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "PayManager.h"

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

#define ZORDER_UI  1003
#define ZORDER_PLAYER_INFO 99
#define ZORDER_CARD 100
#define ANIMATION_LAYER_TAG 1


OfcBackGroundLayer::OfcBackGroundLayer() : mFunctionPlayVoiceCallback(nullptr)
{
    LayerPool::shared_instence().pop_layer(this);
}

OfcBackGroundLayer* OfcBackGroundLayer::create(bool is_Fantasy)
{
    OfcBackGroundLayer* back = new OfcBackGroundLayer;
    if ( back->init(is_Fantasy) )
    {
        back->autorelease();
        return back;
    }
    
    return nullptr;
}

bool OfcBackGroundLayer::init(bool is_Fantasy)
{
    if (!Layer::init()) {
        return false;
    }
    
    root_= CSLoader::createNode("LayerOfcBackGroundPortraitNew.csb");
    this->addChild(root_);
    
    is_fantasy_ = is_Fantasy;
    
    auto size = Director::getInstance()->getWinSize();
    size = Size(std::min(size.width, size.height), std::max(size.width, size.height));
    
    auto layout_move = Layout::create();
    layout_move->setContentSize(size);
    layout_move->setTouchEnabled(true);
    layout_move->setSwallowTouches(false);
    layout_move->setAnchorPoint(Point::ZERO);
    layout_move->addTouchCallback(std::bind(&OfcBackGroundLayer::click_item, this, std::placeholders::_1));
    this->addChild(layout_move, 9);
    
    text_table_note_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_table_note"));
    text_table_note_->setString("");
    btn_pause_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_pause"));
    btn_pause_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_pause, this, std::placeholders::_1));
    
    btn_pause_->setVisible(false);
    
    btn_record_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_record_back"));
    btn_funcitem_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_funcitem"));
    btn_record_last_hand_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_record_last_hand"));
    btn_match_score_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_match"));
    btn_voice_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_voice"));
    btn_chat_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_chat"));
    btn_share_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_share"));
    
    btn_record_back_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_record_back_0"));
    btn_funcitem_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_funcitem_0"));
    btn_record_last_hand_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_record_last_hand_0"));
    btn_match_score_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_match_0"));
    btn_voice_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_voice_0"));
    btn_chat_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_chat_0"));
    btn_share_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_share_0"));
    
    if (is_fantasy_)
    {
        text_table_note_->setVisible(false);
    }
    
//    btn_chat_->setVisible(false);
    btn_record_back_->setVisible(false);
    btn_record_back_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_record_back, this, std::placeholders::_1));
    btn_record_back_->setZOrder(10);
    
    btn_record_back_0_->setVisible(false);
    btn_record_back_0_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_record_back, this, std::placeholders::_1));
    btn_record_back_0_->setZOrder(10);
    
    //设置功能测试
    
    btn_funcitem_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_funcitem, this, std::placeholders::_1));
    btn_funcitem_->setZOrder(10);
    
    btn_funcitem_0_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_funcitem, this, std::placeholders::_1));
    btn_funcitem_0_->setZOrder(10);
    
    btn_record_last_hand_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_record_last_hand, this, std::placeholders::_1));
    btn_record_last_hand_0_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_record_last_hand, this, std::placeholders::_1));
    
    
    btn_match_score_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_match_score, this, std::placeholders::_1));
    btn_match_score_0_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_match_score, this, std::placeholders::_1));
    
    
    btn_voice_->addTouchEventListener(std::bind(&OfcBackGroundLayer::click_btn_voice, this, std::placeholders::_1, std::placeholders::_2));
    btn_voice_0_->addTouchEventListener(std::bind(&OfcBackGroundLayer::click_btn_voice, this, std::placeholders::_1, std::placeholders::_2));
    
    
    btn_chat_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_chat, this, std::placeholders::_1));
    btn_chat_0_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_chat, this, std::placeholders::_1));
    
    image_red_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(btn_chat_, "image_red"));
    text_unread_message_count_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(btn_chat_, "text_unread_message_count"));
    
    image_red_0_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(btn_chat_0_, "image_red"));
    text_unread_message_count_0_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(btn_chat_0_, "text_unread_message_count"));
    
    
    btn_share_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_share, this, std::placeholders::_1));
    btn_share_0_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_share, this, std::placeholders::_1));
    
    
//    uint32_t unread_message_count = ChatMessageManager::getInstance()->get_unread_message_count(GetRoom()->get_group_id(),ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    uint32_t unread_message_count = 0;
    if(unread_message_count != 0) {
        text_unread_message_count_->setString(tools::to_string(unread_message_count));
        image_red_->setVisible(true);
        text_unread_message_count_->setVisible(true);
        
        text_unread_message_count_0_->setString(tools::to_string(unread_message_count));
    }
    else {
        image_red_->setVisible(false);
        text_unread_message_count_->setVisible(false);
    }
    
    btn_funcitem_isShow_ = false;

    btn_chat_0_->setVisible(false);
    btn_voice_0_->setVisible(false);
    btn_funcitem_0_->setVisible(false);
    btn_record_last_hand_0_->setVisible(false);
    btn_match_score_0_->setVisible(false);
    btn_share_0_->setVisible(false);
    btn_record_back_0_->setVisible(false);
//    image_red_0_->setVisible(false);
//    text_unread_message_count_0_->setVisible(false);
    
    register_events();
//    this->update_chat_message();
    
    return true;
}

void OfcBackGroundLayer::update_menu_status(bool is_fantasy)
{
    if (is_fantasy)
    {
        is_fantasy_ = is_fantasy;
        btn_chat_0_->setVisible(true);
        btn_voice_0_->setVisible(true);
        btn_funcitem_0_->setVisible(true);
        btn_record_last_hand_0_->setVisible(true);
        btn_match_score_0_->setVisible(true);
        btn_share_0_->setVisible(true);
        btn_record_back_0_->setVisible(false);
        
        if (image_red_->isVisible())
        {
            image_red_0_->setVisible(true);
            text_unread_message_count_0_->setVisible(true);
            text_unread_message_count_0_->setString(text_unread_message_count_->getString());
        }
        else
        {
            image_red_0_->setVisible(false);
            text_unread_message_count_0_->setVisible(false);
        }
        
        btn_funcitem_->setVisible(false);
        btn_record_last_hand_->setVisible(false);
        btn_match_score_->setVisible(false);
        btn_voice_->setVisible(false);
        btn_chat_->setVisible(false);
        btn_share_->setVisible(false);
        btn_record_back_->setVisible(false);
        text_table_note_->setVisible(false);
        btn_pause_->setVisible(false);
    }
    else
    {
        
        btn_funcitem_->setVisible(true);
        btn_record_last_hand_->setVisible(true);
        btn_match_score_->setVisible(true);
        btn_voice_->setVisible(true);
        btn_chat_->setVisible(true);
        btn_share_->setVisible(true);
        btn_record_back_->setVisible(false);
        
        if (image_red_0_->isVisible())
        {
            image_red_->setVisible(true);
            text_unread_message_count_->setVisible(true);
            text_unread_message_count_->setString(text_unread_message_count_0_->getString());
        }
        else
        {
            image_red_->setVisible(false);
            text_unread_message_count_->setVisible(false);
        }
        
        btn_chat_0_->setVisible(false);
        btn_voice_0_->setVisible(false);
        btn_funcitem_0_->setVisible(false);
        btn_record_last_hand_0_->setVisible(false);
        btn_match_score_0_->setVisible(false);
        btn_share_0_->setVisible(false);
        btn_record_back_0_->setVisible(false);
        text_table_note_->setVisible(true);
    }
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data->get_group_id()==0)
    {
        btn_chat_->setVisible(false);
        btn_chat_0_->setVisible(false);
    }
}

void OfcBackGroundLayer::update_record_status()
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_standup ofc_room_data == nullptr");
        return;
    }
    if (ofc_room_data->get_is_record())
    {
        btn_funcitem_->setVisible(false);
        btn_record_last_hand_->setVisible(false);
        btn_match_score_->setVisible(false);
        btn_voice_->setVisible(false);
        btn_chat_->setVisible(false);
        btn_share_->setVisible(false);
//        btn_record_back_->setVisible(true);
        
        btn_funcitem_0_->setVisible(false);
        btn_record_last_hand_0_->setVisible(false);
        btn_match_score_0_->setVisible(false);
        btn_voice_0_->setVisible(false);
        btn_chat_0_->setVisible(false);
        btn_share_0_->setVisible(false);
//        btn_record_back_0_->setVisible(true);
        
        if (is_fantasy_)
        {
            btn_record_back_->setVisible(false);
            btn_record_back_0_->setVisible(true);
        }
        else
        {
            btn_record_back_->setVisible(true);
            btn_record_back_0_->setVisible(false);
        }
        

    }else
    {
        uint32_t last_hand_id = ofc_room_data->get_last_hand_id();
        if (last_hand_id != 0)
        {
            btn_record_last_hand_->setVisible(true);
//            text_record_last_hand_->setVisible(true);
        }else
        {
            btn_record_last_hand_->setVisible(false);
//            text_record_last_hand_->setVisible(false);
        }
    }
}

void OfcBackGroundLayer::update_game_status(const msg::GameStatus game_status)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::update_game_status ofc_room_data == nullptr");
        btn_pause_->setVisible(false);
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    uint32_t ownerid = ofc_room_data->get_owner_id();
    
    if (game_status == msg::GameStatus::RUNNING)
    {
        
        if (ownerid == GDM->get_user_id() && !is_fantasy_ )
        {
            btn_pause_->setVisible(true);
        }else
        {
            btn_pause_->setVisible(false);
        }
        
    }else if (game_status == msg::GameStatus::PAUSED)
    {
        btn_pause_->setVisible(false);
    }
}

void OfcBackGroundLayer::update_room_note(const std::string room_note)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data->get_group_id()==0)
    {
        btn_chat_->setVisible(false);
        btn_chat_0_->setVisible(false);
//        text_table_note_->setVisible(false);
    }
    else
    {
        if (is_fantasy_)
        {
            btn_chat_->setVisible(false);
            btn_chat_0_->setVisible(true);
        }
        else
        {
            btn_chat_->setVisible(true);
            btn_chat_0_->setVisible(false);
        }
    }
    text_table_note_->setString(room_note);
}

void OfcBackGroundLayer::click_item(Ref* sender)
{
    if (msn != NULL)
    {
        msn->click_btn_close(sender);
        msn = NULL;
    }
}

void OfcBackGroundLayer::click_btn_record_back(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_standup ofc_room_data == nullptr");
        return;
    }
    if (ofc_room_data->get_is_record())
    {
        uint32_t hand_id = ofc_room_data->get_current_hand_id();
        PDM->close_replay_room(hand_id);
    }
}

void OfcBackGroundLayer::click_btn_funcitem(Ref* sender)
{
    LayerOfcPullDown* pull = LayerOfcPullDown::create();
    this->addChild(pull, 0, "LayerOfcPullDown");
}

void OfcBackGroundLayer::click_btn_standup(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_standup ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    auto users = ofc_room_data->get_users();
    
    auto self_user = users.find(GDM->get_user_id());
    if(self_user != users.end() && ((self_user->second->get_status() == 0) || (self_user->second->get_status() == 5) ))//status: inGame || willLeave
    {
        TipView* tip = TipView::showAlertView(tools::local_string("will_quit_room", "本局游戏之后将生效"));
        tip->show_cancel(true);
        tip->set_tipview_close_callback([=](){
            // by lvzhangle --20151226
//            int32_t seat_id = self_user->second->get_seat_num();//
//            if(seat_id != 0) {
//                PDM->send_seat_info(roomid, msg::SittingStatus::OBSERVER, seat_id);
//            }
            int32_t seat_id = 0;
            PDM->send_seat_info(roomid, msg::SittingStatus::OBSERVER, seat_id);
        });
    }else{
        
        //by lvzhangle --20151223
        if (self_user != users.end())
        {
            uint32_t seat_id = self_user->second->get_seat_num();
            if(seat_id != 0) {
                PDM->send_seat_info(roomid, msg::SittingStatus::OBSERVER, seat_id);
            }
        }
    }
}

void OfcBackGroundLayer::click_btn_quit(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_quit ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    auto users = ofc_room_data->get_users();
    
    auto self_user = users.find(GDM->get_user_id());
    if(self_user != users.end() && self_user->second->get_status() == 0)
    {
        TipView* tip = TipView::showAlertView(tools::local_string("will_quit_room", "本局游戏之后将生效"));
        tip->show_cancel(true);
        tip->set_tipview_close_callback([=](){
            //uint32_t seat_id = self_user->second->get_seat_num();//
            uint32_t seat_id = 0;
            PDM->send_seat_info(roomid, msg::SittingStatus::LEAVE, seat_id);
            
        });
    }else
    {
        //by lvzhangle --20151223
        //uint32_t seat_id = self_user->get_seat_num();
        uint32_t seat_id = 0;
        PDM->send_seat_info(roomid, msg::SittingStatus::LEAVE, seat_id);
    }
    
}

void OfcBackGroundLayer::click_btn_setting(Ref* sender)
{
    LayerGameSetting* lgs = LayerGameSetting::create(false);
    lgs->set_change_setting_callback([&](){
        newOfcPokerRoom* opr = dynamic_cast<newOfcPokerRoom*>(SceneManager::GetRoomLayer());
        if (opr)
        {
            opr->refresh_user_poker_cards();
        }
    });
    this->addChild(lgs);
    
}

void OfcBackGroundLayer::show_chat_layer()
{

}

void OfcBackGroundLayer::click_btn_record_last_hand(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_standup ofc_room_data == nullptr");
        return;
    }
    uint32_t last_hand_id = ofc_room_data->get_last_hand_id();
    if (last_hand_id != 0)
    {
        PDM->send_poker_get_hand_history(last_hand_id, false);
    }
}

void OfcBackGroundLayer::click_btn_match_score(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_standup ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    PDM->send_get_score(roomid);
}

void OfcBackGroundLayer::click_btn_voice(Ref* sender, Widget::TouchEventType event)
{
    switch (event) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN: {
            AM->begin_recording();
            
            uint32_t user_id = GDM->get_user_id();
            uint32_t time = (uint32_t)tools::date();
            record_filename_ = tools::to_string(user_id) + "_" + tools::to_string(time);
            log("filename1....:%s", record_filename_.c_str());
            FM.get_recorder()->beginRecordByFileName(record_filename_);
            RecordAnimationLayer* record_animation_layer_ = (RecordAnimationLayer*)getChildByTag(ANIMATION_LAYER_TAG);
            if(!record_animation_layer_) {
                record_animation_layer_ = RecordAnimationLayer::create();
                this->addChild(record_animation_layer_, 1002, ANIMATION_LAYER_TAG);
                record_animation_layer_->record();
            }
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::MOVED: {
            //            Point p = touch->getLocation();
            //            if(!btn_record_->isHighlighted()) {
            //                if(record_animation_layer_) {
            //                    float sub = p.y - touch_record_begin_position_.y;
            //                    log("sub:%f", sub);
            //                    if(fabsf(p.y - touch_record_begin_position_.y) < 100) {
            //                        record_animation_layer_->cancel();
            //                    }
            //                    else {
            //                        record_animation_layer_->giveup();
            //                    }
            //                }
            //            }
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::ENDED: {
            auto &ofc_room_data = this->GetRoom();
            if (ofc_room_data == nullptr)
            {
                cocos2d::log("OfcBackGroundLayer::click_btn_quit ofc_room_data == nullptr");
                break;
            }
            uint32_t roomid = ofc_room_data->get_id();
            uint32_t record_time = FM.get_recorder()->stopRecord();
            if(record_time != 0) {
                log("发送录音时间: %d", record_time);
                PDM->send_game_voice(record_filename_, record_time, roomid);
                ofc_room_data->add_owner_last_voice(GDM->get_user_id(), record_filename_+".amr", record_time);
            }
            else {
                Toast::ToastShow(tools::local_string("record_time_too_shop", "录音时间太短"));
            }
            record_filename_ = "";
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
            
            AM->end_recording();
            break;
    }
}


void OfcBackGroundLayer::update_chat_message()
{
    uint32_t unread_message_count = CM->get_unread_message_count(GetRoom()->get_group_id(), ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    if (btn_chat_->isVisible())
    {
        image_red_->setVisible(unread_message_count != 0);
        text_unread_message_count_->setVisible(unread_message_count != 0);
        text_unread_message_count_->setString(tools::to_string(unread_message_count));
    }
    else if (btn_chat_0_->isVisible())
    {
        image_red_0_->setVisible(unread_message_count != 0);
        text_unread_message_count_0_->setVisible(unread_message_count != 0);
        text_unread_message_count_0_->setString(tools::to_string(unread_message_count));
    }
}

void OfcBackGroundLayer::click_btn_chat(Ref* sender)
{
    ChatMessageManager::getInstance()->clear_unreal_message_by(GetRoom()->get_group_id(), ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    handle_receive_chat(nullptr);//接收信息

    gcl = LayerGameChat::createByGroup(GetRoom()->get_group_id());
    this->addChild(gcl, ZORDER_UI,"LayerGameChat");
}

void OfcBackGroundLayer::click_btn_share(Ref* sender)
{
    uint32_t uintUserID = GDM->get_user_id();
    uint32_t uintGroupID = GetRoom()->get_group_id();
    uint32_t uintRoomID = GetRoom()->get_id();
    
    auto layerShare = LayerShare::create();
    layerShare->setShareGameOfcRoomInfo(uintUserID, uintGroupID, uintRoomID);
    AddGlobalLayer(layerShare);
}

void OfcBackGroundLayer::click_btn_pause(Ref * sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_pause ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    uint32_t ownerid = ofc_room_data->get_owner_id();
    if (ownerid == GDM->get_user_id())
    {
        PDM->send_set_game_status(roomid, msg::GameStatus::PAUSED);
    }else
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_pause not is owner but click button!");
    }
}

void OfcBackGroundLayer::handle_receive_chat(cmd_data_pointer)
{
    uint32_t unread_message_count = ChatMessageManager::getInstance()->get_unread_message_count(GetRoom()->get_group_id(),ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    if((unread_message_count != 0) && (gcl == NULL)) {
        if (btn_chat_->isVisible())
        {
            text_unread_message_count_->setString(tools::to_string(unread_message_count));
            image_red_->setVisible(true);
            text_unread_message_count_->setVisible(true);
        }
        if (btn_chat_0_->isVisible())
        {
            text_unread_message_count_0_->setString(tools::to_string(unread_message_count));
            image_red_0_->setVisible(true);
            text_unread_message_count_0_->setVisible(true);
        }
    }
    else if(unread_message_count == 0){
        if (btn_chat_->isVisible())
        {
            image_red_->setVisible(false);
            text_unread_message_count_->setVisible(false);
        }
        if (btn_chat_0_->isVisible())
        {
            image_red_0_->setVisible(false);
            text_unread_message_count_0_->setVisible(false);
        }
        
    }
}

void OfcBackGroundLayer::handle_download_events(cmd_data_pointer data)
{
    auto recv = std::dynamic_pointer_cast<cmd_data_string>(data);
    if(recv) {  //播放声音
        
        std::string filename = recv->get_value();
        std::string wav_path = GDM->get_user_directory() + filename;
        std::string strFileNameAMR;
        auto pos = filename.find(".");
        strFileNameAMR = filename.substr(0, pos);
        strFileNameAMR += ".amr";
        
        if(GDM->is_file_exist(wav_path)) {
            int duration = 0;
            
            uint32_t user_id = GetRoom()->get_voice_user_id(strFileNameAMR, duration);
            if(user_id != 0)
            {
                log("room 接收录音时间: %d", duration);
                AM->play_game_voice(wav_path);
                if(mFunctionPlayVoiceCallback)
                {
                    mFunctionPlayVoiceCallback(user_id, duration);
                }
            }
        }else
        {
            std::string amr_path = GDM->get_user_directory() + strFileNameAMR;
            if(GDM->is_file_exist(amr_path))
            {
                int duration = 0;
                
                uint32_t user_id = GetRoom()->get_voice_user_id(strFileNameAMR, duration);
                if(user_id != 0)
                {
                    log("接收录音时间: %d", duration);
                    AM->play_game_voice(amr_path);
                    if(mFunctionPlayVoiceCallback)
                    {
                        mFunctionPlayVoiceCallback(user_id, duration);
                    }
                }
            }
        }
        
    }
}

//回调注册
void OfcBackGroundLayer::register_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, OfcBackGroundLayer::handle_download_events));
}

