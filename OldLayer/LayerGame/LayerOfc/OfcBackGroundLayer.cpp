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
#include "LayerChatOld.h"
#include "PokerDataManager.h"
#include "Room.h"
#include "RecordLayer.h"
#include "cmd_def.h"
#include "GameDataManager.h"
#include "UserSettingPropertyCell.h"
#include "FileManager.h"
#include "VoiceRecorderInterface.h"
#include "TipView.h"
#include "AudioManager.h"
#include "RecordAnimationLayer.h"
#include "GameChatLayer.h"
#include "SceneManager.h"
#include "UserPropertyLayer.h"
#include "AudioManager.h"

// 引入相关的头文件
#include "CCUMSocialSDK.h"
#include "UMShareButton.h"
#include "net_manager.h"
// 使用友盟命令空间
USING_NS_UM_SOCIAL;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "PayManager.h"

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

#define ZORDER_UI  1000
#define ZORDER_PLAYER_INFO 99
#define ZORDER_CARD 100
#define ANIMATION_LAYER_TAG 1

bool OfcBackGroundLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    root_= CSLoader::createNode("LayerOfcBackGroundPortrait.csb");
    this->addChild(root_);
    
    auto size = Director::getInstance()->getWinSize();
    size = Size(std::min(size.width, size.height), std::max(size.width, size.height));
    
    auto layout_move = Layout::create();
    layout_move->setContentSize(size);
    layout_move->setTouchEnabled(true);
    layout_move->setSwallowTouches(false);
    layout_move->setAnchorPoint(Point::ZERO);
    layout_move->addTouchCallback(std::bind(&OfcBackGroundLayer::click_item, this, std::placeholders::_1));
    this->addChild(layout_move, 9);
    
    btn_record_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_record_back"));
    btn_record_back_->setVisible(false);
    btn_record_back_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_record_back, this, std::placeholders::_1));
    btn_record_back_->setZOrder(10);
    
    btn_funcitem_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_funcitem"));
    btn_funcitem_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_funcitem, this, std::placeholders::_1));
    btn_funcitem_->setZOrder(10);
    
    btn_standup_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_standup"));
    btn_standup_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_standup, this, std::placeholders::_1));
    btn_standup_->setZOrder(9);
    point_btn_standup_ = btn_standup_->getPosition();
    
    btn_quit_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_quit"));
    btn_quit_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_quit, this, std::placeholders::_1));
    btn_quit_->setZOrder(9);
    point_btn_quit_ = btn_quit_->getPosition();
    
    btn_setting_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_setting"));
    btn_setting_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_setting, this, std::placeholders::_1));
    btn_setting_->setZOrder(9);
    point_btn_setting_ = btn_setting_->getPosition();
    
    text_standup_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_standup"));
    text_standup_->setZOrder(9);
    point_text_standup_ = text_standup_->getPosition();
    
    text_quit_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_quit"));
    text_quit_->setZOrder(9);
    point_text_quit_ = text_quit_->getPosition();
    
    text_setting_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_setting"));
    text_setting_->setZOrder(9);
    point_text_setting_ = text_setting_->getPosition();
    
    btn_record_last_hand_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_record_last_hand"));
    btn_record_last_hand_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_record_last_hand, this, std::placeholders::_1));
    
    btn_match_score_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_match"));
    btn_match_score_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_match_score, this, std::placeholders::_1));
    
    btn_voice_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_voice"));
    btn_voice_->addTouchEventListener(std::bind(&OfcBackGroundLayer::click_btn_voice, this, std::placeholders::_1, std::placeholders::_2));
    
    btn_chat_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_chat"));
    btn_chat_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_chat, this, std::placeholders::_1));
    
    image_red_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_red"));
    text_unread_message_count_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_unread_message_count"));
    
    btn_share_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_share"));
    btn_share_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_share, this, std::placeholders::_1));
    
    btn_pause_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_pause"));
    btn_pause_->addClickEventListener(std::bind(&OfcBackGroundLayer::click_btn_pause, this, std::placeholders::_1));
    
    btn_pause_->setVisible(false);
    
    uint32_t unread_message_count = GDM->get_unread_message_count(GetRoom()->get_group_id());
    if(unread_message_count != 0) {
        text_unread_message_count_->setString(tools::to_string(unread_message_count));
        image_red_->setVisible(true);
        text_unread_message_count_->setVisible(true);
    }
    else {
        image_red_->setVisible(false);
        text_unread_message_count_->setVisible(false);
    }
    
    text_record_last_hand_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_record_last_hand"));
    text_match_score_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_match"));
    text_voice_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_voice"));
    text_chat_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_chat"));
    
    btn_funcitem_->loadTextureNormal("image/ofc_game_res/btn_ofc_funcitem.png");
    btn_funcitem_->loadTexturePressed("image/ofc_game_res/btn_ofc_funcitem_pressed.png");
    btn_funcitem_->loadTextureDisabled("image/ofc_game_res/btn_ofc_funcitem_pressed.png");
    btn_funcitem_isShow_ = false;
    btn_standup_->setPosition(btn_funcitem_->getPosition());
    btn_standup_->setVisible(false);
    text_standup_->setPosition(btn_funcitem_->getPosition());
    text_standup_->setVisible(false);
    btn_quit_->setPosition(btn_funcitem_->getPosition());
    btn_quit_->setVisible(false);
    text_quit_->setPosition(btn_funcitem_->getPosition());
    text_quit_->setVisible(false);
    btn_setting_->setPosition(btn_funcitem_->getPosition());
    btn_setting_->setVisible(false);
    text_setting_->setPosition(btn_funcitem_->getPosition());
    text_setting_->setVisible(false);
    
    register_events();
    
    return true;
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
        btn_standup_->setVisible(false);;
        btn_quit_->setVisible(false);;
        btn_setting_->setVisible(false);;
        btn_record_last_hand_->setVisible(false);;
        btn_match_score_->setVisible(false);;
        btn_voice_->setVisible(false);;
        btn_chat_->setVisible(false);;
        btn_share_->setVisible(false);;
        btn_record_back_->setVisible(true);;
        
        text_standup_->setVisible(false);;
        text_quit_->setVisible(false);;
        text_setting_->setVisible(false);;
        
        text_record_last_hand_->setVisible(false);;
        text_match_score_->setVisible(false);;
        text_voice_->setVisible(false);;
        text_chat_->setVisible(false);;

    }else
    {
        uint32_t last_hand_id = ofc_room_data->get_last_hand_id();
        if (last_hand_id != 0)
        {
            btn_record_last_hand_->setVisible(true);
            text_record_last_hand_->setVisible(true);
        }else
        {
            btn_record_last_hand_->setVisible(false);
            text_record_last_hand_->setVisible(false);
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
    uint32_t roomid = ofc_room_data->get_room_id();
    uint32_t ownerid = ofc_room_data->get_owner_id();
    
    if (game_status == msg::GameStatus::RUNNING)
    {
        
        if (ownerid == GDM->get_user_id())
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
        uint32_t hand_id = ofc_room_data->get_hand_id();
        PDM->close_replay_room(hand_id);
    }
}

void OfcBackGroundLayer::click_btn_funcitem(Ref* sender)
{
    MoveTo* move_to_btn_standup;
    MoveTo* move_to_text_standup;
    MoveTo* move_to_btn_quit;
    MoveTo* move_to_text_quit;
    MoveTo* move_to_btn_setting;
    MoveTo* move_to_text_setting;
    if (btn_funcitem_isShow_)
    {
        btn_funcitem_isShow_ = false;
        btn_funcitem_->loadTextureNormal("image/ofc_game_res/btn_ofc_funcitem.png");
        btn_funcitem_->loadTexturePressed("image/ofc_game_res/btn_ofc_funcitem_pressed.png");
        btn_funcitem_->loadTextureDisabled("image/ofc_game_res/btn_ofc_funcitem_pressed.png");
        move_to_btn_standup = MoveTo::create(.2f, btn_funcitem_->getPosition());
        move_to_text_standup = MoveTo::create(.2f, btn_funcitem_->getPosition());
        move_to_btn_quit = MoveTo::create(.4f, btn_funcitem_->getPosition());
        move_to_text_quit = MoveTo::create(.4f, btn_funcitem_->getPosition());
        move_to_btn_setting = MoveTo::create(.6f, btn_funcitem_->getPosition());
        move_to_text_setting = MoveTo::create(.6f, btn_funcitem_->getPosition());
        
        auto callfunc_btn_standup = CallFunc::create(std::function<void()>([=](){btn_standup_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_btn_standup = Sequence::create(move_to_btn_standup, callfunc_btn_standup, NULL);
        
        auto callfunc_text_standup = CallFunc::create(std::function<void()>([=](){text_standup_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_text_standup = Sequence::create(move_to_text_standup , callfunc_text_standup, NULL);
        
        auto callfunc_btn_quit = CallFunc::create(std::function<void()>([=](){btn_quit_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_btn_quit = Sequence::create(move_to_btn_quit, callfunc_btn_quit, NULL);
        
        auto callfunc_text_quit = CallFunc::create(std::function<void()>([=](){text_quit_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_text_quit = Sequence::create(move_to_text_quit, callfunc_text_quit, NULL);
        
        auto callfunc_btn_setting = CallFunc::create(std::function<void()>([=](){btn_setting_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_btn_setting = Sequence::create(move_to_btn_setting, callfunc_btn_setting, NULL);
        
        auto callfunc_text_setting = CallFunc::create(std::function<void()>([=](){text_setting_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_text_setting = Sequence::create(move_to_text_setting, callfunc_text_setting, NULL);
        
        btn_standup_->runAction(sequence_btn_standup);
        text_standup_->runAction(sequence_text_standup);
        
        btn_quit_->runAction(sequence_btn_quit);
        text_quit_->runAction(sequence_text_quit);
        
        btn_setting_->runAction(sequence_btn_setting );
        text_setting_->runAction(sequence_text_setting);
    }else
    {
        btn_funcitem_isShow_ = true;
        btn_funcitem_->loadTextureNormal("image/ofc_game_res/btn_ofc_funcitem_show.png");
        btn_funcitem_->loadTexturePressed("image/ofc_game_res/btn_ofc_funcitem_show_pressed.png");
        btn_funcitem_->loadTextureDisabled("image/ofc_game_res/btn_ofc_funcitem_show_pressed.png");
        move_to_btn_standup = MoveTo::create(.2f, point_btn_standup_);
        move_to_text_standup = MoveTo::create(.2f, point_text_standup_);
        move_to_btn_quit = MoveTo::create(.4f, point_btn_quit_);
        move_to_text_quit = MoveTo::create(.4f, point_text_quit_);
        move_to_btn_setting = MoveTo::create(.6f, point_btn_setting_);
        move_to_text_setting = MoveTo::create(.6f, point_text_setting_);
        
        auto callfunc_btn_standup = CallFunc::create(std::function<void()>([=](){btn_standup_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_btn_standup = Sequence::create(callfunc_btn_standup, move_to_btn_standup, NULL);
        
        auto callfunc_text_standup = CallFunc::create(std::function<void()>([=](){text_standup_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_text_standup = Sequence::create(callfunc_text_standup, move_to_text_standup , NULL);
        
        auto callfunc_btn_quit = CallFunc::create(std::function<void()>([=](){btn_quit_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_btn_quit = Sequence::create(callfunc_btn_quit, move_to_btn_quit, NULL);
        
        auto callfunc_text_quit = CallFunc::create(std::function<void()>([=](){text_quit_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_text_quit = Sequence::create(callfunc_text_quit, move_to_text_quit, NULL);
        
        auto callfunc_btn_setting = CallFunc::create(std::function<void()>([=](){btn_setting_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_btn_setting = Sequence::create(callfunc_btn_setting, move_to_btn_setting, NULL);
        
        auto callfunc_text_setting = CallFunc::create(std::function<void()>([=](){text_setting_->setVisible(btn_funcitem_isShow_);}));
        Sequence* sequence_text_setting = Sequence::create(callfunc_text_setting, move_to_text_setting, NULL);
        
        btn_standup_->runAction(sequence_btn_standup);
        text_standup_->runAction(sequence_text_standup);
        
        btn_quit_->runAction(sequence_btn_quit);
        text_quit_->runAction(sequence_text_quit);
        
        btn_setting_->runAction(sequence_btn_setting );
        text_setting_->runAction(sequence_text_setting);
    }
}

void OfcBackGroundLayer::click_btn_standup(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_standup ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_room_id();
    auto users = ofc_room_data->get_valid_users();
    
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
    uint32_t roomid = ofc_room_data->get_room_id();
    auto users = ofc_room_data->get_valid_users();
    
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
    //    UserPropertyLayer* upl = UserPropertyLayer::create(true);
    //    auto& winSize = Director::getInstance()->getWinSize();
    //    upl->setPosition(0, winSize.height - 568);
    //    upl->set_btn_close_visible(true);
    //    this->addChild(upl);
    
    UserPropertyLayer* upl = UserPropertyLayer::create(false);
    
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_setting ofc_room_data == nullptr");
    }else
    {
        upl->set_click_item(ofc_room_data->get_room_type());
    }
    
    this->addChild(upl);
}

void OfcBackGroundLayer::show_chat_layer()
{
    LayerChatOld* glc = LayerChatOld::create();
    auto& winSize = Director::getInstance()->getWinSize();
    glc->setScale(0.5f);
    glc->setPosition(-winSize.height / 2, -160 + 80);
    glc->set_not_touch();
    root_->addChild(glc, ZORDER_UI);
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
    uint32_t roomid = ofc_room_data->get_room_id();
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
                this->addChild(record_animation_layer_, 0, ANIMATION_LAYER_TAG);
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
            uint32_t roomid = ofc_room_data->get_room_id();
            uint32_t record_time = FM.get_recorder()->stopRecord();
            if(record_time != 0) {
                PDM->send_game_voice(record_filename_, record_time, roomid);
                
            }
            else {
                TipView::showAlertView(tools::local_string("record_time_too_shop", "录音时间太短"));
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

void OfcBackGroundLayer::click_btn_chat(Ref* sender)
{
    GDM->clear_unreal_message_by_group_id(GetRoom()->get_group_id());
    
    handle_receive_chat(nullptr);//接收信息
    
    gcl = GameChatLayer::create();
    gcl->set_not_touch();
    this->addChild(gcl, ZORDER_UI);
}

void OfcBackGroundLayer::click_btn_share(Ref* sender)
{
    CCUMSocialSDK *sdk;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    sdk = CCUMSocialSDK::create("556d5eaa67e58e5fcd001173");//友盟APP key
#else
    sdk = CCUMSocialSDK::create("557f7d5767e58e9cc9001d65");//友盟APP key
#endif
    sdk->setWeiXinAppInfo("wx659249d10a35848b","5b5b7b4b90c656a426d71c4f1e2bca32");//微信id，key（secret）
    
    vector<int>* platforms = new vector<int>();
    platforms->push_back(umeng::social::WEIXIN);
    platforms->push_back(umeng::social::WEIXIN_CIRCLE);
    
    sdk->setPlatforms(platforms);
    
    char  a[256];
    sprintf(a,"http://%s:8080/poker/share.jsp?userid=%s&groupid=%s&roomid=%s",
            net_manager::shared_instence().FileServerIP.c_str(),
            StringUtils::format("%d", GDM->get_user_id()).c_str(),
            StringUtils::format("%d", GetRoom()->get_group_id()).c_str(),
            StringUtils::format("%d", GetRoom()->get_room_id()).c_str());
    sdk->setTargetUrl(a);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    sdk->openShare ("我正在房间里玩大菠萝，你不来吗？", "icon108.png", nullptr);
#else
    sdk->openShare ("我正在房间里玩大菠萝，你不来吗？", "assets/Icon108.png", nullptr);
#endif

}

void OfcBackGroundLayer::click_btn_pause(Ref * sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_pause ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_room_id();
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
    uint32_t unread_message_count = GDM->get_unread_message_count(GetRoom()->get_group_id());
    if((unread_message_count != 0) && (gcl == NULL)) {
        text_unread_message_count_->setString(tools::to_string(unread_message_count));
        image_red_->setVisible(true);
        text_unread_message_count_->setVisible(true);
    }
    else if(unread_message_count == 0){
        image_red_->setVisible(false);
        text_unread_message_count_->setVisible(false);
    }else if(!gcl->get_isShow_chat()){
        text_unread_message_count_->setString(tools::to_string(unread_message_count));
        image_red_->setVisible(true);
        text_unread_message_count_->setVisible(true);
    }
}

void OfcBackGroundLayer::handle_download_events(cmd_data_pointer data)
{
    auto recv = std::dynamic_pointer_cast<cmd_data_string>(data);
    if(recv) {  //播放声音
        std::string filename = recv->get_value();
        std::string amr_path = GDM->get_user_directory() + filename;
        if(GDM->is_file_exist(filename)) {

            
            int duration = 0;
            
            auto pos = filename.find(".");
            std::string file = filename.substr(0, pos);
            file += ".amr";
            
            uint32_t user_id = GetRoom()->get_voice_user_id(file, duration);
            if(user_id != 0) {
//                auto info_node = get_player_info_node(user_id);
//                if(info_node) {
//                    info_node->show_voice_animation(duration);
//                }
                AM->play_game_voice(amr_path);
            }
        }
    }
}

//回调注册
void OfcBackGroundLayer::register_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, OfcBackGroundLayer::handle_download_events));
    dispatch_.register_event(EVENT_DOWLOAD_SOUND,
                             BIND_FUNC(this, OfcBackGroundLayer::handle_download_events));
    dispatch_.register_event(EVENT_HANDLE_REFRESH_CHAT_RECORD,
                             BIND_FUNC(this, OfcBackGroundLayer::handle_receive_chat));
}