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
//#include "LayerOfcClickPlayerPhoto.h"

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

NodeOfcPlayerOwn* NodeOfcPlayerOwn::create(bool is_other_user)
{
    NodeOfcPlayerOwn* player = new NodeOfcPlayerOwn;
    if ( player->init(is_other_user) )
    {
        player->autorelease();
        return player;
    }
    return nullptr;
}

bool NodeOfcPlayerOwn::init(bool is_other_user)
{
    if (!Node::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeOfcPlayerOwnPortraitNew.csb");
    this->addChild(root_);
    
    is_other_user_ = is_other_user;
    progress_player_ = nullptr;
    
//    image_player_card_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_card_bg"));
//    image_player_card_bg_->setZOrder(ZORDER_IMAGE_PLAYR_BG);
    
    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon"));
    image_icon_->setZOrder(ZORDER_PLAYER_UI);
    image_icon_->setTouchEnabled(true);
    image_icon_->addClickEventListener(std::bind(&NodeOfcPlayerOwn::click_user_icon,this,std::placeholders::_1));
    
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
    
    sprite_dealer_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "sprite_dealer"));
    sprite_dealer_->setVisible(false);
    sprite_dealer_->setZOrder(ZORDER_RESULT+1);
    
//    text_hole_cards_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_hand_card"));
//    text_hole_cards_->setZOrder(ZORDER_IMAGE_PLAYR_BG+2);
    
//    image_hole_cards_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_hand_card_bg"));
//    image_hole_cards_bg_->setZOrder(ZORDER_IMAGE_PLAYR_BG+1);
    
    image_dis_cards_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_discard_bg"));
    image_dis_cards_bg_->setZOrder(ZORDER_IMAGE_PLAYR_BG+1);
    
//    text_dis_cards_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_discard"));
//    text_dis_cards_->setZOrder(ZORDER_IMAGE_PLAYR_BG+2);
    
//    sprite_time_bar_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_time_bar"));
//    sprite_time_bar_->setVisible(false);
//    sprite_time_bar_->setZOrder(ZORDER_PLAYER_UI);
    
    text_time_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_time"));
    text_time_->setZOrder(ZORDER_PLAYER_UI+3);
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
    image_result_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_,"image_result_bg"));
    
    sprite_player_dust_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_bust"));
    sprite_player_fantasy_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_fantasy"));
    
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
    
    image_hand_card_bg_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_hand_card_bg_1"));
    image_hand_card_bg_2_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_hand_card_bg_2"));
    image_hand_card_bg_3_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_hand_card_bg_3"));
    
    image_fantasy_mark_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_fantasy_mark"));
    image_fantasy_mark_->setVisible(false);
    
    image_dust_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_dust_bg"));
    image_dust_bg_->setVisible(false);
    
    btn_sit_down_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_sit_down"));
    btn_sit_down_->setVisible(false);
    
    image_hand_card_bg_1_->setZOrder(ZORDER_IMAGE_PLAYR_BG+1);
    image_hand_card_bg_2_->setZOrder(ZORDER_IMAGE_PLAYR_BG+1);
    image_hand_card_bg_3_->setZOrder(ZORDER_IMAGE_PLAYR_BG+1);
    
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
//    sprite_box_->removeFromParent();
    
    btn_confirm_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_confirm"));
    btn_confirm_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_confirm_->addClickEventListener(std::bind(&NodeOfcPlayerOwn::click_btn_confirm, this, std::placeholders::_1));
    GameTools::set_pic_change(btn_confirm_,"btn_ofc_confirm_normal.png","btn_ofc_confirm_pressed.png");
    
    btn_sit_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_sit_back"));
    btn_sit_back_->addClickEventListener(std::bind(&NodeOfcPlayerOwn::click_btn_sit_back, this, std::placeholders::_1));
    btn_sit_back_->setVisible(false);
    btn_sit_back_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_sit_back_->setTitleText(tools::local_string("sit_back","坐回去"));
    
    btn_will_leave_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_will_leave"));
    btn_will_leave_->addClickEventListener(std::bind(&NodeOfcPlayerOwn::click_btn_will_leave, this, std::placeholders::_1));
    btn_will_leave_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_will_leave_->setTitleText(tools::local_string("leave","暂离"));
    
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
    
    
   
    if (!is_other_user_)
    {
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(false);
        listener->onTouchBegan = CC_CALLBACK_2(NodeOfcPlayerOwn::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(NodeOfcPlayerOwn::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(NodeOfcPlayerOwn::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }
    

    user_icon_ = ShaderSprite::create("Default/image_defualt_user_icon_large.png", "Default/image_mask_circle.png");
    root_->addChild(user_icon_);
    user_icon_->setScale((image_icon_->getContentSize().width-8) / user_icon_->getContentSize().width, (image_icon_->getContentSize().height -8)/ user_icon_->getContentSize().height);

    user_icon_->setZOrder(ZORDER_PLAYER_UI+1);
    user_icon_->setPosition(image_icon_->getPosition());
    
    is_record_ = false;
    
    if (is_other_user_)
    {
        btn_will_leave_->setVisible(false);
        image_hand_card_bg_1_->setVisible(false);
        image_hand_card_bg_2_->setVisible(false);
        image_hand_card_bg_3_->setVisible(false);
        
        image_dis_cards_bg_->setVisible(false);
        btn_confirm_->setVisible(false);
    }
    
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
        image_icon_->setTouchEnabled(false);
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
//                Sprite* headmask = Sprite::create("image/ofc_game_res/image_player_icon_mask_large.png");
                user_icon_->initWithFile(user_->get_head_pic_name(), "Default/image_mask_circle.png");
                user_icon_->setScale((image_icon_->getContentSize().width-8) / user_icon_->getContentSize().width, (image_icon_->getContentSize().height -8)/ user_icon_->getContentSize().height);
//                user_icon_->setScale(image_icon_->getContentSize().width / user_icon_->getContentSize().width, image_icon_->getContentSize().height / user_icon_->getContentSize().height);
                user_icon_->setZOrder(ZORDER_PLAYER_UI+1);
                user_icon_->setPosition(image_icon_->getPosition());
//                user_icon_->setPosition(image_time_->getPosition());
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
void NodeOfcPlayerOwn::update_data(bool isRefreshAll)
{
    if (user_ != nullptr)
    {
        if (!is_record_ && user_->get_ofc_action_status() == msg::OFCActionStatus::CHOOSING)
        {
            update_hand_cards();
            update_player_cards();
            update_dis_cards();
            
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
        
        update_first_row_type(isRefreshAll);
        update_first_row_score();
        update_middle_row_type(isRefreshAll);
        update_middle_row_score();
        update_last_row_type(isRefreshAll);
        update_last_row_score();
//        update_fantasy_result();
    }
}

void NodeOfcPlayerOwn::update_propupdate(bool isRefreshAll)
{
    if (user_ != nullptr)
    {
        if (!is_record_ && user_->get_ofc_action_status() == msg::OFCActionStatus::CHOOSING)
        {
            update_hand_cards();
            update_player_cards();
            update_dis_cards();
            
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
        
        update_first_row_type(isRefreshAll);
        update_first_row_score();
        update_middle_row_type(isRefreshAll);
        update_middle_row_score();
        update_last_row_type(isRefreshAll);
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
                
                root_->addChild(sprite_handpokers_[i]);
                sprite_handpokers_[i]->setPosition(image_pokergrooves_[i]->getPosition());
                sprite_handpokers_[i]->setScale(image_pokergrooves_[i]->getContentSize().width * 0.95 / sprite_handpokers_[i]->getContentSize().width, image_pokergrooves_[i]->getContentSize().height * 0.95 / sprite_handpokers_[i]->getContentSize().height);
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
//                holepoker->setScale(image_hole_cards_bg_->getContentSize().height / holepoker->getContentSize().height);
                holepoker->setScale(image_hand_card_bg_1_->getContentSize().height / holepoker->getContentSize().height);
                root_->addChild(holepoker);
                switch (i)
                {
                    case 0:{
                        holepoker->setPosition(image_hand_card_bg_1_->getPosition());
                        break;
                    }
                    case 1:{
                        
                        holepoker->setPosition(image_hand_card_bg_2_->getPosition());
                        break;
                    }
                    case 2:{
                        
                        holepoker->setPosition(image_hand_card_bg_3_->getPosition());
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

void NodeOfcPlayerOwn::refresh_poker_card()
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
        for (int i = 0; i < sprite_holepokers_.size(); i++)
        {
            if (sprite_holepokers_[i] != nullptr)
            {
                PokerCardSprite::setCardSprite(int_holepokerIDs_[i], sprite_holepokers_[i]);
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
//                image_time_
                user_icon_->setVisible(true);
                text_player_nickname_->setVisible(true);
                text_player_score_->setVisible(true);
                text_player_status_->setString("");
                btn_sit_back_->setVisible(false);
                if (!is_record_)
                    btn_will_leave_->setVisible(true);
                //listview_autobutton_->setVisible(true);
                btn_sit_down_->setVisible(false);
                break;
            }
            case msg::SittingStatus::SITOUT:{
                text_player_status_->setString(tools::local_string("player_sitting_out", "玩家Sitout"));
                if (!is_record_)
                {
                    btn_sit_back_->setVisible(true);
                    btn_will_leave_->setVisible(false);
                }
                //listview_autobutton_->setVisible(false);
//                image_time_->setVisible(true);
                image_icon_->setVisible(true);
                user_icon_->setVisible(true);
//                if (is_other_user_)
//                {
                    btn_sit_down_->setVisible(false);
//                }
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
                if (is_other_user_)
                {
                    btn_sit_down_->setVisible(true);
                }
                else
                {
                    btn_sit_down_->setVisible(false);
                }
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
                if (is_other_user_)
                {
                    btn_sit_down_->setVisible(true);
                }
                else
                {
                    btn_sit_down_->setVisible(false);
                }
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

void NodeOfcPlayerOwn::update_first_row_type(bool isRefreshAll)
{
    if (user_ != nullptr)
    {

        if (user_->get_first_row() != msg::OFCCardType::TYPE_NORMAL && user_->get_first_row() != msg::OFCCardType::NO_TYPE)
        {
            if (first_row_type_ != user_->get_first_row())
            {
                first_row_type_ = user_->get_first_row();
            }

            std::string type_name = user_->get_pattern_image_name(user_->get_first_row(), true);
//            image_first_row_patterns_->loadTexture(type_name);
            image_first_row_patterns_->setTexture(type_name);
            image_first_row_patterns_->setVisible(true);
        }
        else
        {
            image_first_row_patterns_->setVisible(false);
        }
    }
}
void NodeOfcPlayerOwn::update_middle_row_type(bool isRefreshAll)
{
    if (user_ != nullptr)
    {

        if (user_->get_middle_row() != msg::OFCCardType::TYPE_NORMAL && user_->get_middle_row() != msg::OFCCardType::NO_TYPE)
        {
            if (middle_row_type_ != user_->get_middle_row())
            {
                middle_row_type_ = user_->get_middle_row();
            }
            
            std::string type_name = user_->get_pattern_image_name(user_->get_middle_row(), true);
//            image_middle_row_patterns_->loadTexture(type_name);
            image_middle_row_patterns_->setTexture(type_name);
            image_middle_row_patterns_->setVisible(true);
        }
        else
        {
            image_middle_row_patterns_->setVisible(false);
        }
    }
}
void NodeOfcPlayerOwn::update_last_row_type(bool isRefreshAll)
{
    if (user_ != nullptr)
    {
        if (user_->get_last_row() != msg::OFCCardType::TYPE_NORMAL && user_->get_last_row() != msg::OFCCardType::NO_TYPE)
        {
            if (last_row_type_ != user_->get_last_row())
            {
                last_row_type_ = user_->get_last_row();
            }
            
            std::string type_name = user_->get_pattern_image_name(user_->get_last_row(), true);
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

void NodeOfcPlayerOwn::update_first_row_score()
{
    if (user_ != nullptr)
    {
        if (user_->get_first_row_score() > 0)
        {
            std::vector<std::string> image_name = user_->get_score_image_name(user_->get_first_row_score(), true);
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
void NodeOfcPlayerOwn::update_middle_row_score()
{
    if (user_ != nullptr)
    {
        if (user_->get_middle_row_score() > 0)
        {
            std::vector<std::string> image_name = user_->get_score_image_name(user_->get_middle_row_score(), true);
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
void NodeOfcPlayerOwn::update_last_row_score()
{
    if (user_ != nullptr)
    {
        if (user_->get_last_row_score() > 0)
        {
            std::vector<std::string> image_name = user_->get_score_image_name(user_->get_last_row_score(), true);
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

void NodeOfcPlayerOwn::update_fantasy_result()
{
    if (user_ != nullptr)
    {
        int32_t fantasynum = user_->get_fantasy();
        
        if (fantasynum == -1)
        {
            sprite_player_dust_->setVisible(true);
            image_dust_bg_->setVisible(true);
            //if you need add other language result
            //you can use "addSpriteFrameWithFile" add other language result picture
            std::string baopai_image_root=GameTools::get_languages_pic_key("baopai_2.png");
            if(baopai_image_root.compare("Nopic")==0)//字典里面没有找到对应的图片
            {
                sprite_player_dust_->setTexture("image/ofc_game_new_res/image_baopai_own.png");
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
                sprite_player_fantasy_->setTexture("image/ofc_game_new_res/image_refantasy_own_en.png");
            }else
            {
                sprite_player_fantasy_->setTexture("image/ofc_game_new_res/image_fantasy_own_en.png");
            }
        }
        
    }
}

void NodeOfcPlayerOwn::update_auto_buttons(const msg::OFCPlayerSetting& setting)
{
    playersetting_.CopyFrom(setting);

    if (setting.has_sitoutnexthand())
    {
        bool sitoutnexthand = setting.sitoutnexthand();
        if (sitoutnexthand)
        {
            btn_will_leave_->setTitleText(tools::local_string("continue","继续"));
        }else
        {
            btn_will_leave_->setTitleText(tools::local_string("leave","暂离"));
        }
    }
    
}

#pragma mark ClickEvent
void NodeOfcPlayerOwn::click_user_icon(Ref* sender)
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
//        user_info->setScale(0);
//        CCScaleTo* scaleTo = CCScaleTo::create(0.15, 1.05);
//        CCScaleTo* scaleTo2 = CCScaleTo::create(0.1, 1);
//        user_info->runAction(CCSequence::createWithTwoActions(scaleTo,scaleTo2));
//        
//        user_info->set_player_info(info, group_id);
//    }
}
void NodeOfcPlayerOwn::click_btn_confirm(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    uint32_t room_id = ofc_room_data->get_id();
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
    uint32_t room_id = ofc_room_data->get_id();
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
    uint32_t room_id = ofc_room_data->get_id();
    if (user_ != nullptr)
    {
        if (playersetting_.has_sitoutnexthand())
        {
            if (playersetting_.sitoutnexthand())
            {
                playersetting_.set_sitoutnexthand(false);
                btn_will_leave_->setTitleText(tools::local_string("leave","暂离"));
            }else
            {
                playersetting_.set_sitoutnexthand(true);
                btn_will_leave_->setTitleText(tools::local_string("continue","继续"));
            }
        }else
        {
            playersetting_.set_sitoutnexthand(true);
            btn_will_leave_->setTitleText(tools::local_string("continue","继续"));
        }
        
        PDM->send_ofc_set_change(playersetting_, room_id);
    }
}
void NodeOfcPlayerOwn::click_image_player_card_bg(Ref *sender)
{
    if (sitdown_callback_)
        sitdown_callback_();
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
        (image_hand_card_bg_1_->getBoundingBox().containsPoint(selSprite->getPosition()) || image_hand_card_bg_2_->getBoundingBox().containsPoint(selSprite->getPosition()) || image_hand_card_bg_3_->getBoundingBox().containsPoint(selSprite->getPosition())))
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
    selSprite->setScale(1);
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
            selSprite->setScale(1);
        }
        RotateBy * rotLeft = RotateBy::create(0.1, -14.0);
        RotateBy * rotCenter = RotateBy::create(0.1, 0.0);
        RotateBy * rotRight = RotateBy::create(0.1, 14.0);
        Sequence * rotSeq = Sequence::create(rotLeft, rotCenter, rotRight, rotCenter, NULL);
        newSprite->runAction(RepeatForever::create(rotSeq));
        
        selSprite = newSprite;
        selSprite->setScale(1.4);
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
        sprite_box_->setTexture("image/ofc_game_new_res/image_time_back_own.png");
    }else
    {
        sprite_box_->setTexture("image/ofc_game_new_res/image_time_own.png");
    }
//
//    sprite_box_->setVisible(true);
    current_operator_time_ = 0.0;
    text_time_->setString(tools::to_string(operator_time_) + "s");
    text_time_->setVisible(true);
    //text_player_status_->setVisible(false);
//    
//    image_player_card_bg_->loadTexture("image/ofc_game_res/image_player_bg_large_select.png");
//    
//    Sprite* sprite_time_bar_normal = Sprite::create("image/ofc_game_res/time_normal.png");
    progress_player_ = ProgressTimer::create(sprite_box_);
//    progress_player_->setScale(sprite_box_->getScaleX(), sprite_box_->getScaleY());
    progress_player_->setPosition(sprite_box_->getPosition());
    progress_player_->setType(ProgressTimer::Type::RADIAL);
    progress_player_->setMidpoint(Vec2(0.5, 0.5));
//    progress_player_->setBarChangeRate(Vec2(1, 0));
//    progress_player_->setZOrder(ZORDER_PLAYER_UI+1);
    progress_player_->setScaleX(-1);
//
    root_->addChild(progress_player_);
//
    root_->schedule(std::bind(&NodeOfcPlayerOwn::schedule_player_func, this, std::placeholders::_1), 1, "TAG_PROGRESS_TIME_PLAYER");
}

void NodeOfcPlayerOwn::time_player_close()
{
//    cocos2d::log("time_player_close");
    root_->unschedule("TAG_PROGRESS_TIME_PLAYER");
    
    if (progress_player_)
    {
        progress_player_->removeFromParent();
        progress_player_ = nullptr;
    }else{
        cocos2d::log("Error : NodeOfcPlayer Time_Close() progress_player = nullptr!");
    }
//
    if (sprite_box_->isVisible())
    {
        sprite_box_->setVisible(false);
    }else{
        cocos2d::log("Error : NodeOfcPlayer Time_Close() sprite_time_bar isVisible false!");
    }
//
    current_operator_time_ = 0.0;
    text_time_->setString("");
    text_time_->setVisible(false);
}

#pragma mark schedule
void NodeOfcPlayerOwn::update_type_and_score()
{
    if (image_first_row_patterns_->isVisible())
    {
        image_first_row_patterns_->setVisible(false);
    }
    if (image_player_score_first_0_->isVisible())
    {
        image_player_score_first_0_->setVisible(false);
        image_player_score_first_1_->setVisible(false);
        image_player_score_first_2_->setVisible(false);
        image_player_score_first_3_->setVisible(false);
    }
    
    if (image_middle_row_patterns_->isVisible())
    {
        image_middle_row_patterns_->setVisible(false);
    }
    if (image_player_score_middle_0_->isVisible())
    {
        image_player_score_middle_0_->setVisible(false);
        image_player_score_middle_1_->setVisible(false);
        image_player_score_middle_2_->setVisible(false);
        image_player_score_middle_3_->setVisible(false);
    }
    
    
    if (image_last_row_patterns_->isVisible())
    {
        image_last_row_patterns_->setVisible(false);
    }
    if (image_player_score_last_0_->isVisible())
    {
        image_player_score_last_0_->setVisible(false);
        image_player_score_last_1_->setVisible(false);
        image_player_score_last_2_->setVisible(false);
        image_player_score_last_3_->setVisible(false);
    }
}
void NodeOfcPlayerOwn::schedule_dust_visible(float dt)
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
    this->update_type_and_score();
    
    sprite_player_dust_->setVisible(true);
    image_dust_bg_->setVisible(true);
}

void NodeOfcPlayerOwn::schedule_fantasy_visible(float dt)
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
    this->update_type_and_score();
    sprite_player_fantasy_->setVisible(true);
}

void NodeOfcPlayerOwn::schedule_close_all_result(float dt)
{
    this->update_type_and_score();
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
    
    if (sprite_player_dust_->isVisible())
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
    for (auto iterPokerGroove : image_pokergrooves_) { iterPokerGroove->loadTexture("image/ofc_game_new_res/own_poker_bg.png"); }
}


void NodeOfcPlayerOwn::schedule_player_func(float dt)
{
//    cocos2d::log("schedule_player_func");
    if (progress_player_)
    {
//        cocos2d::log("schedule_player_func : all:%f current:%f", operator_time_, 100.0 - progress_player_->getPercentage());
        
        current_operator_time_ += 1.0;
        float ispercent = current_operator_time_ / operator_time_ * 100.0;
        
        progress_player_->setPercentage(100.0 - ispercent);
        
        float time = operator_time_ - current_operator_time_;
        if (time < 0.0)
            time = 0.0;
        else if (time > 0.0 && time < 1.0)
            time = 1.0;
        text_time_->setString(tools::to_string(time) + "s");
        
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