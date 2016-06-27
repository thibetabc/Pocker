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
#include "LayerOfcClickPlayerPhoto.h"

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
#define TAG_SPRITE_FANTASY_BG 310

#define TAG_FIRST_ROW_TYPE_EFFECT 321
#define TAG_MIDDLE_ROW_TYPE_EFFECT 322
#define TAG_LAST_ROW_TYPE_EFFECT 323

NodeOfcPlayer* NodeOfcPlayer::create(bool is_large_cell)
{
    NodeOfcPlayer* player = new NodeOfcPlayer;
    if (player->init(is_large_cell))
    {
        player->autorelease();
        return player;
    }
    return nullptr;
}

bool NodeOfcPlayer::init(bool is_large_cell)
{
    if (!Node::init()) {
        return false;
    }
    is_large_cell_ = is_large_cell;
    progress_player_ = nullptr;
    
    first_row_type_ = msg::OFCCardType::NO_TYPE;
    middle_row_type_ = msg::OFCCardType::NO_TYPE;
    last_row_type_ = msg::OFCCardType::NO_TYPE;
    
    if (is_large_cell_)
    {
        root_ = CSLoader::createNode("NodeOfcPlayerOwnPortraitNew.csb");
    }
    else
    {
        root_ = CSLoader::createNode("NodeOfcPlayerPortraitNew.csb");
    }
    
    this->addChild(root_);
    hide_ui();
    
//    image_player_card_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_card_bg"));
//    image_player_card_bg_->addClickEventListener(std::bind(&NodeOfcPlayer::click_image_player_card_bg, this, std::placeholders::_1));
//    image_player_card_bg_->setZOrder(ZORDER_IMAGE_PLAYR_BG);
    
    btn_sitdown_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_sit_down"));
    btn_sitdown_->addClickEventListener(std::bind(&NodeOfcPlayer::click_image_player_card_bg, this, std::placeholders::_1));
    btn_sitdown_->setZOrder(ZORDER_PLAYER_UI);
    
    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon"));
    image_icon_->setTouchEnabled(true);
    image_icon_->addClickEventListener(std::bind(&NodeOfcPlayer::click_user_icon, this, std::placeholders::_1));
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
    
    sprite_dealer_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "sprite_dealer"));
    sprite_dealer_->setVisible(false);
    sprite_dealer_->setZOrder(ZORDER_RESULT+1);
    
//    sprite_time_bar_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_time_bar"));
//    sprite_time_bar_->setVisible(false);
//    sprite_time_bar_->setZOrder(ZORDER_PLAYER_UI);
    
    text_time_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_time"));
    text_time_->setZOrder(ZORDER_PLAYER_UI+2);
    text_time_->setString("");
    text_time_->setVisible(false);
    
//    text_player_score_first_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_score_first"));
//    text_player_score_middle_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_score_middle"));
//    text_player_score_last_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_score_last"));
//    
//    text_player_patterns_first_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_first_row_patterns"));
//    text_player_patterns_middle_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_middle_row_patterns"));
//    text_player_patterns_last_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_last_row_patterns"));
    
    //text_player_result_;
    text_player_result_score_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_result_score"));
    image_result_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_result_bg"));

    sprite_player_dust_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_bust"));
    sprite_player_fantasy_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_fantasy"));
    //sprite_player_dust_->setTexture(GameTools::get_languages_pic("baopai_1.png"));
    
    image_fantasy_mark_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_fantasy_mark"));
    image_fantasy_mark_->setVisible(false);
    
    image_dust_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_dust_bg"));
    image_dust_bg_->setVisible(false);
    
//    btn_kick_player_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_kick_player"));
//    btn_kick_player_->addClickEventListener(std::bind(&NodeOfcPlayer::click_btn_kick_player, this, std::placeholders::_1));
//    btn_kick_player_->setVisible(false);
//    btn_kick_player_->setZOrder(ZORDER_SEL_SPRITE);
    
    
    //新增
    image_player_score_last_0_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_last_0"));
    image_player_score_last_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_last_1"));
    image_player_score_last_2_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_last_2"));
    image_player_score_last_3_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_last_3"));
    
    image_player_score_first_0_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_first_0"));
    image_player_score_first_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_first_1"));
    image_player_score_first_2_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_first_2"));
    image_player_score_first_3_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_first_3"));
    
    image_player_score_middle_0_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_middle_0"));
    image_player_score_middle_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_middle_1"));
    image_player_score_middle_2_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_middle_2"));
    image_player_score_middle_3_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_score_middle_3"));
    
//    image_time_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_time"));
    
    
    image_last_row_patterns_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_last_row_patterns"));
    image_first_row_patterns_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_first_row_patterns"));
    image_middle_row_patterns_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_middle_row_patterns"));
    
    image_player_score_last_0_->setZOrder(ZORDER_RESULT);
    image_player_score_last_1_->setZOrder(ZORDER_RESULT);
    image_player_score_last_2_->setZOrder(ZORDER_RESULT);
    image_player_score_last_3_->setZOrder(ZORDER_RESULT);
    
    image_player_score_first_0_->setZOrder(ZORDER_RESULT);
    image_player_score_first_1_->setZOrder(ZORDER_RESULT);
    image_player_score_first_2_->setZOrder(ZORDER_RESULT);
    image_player_score_first_3_->setZOrder(ZORDER_RESULT);
    
    image_player_score_middle_0_->setZOrder(ZORDER_RESULT);
    image_player_score_middle_1_->setZOrder(ZORDER_RESULT);
    image_player_score_middle_2_->setZOrder(ZORDER_RESULT);
    image_player_score_middle_3_->setZOrder(ZORDER_RESULT);
    
    image_last_row_patterns_->setZOrder(ZORDER_RESULT);
    image_first_row_patterns_->setZOrder(ZORDER_RESULT);
    image_middle_row_patterns_->setZOrder(ZORDER_RESULT);
    
    image_player_score_last_0_->setVisible(false);
    image_player_score_last_1_->setVisible(false);
    image_player_score_last_2_->setVisible(false);
    image_player_score_last_3_->setVisible(false);
    
    image_player_score_first_0_->setVisible(false);
    image_player_score_first_1_->setVisible(false);
    image_player_score_first_2_->setVisible(false);
    image_player_score_first_3_->setVisible(false);
    
    image_player_score_middle_0_->setVisible(false);
    image_player_score_middle_1_->setVisible(false);
    image_player_score_middle_2_->setVisible(false);
    image_player_score_middle_3_->setVisible(false);
    
    image_last_row_patterns_->setVisible(false);
    image_first_row_patterns_->setVisible(false);
    image_middle_row_patterns_->setVisible(false);
    
//    image_time_->setVisible(false);
    
    
//    text_player_score_first_->setZOrder(ZORDER_RESULT);
//    text_player_score_middle_->setZOrder(ZORDER_RESULT);
//    text_player_score_last_->setZOrder(ZORDER_RESULT);
//    text_player_patterns_first_->setZOrder(ZORDER_RESULT);
//    text_player_patterns_middle_->setZOrder(ZORDER_RESULT);
//    text_player_patterns_last_->setZOrder(ZORDER_RESULT);
    text_player_result_score_->setZOrder(ZORDER_RESULT+1);
    image_result_bg_->setZOrder(ZORDER_RESULT);
    sprite_player_dust_->setZOrder(ZORDER_RESULT);
    sprite_player_fantasy_->setZOrder(ZORDER_RESULT);
    image_fantasy_mark_->setZOrder(ZORDER_RESULT);
    image_dust_bg_->setLocalZOrder(ZORDER_SPRITE_CARD + 20);
    
//    text_player_score_first_->setVisible(false);
//    text_player_score_middle_->setVisible(false);
//    text_player_score_last_->setVisible(false);
//    text_player_patterns_first_->setVisible(false);
//    text_player_patterns_middle_->setVisible(false);
//    text_player_patterns_last_->setVisible(false);
    text_player_result_score_->setVisible(false);
    image_result_bg_->setVisible(false);
    sprite_player_dust_->setVisible(false);
    sprite_player_fantasy_->setVisible(false);
    
    sprite_box_ = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root_, "sprite_box"));
    sprite_box_->setVisible(false);
    
    for (int i = 0; i < string_image_pokergrooves_name_.size(); i++)
    {
        auto pokergroove = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, string_image_pokergrooves_name_[i]));
        pokergroove->setZOrder(ZORDER_IMAGE_CARD_BG);
        image_pokergrooves_.push_back(pokergroove);
        int_pokerIDs_.push_back(100);
        sprite_handpokers_.push_back(nullptr);
    }
    
//    Sprite* headmask = Sprite::create("Default/image_mask_circle.png");
//    user_icon_ = nullptr;
    user_icon_ = ShaderSprite::create("Default/image_defualt_user_icon_large.png", "Default/image_mask_circle.png");
    root_->addChild(user_icon_);
    user_icon_->setScale((image_icon_->getContentSize().width-8) / user_icon_->getContentSize().width, (image_icon_->getContentSize().height -8)/ user_icon_->getContentSize().height);
//    user_icon_->setScale(image_icon_->getContentSize().width / user_icon_->getContentSize().width, image_icon_->getContentSize().height / user_icon_->getContentSize().height);
    user_icon_->setZOrder(ZORDER_PLAYER_UI+1);
    user_icon_->setPosition(image_icon_->getPosition());
//    user_icon_->setPosition(image_time_->getPosition());
    
    
    user_icon_->setVisible(false);
    image_icon_->setVisible(false);
//    image_time_->setVisible(false);
    
    
    return true;
}
void NodeOfcPlayer::update_is_large_cell(bool is_large_cell)
{
    is_large_cell_ = is_large_cell;
    hide_ui();
}
void NodeOfcPlayer::hide_ui()
{
    if (is_large_cell_)
    {
        dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_hand_card_bg_1"))->setVisible(false);
        dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_hand_card_bg_2"))->setVisible(false);
        dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_hand_card_bg_3"))->setVisible(false);
        
        dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_discard_bg"))->setVisible(false);
        
        dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_confirm"))->setVisible(false);
        dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_sit_back"))->setVisible(false);
        dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_will_leave"))->setVisible(false);
    }
}

void NodeOfcPlayer::set_dismiss()
{
//    if (btn_kick_player_->isVisible())
//    {
//        btn_kick_player_->setVisible(false);
//    }
}
void NodeOfcPlayer::click_user_icon(Ref* sender)
{
    if (click_photo_callback_)
    {
        click_photo_callback_(user_);
    }
//    auto &ofc_room_data = this->GetRoom();
//    if (ofc_room_data == nullptr)
//    {
//        cocos2d::log("NodeOfcPlayer::click_user_icon ofc_room_data == nullptr");
//        
//        
//        return;
//    }else
//    {
//        msg::UserDetailInfo info;
//        info.set_sex(user_->get_sex());
//        info.set_nickname(user_->get_nickname());
//        info.set_picname(user_->get_head_pic_name());
//        info.set_userid(user_->get_id());
//        
//        uint32_t group_id = ofc_room_data->get_group_id();
    
//        LayerOfcClickPlayerPhoto* user_info = LayerOfcClickPlayerPhoto::create();
//        this->getParent()->getParent()->addChild(user_info, 1001, "LayerOfcClickPlayerPhoto");
//
//        user_info->setScale(0);
//        CCScaleTo* scaleTo = CCScaleTo::create(0.15, 1.05);
//        CCScaleTo* scaleTo2 = CCScaleTo::create(0.1, 1);
//        user_info->runAction(CCSequence::createWithTwoActions(scaleTo,scaleTo2));
        
        
//        user_info->set_player_info(info, group_id);
//    }
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
    
    uint32_t roomid = ofc_room_data->get_id();
    uint32_t ownerid = ofc_room_data->get_owner_id();
    bool is_tournament = ofc_room_data->get_is_tournament();
    
    if (ownerid == GDM->get_user_id())
    {
        TipView* tip = TipView::showAlertView(tools::local_string("kick_off_players","确定要踢走该玩家吗？"));
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
    uint32_t roomid = ofc_room_data->get_id();
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
//    uint32_t room_id = ofc_room_data->get_id();
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
//                image_time_->setVisible(true);
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
//                image_time_->setVisible(true);
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
//                image_time_->setVisible(false);
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
//                image_time_->setVisible(false);
                user_icon_->setVisible(false);
                text_player_nickname_->setVisible(false);
                text_player_score_->setVisible(false);
                text_player_status_->setVisible(false);
                btn_sitdown_->setVisible(true);
                
                break;
            }
            case msg::SittingStatus::SITDOWN:{
                image_icon_->setVisible(true);
//                image_time_->setVisible(true);
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
//                image_time_->setVisible(true);
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
//        image_time_->setVisible(false);
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
        
        text_player_score_->setString(tools::local_string("score", "得分：") +":"+ tools::to_string(user_->get_all_score()));

        if (!user_->get_head_pic_name().empty())
        {
            if(GDM->is_file_exist(user_->get_head_pic_name()))
            {
//                Sprite* headmask = Sprite::create("Default/image_mask_circle.png");
                if (user_icon_ && user_icon_->getParent())
                {
                    user_icon_->removeFromParent();
                    user_icon_ = nullptr;
                }
                user_icon_ = ShaderSprite::create(user_->get_head_pic_name(), "Default/image_mask_circle.png");
//                user_icon_->initWithFile(user_->get_head_pic_name(), "Default/image_mask_circle.png");
                user_icon_->setScale((image_icon_->getContentSize().width-8) / user_icon_->getContentSize().width, (image_icon_->getContentSize().height -8)/ user_icon_->getContentSize().height);
                user_icon_->setZOrder(ZORDER_PLAYER_UI+1);
                root_->addChild(user_icon_);
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
        text_player_score_->setString(tools::local_string("score", "得分：")+": "+ tools::to_string(user_->get_all_score()));
    }
}

void NodeOfcPlayer::update_data(bool isRefreshAll)
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
        
        update_first_row_type(isRefreshAll);
        update_first_row_score();
        update_middle_row_type(isRefreshAll);
        update_middle_row_score();
        update_last_row_type(isRefreshAll);
        update_last_row_score();
//        update_fantasy_result();
    }
}

void NodeOfcPlayer::update_propupdate(bool isRefreshAll)
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
        
        update_first_row_type(isRefreshAll);
        update_first_row_score();
        update_middle_row_type(isRefreshAll);
        update_middle_row_score();
        update_last_row_type(isRefreshAll);
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
                sprite_handpokers_[i]->setScale(image_pokergrooves_[i]->getContentSize().width * 0.95 / sprite_handpokers_[i]->getContentSize().width,
                                                image_pokergrooves_[i]->getContentSize().height * 0.95 / sprite_handpokers_[i]->getContentSize().height);
                int_pokerIDs_[i] = allcards[i];
            }else
            {
                sprite_handpokers_[i] = nullptr;
            }
            
        }
    }
}

void NodeOfcPlayer::refresh_poker_card()
{
    if (user_ != nullptr)
    {
        for (int i = 0; i < sprite_handpokers_.size(); i++)
        {
            if (sprite_handpokers_[i] != nullptr)
            {
                PokerCardSprite::setCardSprite(int_pokerIDs_[i], sprite_handpokers_[i]);
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
        int32_t fantasynum = user_->get_fantasy();
        text_player_result_score_->setString((user_->get_step_score() > 0 ? "+" : "") + tools::to_string(user_->get_step_score()));
        if (user_->get_step_score() < 0)
        {
            text_player_result_score_->setColor(Color3B(238, 1, 29));
        }else
        {
            text_player_result_score_->setColor(Color3B(255, 238, 0));
        }
        text_player_result_score_->enableOutline(Color4B(0, 0, 0, 255), 3);
        text_player_result_score_->setVisible(true);
        image_result_bg_->setVisible(true);
//        if (fantasynum == -1)
//        {
//            root_->scheduleOnce(std::bind(&NodeOfcPlayer::schedule_dust_visible, this, std::placeholders::_1), 3, "schedule_dust_visible");
//        }
//        if (fantasynum > 0)
//        {
//            root_->scheduleOnce(std::bind(&NodeOfcPlayer::schedule_fantasy_visible, this, std::placeholders::_1), 3, "schedule_fantasy_visible");
//        }
//        root_->scheduleOnce(std::bind(&NodeOfcPlayer::schedule_close_all_result, this, std::placeholders::_1), 8, "schedule_close_all_result");
    }
}

#pragma mark schedule
void NodeOfcPlayer::schedule_dust_visible(float dt)
{
    if (image_first_row_patterns_->isVisible())
        image_first_row_patterns_->setVisible(false);
    if (image_player_score_first_0_->isVisible())
    {
        image_player_score_first_0_->setVisible(false);
        image_player_score_first_1_->setVisible(false);
        image_player_score_first_2_->setVisible(false);
        image_player_score_first_3_->setVisible(false);
    }
    
    if (image_middle_row_patterns_->isVisible())
        image_middle_row_patterns_->setVisible(false);
    if (image_player_score_middle_0_->isVisible())
    {
        image_player_score_middle_0_->setVisible(false);
        image_player_score_middle_1_->setVisible(false);
        image_player_score_middle_2_->setVisible(false);
        image_player_score_middle_3_->setVisible(false);
    }
    
    
    if (image_last_row_patterns_->isVisible())
        image_last_row_patterns_->setVisible(false);
    if (image_player_score_last_0_->isVisible())
    {
        image_player_score_last_0_->setVisible(false);
        image_player_score_last_1_->setVisible(false);
        image_player_score_last_2_->setVisible(false);
        image_player_score_last_3_->setVisible(false);
    }
    
    sprite_player_dust_->setVisible(true);
    image_dust_bg_->setVisible(true);
}

void NodeOfcPlayer::schedule_fantasy_visible(float dt)
{
//    if (text_player_patterns_first_->isVisible())
//        text_player_patterns_first_->setVisible(false);
//    if (text_player_score_first_->isVisible())
//        text_player_score_first_->setVisible(false);
//    
//    if (text_player_patterns_middle_->isVisible())
//        text_player_patterns_middle_->setVisible(false);
//    if (text_player_score_middle_->isVisible())
//        text_player_score_middle_->setVisible(false);
//    
//    if (text_player_patterns_last_->isVisible())
//        text_player_patterns_last_->setVisible(false);
//    if (text_player_score_last_->isVisible())
//        text_player_score_last_->setVisible(false);
    
    if (image_first_row_patterns_->isVisible())
        image_first_row_patterns_->setVisible(false);
    if (image_player_score_first_0_->isVisible())
    {
        image_player_score_first_0_->setVisible(false);
        image_player_score_first_1_->setVisible(false);
        image_player_score_first_2_->setVisible(false);
        image_player_score_first_3_->setVisible(false);
    }
    
    if (image_middle_row_patterns_->isVisible())
        image_middle_row_patterns_->setVisible(false);
    if (image_player_score_middle_0_->isVisible())
    {
        image_player_score_middle_0_->setVisible(false);
        image_player_score_middle_1_->setVisible(false);
        image_player_score_middle_2_->setVisible(false);
        image_player_score_middle_3_->setVisible(false);
    }
    
    
    if (image_last_row_patterns_->isVisible())
        image_last_row_patterns_->setVisible(false);
    if (image_player_score_last_0_->isVisible())
    {
        image_player_score_last_0_->setVisible(false);
        image_player_score_last_1_->setVisible(false);
        image_player_score_last_2_->setVisible(false);
        image_player_score_last_3_->setVisible(false);
    }
    sprite_player_fantasy_->setVisible(true);
}

void NodeOfcPlayer::schedule_close_all_result(float dt)
{
//    if (text_player_patterns_first_->isVisible())
//        text_player_patterns_first_->setVisible(false);
//    if (text_player_score_first_->isVisible())
//        text_player_score_first_->setVisible(false);
//    
//    if (text_player_patterns_middle_->isVisible())
//        text_player_patterns_middle_->setVisible(false);
//    if (text_player_score_middle_->isVisible())
//        text_player_score_middle_->setVisible(false);
//    
//    if (text_player_patterns_last_->isVisible())
//        text_player_patterns_last_->setVisible(false);
//    if (text_player_score_last_->isVisible())
//        text_player_score_last_->setVisible(false);
    if (image_first_row_patterns_->isVisible())
        image_first_row_patterns_->setVisible(false);
    if (image_player_score_first_0_->isVisible())
    {
        image_player_score_first_0_->setVisible(false);
        image_player_score_first_1_->setVisible(false);
        image_player_score_first_2_->setVisible(false);
        image_player_score_first_3_->setVisible(false);
    }
    
    if (image_middle_row_patterns_->isVisible())
        image_middle_row_patterns_->setVisible(false);
    if (image_player_score_middle_0_->isVisible())
    {
        image_player_score_middle_0_->setVisible(false);
        image_player_score_middle_1_->setVisible(false);
        image_player_score_middle_2_->setVisible(false);
        image_player_score_middle_3_->setVisible(false);
    }
    
    
    if (image_last_row_patterns_->isVisible())
        image_last_row_patterns_->setVisible(false);
    if (image_player_score_last_0_->isVisible())
    {
        image_player_score_last_0_->setVisible(false);
        image_player_score_last_1_->setVisible(false);
        image_player_score_last_2_->setVisible(false);
        image_player_score_last_3_->setVisible(false);
    }
    
    //sprite_player_dust_->stopAllActions();
    sprite_player_dust_->setVisible(false);
    
    if (sprite_player_fantasy_->isVisible())
        sprite_player_fantasy_->setVisible(false);
    
    if (text_player_result_score_->isVisible())
        text_player_result_score_->setVisible(false);
    if (image_result_bg_->isVisible())
    {
        image_result_bg_->setVisible(false);
    }
    image_dust_bg_->setVisible(false);
    
    Sprite* sprite_fantasy_result_bg = dynamic_cast<Sprite*>(root_->getChildByTag(TAG_SPRITE_FANTASY_BG));
    if (sprite_fantasy_result_bg)
    {
        sprite_fantasy_result_bg->removeFromParent();
    }
    Sprite* spriteFirstRowType = dynamic_cast<Sprite*>(root_->getChildByTag(TAG_MIDDLE_ROW_TYPE_EFFECT));
    if (spriteFirstRowType)
    {
        spriteFirstRowType->removeFromParent();
    }
    Sprite* sprite_middle_result_bg = dynamic_cast<Sprite*>(root_->getChildByTag(TAG_MIDDLE_ROW_TYPE_EFFECT));
    if (sprite_middle_result_bg)
    {
        sprite_middle_result_bg->removeFromParent();
    }
    Sprite* spriteLastRowType = dynamic_cast<Sprite*>(root_->getChildByTag(TAG_LAST_ROW_TYPE_EFFECT));
    if (spriteLastRowType)
    {
        spriteLastRowType->removeFromParent();
    }
    for (auto iterPokerGroove : image_pokergrooves_) { if (is_large_cell_) { iterPokerGroove->loadTexture("image/ofc_game_new_res/own_poker_bg.png"); }else{ iterPokerGroove->loadTexture("image/ofc_game_new_res/other_poker_bg.png"); } }
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
        if (is_large_cell_)
        {
            sprite_box_->setTexture("image/ofc_game_new_res/image_time_back_own.png");
        }
        else
        {
            sprite_box_->setTexture("image/ofc_game_new_res/image_time_back_other.png");
        }
    }else
    {
        if (is_large_cell_)
        {
            sprite_box_->setTexture("image/ofc_game_new_res/image_time_own.png");
        }
        else
        {
            sprite_box_->setTexture("image/ofc_game_new_res/image_time_other.png");
        }
    }
    
    current_operator_time_ = 0.0;
    text_time_->setString(tools::to_string(operator_time_) + "s");
    text_time_->setVisible(true);
    text_player_status_->setVisible(false);
    
   
    progress_player_ = ProgressTimer::create(sprite_box_);
    
    progress_player_->setPosition(sprite_box_->getPosition());
    progress_player_->setType(ProgressTimer::Type::RADIAL);
    progress_player_->setMidpoint(Vec2(0.5, 0.5));

    progress_player_->setReverseDirection(true);
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
    
    if (sprite_box_->isVisible())
    {
        sprite_box_->setVisible(false);
    }else{
        cocos2d::log("Error : NodeOfcPlayer Time_Close() sprite_time_bar isVisible false!");
    }
    
    current_operator_time_ = 0.0;
    text_time_->setString("");
    text_time_->setVisible(false);
    text_player_status_->setVisible(true);

}

void NodeOfcPlayer::schedule_player_func(float dt)
{
//    cocos2d::log("schedule_player_func");
    if (progress_player_)
    {
//        cocos2d::log("schedule_player_func : all:%f current:%f", operator_time_, progress_player_->getPercentage());
        
        current_operator_time_ += 1.0;
        float ispercent = current_operator_time_ / operator_time_ * 100.0;
        
        progress_player_->setPercentage(100.0 - ispercent);
//
        float time = operator_time_ - current_operator_time_;
        if (time < 0.0)
            time = 0.0;
        else if (time > 0.0 && time < 1.0)
            time = 1.0;
        text_time_->setString(tools::to_string(time) + "s");
//
        if (progress_player_->getPercentage() <= 0.0)
        {
            root_->unschedule("TAG_PROGRESS_TIME_PLAYER");
            progress_player_->removeFromParent();
            progress_player_ = nullptr;
            sprite_box_->setVisible(false);
            current_operator_time_ = 0.0;
            text_time_->setString("");
            text_time_->setVisible(false);
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

void NodeOfcPlayer::update_first_row_type(bool isRefreshAll)
{
    if (user_ != nullptr)
    {
        if (user_->get_first_row() != msg::OFCCardType::TYPE_NORMAL && user_->get_first_row() != msg::OFCCardType::NO_TYPE)
        {
            if (first_row_type_ != user_->get_first_row())
            {
                first_row_type_ = user_->get_first_row();
            }
            
            std::string type_name = user_->get_pattern_image_name(user_->get_first_row(), is_large_cell_);
//            image_first_row_patterns_->loadTexture(type_name);
            image_first_row_patterns_->setTexture(type_name);
            image_first_row_patterns_->setVisible(true);
        }
        else
        {
            image_first_row_patterns_->setVisible(false);
        }
        //text_player_patterns_first_->enableOutline(Color4B(0, 0, 0, 255), 5);
        
        
        //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_patterns_first_->setVisible(false);}), 3, "schedule_first_row_type_visible");
    }
}
void NodeOfcPlayer::update_middle_row_type(bool isRefreshAll)
{
    if (user_ != nullptr)
    {
        
        if (user_->get_middle_row() != msg::OFCCardType::TYPE_NORMAL && user_->get_middle_row() != msg::OFCCardType::NO_TYPE)
        {
            if (middle_row_type_ != user_->get_middle_row())
            {
                middle_row_type_ = user_->get_middle_row();
            }
            
            std::string type_name = user_->get_pattern_image_name(user_->get_middle_row(), is_large_cell_);
//            image_middle_row_patterns_->loadTexture(type_name);
            image_middle_row_patterns_->setTexture(type_name);
            image_middle_row_patterns_->setVisible(true);
        }
        else
        {
            image_middle_row_patterns_->setVisible(false);
        }
        
        //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_patterns_middle_->setVisible(false);}), 3, "schedule_middle_row_type_visible");
    }
}
void NodeOfcPlayer::update_last_row_type(bool isRefreshAll)
{
    if (user_ != nullptr)
    {
//        text_player_patterns_last_->setString(user_->get_last_row());
//        //text_player_patterns_last_->enableOutline(Color4B(0, 0, 0, 255), 5);
//        text_player_patterns_last_->setVisible(true);
//        if (user_->get_last_row() == msg::OFCCardType::TYPE_NORMAL)
//        {
//            log("update_last_row_type=====normal----%d",image_last_row_patterns_->isVisible());
//        }
//        else if (user_->get_last_row() == msg::OFCCardType::NO_TYPE)
//        {
//            log("update_last_row_type=====no type----%d",image_last_row_patterns_->isVisible());
//        }
//        else
//        {
//            log("update_last_row_type=====qita----%d",image_last_row_patterns_->isVisible());
//        }
        if (user_->get_last_row() != msg::OFCCardType::TYPE_NORMAL && user_->get_last_row() != msg::OFCCardType::NO_TYPE)
        {
            if (last_row_type_ != user_->get_last_row())
            {
                last_row_type_ = user_->get_last_row();
            }
            
            std::string type_name = user_->get_pattern_image_name(user_->get_last_row(), is_large_cell_);
//            image_last_row_patterns_->loadTexture(type_name);
            image_last_row_patterns_->setTexture(type_name);
            image_last_row_patterns_->setVisible(true);
        }
        else
        {
            image_last_row_patterns_->setVisible(false);
        }
        
        //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_patterns_last_->setVisible(false);}), 3, "schedule_last_row_type_visible");
    }
}

void NodeOfcPlayer::update_first_row_score()
{
    if (user_ != nullptr)
    {
        if (user_->get_first_row_score() > 0)
        {
            std::vector<std::string> image_name = user_->get_score_image_name(user_->get_first_row_score(), is_large_cell_);
            if (image_name.size() > 0)
            {
                image_player_score_first_0_->loadTexture(image_name.at(0));
                image_player_score_first_0_->setVisible(true);
            }
            if (image_name.size() > 1)
            {
                image_player_score_first_1_->loadTexture(image_name.at(1));
                image_player_score_first_1_->setVisible(true);
            }
            if (image_name.size() > 2)
            {
                image_player_score_first_2_->loadTexture(image_name.at(2));
                image_player_score_first_2_->setVisible(true);
            }
            if (image_name.size() > 3)
            {
                image_player_score_first_3_->loadTexture(image_name.at(3));
                image_player_score_first_3_->setVisible(true);
            }
        }else
        {
//            text_player_score_first_->setVisible(false);
            image_player_score_first_0_->setVisible(false);
            image_player_score_first_1_->setVisible(false);
            image_player_score_first_2_->setVisible(false);
            image_player_score_first_3_->setVisible(false);
        }
    }
}
void NodeOfcPlayer::update_middle_row_score()
{
    if (user_ != nullptr)
    {
        if (user_->get_middle_row_score() > 0)
        {
            std::vector<std::string> image_name = user_->get_score_image_name(user_->get_middle_row_score(), is_large_cell_);
            if (image_name.size() > 0)
            {
                image_player_score_middle_0_->loadTexture(image_name.at(0));
                image_player_score_middle_0_->setVisible(true);
            }
            if (image_name.size() > 1)
            {
                image_player_score_middle_1_->loadTexture(image_name.at(1));
                image_player_score_middle_1_->setVisible(true);
            }
            if (image_name.size() > 2)
            {
                image_player_score_middle_2_->loadTexture(image_name.at(2));
                image_player_score_middle_2_->setVisible(true);
            }
            if (image_name.size() > 3)
            {
                image_player_score_middle_3_->loadTexture(image_name.at(3));
                image_player_score_middle_3_->setVisible(true);
            }
            
        }else
        {
//            text_player_score_middle_->setVisible(false);
            image_player_score_middle_0_->setVisible(false);
            image_player_score_middle_1_->setVisible(false);
            image_player_score_middle_2_->setVisible(false);
            image_player_score_middle_3_->setVisible(false);
        }
    }
}
void NodeOfcPlayer::update_last_row_score()
{
    if (user_ != nullptr)
    {
        if (user_->get_last_row_score() > 0)
        {
            std::vector<std::string> image_name = user_->get_score_image_name(user_->get_last_row_score(), is_large_cell_);
            if (image_name.size() > 0)
            {
                image_player_score_last_0_->loadTexture(image_name.at(0));
                image_player_score_last_0_->setVisible(true);
            }
            if (image_name.size() > 1)
            {
                image_player_score_last_1_->loadTexture(image_name.at(1));
                image_player_score_last_1_->setVisible(true);
            }
            if (image_name.size() > 2)
            {
                image_player_score_last_2_->loadTexture(image_name.at(2));
                image_player_score_last_2_->setVisible(true);
            }
            if (image_name.size() > 3)
            {
                image_player_score_last_3_->loadTexture(image_name.at(3));
                image_player_score_last_3_->setVisible(true);
            }
        }else
        {
//            text_player_score_last_->setVisible(false);
            image_player_score_last_0_->setVisible(false);
            image_player_score_last_1_->setVisible(false);
            image_player_score_last_2_->setVisible(false);
            image_player_score_last_3_->setVisible(false);
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
            image_dust_bg_->setVisible(true);
            
            //if you need add other language result
            //you can use "addSpriteFrameWithFile" add other language result picture
            std::string baopai_image_root = "";
            if (is_large_cell_)
            {
                baopai_image_root=GameTools::get_languages_pic_key("baopai_2.png");
            }else{
                baopai_image_root=GameTools::get_languages_pic_key("baopai_1.png");
            }
            if(baopai_image_root.compare("Nopic")==0)//字典里面没有找到对应的图片
            {
                if (is_large_cell_)
                {
                    sprite_player_dust_->setTexture("image/ofc_game_new_res/image_baopai_own.png");
                }else{
                    sprite_player_dust_->setTexture("image/ofc_game_new_res/image_baopai_other.png");
                }
            }
            else
            {
                sprite_player_dust_->setTexture(baopai_image_root);
            }
        }
        if (fantasynum > 0)
        {
            sprite_player_fantasy_->setVisible(true);
            if (user_->get_refantasy())
            {
                if (is_large_cell_)
                {
                    sprite_player_fantasy_->setTexture("image/ofc_game_new_res/image_refantasy_own_en.png");
                }else{
                    sprite_player_fantasy_->setTexture("image/ofc_game_new_res/image_refantasy_other_en.png");
                }
            }else
            {
                if (is_large_cell_)
                {
                    sprite_player_fantasy_->setTexture("image/ofc_game_new_res/image_fantasy_own_en.png");
                }else{
                    sprite_player_fantasy_->setTexture("image/ofc_game_new_res/image_rantasy_other_en.png");
                }
            }
        }
        
    }
}

void NodeOfcPlayer::update_record_status()
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    bool is_record_ = ofc_room_data->get_is_record();
    if (is_record_)
    {
//        btn_confirm_->setVisible(false);
//        btn_sit_back_->setVisible(false);
//        btn_will_leave_->setVisible(false);
        image_icon_->setTouchEnabled(false);
    }else
    {
        
    }
}