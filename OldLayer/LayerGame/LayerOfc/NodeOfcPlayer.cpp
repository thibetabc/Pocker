//
//  NodeOfcPlayer.cpp
//  pokerclient
//
//  Created by ll on 15/10/29.
//
//

#include "NodeOfcPlayer.h"


#include "utils.h"
#include "GameTools.h"
#include "cmd_def.h"

#include "PokerCardSprite.h"

#include "PokerDataManager.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "TipView.h"

#include "Room.h"

#include "newOfcPokerRoom.h"

#define ZORDER_IMAGE_PLAYR_BG 100
#define ZORDER_IMAGE_CARD_BG 200
#define ZORDER_PLAYER_UI 300
#define ZORDER_SPRITE_CARD 400
#define ZORDER_RESULT 500
#define ZORDER_SEL_SPRITE 1000

#define TAG_PROGRESS_ICON_CURRENT 299
#define TAG_PROGRESS_TIME_PLAYER 300
#define TAG_PROGRESS_TIME_OWN 301



bool NodeOfcPlayer::init()
{
    if (!Node::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeOfcPlayerPortrait.csb");
    this->addChild(root_);
    
    image_player_card_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_card_bg"));
    image_player_card_bg_->addClickEventListener(std::bind(&NodeOfcPlayer::click_image_player_card_bg, this, std::placeholders::_1));
    image_player_card_bg_->setZOrder(ZORDER_IMAGE_PLAYR_BG);
    
    btn_sitdown_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_sit_down"));
    btn_sitdown_->addClickEventListener(std::bind(&NodeOfcPlayer::click_image_player_card_bg, this, std::placeholders::_1));
    btn_sitdown_->setZOrder(ZORDER_PLAYER_UI);
    
    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon"));
    image_icon_->addClickEventListener(std::bind(&NodeOfcPlayer::click_image_icon, this, std::placeholders::_1));
    image_icon_->setZOrder(ZORDER_PLAYER_UI);
    
    text_player_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_nickname"));
    text_player_nickname_->setString("");
    text_player_nickname_->setVisible(false);
    text_player_nickname_->setZOrder(ZORDER_PLAYER_UI);
    
    text_player_score_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_score"));
    text_player_score_->setString(tools::local_string("score", "得分：")+ "：");
    text_player_score_->setVisible(false);
    text_player_score_->setZOrder(ZORDER_PLAYER_UI);
    
    text_player_status_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_status"));
    text_player_status_->setString("");
    //text_player_status_->setVisible(false);
    
    sprite_dealer_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_dealer"));
    sprite_dealer_->setVisible(false);
    sprite_dealer_->setZOrder(ZORDER_PLAYER_UI);
    
    sprite_time_bar_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_time_bar"));
    sprite_time_bar_->setVisible(false);
    sprite_time_bar_->setZOrder(ZORDER_PLAYER_UI);
    
    text_time_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_time"));
    text_time_->setZOrder(ZORDER_PLAYER_UI);
    text_time_->setString("");
    text_time_->setVisible(false);
    
    text_player_score_first_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_score_first"));
    text_player_score_middle_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_score_middle"));
    text_player_score_last_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_score_last"));
    
    text_player_patterns_first_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_first_row_patterns"));
    text_player_patterns_middle_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_middle_row_patterns"));
    text_player_patterns_last_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_last_row_patterns"));
    
    //text_player_result_;
    text_player_result_score_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_result_score"));
    
    sprite_player_dust_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_bust"));
    sprite_player_fantasy_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_fantasy"));
    
    image_fantasy_mark_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_fantasy_mark"));
    image_fantasy_mark_->setVisible(false);
    
    btn_kick_player_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_kick_player"));
    btn_kick_player_->addClickEventListener(std::bind(&NodeOfcPlayer::click_btn_kick_player, this, std::placeholders::_1));
    btn_kick_player_->setVisible(false);
    btn_kick_player_->setZOrder(ZORDER_SEL_SPRITE);
    
    text_player_score_first_->setZOrder(ZORDER_RESULT);
    text_player_score_middle_->setZOrder(ZORDER_RESULT);
    text_player_score_last_->setZOrder(ZORDER_RESULT);
    text_player_patterns_first_->setZOrder(ZORDER_RESULT);
    text_player_patterns_middle_->setZOrder(ZORDER_RESULT);
    text_player_patterns_last_->setZOrder(ZORDER_RESULT);
    text_player_result_score_->setZOrder(ZORDER_RESULT);
    sprite_player_dust_->setZOrder(ZORDER_RESULT);
    sprite_player_fantasy_->setZOrder(ZORDER_RESULT);
    image_fantasy_mark_->setZOrder(ZORDER_RESULT);
    
    text_player_score_first_->setVisible(false);
    text_player_score_middle_->setVisible(false);
    text_player_score_last_->setVisible(false);
    text_player_patterns_first_->setVisible(false);
    text_player_patterns_middle_->setVisible(false);
    text_player_patterns_last_->setVisible(false);
    text_player_result_score_->setVisible(false);
    sprite_player_dust_->setVisible(false);
    sprite_player_fantasy_->setVisible(false);
    
    for (int i = 0; i < string_image_pokergrooves_name_.size(); i++)
    {
        auto pokergroove = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, string_image_pokergrooves_name_[i]));
        pokergroove->setZOrder(ZORDER_IMAGE_CARD_BG);
        image_pokergrooves_.push_back(pokergroove);
        sprite_handpokers_.push_back(nullptr);
    }
    
    Sprite* headmask = Sprite::create("image/ofc_game_res/image_player_icon_mask_large.png");
    user_icon_ = ShaderSprite::create("image/ofc_game_res/image_player_icon_large.png", "image/ofc_game_res/image_player_icon_mask_large.png");
    root_->addChild(user_icon_);
    user_icon_->setScale(headmask->getContentSize().width / user_icon_->getContentSize().width, headmask->getContentSize().height / user_icon_->getContentSize().height);
    user_icon_->setZOrder(ZORDER_PLAYER_UI+1);
    user_icon_->setPosition(image_icon_->getPosition());
    
    user_icon_->setVisible(false);
    image_icon_->setVisible(false);
    
    return true;
}

void NodeOfcPlayer::set_dismiss()
{
    if (btn_kick_player_->isVisible())
    {
        btn_kick_player_->setVisible(false);
    }
}

void NodeOfcPlayer::click_btn_kick_player(Ref * sender)
{
    if (user_ == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::click_btn_kick_player user_ == nullptr");

        btn_kick_player_->setVisible(false);
        
        return;
    }
    
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::click_btn_kick_player ofc_room_data == nullptr");
        
        btn_kick_player_->setVisible(false);
        
        return;
    }
    
    uint32_t roomid = ofc_room_data->get_room_id();
    uint32_t ownerid = ofc_room_data->get_owner_id();
    bool is_tournament = ofc_room_data->get_is_tournament();
    
    if (ownerid == GDM->get_user_id())
    {
        TipView* tip = TipView::showAlertView("确定要踢走该玩家吗？");
        tip->show_cancel(true);
        tip->set_tipview_close_callback([=](){
            PDM->send_kick_player(roomid, user_->get_id());
        });
    }else
    {
        cocos2d::log("newOfcPokerRoom::click_btn_kick_player not is owner but click button!");
    }
    
    btn_kick_player_->setVisible(false);

}

void NodeOfcPlayer::click_image_icon(Ref * sender)
{
    if (user_ == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::click_image_icon user_ == nullptr");
        if (btn_kick_player_->isVisible())
        {
            btn_kick_player_->setVisible(false);
        }
        return;
    }
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::click_image_icon ofc_room_data == nullptr");
        
        if (btn_kick_player_->isVisible())
        {
            btn_kick_player_->setVisible(false);
        }
        return;
    }
    uint32_t roomid = ofc_room_data->get_room_id();
    uint32_t ownerid = ofc_room_data->get_owner_id();
    
    if (ownerid == GDM->get_user_id())
    {
        btn_kick_player_->setVisible(true);
    }else
    {
        if (btn_kick_player_->isVisible())
        {
            btn_kick_player_->setVisible(false);
        }
    }
}

void NodeOfcPlayer::click_image_player_card_bg(Ref *sender)
{
    if (sitdown_callback_)
        sitdown_callback_();
    
//    auto &ofc_room_data = this->GetRoom();
//    if (ofc_room_data == nullptr)
//    {
//        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
//        return;
//    }
//    uint32_t room_id = ofc_room_data->get_room_id();
//    PDM->send_seat_info(room_id, msg::SittingStatus::SITDOWN, nodeNum_);
}

void NodeOfcPlayer::update_seat_info()
{
    auto user_data = user_;
    if (user_data)
    {
        msg::SittingStatus status = user_data->get_status();
        switch (status)
        {
            case msg::SittingStatus::PLAYING:{
                image_icon_->setVisible(true);
                user_icon_->setVisible(true);
                text_player_nickname_->setVisible(true);
                text_player_score_->setVisible(true);
                text_player_status_->setString("");
                text_player_status_->setVisible(true);
                btn_sitdown_->setVisible(false);
                break;
            }
            case msg::SittingStatus::SITOUT:{
                image_icon_->setVisible(true);
                user_icon_->setVisible(true);
                text_player_nickname_->setVisible(true);
                text_player_score_->setVisible(true);
                text_player_status_->setString(tools::local_string("player_sitting_out", "玩家Sitout"));
                text_player_status_->setVisible(true);
                btn_sitdown_->setVisible(false);
                break;
            }
            case msg::SittingStatus::OBSERVER:{
                user_ = nullptr;
                image_icon_->setVisible(false);
                user_icon_->setVisible(false);
                text_player_nickname_->setVisible(false);
                text_player_score_->setVisible(false);
                text_player_status_->setVisible(false);
                btn_sitdown_->setVisible(true);
                break;
            }
            case msg::SittingStatus::LEAVE:{
                user_ = nullptr;
                image_icon_->setVisible(false);
                user_icon_->setVisible(false);
                text_player_nickname_->setVisible(false);
                text_player_score_->setVisible(false);
                text_player_status_->setVisible(false);
                btn_sitdown_->setVisible(true);
                
                break;
            }
            case msg::SittingStatus::SITDOWN:{
                image_icon_->setVisible(true);
                user_icon_->setVisible(true);
                text_player_nickname_->setVisible(true);
                text_player_score_->setVisible(true);
                text_player_status_->setString(tools::local_string("waiting_for_next_game", "等待下局游戏"));
                text_player_status_->setVisible(true);
                btn_sitdown_->setVisible(false);
                break;
            }
            case msg::SittingStatus::WANTTOLEAVE:{
                image_icon_->setVisible(true);
                user_icon_->setVisible(true);
                text_player_nickname_->setVisible(true);
                text_player_score_->setVisible(true);
                text_player_status_->setString(tools::local_string("leaving_soon", "即将离开"));
                text_player_status_->setVisible(true);
                btn_sitdown_->setVisible(false);
                break;
            }
        }
    }else
    {
        set_clear_result();
        image_icon_->setVisible(false);
        user_icon_->setVisible(false);
        text_player_nickname_->setVisible(false);
        text_player_score_->setVisible(false);
        text_player_status_->setVisible(false);
        btn_sitdown_->setVisible(true);
    }
}

void NodeOfcPlayer::set_user(std::shared_ptr<user_ofc> user)
{
    user_ = user;
    update_user_ui();
}

void NodeOfcPlayer::update_user_ui(cmd_data_pointer)
{
    if (user_ != nullptr)
    {
        //image_icon_;
        
        text_player_nickname_->setString(user_->get_nickname());
        
        text_player_score_->setString(tools::local_string("score", "得分：") + tools::to_string(user_->get_all_score()));

        if (!user_->get_head_pic_name().empty())
        {
            if(GDM->is_file_exist(user_->get_head_pic_name()))
            {
                Sprite* headmask = Sprite::create("image/ofc_game_res/image_player_icon_mask_large.png");
                user_icon_->initWithFile(user_->get_head_pic_name(), "image/ofc_game_res/image_player_icon_mask_large.png");
                user_icon_->setScale(headmask->getContentSize().width / user_icon_->getContentSize().width, headmask->getContentSize().height / user_icon_->getContentSize().height);
                user_icon_->setZOrder(ZORDER_PLAYER_UI+1);
                user_icon_->setPosition(image_icon_->getPosition());
            }else{
                FM.download_file(user_->get_head_pic_name());
                dispatch_.register_event(EVENT_DOWLOAD_FILE, BIND_FUNC(this, NodeOfcPlayer::update_user_ui));
            }
        }
    }
}

void NodeOfcPlayer::update_user_score()
{
    if (user_ != nullptr)
    {
        text_player_score_->setString("得分：" + tools::to_string(user_->get_all_score()));
    }
}

void NodeOfcPlayer::update_propupdate()
{
    if (user_ != nullptr)
    {
        update_player_cards();
        update_user_score();
        if (user_->get_ofc_action_status() == msg::OFCActionStatus::FANTASY)
        {
            image_fantasy_mark_->setVisible(true);
        }else
        {
            image_fantasy_mark_->setVisible(false);
        }
        
        update_first_row_type();
        update_first_row_score();
        update_middle_row_type();
        update_middle_row_score();
        update_last_row_type();
        update_last_row_score();
        update_fantasy_result();
    }
}

void NodeOfcPlayer::update_player_cards()
{
    if (user_ != nullptr)
    {
        std::vector<uint32_t> allcards = user_->get_all_cards();
        
        
        for (int i = 0; i < sprite_handpokers_.size(); i++)
        {
            if (sprite_handpokers_[i] != nullptr && sprite_handpokers_[i]->getParent())
            {
                sprite_handpokers_[i]->removeFromParent();
                sprite_handpokers_[i] = nullptr;
            }
            if (allcards[i] != 100)
            {
                sprite_handpokers_[i] = PokerCardSprite::createCardSprite(allcards[i]);
                sprite_handpokers_[i]->setZOrder(ZORDER_SPRITE_CARD);
                root_->addChild(sprite_handpokers_[i]);
                sprite_handpokers_[i]->setPosition(image_pokergrooves_[i]->getPosition());
                sprite_handpokers_[i]->setScale(image_pokergrooves_[i]->getContentSize().width / sprite_handpokers_[i]->getContentSize().width,
                                                image_pokergrooves_[i]->getContentSize().height / sprite_handpokers_[i]->getContentSize().height);
            }else
            {
                sprite_handpokers_[i] = nullptr;
            }
            
        }
    }
}

void NodeOfcPlayer::update_dealer()
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    uint32_t dealer = ofc_room_data->get_dealer_position();
    if (nodeNum_ == dealer)
    {
        set_dealer(true);
    }else
    {
        set_dealer(false);
    }
}

void NodeOfcPlayer::update_poker_result()
{
    if (user_ != nullptr &&
        user_->get_status() != msg::SittingStatus::SITOUT &&
        user_->get_status() != msg::SittingStatus::SITDOWN)
    {
        //int32_t fantasynum = user_->get_fantasy();
        text_player_result_score_->setString((user_->get_step_score() > 0 ? "+" : "") + tools::to_string(user_->get_step_score()));
        if (user_->get_step_score() < 0)
        {
            text_player_result_score_->setColor(Color3B(255, 0, 0));
        }else
        {
            text_player_result_score_->setColor(Color3B(60, 86, 208));
        }
        text_player_result_score_->enableOutline(Color4B(0, 0, 0, 255), 3);
        text_player_result_score_->setVisible(true);
//        if (fantasynum == -1)
//        {
//            root_->scheduleOnce(std::bind(&NodeOfcPlayer::schedule_dust_visible, this, std::placeholders::_1), 3, "schedule_dust_visible");
//        }
//        if (fantasynum > 0)
//        {
//            root_->scheduleOnce(std::bind(&NodeOfcPlayer::schedule_fantasy_visible, this, std::placeholders::_1), 3, "schedule_fantasy_visible");
//        }
        //root_->scheduleOnce(std::bind(&NodeOfcPlayer::schedule_close_all_result, this, std::placeholders::_1), 8, "schedule_close_all_result");
    }
}

#pragma mark schedule
void NodeOfcPlayer::schedule_dust_visible(float dt)
{
    if (text_player_patterns_first_->isVisible())
        text_player_patterns_first_->setVisible(false);
    if (text_player_score_first_->isVisible())
        text_player_score_first_->setVisible(false);
    
    if (text_player_patterns_middle_->isVisible())
        text_player_patterns_middle_->setVisible(false);
    if (text_player_score_middle_->isVisible())
        text_player_score_middle_->setVisible(false);
    
    if (text_player_patterns_last_->isVisible())
        text_player_patterns_last_->setVisible(false);
    if (text_player_score_last_->isVisible())
        text_player_score_last_->setVisible(false);
    
    sprite_player_dust_->setVisible(true);
}

void NodeOfcPlayer::schedule_fantasy_visible(float dt)
{
    if (text_player_patterns_first_->isVisible())
        text_player_patterns_first_->setVisible(false);
    if (text_player_score_first_->isVisible())
        text_player_score_first_->setVisible(false);
    
    if (text_player_patterns_middle_->isVisible())
        text_player_patterns_middle_->setVisible(false);
    if (text_player_score_middle_->isVisible())
        text_player_score_middle_->setVisible(false);
    
    if (text_player_patterns_last_->isVisible())
        text_player_patterns_last_->setVisible(false);
    if (text_player_score_last_->isVisible())
        text_player_score_last_->setVisible(false);
    
    sprite_player_fantasy_->setVisible(true);
}

void NodeOfcPlayer::schedule_close_all_result(float dt)
{
    if (text_player_patterns_first_->isVisible())
        text_player_patterns_first_->setVisible(false);
    if (text_player_score_first_->isVisible())
        text_player_score_first_->setVisible(false);
    
    if (text_player_patterns_middle_->isVisible())
        text_player_patterns_middle_->setVisible(false);
    if (text_player_score_middle_->isVisible())
        text_player_score_middle_->setVisible(false);
    
    if (text_player_patterns_last_->isVisible())
        text_player_patterns_last_->setVisible(false);
    if (text_player_score_last_->isVisible())
        text_player_score_last_->setVisible(false);
    
    if (sprite_player_dust_->isVisible())
        sprite_player_dust_->setVisible(false);
    if (sprite_player_fantasy_->isVisible())
        sprite_player_fantasy_->setVisible(false);
    
    if (text_player_result_score_->isVisible())
        text_player_result_score_->setVisible(false);
}

void NodeOfcPlayer::set_ProgressTimer(bool isOn, float operator_time, bool is_using_timebank)
{
    if (user_ != nullptr)
    {
        if (isOn)
        {
            is_using_timebank_ = is_using_timebank;
            operator_time_ = operator_time;
            if (operator_time_ == 0.0)
                time_player_close();
            else
                time_player_start();
        }else{
            time_player_close();
        }
    }
}

void NodeOfcPlayer::time_player_start()
{
    //计时器开始之前， 之前的计时器还在
    cocos2d::log("time_player_start user:%s all:%f", user_->get_nickname().c_str(), operator_time_);
    if (progress_player_)
    {
        time_player_close();
    }
    
    if (is_using_timebank_)
    {
        sprite_time_bar_->setTexture("image/ofc_game_res/time_timebank_select.png");
    }else
    {
        sprite_time_bar_->setTexture("image/ofc_game_res/time_select.png");
    }
    
    sprite_time_bar_->setVisible(true);
    current_operator_time_ = 0.0;
    text_time_->setString(tools::to_string(operator_time_) + "s");
    text_time_->setVisible(true);
    //text_player_status_->setVisible(false);
    
    image_player_card_bg_->loadTexture("image/ofc_game_res/image_player_bg_large_select.png");
    
    Sprite* sprite_time_bar_normal = Sprite::create("image/ofc_game_res/time_normal.png");
    
    progress_player_ = ProgressTimer::create(sprite_time_bar_normal);
    progress_player_->setScale(sprite_time_bar_->getScaleX(), sprite_time_bar_->getScaleY());
    
    progress_player_->setPosition(sprite_time_bar_->getPosition());
    progress_player_->setType(ProgressTimer::Type::BAR);
    progress_player_->setMidpoint(Vec2(1, 0.5));
    progress_player_->setBarChangeRate(Vec2(1, 0));
    progress_player_->setZOrder(ZORDER_PLAYER_UI+1);
    
    root_->addChild(progress_player_);
    
    root_->schedule(std::bind(&NodeOfcPlayer::schedule_player_func, this, std::placeholders::_1), 1, "TAG_PROGRESS_TIME_PLAYER");
}

void NodeOfcPlayer::time_player_close()
{
    cocos2d::log("time_player_close");
    root_->unschedule("TAG_PROGRESS_TIME_PLAYER");
    
    if (progress_player_)
    {
        progress_player_->removeFromParent();
        progress_player_ = nullptr;
    }else{
        cocos2d::log("Error : NodeOfcPlayer Time_Close() progress_player = nullptr!");
    }
    
    if (sprite_time_bar_->isVisible())
    {
        sprite_time_bar_->setVisible(false);
    }else{
        cocos2d::log("Error : NodeOfcPlayer Time_Close() sprite_time_bar isVisible false!");
    }
    
    current_operator_time_ = 0.0;
    text_time_->setString("");
    text_time_->setVisible(false);
    //text_player_status_->setVisible(true);
    image_player_card_bg_->loadTexture("image/ofc_game_res/image_player_bg_low.png");
}

void NodeOfcPlayer::schedule_player_func(float dt)
{
    cocos2d::log("schedule_player_func");
    if (progress_player_)
    {
        cocos2d::log("schedule_player_func : all:%f current:%f", operator_time_, progress_player_->getPercentage());
        
        current_operator_time_ += 1.0;
        float ispercent = current_operator_time_ / operator_time_ * 100.0;
        
        progress_player_->setPercentage(ispercent);
        
        float time = operator_time_ - current_operator_time_;
        if (time < 0.0)
            time = 0.0;
        else if (time > 0.0 && time < 1.0)
            time = 1.0;
        text_time_->setString(tools::to_string(time) + "s");
        
        if (progress_player_->getPercentage() >= 100)
        {
            root_->unschedule("TAG_PROGRESS_TIME_PLAYER");
            progress_player_->removeFromParent();
            progress_player_ = nullptr;
            sprite_time_bar_->setVisible(false);
            current_operator_time_ = 0.0;
            text_time_->setString("");
            text_time_->setVisible(false);
            //text_player_status_->setVisible(true);
            image_player_card_bg_->loadTexture("image/ofc_game_res/image_player_bg_large.png");
        }
    }else{
        cocos2d::log("Error : Own schedule_player_func progress_player is null!");
    }
}

void NodeOfcPlayer::set_dealer(bool isOn)
{
    if (isOn)
        sprite_dealer_->setVisible(true);
    else
        sprite_dealer_->setVisible(false);
}

void NodeOfcPlayer::set_clear_result()
{
    schedule_close_all_result(0);
    if (user_ != nullptr)
    {
        user_->clean_all_cards();
    }
    for (int i = 0; i < 13; i++)
    {
        if (sprite_handpokers_[i] != nullptr && sprite_handpokers_[i]->getParent())
        {
            sprite_handpokers_[i]->removeFromParent();
            sprite_handpokers_[i] = nullptr;
        }
    }
}

void NodeOfcPlayer::update_first_row_type()
{
    if (user_ != nullptr)
    {
        text_player_patterns_first_->setString(user_->get_first_row());
        //text_player_patterns_first_->enableOutline(Color4B(0, 0, 0, 255), 5);
        text_player_patterns_first_->setVisible(true);
        
        //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_patterns_first_->setVisible(false);}), 3, "schedule_first_row_type_visible");
    }
}
void NodeOfcPlayer::update_middle_row_type()
{
    if (user_ != nullptr)
    {
        text_player_patterns_middle_->setString(user_->get_middle_row());
        //text_player_patterns_middle_->enableOutline(Color4B(0, 0, 0, 255), 5);
        text_player_patterns_middle_->setVisible(true);
        
        //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_patterns_middle_->setVisible(false);}), 3, "schedule_middle_row_type_visible");
    }
}
void NodeOfcPlayer::update_last_row_type()
{
    if (user_ != nullptr)
    {
        text_player_patterns_last_->setString(user_->get_last_row());
        //text_player_patterns_last_->enableOutline(Color4B(0, 0, 0, 255), 5);
        text_player_patterns_last_->setVisible(true);
        
        //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_patterns_last_->setVisible(false);}), 3, "schedule_last_row_type_visible");
    }
}

void NodeOfcPlayer::update_first_row_score()
{
    if (user_ != nullptr)
    {
        if (user_->get_first_row_score() > 0)
        {
            text_player_score_first_->setString((user_->get_first_row_score() > 0 ? "+" : "") + tools::to_string(user_->get_first_row_score()));
            //text_player_score_first_->enableOutline(Color4B(0, 0, 0, 255), 5);
            text_player_score_first_->setVisible(true);
            
            //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_score_first_->setVisible(false);}), 3, "schedule_first_row_score_visible");
        }else
        {
            text_player_score_first_->setVisible(false);
        }
    }
}
void NodeOfcPlayer::update_middle_row_score()
{
    if (user_ != nullptr)
    {
        if (user_->get_middle_row_score() > 0)
        {
            text_player_score_middle_->setString((user_->get_middle_row_score() > 0 ? "+" : "") + tools::to_string(user_->get_middle_row_score()));
            //text_player_score_middle_->enableOutline(Color4B(0, 0, 0, 255), 5);
            text_player_score_middle_->setVisible(true);
            
            //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_score_middle_->setVisible(false);}), 3, "schedule_middle_row_score_visible");
        }else
        {
            text_player_score_middle_->setVisible(false);
        }
    }
}
void NodeOfcPlayer::update_last_row_score()
{
    if (user_ != nullptr)
    {
        if (user_->get_last_row_score() > 0)
        {
            text_player_score_last_->setString((user_->get_last_row_score() > 0 ? "+" : "") + tools::to_string(user_->get_last_row_score()));
            //text_player_score_last_->enableOutline(Color4B(0, 0, 0, 255), 5);
            text_player_score_last_->setVisible(true);
            
            //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_score_last_->setVisible(false);}), 3, "schedule_last_row_score_visible");
        }else
        {
            text_player_score_last_->setVisible(false);
        }
    }
}

void NodeOfcPlayer::update_fantasy_result()
{
    if (user_ != nullptr)
    {
        int32_t fantasynum = user_->get_fantasy();
        
        if (fantasynum == -1)
        {
            sprite_player_dust_->setVisible(true);
        }
        if (fantasynum > 0)
        {
            sprite_player_fantasy_->setVisible(true);
        }
        
    }
}