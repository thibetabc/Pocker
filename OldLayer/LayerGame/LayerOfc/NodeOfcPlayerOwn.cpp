//
//  NewOfcPlayerOwn.cpp
//  pokerclient
//
//  Created by ll on 15/10/29.
//
//

#include "NodeOfcPlayerOwn.h"

#include "utils.h"
#include "GameTools.h"
#include "cmd_def.h"

#include "PokerCardSprite.h"

#include "PokerDataManager.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "AudioManager.h"

#include "Room.h"
#include "UserSettingPropertyCell.h"

#define ZORDER_IMAGE_PLAYR_BG 100
#define ZORDER_IMAGE_CARD_BG 200
#define ZORDER_PLAYER_UI 300
#define ZORDER_SPRITE_CARD 400
#define ZORDER_RESULT 500
#define ZORDER_SEL_SPRITE 1000

#define TAG_PROGRESS_ICON_CURRENT 299
#define TAG_PROGRESS_TIME_PLAYER 300
#define TAG_PROGRESS_TIME_OWN 301

bool NodeOfcPlayerOwn::init()
{
    if (!Node::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeOfcPlayerOwnPortrait.csb");
    this->addChild(root_);
    
    image_player_card_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_card_bg"));
    image_player_card_bg_->setZOrder(ZORDER_IMAGE_PLAYR_BG);
    
    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon"));
    image_icon_->setZOrder(ZORDER_PLAYER_UI);
    
    text_player_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_nickname"));
    text_player_nickname_->setString("");
    text_player_nickname_->setZOrder(ZORDER_PLAYER_UI);
    
    
    text_player_score_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_score"));
    text_player_score_->setString(tools::local_string("score", "得分：")+ "：");
    text_player_score_->setZOrder(ZORDER_PLAYER_UI);
    
    text_player_status_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_status"));
    text_player_status_->setString("");
    //text_player_status_->setVisible(false);
    text_player_status_->setZOrder(ZORDER_PLAYER_UI);
    
    sprite_dealer_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_dealer"));
    sprite_dealer_->setVisible(false);
    sprite_dealer_->setZOrder(ZORDER_PLAYER_UI);
    
    text_hole_cards_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_hand_card"));
    text_hole_cards_->setZOrder(ZORDER_IMAGE_PLAYR_BG+2);
    
    image_hole_cards_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_hand_card_bg"));
    image_hole_cards_bg_->setZOrder(ZORDER_IMAGE_PLAYR_BG+1);
    
    image_dis_cards_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_discard_bg"));
    image_dis_cards_bg_->setZOrder(ZORDER_IMAGE_PLAYR_BG+1);
    
    text_dis_cards_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_discard"));
    text_dis_cards_->setZOrder(ZORDER_IMAGE_PLAYR_BG+2);
    
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
    
    text_player_score_first_->setZOrder(ZORDER_RESULT);
    text_player_score_middle_->setZOrder(ZORDER_RESULT);
    text_player_score_last_->setZOrder(ZORDER_RESULT);
    text_player_patterns_first_->setZOrder(ZORDER_RESULT);
    text_player_patterns_middle_->setZOrder(ZORDER_RESULT);
    text_player_patterns_last_->setZOrder(ZORDER_RESULT);
    text_player_result_score_->setZOrder(ZORDER_RESULT);
    sprite_player_dust_->setZOrder(ZORDER_RESULT);
    sprite_player_fantasy_->setZOrder(ZORDER_RESULT);
    
    text_player_score_first_->setVisible(false);
    text_player_score_middle_->setVisible(false);
    text_player_score_last_->setVisible(false);
    text_player_patterns_first_->setVisible(false);
    text_player_patterns_middle_->setVisible(false);
    text_player_patterns_last_->setVisible(false);
    text_player_result_score_->setVisible(false);
    sprite_player_dust_->setVisible(false);
    sprite_player_fantasy_->setVisible(false);
    
    btn_confirm_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_confirm"));
    btn_confirm_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_confirm_->addClickEventListener(std::bind(&NodeOfcPlayerOwn::click_btn_confirm, this, std::placeholders::_1));
    
    btn_sit_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_sit_back"));
    btn_sit_back_->addClickEventListener(std::bind(&NodeOfcPlayerOwn::click_btn_sit_back, this, std::placeholders::_1));
    btn_sit_back_->setVisible(false);
    btn_sit_back_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    
    btn_will_leave_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_will_leave"));
    btn_will_leave_->addClickEventListener(std::bind(&NodeOfcPlayerOwn::click_btn_will_leave, this, std::placeholders::_1));
    btn_will_leave_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    
    for (int i = 0; i < string_image_pokergrooves_name_.size(); i++)
    {
        auto pokergroove = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, string_image_pokergrooves_name_[i]));
        pokergroove->setZOrder(ZORDER_IMAGE_CARD_BG);
        image_pokergrooves_.push_back(pokergroove);
        int_pokerIDs_.push_back(100);
        sprite_handpokers_.push_back(nullptr);
    }
    
    for (int i = 0; i < 3; i++)
    {
        sprite_holepokers_.push_back(nullptr);
        int_holepokerIDs_.push_back(100);
    }
    
    
    for (int i = 0; i < 4; i++)
    {
        sprite_dispokers_.push_back(nullptr);
    }
    
    
//    listview_autobutton_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview_autobutton"));
//    listview_autobutton_->setVisible(false);
//    
//    update_auto_buttons();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(NodeOfcPlayerOwn::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(NodeOfcPlayerOwn::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(NodeOfcPlayerOwn::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    Sprite* headmask = Sprite::create("image/ofc_game_res/image_player_icon_mask_large.png");
    user_icon_ = ShaderSprite::create("image/ofc_game_res/image_player_icon_large.png", "image/ofc_game_res/image_player_icon_mask_large.png");
    root_->addChild(user_icon_);
    user_icon_->setScale(headmask->getContentSize().width / user_icon_->getContentSize().width, headmask->getContentSize().height / user_icon_->getContentSize().height);
    user_icon_->setZOrder(ZORDER_PLAYER_UI+1);
    user_icon_->setPosition(image_icon_->getPosition());
    
    is_record_ = false;
    
    return true;
}

void NodeOfcPlayerOwn::update_record_status()
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    is_record_ = ofc_room_data->get_is_record();
    if (is_record_)
    {
        btn_confirm_->setVisible(false);
        btn_sit_back_->setVisible(false);
        btn_will_leave_->setVisible(false);
    }else
    {
        
    }
}

void NodeOfcPlayerOwn::set_user(std::shared_ptr<user_ofc> user)
{
    user_ = user;
    update_user_ui();
}

void NodeOfcPlayerOwn::update_user_ui(cmd_data_pointer)
{
    if (user_ != nullptr)
    {
        text_player_nickname_->setString(user_->get_nickname());
        
        text_player_score_->setString(tools::local_string("score", "得分") + "：" +tools::to_string(user_->get_all_score()));
        
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
                dispatch_.register_event(EVENT_DOWLOAD_FILE, BIND_FUNC(this, NodeOfcPlayerOwn::update_user_ui));
            }
        }
    }
}

void NodeOfcPlayerOwn::update_user_score()
{
    if (user_ != nullptr)
    {
        text_player_score_->setString(tools::local_string("score", "得分：")+ "：" + tools::to_string(user_->get_all_score()));
    }
}

void NodeOfcPlayerOwn::update_propupdate()
{
    if (user_ != nullptr)
    {
         if (!is_record_ && user_->get_ofc_action_status() == msg::OFCActionStatus::CHOOSING)
        {
            if (user_->get_is_ofc_hand_card_change())
            {
                update_hand_cards();
                update_player_cards();
                update_dis_cards();
            }
            btn_confirm_->setVisible(true);
        }else
        {
            update_hand_cards();
            update_player_cards();
            update_dis_cards();
            btn_confirm_->setVisible(false);
        }
        
        update_user_score();
        
        if (user_->get_ofc_action_status() == msg::OFCActionStatus::FANTASY)
        {
            
        }else
        {
            
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

void NodeOfcPlayerOwn::update_player_cards()
{
    if (user_ != nullptr)
    {
        std::vector<uint32_t> allcards = user_->get_all_cards();
        std::vector<uint32_t> holecards = user_->get_all_hand_cards();
        
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
                if (user_->find_hand_card(allcards[i]))
                {
                    Sprite* hole_card_mark = Sprite::create("image/ofc_game_res/image_hole_card_mark.png");
                    sprite_handpokers_[i]->addChild(hole_card_mark);
                    hole_card_mark->setScale(0.8, 0.8);
                    hole_card_mark->setAnchorPoint(Vec2(1, 1));
                    hole_card_mark->setPosition(Point(sprite_handpokers_[i]->getContentSize().width, sprite_handpokers_[i]->getContentSize().height));
                }
//                for (int j = 0; j < holecards.size(); j++)
//                {
//                    if (allcards[i] == holecards[j])
//                    {
//                        Sprite* hole_card_mark = Sprite::create("image/ofc_game_res/image_hole_card_mark.png");
//                        sprite_handpokers_[i]->addChild(hole_card_mark);
//                        hole_card_mark->setScale(0.8, 0.8);
//                        hole_card_mark->setAnchorPoint(Vec2(1, 1));
//                        hole_card_mark->setPosition(Point(sprite_handpokers_[i]->getContentSize().width, sprite_handpokers_[i]->getContentSize().height));
//                    }
//                }
                
                root_->addChild(sprite_handpokers_[i]);
                sprite_handpokers_[i]->setPosition(image_pokergrooves_[i]->getPosition());
                sprite_handpokers_[i]->setScale(image_pokergrooves_[i]->getContentSize().width / sprite_handpokers_[i]->getContentSize().width, image_pokergrooves_[i]->getContentSize().height / sprite_handpokers_[i]->getContentSize().height);
                int_pokerIDs_[i] = allcards[i];
            }else
            {
                sprite_handpokers_[i] = nullptr;
            }
            
        }
    }
}

void NodeOfcPlayerOwn::update_hand_cards()
{
    if (user_ != nullptr)
    {
        selSprite = nullptr;
        std::vector<uint32_t> cards = user_->get_hand_cards();
        std::vector<uint32_t> allholecards = user_->get_all_hand_cards();
        
        if (!is_record_)
            btn_confirm_->setVisible(allholecards.size() > 0);
        
        for (int i = 0; i < 3; i++)
        {
            if (sprite_holepokers_[i] != nullptr && sprite_holepokers_[i]->getParent())
            {
                sprite_holepokers_[i]->removeFromParent();
                sprite_holepokers_[i] = nullptr;
            }
        }
        
        if (cards.size() > 5)
        {
            
        }else if (cards.size() <=5 && cards.size() > 3)
        {
            if (!is_record_)
                btn_confirm_->setVisible(true);
            for (int i = 0; i < cards.size(); i++)
            {
                user_->putcard(cards[i], msg::OFCCardLocation::LAST);
            }
            update_player_cards();
        }else if (cards.size() <=3 && cards.size() > 0)
        {
            for (int i = 0; i < cards.size(); i++)
            {
                auto & holepokerID = int_holepokerIDs_[i];
                auto & holepoker = sprite_holepokers_[i];
                
                holepokerID = cards[i];
                holepoker = PokerCardSprite::createCardSprite(int_holepokerIDs_[i]);
                Sprite* hole_card_mark = Sprite::create("image/ofc_game_res/image_hole_card_mark.png");
                holepoker->addChild(hole_card_mark);
                hole_card_mark->setScale(0.8, 0.8);
                hole_card_mark->setAnchorPoint(Vec2(1, 1));
                hole_card_mark->setPosition(Point(holepoker->getContentSize().width, holepoker->getContentSize().height));
                holepoker->setZOrder(ZORDER_SPRITE_CARD);
                holepoker->setScale(image_hole_cards_bg_->getContentSize().height / holepoker->getContentSize().height);
                root_->addChild(holepoker);
                switch (i)
                {
                    case 0:{
                        holepoker->setPosition(image_hole_cards_bg_->getPosition()-Point(image_hole_cards_bg_->getContentSize().width/3,0));
                        break;
                    }
                    case 1:{
                        
                        holepoker->setPosition(image_hole_cards_bg_->getPosition());
                        break;
                    }
                    case 2:{
                        
                        holepoker->setPosition(image_hole_cards_bg_->getPosition()+Point(image_hole_cards_bg_->getContentSize().width/3, 0));
                        break;
                    }
                    default:{
                        cocos2d::log("Error : NodeOfcPlayerOwn Update_Hand_cards!");
                        break;
                    }
                }
            }

        }
        
    }
}

void NodeOfcPlayerOwn::update_seat_info()
{
    auto user_data = user_;
    if (user_data)
    {
        msg::SittingStatus status = user_data->get_status();
        switch (status)
        {
            case msg::SittingStatus::PLAYING:{
                user_icon_->setVisible(true);
                text_player_nickname_->setVisible(true);
                text_player_score_->setVisible(true);
                text_player_status_->setString("");
                btn_sit_back_->setVisible(false);
                if (!is_record_)
                     btn_will_leave_->setVisible(true);
                //listview_autobutton_->setVisible(true);
                break;
            }
            case msg::SittingStatus::SITOUT:{
                text_player_status_->setString(tools::local_string("player_sitting_out", "玩家Sitout"));
                if (!is_record_)
                    btn_sit_back_->setVisible(true);
                //listview_autobutton_->setVisible(false);
                btn_will_leave_->setVisible(false);
                break;
            }
            case msg::SittingStatus::OBSERVER:{
                user_ = nullptr;
                user_icon_->setVisible(false);
                text_player_nickname_->setVisible(false);
                text_player_score_->setVisible(false);
                text_player_status_->setVisible(false);
                btn_sit_back_->setVisible(false);
                //listview_autobutton_->setVisible(false);
                btn_will_leave_->setVisible(false);
                break;
            }
            case msg::SittingStatus::LEAVE:{
                user_ = nullptr;
                user_icon_->setVisible(false);
                text_player_nickname_->setVisible(false);
                text_player_score_->setVisible(false);
                text_player_status_->setVisible(false);
                btn_sit_back_->setVisible(false);
                //listview_autobutton_->setVisible(false);
                btn_will_leave_->setVisible(false);
                break;
            }
            case msg::SittingStatus::SITDOWN:{
                user_icon_->setVisible(true);
                text_player_nickname_->setVisible(true);
                text_player_score_->setVisible(true);
                text_player_status_->setString(tools::local_string("waiting_for_next_game", "等待下局游戏"));
                btn_sit_back_->setVisible(false);
                //listview_autobutton_->setVisible(false);
                btn_will_leave_->setVisible(false);
                break;
            }
            case msg::SittingStatus::WANTTOLEAVE:{
                user_icon_->setVisible(true);
                text_player_nickname_->setVisible(true);
                text_player_score_->setVisible(true);
                text_player_status_->setString(tools::local_string("leaving_soon", "即将离开"));
                btn_sit_back_->setVisible(false);
                //listview_autobutton_->setVisible(false);
                if (!is_record_)
                     btn_will_leave_->setVisible(true);
                break;
            }
        }
    }
}

void NodeOfcPlayerOwn::update_dis_cards()
{
    if (user_ != nullptr)
    {
        selSprite = nullptr;
        std::vector<uint32_t> cards = user_->get_dis_cards();
        
        for (int i = 0; i < sprite_dispokers_.size(); i++)
        {
            if (sprite_dispokers_[i] != nullptr && sprite_dispokers_[i]->getParent())
            {
                sprite_dispokers_[i]->removeFromParent();
                sprite_dispokers_[i] = nullptr;
            }
        }
        for (int i = 0; i < cards.size(); i++)
        {
            auto & dispoker = sprite_dispokers_[i];
            
            dispoker = PokerCardSprite::createCardSprite(cards[i]);
            dispoker->setZOrder(ZORDER_SPRITE_CARD);
            dispoker->setScale(image_dis_cards_bg_->getContentSize().height / dispoker->getContentSize().height);
            root_->addChild(dispoker);

            float image_dis_cards_bg_point_X = image_dis_cards_bg_->getPosition().x;
            float image_dis_cards_bg_point_Y = image_dis_cards_bg_->getPosition().y;
            Vec2 image_dis_cards_bg_Anchor = image_dis_cards_bg_->getAnchorPoint();
            Size image_dis_cards_bg_Size = image_dis_cards_bg_->getContentSize();
            
            float dispoker_X = image_dis_cards_bg_point_X - image_dis_cards_bg_Anchor.x * image_dis_cards_bg_Size.width + (image_dis_cards_bg_Size.width / cards.size())* (i+1);
            
            dispoker->setPosition((image_dis_cards_bg_->getPosition().x -
                                   (image_dis_cards_bg_->getAnchorPoint().x * image_dis_cards_bg_->getContentSize().width) + ((image_dis_cards_bg_->getContentSize().width / 4) * (i)) + dispoker->getContentSize().width * (image_dis_cards_bg_->getContentSize().height / dispoker->getContentSize().height) / 2),
                                  image_dis_cards_bg_->getPosition().y);

        }
        
    }
}

void NodeOfcPlayerOwn::update_dealer()
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    uint32_t dealer = ofc_room_data->get_dealer_position();
    if (user_ != nullptr &&
        user_->get_seat_num() == dealer)
    {
        set_dealer(true);
    }else
    {
        set_dealer(false);
    }
}

void NodeOfcPlayerOwn::update_poker_result()
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
//            root_->scheduleOnce(std::bind(&NodeOfcPlayerOwn::schedule_dust_visible, this, std::placeholders::_1), 3, "schedule_dust_visible");
//        }
//        if (fantasynum > 0)
//        {
//            root_->scheduleOnce(std::bind(&NodeOfcPlayerOwn::schedule_fantasy_visible, this, std::placeholders::_1), 3, "schedule_fantasy_visible");
//        }
        //root_->scheduleOnce(std::bind(&NodeOfcPlayer::schedule_close_all_result, this, std::placeholders::_1), 8, "schedule_close_all_result");
    }
}

void NodeOfcPlayerOwn::set_dealer(bool isOn)
{
    if (isOn)
        sprite_dealer_->setVisible(true);
    else
        sprite_dealer_->setVisible(false);
}

void NodeOfcPlayerOwn::set_clear_result()
{
    schedule_close_all_result(0);
    if (user_ != nullptr)
    {
        user_->clean_all_cards();
        user_->clean_dis_cards();
    }
    for (int i = 0; i < 13; i++)
    {
        if (sprite_handpokers_[i] != nullptr && sprite_handpokers_[i]->getParent())
        {
            sprite_handpokers_[i]->removeFromParent();
            sprite_handpokers_[i] = nullptr;
        }
    }
    
    for (int i = 0; i < sprite_dispokers_.size(); i++)
    {
        if (sprite_dispokers_[i] != nullptr && sprite_dispokers_[i]->getParent())
        {
            sprite_dispokers_[i]->removeFromParent();
            sprite_dispokers_[i] = nullptr;
        }
    }
}

void NodeOfcPlayerOwn::update_first_row_type()
{
    if (user_ != nullptr)
    {
        text_player_patterns_first_->setString(user_->get_first_row());
        //text_player_patterns_first_->enableOutline(Color4B(0, 0, 0, 255), 5);
        text_player_patterns_first_->setVisible(true);
        
        //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_patterns_first_->setVisible(false);}), 3, "schedule_first_row_type_visible");
    }
}
void NodeOfcPlayerOwn::update_middle_row_type()
{
    if (user_ != nullptr)
    {
        text_player_patterns_middle_->setString(user_->get_middle_row());
        //text_player_patterns_middle_->enableOutline(Color4B(0, 0, 0, 255), 5);
        text_player_patterns_middle_->setVisible(true);
        
        //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_patterns_middle_->setVisible(false);}), 3, "schedule_middle_row_type_visible");
    }
}
void NodeOfcPlayerOwn::update_last_row_type()
{
    if (user_ != nullptr)
    {
        text_player_patterns_last_->setString(user_->get_last_row());
        //text_player_patterns_last_->enableOutline(Color4B(0, 0, 0, 255), 5);
        text_player_patterns_last_->setVisible(true);
        
        //root_->scheduleOnce(std::function<void(float)>([=](float dt){text_player_patterns_last_->setVisible(false);}), 3, "schedule_last_row_type_visible");
    }
}

void NodeOfcPlayerOwn::update_first_row_score()
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
void NodeOfcPlayerOwn::update_middle_row_score()
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
void NodeOfcPlayerOwn::update_last_row_score()
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

void NodeOfcPlayerOwn::update_fantasy_result()
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

void NodeOfcPlayerOwn::update_auto_buttons(msg::OFCPlayerSetting& setting)
{
    if (user_ != nullptr)
    {
        user_->set_ofc_player_setting(setting);
    }
    if (setting.has_sitoutnexthand())
    {
        bool sitoutnexthand = setting.sitoutnexthand();
        if (sitoutnexthand)
        {
            btn_will_leave_->loadTextureNormal("image/ofc_game_res/btn_will_leave_pressed.png");
            btn_will_leave_->loadTexturePressed("image/ofc_game_res/btn_will_leave_normal.png");
            btn_will_leave_->loadTextureDisabled("image/ofc_game_res/btn_will_leave_normal.png");
        }else
        {
            btn_will_leave_->loadTextureNormal("image/ofc_game_res/btn_will_leave_normal.png");
            btn_will_leave_->loadTexturePressed("image/ofc_game_res/btn_will_leave_pressed.png");
            btn_will_leave_->loadTextureDisabled("image/ofc_game_res/btn_will_leave_pressed.png");
        }
    }

}

#pragma mark ClickEvent
void NodeOfcPlayerOwn::click_btn_confirm(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    uint32_t room_id = ofc_room_data->get_room_id();
    if (user_ != nullptr)
    {
        std::vector<msg::OFCCardLocation> result;
        std::vector<msg::OFCCardLocation> location = user_->get_hand_card_location();
        if (location.size() > 0)
            for(int i = 0; i < location.size(); i++)
                result.push_back(location[i]);
        PDM->send_player_choose_cards(result, room_id);
        for (int i = 0; i < sprite_holepokers_.size(); i++)
        {
            if (sprite_holepokers_[i] != nullptr)
            {
                sprite_holepokers_[i]->removeFromParent();
                sprite_holepokers_[i] = nullptr;
            }
        }
        
        btn_confirm_->setVisible(false);
    }
}

void NodeOfcPlayerOwn::click_btn_sit_back(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    uint32_t room_id = ofc_room_data->get_room_id();
    if (user_ != nullptr)
    {
        uint32_t seat_id = user_->get_seat_num();
        PDM->send_seat_info(room_id, msg::SittingStatus::SITDOWN, seat_id);
    }else
    {
        cocos2d::log("NodeOfcPlayerOwn : click_btn_sit_back user_ = nullptr!");
    }

}

void NodeOfcPlayerOwn::click_btn_will_leave(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    uint32_t room_id = ofc_room_data->get_room_id();
    if (user_ != nullptr)
    {
        if (user_->get_ofc_player_setting().has_sitoutnexthand())
        {
            if (user_->get_ofc_player_setting().sitoutnexthand())
            {
                user_->get_ofc_player_setting().set_sitoutnexthand(false);
                btn_will_leave_->loadTextureNormal("image/ofc_game_res/btn_will_leave_normal.png");
                btn_will_leave_->loadTexturePressed("image/ofc_game_res/btn_will_leave_pressed.png");
                btn_will_leave_->loadTextureDisabled("image/ofc_game_res/btn_will_leave_pressed.png");
            }else
            {
                user_->get_ofc_player_setting().set_sitoutnexthand(true);
                btn_will_leave_->loadTextureNormal("image/ofc_game_res/btn_will_leave_pressed.png");
                btn_will_leave_->loadTexturePressed("image/ofc_game_res/btn_will_leave_normal.png");
                btn_will_leave_->loadTextureDisabled("image/ofc_game_res/btn_will_leave_normal.png");
            }
        }else
        {
            user_->get_ofc_player_setting().set_sitoutnexthand(true);
            
            btn_will_leave_->loadTextureNormal("image/ofc_game_res/btn_will_leave_pressed.png");
            btn_will_leave_->loadTexturePressed("image/ofc_game_res/btn_will_leave_normal.png");
            btn_will_leave_->loadTextureDisabled("image/ofc_game_res/btn_will_leave_normal.png");
        }
        
        PDM->send_ofc_set_change(user_->get_ofc_player_setting(), room_id);
    }
}


#pragma mark TouchEvent
bool NodeOfcPlayerOwn::onTouchBegan(Touch* touch, Event* event)
{
    //将openGL坐标转化为node坐标
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    this->selectSpriteForTouch(touchLocation);
    return true;
}

void NodeOfcPlayerOwn::onTouchMoved(Touch* touch, Event* event)
{
    if(NULL == selSprite || selSprite->getReferenceCount() == 0)
    {
        return;
    }
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    
    Point oldTouchLocation = touch->getPreviousLocation();
    oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
    
    Point translation = touchLocation - oldTouchLocation;
    
    Point newPos = selSprite->getPosition() + translation;
    selSprite->setPosition(newPos);
}

void NodeOfcPlayerOwn::onTouchEnded(Touch* touch, Event* event)
{
    if(NULL == selSprite)
    {
        return;
    }
    
    //遍历判断交换的目的位置是否在 5张区
    bool hole_card_change = true;
    for (int i = 0; i < sprite_holepokers_.size(); i++)
    {
        auto & poker = sprite_holepokers_[i];
        auto & pokerID = int_holepokerIDs_[i];
        
        if (poker != nullptr &&
            poker->getBoundingBox().containsPoint(selSprite->getPosition()))
        {
            hole_card_change = false;
            user_->swap(selCard, pokerID);
        }
    }
    
    if (hole_card_change &&
        image_hole_cards_bg_->getBoundingBox().containsPoint(selSprite->getPosition()))
    {
        for (int i = 0; i < sprite_holepokers_.size(); i++)
        {
            if (sprite_holepokers_[i] == nullptr)
            {
                user_->putcard(selCard, msg::OFCCardLocation::DISCARD);
            }
        }
    }
    //遍历13张手牌区，如果目标位置没有手牌，则放置手牌到目标位置
    //如果目标位置有手牌，且已经被确认过，则该手牌回到原始位置
    //如果目标位置有手牌，但没被确认过，则两张手牌交换。
    for (int i = 0; i < 13; i++)
    {
        auto & groove = image_pokergrooves_[i];
        auto & poker = sprite_handpokers_[i];
        auto & pokerID = int_pokerIDs_[i];

        if (groove->getBoundingBox().containsPoint(selSprite->getPosition()))
        {
            if (poker != nullptr &&
                user_->swap(selCard, pokerID))
            {
                
            }else
            {
                if (i < 3)
                    user_->putcard(selCard, msg::OFCCardLocation::FIRST);
                else if (i < 8)
                    user_->putcard(selCard, msg::OFCCardLocation::MIDDLE);
                else
                    user_->putcard(selCard, msg::OFCCardLocation::LAST);
            }
        }
    }
    
    //selSprite->setPosition(selStartPoint);
    //selSprite->setLocalZOrder(ZORDER_CARD);
    selSprite->stopAllActions();
    selSprite->runAction(RotateTo::create(0.1, 0));
    selSprite = nullptr; //触摸结束以后，不再有被选中精灵
//    refresh_hand_cards();
//    show_all_player();
    update_player_cards();
    update_hand_cards();
}

void NodeOfcPlayerOwn::selectSpriteForTouch(Point touchLocation)
{
    Sprite* newSprite = nullptr;
    
    //遍历5张临时手牌区，如果有手牌且触摸点在手牌矩形内，则选中精灵为该手牌
    for (int i = 0; i < sprite_holepokers_.size(); i++)
    {
        auto & poker = sprite_holepokers_[i];
        auto & pokerID = int_holepokerIDs_[i];
        if (poker != nullptr)
        {
            if (poker->getBoundingBox().containsPoint(touchLocation))
            {
                newSprite = poker;
                selCard = pokerID;
            }
        }
    }
    
    //遍历13张手牌区，如果有手牌且触摸点在手牌矩形内，且该手牌未被确认，则选中精灵为该手牌
    for (int i = 0; i < sprite_handpokers_.size(); i++)
    {
        auto & poker = sprite_handpokers_[i];
        auto & pokerID = int_pokerIDs_[i];
        
        if (poker != nullptr &&
            poker->getBoundingBox().containsPoint(touchLocation))
        {
            std::vector<uint32_t> cards = user_->get_all_hand_cards();
            for (int j = 0; j < cards.size(); j++)
            {
                if (pokerID == cards[j])
                {
                    newSprite = poker;
                    selCard = pokerID;
                }
            }
        }
    }
    
    
    //被选中的手牌摇动，表示被选中
    if (newSprite!= selSprite && NULL != newSprite)
    {
        if (NULL != selSprite)
        {
            selSprite->stopAllActions();
            selSprite->runAction(RotateTo::create(0.1, 0));
        }
        RotateBy * rotLeft = RotateBy::create(0.1, -14.0);
        RotateBy * rotCenter = RotateBy::create(0.1, 0.0);
        RotateBy * rotRight = RotateBy::create(0.1, 14.0);
        Sequence * rotSeq = Sequence::create(rotLeft, rotCenter, rotRight, rotCenter, NULL);
        newSprite->runAction(RepeatForever::create(rotSeq));
        selSprite = newSprite;
        //selStartPoint = selSprite->getPosition();
        selSprite->setLocalZOrder(ZORDER_SEL_SPRITE);
    }
}

void NodeOfcPlayerOwn::set_ProgressTimer(bool isOn, float operator_time, bool is_using_timebank)
{
    if (user_ != nullptr)
    {
        if (isOn)
        {
            is_using_timebank_ = is_using_timebank;
            operator_time_ = operator_time;
            if (operator_time_ == 0.0)
            {
                time_player_close();
            }
            else
            {
                time_player_start();
                AM->playEffect("sound/alarm_active.mp3");
            }
        }else{
            time_player_close();
        }
    }
}

void NodeOfcPlayerOwn::time_player_start()
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
    
    root_->schedule(std::bind(&NodeOfcPlayerOwn::schedule_player_func, this, std::placeholders::_1), 1, "TAG_PROGRESS_TIME_PLAYER");
}

void NodeOfcPlayerOwn::time_player_close()
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
    image_player_card_bg_->loadTexture("image/ofc_game_res/image_player_bg_large.png");
}

#pragma mark schedule
void NodeOfcPlayerOwn::schedule_dust_visible(float dt)
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

void NodeOfcPlayerOwn::schedule_fantasy_visible(float dt)
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

void NodeOfcPlayerOwn::schedule_close_all_result(float dt)
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


void NodeOfcPlayerOwn::schedule_player_func(float dt)
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