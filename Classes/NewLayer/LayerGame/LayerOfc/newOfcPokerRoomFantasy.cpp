//
//  newOfcPokerRoomFantasy.cpp
//  pokerclient
//
//  Created by ll on 15/11/5.
//
//

#include "newOfcPokerRoomFantasy.h"

#include "utils.h"
#include "GameTools.h"

#include "PokerDataManager.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "PlatformSdk.h"
#include "PokerCardSprite.h"

#include "Room.h"

#define ZORDER_IMAGE_PLAYR_BG 100
#define ZORDER_IMAGE_CARD_BG 200
#define ZORDER_PLAYER_UI 300
#define ZORDER_SPRITE_CARD 400
#define ZORDER_RESULT 500
#define ZORDER_SEL_SPRITE 1000

#define TAG_PROGRESS_ICON_CURRENT 299
#define TAG_PROGRESS_TIME_PLAYER 300
#define TAG_PROGRESS_TIME_OWN 301

bool newOfcPokerRoomFantasy::init()
{
    if (!OFCChildLayer::init())
    {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerOfcFantasyPortraitNew.csb");
    
    this->addChild(root_);
    
//    OfcBackGroundLayer* obgl = OfcBackGroundLayer::create(true);
    //obgl->setZOrder(1000);
//    this->addChild(obgl,1000, "FANTASY_BACKGROUND_LAYER_TAG");
    
//    image_player_card_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_player_card_bg"));
//    image_player_card_bg_->setZOrder(ZORDER_IMAGE_PLAYR_BG);
    
//    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon"));
//    image_icon_->setZOrder(ZORDER_PLAYER_UI);
    
//    text_player_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_nickname"));
//    text_player_nickname_->setString(tools::local_string("nickname", "昵称：") + "");
//    text_player_nickname_->setZOrder(ZORDER_PLAYER_UI);
    
//    text_player_score_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_score"));
//    text_player_score_->setString(tools::local_string("score", "得分：")+ "：");
//    text_player_score_->setZOrder(ZORDER_PLAYER_UI);
    
//    text_player_status_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_player_status"));
//    text_player_status_->setString("");
//    text_player_status_->setVisible(false);
//    text_player_status_->setZOrder(ZORDER_PLAYER_UI);
    
//    sprite_dealer_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_dealer"));
//    sprite_dealer_->setVisible(false);
//    sprite_dealer_->setZOrder(ZORDER_PLAYER_UI);
    
    sprite_time_bar_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_time_bar"));
    sprite_time_bar_->setVisible(false);
    sprite_time_bar_->setZOrder(ZORDER_PLAYER_UI);
    
    image_loading_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_loading_bg"));
    image_loading_bg_->setVisible(false);
    image_loading_bg_->setZOrder(ZORDER_PLAYER_UI);
    
    text_time_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_time"));
    text_time_->setZOrder(ZORDER_PLAYER_UI);
    text_time_->setString("");
    text_time_->setVisible(false);
    
//    btn_will_leave_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_will_leave"));
//    //btn_will_leave_->addClickEventListener(std::bind(&NodeOfcPlayerOwn::newOfcPokerRoomFantasy, this, std::placeholders::_1));
//    btn_will_leave_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
//    btn_will_leave_->setVisible(false);
    
//    btn_confirm_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_confirm"));
//    btn_confirm_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
//    btn_confirm_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_confirm, this, std::placeholders::_1));

    
    btn_hand_sort_1_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_poker_sort_1"));
    btn_hand_sort_2_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_poker_sort_2"));
    btn_hand_sort_3_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_poker_sort_3"));
    btn_hand_sort_1_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_hand_sort_1, this, std::placeholders::_1));
    btn_hand_sort_2_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_hand_sort_2, this, std::placeholders::_1));
    btn_hand_sort_3_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_hand_sort_3, this, std::placeholders::_1));
    btn_hand_sort_1_->setTitleText(tools::local_string("color","花色"));
    btn_hand_sort_2_->setTitleText(tools::local_string("order","大小"));
    btn_hand_sort_3_->setTitleText(tools::local_string("pair","对子"));
    btn_hand_sort_1_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_hand_sort_2_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_hand_sort_3_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    
    btn_confirm_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_confirm"));
    btn_confirm_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_confirm_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_confirm, this, std::placeholders::_1));
    GameTools::set_pic_change(btn_confirm_,"btn_ofc_confirm_normal.png","btn_ofc_confirm_pressed.png");
    
    btn_first_row_refresh_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_first_row_refresh"));
    btn_first_row_refresh_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_first_row_refresh_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_first_row_refresh, this, std::placeholders::_1));
//    btn_first_row_refresh_->setEnabled(false);
    
    btn_middle_row_refresh_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_middle_row_refresh"));
    btn_middle_row_refresh_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_middle_row_refresh_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_middle_row_refresh, this, std::placeholders::_1));
//    btn_middle_row_refresh_->setEnabled(false);
    
    btn_last_row_refresh_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_last_row_refresh"));
    btn_last_row_refresh_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_last_row_refresh_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_last_row_refresh, this, std::placeholders::_1));
//    btn_last_row_refresh_->setEnabled(false);
    
    btn_all_row_refresh_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_all_row_refresh"));
    btn_all_row_refresh_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_all_row_refresh_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_all_row_refresh, this, std::placeholders::_1));
    btn_all_row_refresh_->setTitleText(tools::local_string("reset_cards","手牌重置"));
    
    btn_put_first_row_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_put_first_row"));
    btn_put_first_row_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_put_first_row_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_put_first_row, this, std::placeholders::_1));
    btn_put_first_row_->setVisible(false);
    btn_put_middle_row_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_put_middle_row"));
    btn_put_middle_row_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_put_middle_row_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_put_middle_row, this, std::placeholders::_1));
    btn_put_middle_row_->setVisible(false);
    btn_put_last_row_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_put_last_row"));
    btn_put_last_row_->setZOrder(ZORDER_IMAGE_PLAYR_BG+3);
    btn_put_last_row_->addClickEventListener(std::bind(&newOfcPokerRoomFantasy::click_btn_put_last_row, this, std::placeholders::_1));
    btn_put_last_row_->setVisible(false);
    
    btn_put_first_row_->setTitleText(tools::local_string("head","头道"));
    btn_put_middle_row_->setTitleText(tools::local_string("middle","中道"));
    btn_put_last_row_->setTitleText(tools::local_string("botton","底道"));
    auto language = (PlatformSdk::LanguageType)GameTools::getLanguage();
    if(language == PlatformSdk::LanguageType::ENGLISH)
    {
        btn_put_first_row_->setTitleFontSize(18);
        btn_put_middle_row_->setTitleFontSize(18);
        btn_put_last_row_->setTitleFontSize(18);
    }
    btn_suit_pattern_sort_1_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_suit_pattern_sort_1"));
    btn_suit_pattern_sort_2_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_suit_pattern_sort_2"));
    btn_suit_pattern_sort_3_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_suit_pattern_sort_3"));
    btn_suit_pattern_sort_4_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_suit_pattern_sort_4"));
    btn_suit_pattern_sort_5_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_suit_pattern_sort_5"));
    btn_suit_pattern_sort_6_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_suit_pattern_sort_6"));
    btn_suit_pattern_sort_7_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_suit_pattern_sort_7"));
    btn_suit_pattern_sort_8_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_suit_pattern_sort_8"));
    btn_suit_pattern_sort_9_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_suit_pattern_sort_9"));
    
    btn_suit_pattern_sort_1_->setTitleText(tools::local_string("royal_and_flush","黄同"));
    btn_suit_pattern_sort_2_->setTitleText(tools::local_string("straight_flush_","同花顺"));
    btn_suit_pattern_sort_3_->setTitleText(tools::local_string("four_of_kind","四条"));
    btn_suit_pattern_sort_4_->setTitleText(tools::local_string("full_house","葫芦"));
    btn_suit_pattern_sort_5_->setTitleText(tools::local_string("flush","同花"));
    btn_suit_pattern_sort_6_->setTitleText(tools::local_string("straight","顺子"));
    btn_suit_pattern_sort_7_->setTitleText(tools::local_string("three_of_kind","三条"));
    btn_suit_pattern_sort_8_->setTitleText(tools::local_string("two_pairs","两对"));
    btn_suit_pattern_sort_9_->setTitleText(tools::local_string("one_pair","一对"));
    if(!GameTools::isChineseLanguage())
    {
        btn_suit_pattern_sort_1_->setTitleFontSize(26);
        btn_suit_pattern_sort_2_->setTitleFontSize(24);
        btn_suit_pattern_sort_3_->setTitleFontSize(26);
        btn_suit_pattern_sort_4_->setTitleFontSize(26);
        btn_suit_pattern_sort_5_->setTitleFontSize(26);
        btn_suit_pattern_sort_6_->setTitleFontSize(24);
        btn_suit_pattern_sort_7_->setTitleFontSize(26);
        btn_suit_pattern_sort_8_->setTitleFontSize(26);
        btn_suit_pattern_sort_9_->setTitleFontSize(26);
        btn_all_row_refresh_->setTitleFontSize(26);
    }
//    btn_suit_pattern_sort_1_->getTitleRenderer()->setVerticalAlignment(TextVAlignment::CENTER);
//    btn_suit_pattern_sort_1_->getTitleRenderer()->
//    btn_suit_pattern_sort_10_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_suit_pattern_sort_10"));
    
//    btn_suit_pattern_sort_1_->setVisible(false);
//    btn_suit_pattern_sort_2_->setVisible(false);
//    btn_suit_pattern_sort_3_->setVisible(false);
//    btn_suit_pattern_sort_4_->setVisible(false);
//    btn_suit_pattern_sort_5_->setVisible(false);
//    btn_suit_pattern_sort_6_->setVisible(false);
//    btn_suit_pattern_sort_7_->setVisible(false);
//    btn_suit_pattern_sort_8_->setVisible(false);
//    btn_suit_pattern_sort_9_->setVisible(false);
//    btn_suit_pattern_sort_10_->setVisible(false);
    
    suit_poker_result_.insert(std::make_pair(SUITPOKERTYPE::TYPE_ROYAL_STRAIGHT_FLUSH, std::make_pair(btn_suit_pattern_sort_1_, std::vector<std::vector<uint32_t>>())));
    suit_poker_result_.insert(std::make_pair(SUITPOKERTYPE::TYPE_STRAIGHT_FLUSH, std::make_pair(btn_suit_pattern_sort_2_, std::vector<std::vector<uint32_t>>())));
    suit_poker_result_.insert(std::make_pair(SUITPOKERTYPE::TYPE_FOUR_OF_A_KIND, std::make_pair(btn_suit_pattern_sort_3_, std::vector<std::vector<uint32_t>>())));
    suit_poker_result_.insert(std::make_pair(SUITPOKERTYPE::TYPE_FULL_HOUSE, std::make_pair(btn_suit_pattern_sort_4_, std::vector<std::vector<uint32_t>>())));
    suit_poker_result_.insert(std::make_pair(SUITPOKERTYPE::TYPE_FLUSH, std::make_pair(btn_suit_pattern_sort_5_, std::vector<std::vector<uint32_t>>())));
    suit_poker_result_.insert(std::make_pair(SUITPOKERTYPE::TYPE_STRAIGHT, std::make_pair(btn_suit_pattern_sort_6_, std::vector<std::vector<uint32_t>>())));
    suit_poker_result_.insert(std::make_pair(SUITPOKERTYPE::TYPE_THREE_OF_A_KIND, std::make_pair(btn_suit_pattern_sort_7_, std::vector<std::vector<uint32_t>>())));
    suit_poker_result_.insert(std::make_pair(SUITPOKERTYPE::TYPE_TWO_PAIRS, std::make_pair(btn_suit_pattern_sort_8_, std::vector<std::vector<uint32_t>>())));
    suit_poker_result_.insert(std::make_pair(SUITPOKERTYPE::TYPE_ONE_PAIR, std::make_pair(btn_suit_pattern_sort_9_, std::vector<std::vector<uint32_t>>())));
//    suit_poker_result_.insert(std::make_pair(SUITPOKERTYPE::TYPE_HIGH_CARDS, std::make_pair(btn_suit_pattern_sort_10_, std::vector<std::vector<uint32_t>>())));
    
    for (auto & it : suit_poker_result_)
    {
        it.second.first->addClickEventListener(std::function<void (Ref*)>([=](Ref * sender){this->click_btn_suit_poker_sort(it.first);}));
    }
    
    for (int i = 0; i < string_image_pokergrooves_name_.size(); i++)
    {
        auto pokergroove = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, string_image_pokergrooves_name_[i]));
        pokergroove->setZOrder(ZORDER_IMAGE_CARD_BG);
        image_pokergrooves_.push_back(pokergroove);
        int_pokerIDs_.push_back(100);
        sprite_handpokers_.push_back(nullptr);
    }
    
    for (int i = 0; i < string_image_holegrooves_name_.size(); i++)
    {
        auto pokergroove = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, string_image_holegrooves_name_[i]));
        pokergroove->setZOrder(ZORDER_IMAGE_CARD_BG);
        image_holegrooves_.push_back(pokergroove);
        sprite_holepokers_.push_back(nullptr);
        int_holepokerIDs_.push_back(100);
        pokergroove->setVisible(false);
        

    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(newOfcPokerRoomFantasy::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(newOfcPokerRoomFantasy::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(newOfcPokerRoomFantasy::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    handcard_sort_type_ = user_ofc::HAND_SORT_TYPE::NONE;
    
    register_events();
    
    return true;
}

#pragma mark ofc_backgroup_update
void newOfcPokerRoomFantasy::update_bcakground_record_status()
{
//    OfcBackGroundLayer* layer_background_ = dynamic_cast<OfcBackGroundLayer*>(this->getChildByName("FANTASY_BACKGROUND_LAYER_TAG"));
//    if (layer_background_ != nullptr)
//    {
//        layer_background_->update_record_status();
//    }
}
void newOfcPokerRoomFantasy::update_bcakground_game_status(msg::GameStatus game_status)
{
//    OfcBackGroundLayer* layer_background_ = dynamic_cast<OfcBackGroundLayer*>(this->getChildByName("FANTASY_BACKGROUND_LAYER_TAG"));
//    if (layer_background_ != nullptr)
//    {
//        layer_background_->update_game_status(game_status);
//    }

}

#pragma mark Update
void newOfcPokerRoomFantasy::set_user(std::shared_ptr<user_ofc> user)
{
    user_ = user;
    
    update_player_cards();
    update_hand_cards();
    
    click_btn_hand_sort_3(btn_hand_sort_3_);
}

void newOfcPokerRoomFantasy::update_hand_cards()
{
    if (user_ != nullptr)
    {
        hand_card_suit_sort();
        selSprite = nullptr;
        std::vector<uint32_t> cards = user_->get_hand_cards(handcard_sort_type_);
        for (int i = 0; i < sprite_holepokers_.size(); i++)
        {
            if (sprite_holepokers_[i] && sprite_holepokers_[i]->getParent())
            {
                sprite_holepokers_[i]->removeFromParent();
                sprite_holepokers_[i] = nullptr;
            }
            image_holegrooves_[i]->setVisible(false);
        }
        for (int i = 0; i < cards.size(); i++)
        {
            auto & holepokerID = int_holepokerIDs_[i];
            auto & holepoker = sprite_holepokers_[i];
            auto & holegroove = image_holegrooves_[i];
            
            holepokerID = cards[i];
            holepoker = PokerCardSprite::createCardSprite(int_holepokerIDs_[i]);
            holepoker->setZOrder(ZORDER_SPRITE_CARD);
            holepoker->setScale(holegroove->getContentSize().width / holepoker->getContentSize().width, holegroove->getContentSize().height/ holepoker->getContentSize().height);
            root_->addChild(holepoker);
            holepoker->setPosition(holegroove->getPosition());
            holegroove->setVisible(true);
            
            bool isfind = false;
            int32_t current_mark = 0;
            
            for (int markiter = 0; markiter < int_sel_mark_.size(); markiter++)
            {
                if (int_sel_mark_[markiter] == int_holepokerIDs_[i])
                {
                    isfind = true;
                    current_mark = markiter;
                    break;
                }
            }
            
            if (isfind)
            {
                Sprite* hole_card_mark = Sprite::create("image/ofc_game_res/image_hole_card_mark.png");
                holepoker->addChild(hole_card_mark);
                hole_card_mark->setScale(0.8, 0.8);
                hole_card_mark->setAnchorPoint(Vec2(1, 1));
                hole_card_mark->setPosition(Point(holepoker->getContentSize().width, holepoker->getContentSize().height));
            }
        }
    }
}

void newOfcPokerRoomFantasy::update_player_cards()
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
                sprite_handpokers_[i]->setScale(image_pokergrooves_[i]->getContentSize().width / sprite_handpokers_[i]->getContentSize().width, image_pokergrooves_[i]->getContentSize().height / sprite_handpokers_[i]->getContentSize().height);
                int_pokerIDs_[i] = allcards[i];
            }else
            {
                sprite_handpokers_[i] = nullptr;
            }
        }
    }
}

void newOfcPokerRoomFantasy::update_user_ui(cmd_data_pointer)
{
    if (user_ != nullptr)
    {
        text_player_nickname_->setString(tools::local_string("nickname", "昵称：") + user_->get_nickname());
        
        text_player_score_->setString(tools::local_string("score", "得分：") + tools::to_string(user_->get_all_score()));
        
        if (!user_->get_head_pic_name().empty())
        {
            if(GDM->is_file_exist(user_->get_head_pic_name()))
            {
            }else{
                FM.download_file(user_->get_head_pic_name());
                dispatch_.register_event(EVENT_DOWLOAD_FILE, BIND_FUNC(this, newOfcPokerRoomFantasy::update_user_ui));
            }
        }
    }
}

void newOfcPokerRoomFantasy::update_propupdate()
{
    if (user_ != nullptr)
    {
        if (user_->get_ofc_action_status() == msg::OFCActionStatus::FANTASY)
        {
            if (user_->get_is_ofc_hand_card_change())
            {
                update_hand_cards();
                update_player_cards();
            }
        }else
        {
            set_ProgressTimer(false);
            OfcBackGroundLayer* layer_background_ = dynamic_cast<OfcBackGroundLayer*>(this->getParent()->getChildByName("BACKGROUND_LAYER_TAG"));
            if (layer_background_ != nullptr)
            {
                layer_background_->update_menu_status(false);
            }
            
            this->removeFromParent();
        }
    }
}

void newOfcPokerRoomFantasy::update_ofcfantasy_player_choose_card_success()
{
    set_ProgressTimer(false);
    OfcBackGroundLayer* layer_background_ = dynamic_cast<OfcBackGroundLayer*>(this->getParent()->getChildByName("BACKGROUND_LAYER_TAG"));
    if (layer_background_ != nullptr)
    {
        layer_background_->update_menu_status(false);
    }
    this->removeFromParent();
}

void newOfcPokerRoomFantasy::update_put_row_btn()
{
    std::vector<uint32_t> allcards = user_->get_all_cards();
    uint32_t first_groove_num = 0;
    uint32_t middle_groove_num = 0;
    uint32_t last_groove_num = 0;
    
    for (int i = 0; i < allcards.size(); i++)
    {
        if (allcards[i] == 100)
        {
            if (i < 3)
                first_groove_num++;
            else if (i < 8)
                middle_groove_num++;
            else
                last_groove_num++;
        }
    }
    
    
    btn_put_first_row_->setVisible(int_sel_mark_.size() <= first_groove_num);
    
    btn_put_middle_row_->setVisible(int_sel_mark_.size() <= middle_groove_num);
    
    btn_put_last_row_->setVisible(int_sel_mark_.size() <= last_groove_num);
    
    if (int_sel_mark_.size() == 0)
    {
        btn_put_first_row_->setVisible(false);
        btn_put_middle_row_->setVisible(false);
        btn_put_last_row_->setVisible(false);
    }
    //cocos2d::log("队列大小 %u", int_sel_mark_.size());
}

#pragma mark ProgressTimer
void newOfcPokerRoomFantasy::set_ProgressTimer(bool isOn, float operator_time, bool is_using_timebank)
{
    if (isOn)
    {
        is_using_timebank_ = is_using_timebank;
        operator_time_ = operator_time;
        time_player_start();
    }else{
        time_player_close();
    }
}

void newOfcPokerRoomFantasy::time_player_start()
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
    
    Sprite* sprite_time_bar_normal = Sprite::create("image/ofc_game_res/time_normal.png");
    progress_player_ = ProgressTimer::create(sprite_time_bar_normal);
    
    progress_player_->setPosition(sprite_time_bar_->getPosition());
    progress_player_->setType(ProgressTimer::Type::BAR);
    progress_player_->setMidpoint(Vec2(1, 0.5));
    progress_player_->setBarChangeRate(Vec2(1, 0));
    progress_player_->setZOrder(ZORDER_PLAYER_UI+1);
    
    root_->addChild(progress_player_);
    
    root_->schedule(std::bind(&newOfcPokerRoomFantasy::schedule_player_func, this, std::placeholders::_1), 1, "TAG_PROGRESS_TIME_PLAYER");
}

void newOfcPokerRoomFantasy::time_player_close()
{
    cocos2d::log("time_player_close");
    root_->unschedule("TAG_PROGRESS_TIME_PLAYER");
    
    if (progress_player_)
    {
        progress_player_->removeFromParent();
        progress_player_ = nullptr;
    }else{
        cocos2d::log("Error : OfcPokerRoomFantasy Time_Close() progress_player = nullptr!");
    }
    
    if (sprite_time_bar_->isVisible())
    {
        sprite_time_bar_->setVisible(false);
    }else{
        cocos2d::log("Error : OfcPokerRoomFantasy Time_Close() sprite_time_bar isVisible false!");
    }
    
    current_operator_time_ = 0.0;
    text_time_->setString("");
    text_time_->setVisible(false);
}

#pragma mark touch
bool newOfcPokerRoomFantasy::onTouchBegan(Touch* touch, Event* event)
{
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    this->selectSpriteForTouch(touchLocation);
    return true;
}

void newOfcPokerRoomFantasy::onTouchMoved(Touch* touch, Event* evnet)
{
    if (NULL == selSprite)
        return;
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    
    Point oldTouchLocation = touch->getPreviousLocation();
    oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
    
    Point translation = touchLocation - oldTouchLocation;
    
    Point newPos = selSprite->getPosition() + translation;
    selSprite->setPosition(newPos);

}

void newOfcPokerRoomFantasy::onTouchEnded(Touch* touch, Event* event)
{
    if (NULL == selSprite)
    {
        return;
    }
    
    for (int i = 0; i < sprite_holepokers_.size(); i++)
    {
        auto & poker = sprite_holepokers_[i];
        auto & pokergroove = image_holegrooves_[i];
        auto & pokerID = int_holepokerIDs_[i];
        if (pokergroove->getBoundingBox().containsPoint(selSprite->getPosition()))
        {
            if (nullptr == poker)
            {
                user_->putcard(selcard, msg::OFCCardLocation::DISCARD);
            }else
            {
                user_->swap(selcard, pokerID);
                if (pokerID == selcard)
                {
                    bool isfind = false;
                    int32_t current_mark = 0;
                    
                    for (int markiter = 0; markiter < int_sel_mark_.size(); markiter++)
                    {
                        if (int_sel_mark_[markiter] == selcard)
                        {
                            isfind = true;
                            current_mark = markiter;
                            break;
                        }
                    }
                    if (isfind)
                    {
                        int_sel_mark_.erase(int_sel_mark_.begin() + current_mark);
                        //cocos2d::log("Ofc : 删除队列");
                    }else if (int_sel_mark_.size() < 5)
                    {
                        int_sel_mark_.push_back(pokerID);
                        //cocos2d::log("Ofc : 加入队列");
                    }

                }
            }

        }
    }
    
    for (int i = 0; i < sprite_handpokers_.size(); i++)
    {
        auto & poker = sprite_handpokers_[i];
        auto & pokergroove = image_pokergrooves_[i];
        auto & pokerid = int_pokerIDs_[i];
        if (pokergroove->getBoundingBox().containsPoint(selSprite->getPosition()))
        {
            if (nullptr == poker)
            {
                if (i < 3)
                    user_->putcard(selcard, msg::OFCCardLocation::FIRST);
                else if (i < 8)
                    user_->putcard(selcard, msg::OFCCardLocation::MIDDLE);
                else
                    user_->putcard(selcard, msg::OFCCardLocation::LAST);
            }else
            {
                user_->swap(selcard, pokerid);
            }
            bool isfind = false;
            int32_t current_mark = 0;
            
            for (int markiter = 0; markiter < int_sel_mark_.size(); markiter++)
            {
                if (int_sel_mark_[markiter] == selcard)
                {
                    isfind = true;
                    current_mark = markiter;
                    break;
                }
            }
            if (isfind)
            {
                int_sel_mark_.erase(int_sel_mark_.begin() + current_mark);
                //cocos2d::log("Ofc : 删除队列");
            }
        }
        
    }
    
    
    selSprite->stopAllActions();
    selSprite->runAction(RotateTo::create(0.1, 0));
    selSprite->setScale(1);
    selSprite = nullptr; //触摸结束以后，不再有被选中精灵
    update_player_cards();
    update_hand_cards();
    update_put_row_btn();
}

void newOfcPokerRoomFantasy::selectSpriteForTouch(Point touchLocation)
{
    Sprite* newSprite = nullptr;
    
    for (int i = 0; i < sprite_holepokers_.size(); i++)
    {
        auto & poker = sprite_holepokers_[i];
        auto & pokergroove = image_holegrooves_[i];
        auto & holepokerID = int_holepokerIDs_[i];
        if (nullptr != poker)
        {
            if (pokergroove->getBoundingBox().containsPoint(touchLocation))
            {
                newSprite = poker;
                selcard = holepokerID;
            }
        }
    }
    
    for (int i = 0; i < sprite_handpokers_.size(); i++)
    {
        auto &poker = sprite_handpokers_[i];
        auto & pokerID = int_pokerIDs_[i];
        if (nullptr != poker)
        {
            if (poker->getBoundingBox().containsPoint(touchLocation))
            {
                newSprite = poker;
                selcard = pokerID;
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


#pragma mark ClickEvent
void newOfcPokerRoomFantasy::click_btn_first_row_refresh(Ref* sender)
{
    current_suit_poker_type_ = SUITPOKERTYPE::TYPE_NORMAL;
    user_->reset_hand_cards_fantansy_first();
    update_player_cards();
    update_hand_cards();
}

void newOfcPokerRoomFantasy::click_btn_middle_row_refresh(Ref* sender)
{
    current_suit_poker_type_ = SUITPOKERTYPE::TYPE_NORMAL;
    user_->reset_hand_cards_fantansy_middle();
    update_player_cards();
    update_hand_cards();
}

void newOfcPokerRoomFantasy::click_btn_last_row_refresh(Ref* sender)
{
    current_suit_poker_type_ = SUITPOKERTYPE::TYPE_NORMAL;
    user_->reset_hand_cards_fantansy_last();
    update_player_cards();
    update_hand_cards();
}

void newOfcPokerRoomFantasy::click_btn_all_row_refresh(Ref* sender)
{
    current_suit_poker_type_ = SUITPOKERTYPE::TYPE_NORMAL;
    user_->reset_hand_cards_fantansy_first();
    user_->reset_hand_cards_fantansy_middle();
    user_->reset_hand_cards_fantansy_last();
    
    update_player_cards();
    update_hand_cards();
}

void newOfcPokerRoomFantasy::click_btn_hand_sort_1(Ref* sender)
{
    handcard_sort_type_ = user_ofc::HAND_SORT_TYPE::FUNCTION1;
    btn_hand_sort_1_->setEnabled(false);
    //btn_hand_sort_1_->setTitleColor(Color3B(141, 141, 141));
    btn_hand_sort_2_->setEnabled(true);
    //btn_hand_sort_2_->setTitleColor(Color3B(86, 62, 1));
    btn_hand_sort_3_->setEnabled(true);
    //btn_hand_sort_3_->setTitleColor(Color3B(86, 62, 1));
    update_hand_cards();
}
void newOfcPokerRoomFantasy::click_btn_hand_sort_2(Ref* sender)
{
    handcard_sort_type_ = user_ofc::HAND_SORT_TYPE::FUNCTION2;
    btn_hand_sort_1_->setEnabled(true);
    //btn_hand_sort_1_->setTitleColor(Color3B(86, 62, 1));
    btn_hand_sort_2_->setEnabled(false);
    //btn_hand_sort_2_->setTitleColor(Color3B(141, 141, 141));
    btn_hand_sort_3_->setEnabled(true);
    //btn_hand_sort_3_->setTitleColor(Color3B(86, 62, 1));
    update_hand_cards();
}
void newOfcPokerRoomFantasy::click_btn_hand_sort_3(Ref* sender)
{
    handcard_sort_type_ = user_ofc::HAND_SORT_TYPE::FUNCTION3;
    btn_hand_sort_1_->setEnabled(true);
    //btn_hand_sort_1_->setTitleColor(Color3B(86, 62, 1));
    btn_hand_sort_2_->setEnabled(true);
    //btn_hand_sort_2_->setTitleColor(Color3B(86, 62, 1));
    btn_hand_sort_3_->setEnabled(false);
   // btn_hand_sort_3_->setTitleColor(Color3B(141, 141, 141));
    update_hand_cards();
}

void newOfcPokerRoomFantasy::click_btn_confirm(Ref* sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    std::vector<msg::OFCCardLocation> result;
    std::vector<msg::OFCCardLocation> location = user_->get_hand_card_location();
    for (int i = 0; i < location.size(); i++)
    {
        result.push_back(location[i]);
    }
    PDM->send_player_choose_cards(result, ofc_room_data->get_id());
}

void newOfcPokerRoomFantasy::click_btn_put_first_row(Ref* sender)
{
    current_suit_poker_type_ = SUITPOKERTYPE::TYPE_NORMAL;
    for (int i = 0; i < int_sel_mark_.size(); i++)
    {
        user_->putcard(int_sel_mark_[i], msg::OFCCardLocation::FIRST);
    }
    int_sel_mark_.clear();
    update_player_cards();
    update_hand_cards();
    
    btn_put_first_row_->setVisible(false);
    btn_put_middle_row_->setVisible(false);
    btn_put_last_row_->setVisible(false);
}

void newOfcPokerRoomFantasy::click_btn_put_middle_row(Ref* sender)
{
    current_suit_poker_type_ = SUITPOKERTYPE::TYPE_NORMAL;
    for (int i = 0; i < int_sel_mark_.size(); i++)
    {
        user_->putcard(int_sel_mark_[i], msg::OFCCardLocation::MIDDLE);
    }
    int_sel_mark_.clear();
    update_player_cards();
    update_hand_cards();
    
    btn_put_first_row_->setVisible(false);
    btn_put_middle_row_->setVisible(false);
    btn_put_last_row_->setVisible(false);
}

void newOfcPokerRoomFantasy::click_btn_put_last_row(Ref* sender)
{
    current_suit_poker_type_ = SUITPOKERTYPE::TYPE_NORMAL;
    for (int i = 0; i < int_sel_mark_.size(); i++)
    {
        user_->putcard(int_sel_mark_[i], msg::OFCCardLocation::LAST);
    }
    int_sel_mark_.clear();
    update_player_cards();
    update_hand_cards();
    
    btn_put_first_row_->setVisible(false);
    btn_put_middle_row_->setVisible(false);
    btn_put_last_row_->setVisible(false);
}

#pragma mark SuitSort
void newOfcPokerRoomFantasy::hand_card_suit_sort()
{
    std::vector<std::vector<uint32_t>> royal_straight_flush;
    std::vector<std::vector<uint32_t>> straight_flush;
    std::vector<std::vector<uint32_t>> four_of_a_kind;
    std::vector<std::vector<uint32_t>> full_house;
    std::vector<std::vector<uint32_t>> flush;
    std::vector<std::vector<uint32_t>> straight;
    std::vector<std::vector<uint32_t>> three_of_a_kind;
    std::vector<std::vector<uint32_t>> two_pairs;
    std::vector<std::vector<uint32_t>> one_pair;
//    std::vector<std::vector<uint32_t>> high_cards;
    
    std::vector<uint32_t> cards = user_->get_hand_cards();
    
    auto cards_sort_func = [](uint32_t card1, uint32_t card2){
        return (card1- 1) % 13 > (card2 - 1) % 13;
    };
    std::sort(cards.begin(), cards.end(), cards_sort_func);
    
    //one_pair
    auto pair_sort_func = [](std::vector<uint32_t> vec_one_pair_1, std::vector<uint32_t> vec_one_pair_2){
        return (vec_one_pair_1[0]- 1) % 13 > (vec_one_pair_2[0] - 1) % 13;
    };
    if (cards.size() > 1)
    {
        for (int i = 0; i < cards.size()-1; i++)
        {
            uint32_t card1 = cards[i];
            for (int j = i+1; j < cards.size(); j++)
            {
                uint32_t card2 = cards[j];
                if (card1 % 13 == card2 % 13)
                {
                    std::vector<uint32_t> vec_one_pair;
                    vec_one_pair.push_back(card1);
                    vec_one_pair.push_back(card2);
                    one_pair.push_back(vec_one_pair);
                }
            }
        }
        std::sort(one_pair.begin(), one_pair.end(), pair_sort_func);
    }
    
    //two_pairs
    auto two_pair_has_same_poker_func = [](std::vector<uint32_t> vec_one_pair_1, std::vector<uint32_t> vec_one_pair_2){
        for (int i = 0; i < vec_one_pair_1.size(); i++)
        {
            for (int j = 0; j < vec_one_pair_2.size(); j++) {
                if (((vec_one_pair_1[i]-1) % 13) == ((vec_one_pair_2[j]-1) % 13))
                    return true;
            }
        }
        return false;
    };
    auto to_vec_two_pair_func = [](std::vector<uint32_t> vec_one_pair_1, std::vector<uint32_t> vec_one_pair_2){
        std::vector<uint32_t> result;
        for (int i = 0; i < vec_one_pair_1.size(); i++)
        {
            result.push_back(vec_one_pair_1[i]);
        }
        for (int i = 0; i < vec_one_pair_2.size(); i++)
        {
            result.push_back(vec_one_pair_2[i]);
        }
        return result;
    };
    if (one_pair.size() > 1)
    {
        for (int i = 0; i < one_pair.size() - 1; i++)
        {
            for (int j = i+1; j < one_pair.size(); j++)
            {
                if (!two_pair_has_same_poker_func(one_pair[i], one_pair[j]))
                    two_pairs.push_back(to_vec_two_pair_func(one_pair[i], one_pair[j]));
            }
        }
    }
    
    //three_of_a_kind
    if (cards.size() > 2)
    {
        for (int i = 0; i < cards.size()-2; i++)
        {
            for (int j = i+1; j < cards.size()-1; j++)
            {
                for (int k = j+1; k < cards.size(); k++)
                {
                    if ((cards[i] % 13 == cards[j] % 13) && (cards[j] % 13 == cards[k] % 13) && (cards[i] % 13 == cards[k] % 13))
                    {
                        std::vector<uint32_t> vec_three_of_a_kind;
                        vec_three_of_a_kind.push_back(cards[i]);
                        vec_three_of_a_kind.push_back(cards[j]);
                        vec_three_of_a_kind.push_back(cards[k]);
                        three_of_a_kind.push_back(vec_three_of_a_kind);
                    }
                }
            }
        }
    }
    std::sort(three_of_a_kind.begin(), three_of_a_kind.end(), pair_sort_func);
    
    //four_of_a_kind
    if (cards.size() >= 4)
    {
        for (int i = 0; i < cards.size()-3; i++)
            for (int j = i+1; j < cards.size()-2; j++)
                for (int k = j+1; k < cards.size()-1; k++)
                    for (int l = k+1; l < cards.size(); l++)
                    {
                        std::vector<uint32_t> vec_four_of_a_kind;
                        vec_four_of_a_kind.push_back(cards[i]);
                        vec_four_of_a_kind.push_back(cards[j]);
                        vec_four_of_a_kind.push_back(cards[k]);
                        vec_four_of_a_kind.push_back(cards[l]);
                        bool is_same_poker = true;
                        for (auto & iter1 : vec_four_of_a_kind)
                        {
                            for (auto & iter2 : vec_four_of_a_kind)
                            {
                                if (iter1 != iter2)
                                {
                                    if (iter1 % 13 != iter2 % 13)
                                    {
                                        is_same_poker = false;
                                        break;
                                    }
                                }
                            }
                            if (!is_same_poker)
                                break;
                        }
                        if (is_same_poker)
                            four_of_a_kind.push_back(vec_four_of_a_kind);
                    }
    }
    std::sort(four_of_a_kind.begin(), four_of_a_kind.end(), pair_sort_func);
    
    //full_house
    if (three_of_a_kind.size() > 0 && one_pair.size() > 3)
    {
        for (int i = 0; i < three_of_a_kind.size(); i++)
        {
            for (int j = 0; j < one_pair.size(); j++)
            {
                if (!two_pair_has_same_poker_func(three_of_a_kind[i], one_pair[j]))
                    full_house.push_back(to_vec_two_pair_func(three_of_a_kind[i], one_pair[j]));
            }
        }
    }
    

    //flush
    auto get_card_suit_func = [](uint32_t current_card_num){
        return (current_card_num - 1) / 13;
    };
    if (cards.size() >= 5)
    {
        for (int i = 0; i < cards.size()-4; i++)
            for (int j = i+1; j < cards.size()-3; j++)
                for (int k = j+1; k < cards.size()-2; k++)
                    for (int l = k+1; l < cards.size()-1; l++)
                        for (int m = l+1; m < cards.size(); m++)
                        {
                            std::vector<uint32_t> vec_flush;
                            vec_flush.push_back(cards[i]);
                            vec_flush.push_back(cards[j]);
                            vec_flush.push_back(cards[k]);
                            vec_flush.push_back(cards[l]);
                            vec_flush.push_back(cards[m]);
                            bool is_same_poker = true;
                            for (auto & iter1 : vec_flush)
                            {
                                for (auto & iter2 : vec_flush)
                                {
                                    if (iter1 != iter2)
                                    {
                                        if (get_card_suit_func(iter1) != get_card_suit_func(iter2))
                                        {
                                            is_same_poker = false;
                                            break;
                                        }
                                    }
                                }
                                if (!is_same_poker)
                                    break;
                            }
                            if (is_same_poker)
                            {
                                std::sort(vec_flush.begin(), vec_flush.end(), cards_sort_func);
                                flush.push_back(vec_flush);
                            }
                        }
    }
    auto flush_sort_func = [](std::vector<uint32_t> vec_one_pair_1, std::vector<uint32_t> vec_one_pair_2){
//        std::sort(vec_one_pair_1.begin(), vec_one_pair_1.end(), pair_sort_func);
//        std::sort(vec_one_pair_2.begin(), vec_one_pair_2.end(), pair_sort_func);
        
        int count = 0;
        uint32_t pair1_big_card;
        uint32_t pair2_big_card;
        while (count < 5)
        {
            pair1_big_card = vec_one_pair_1[count];
            pair2_big_card = vec_one_pair_2[count];
            
            if ((pair1_big_card - 1) % 13 == (pair2_big_card - 1) % 13)
            {
                count++;
            }else{
                break;
            }
        }
        
        return (pair1_big_card - 1) % 13 > (pair2_big_card - 1) % 13;
    };
    std::sort(flush.begin(), flush.end(), flush_sort_func);

    
    //straight
    for (int i = 0; i < cards.size(); i++)
    {
        uint32_t card1 = cards[i];
        std::vector<uint32_t> result;
        result.push_back(card1);
        std::function<void (std::vector<uint32_t>, std::vector<uint32_t>)> straight_find_sort = [&](std::vector<uint32_t> vec_result_cards, std::vector<uint32_t> allcard){
            for (auto iter_card : allcard)
            {
                uint32_t last_card = vec_result_cards[vec_result_cards.size() - 1];
                std::vector<uint32_t> tmp_result_cards = vec_result_cards;
                if ( (((last_card-1) % 13) + 1  == ((iter_card-1) % 13) ) ||
                    ((last_card % 13 == 0) && (vec_result_cards[0] == last_card) && ((last_card) % 13)+1 == ((iter_card) % 13)))
                {
                    tmp_result_cards.push_back(iter_card);
                    if (vec_result_cards.size() == 4)
                    {
                        straight.push_back(tmp_result_cards);
                    }
                    else
                    {
                        straight_find_sort(tmp_result_cards, allcard);
                    }
                }
            }
        };
        straight_find_sort(result, cards);
    }
    auto straight_sort_func = [](std::vector<uint32_t> vec_one_pair_1, std::vector<uint32_t> vec_one_pair_2){
        return (vec_one_pair_1[0]) % 13 > (vec_one_pair_2[0]) % 13;
    };
    std::sort(straight.begin(), straight.end(), straight_sort_func);

    //straight_flush
    auto is_same_vec = [](std::vector<uint32_t> vec_cards1, std::vector<uint32_t> vec_cards2){
        for (auto card1 : vec_cards1)
        {
            bool is_same = false;
            for (auto card2 : vec_cards2)
            {
                if (card1 == card2)
                    is_same = true;
            }
            if (!is_same)
                return false;
        }
        return true;
    };
    for (auto iter_straight = straight.begin(); iter_straight != straight.end();)
    {
        bool is_same = false;
        for (auto iter_flush = flush.begin(); iter_flush != flush.end(); )
        {
            if (is_same_vec(*iter_straight,*iter_flush))
            {
                is_same = true;
                iter_flush = flush.erase(iter_flush);
                break;
            }else
                iter_flush++;
        }
        if (is_same)
        {
            straight_flush.push_back(*iter_straight);
            iter_straight = straight.erase(iter_straight);
        }else
            iter_straight++;
    }
    
    std::sort(straight_flush.begin(), straight_flush.end(), straight_sort_func);

    
    //royal_straight_flush
//    for (auto & iter_straight_flush : straight_flush)
//    {
//        uint32_t big_card = iter_straight_flush[0];
//        uint32_t small_card = iter_straight_flush[0];
//        for (auto & iter_card : iter_straight_flush)
//        {
//            if ((iter_card - 1) % 13 > (big_card - 1) % 13)
//                big_card = iter_card;
//            if ((iter_card - 1) % 13 < (big_card - 1) % 13)
//                small_card = iter_card;
//        }
//        if ((big_card - 1) % 13 == 12 && (small_card - 1) % 13 == 8)
//        {
//            royal_straight_flush.push_back(iter_straight_flush);
//        }
//    }
    
    for (auto iter_straight_flush = straight_flush.begin(); iter_straight_flush != straight_flush.end();)
    {
        uint32_t big_card = (*iter_straight_flush)[0];
        uint32_t small_card = (*iter_straight_flush)[0];
        for (auto iter_card : *iter_straight_flush)
        {
            if ((iter_card - 1) % 13 > (big_card - 1) % 13)
                big_card = iter_card;
            if ((iter_card - 1) % 13 < (big_card - 1) % 13)
                small_card = iter_card;
        }
        if ((big_card - 1) % 13 == 12 && (small_card - 1) % 13 == 8)
        {
            
            royal_straight_flush.push_back(*iter_straight_flush);
            iter_straight_flush = straight_flush.erase(iter_straight_flush);
        }else
        {
            iter_straight_flush++;
        }
    }
    
//    suit_poker_result_[TYPE_HIGH_CARDS].second = high_cards;
    suit_poker_result_[TYPE_ONE_PAIR].second = one_pair;
    suit_poker_result_[TYPE_TWO_PAIRS].second = two_pairs;
    suit_poker_result_[TYPE_THREE_OF_A_KIND].second = three_of_a_kind;
    suit_poker_result_[TYPE_STRAIGHT].second = straight;
    suit_poker_result_[TYPE_FLUSH].second = flush;
    suit_poker_result_[TYPE_FULL_HOUSE].second = full_house;
    suit_poker_result_[TYPE_FOUR_OF_A_KIND].second = four_of_a_kind;
    suit_poker_result_[TYPE_STRAIGHT_FLUSH].second = straight_flush;
    suit_poker_result_[TYPE_ROYAL_STRAIGHT_FLUSH] .second = royal_straight_flush;
    
    for (auto & it : suit_poker_result_)
    {
        if ( it.first == SUITPOKERTYPE::TYPE_ROYAL_STRAIGHT_FLUSH)
        {
            it.second.first->loadTextures("image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_normal.png", "image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_pressed.png");
        }
        else if (it.first == SUITPOKERTYPE::TYPE_ONE_PAIR)
        {
            it.second.first->loadTextures("image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_pair_normal.png", "image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_pair_pressed.png");
        }
        else
        {
            it.second.first->loadTextures("image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_other_normal.png", "image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_other_pressed.png");
        }
        if (it.second.second.size() == 0)
        {
            it.second.first->setEnabled(false);
            it.second.first->setTitleColor(Color3B(174, 167, 167));
        }else
        {
            it.second.first->setEnabled(true);
            it.second.first->setTitleColor(Color3B(255, 255, 255));
        }
    }
    if ( current_suit_poker_type_ != SUITPOKERTYPE::TYPE_NORMAL && suit_poker_result_[current_suit_poker_type_].first->isEnabled())
    {
        if (current_suit_poker_type_ == SUITPOKERTYPE::TYPE_ROYAL_STRAIGHT_FLUSH)
        {
            suit_poker_result_[current_suit_poker_type_].first->loadTextures("image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_pressed.png", "image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_normal.png");
        }
        else if (current_suit_poker_type_ == SUITPOKERTYPE::TYPE_ONE_PAIR)
        {
            suit_poker_result_[current_suit_poker_type_].first->loadTextures("image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_pair_pressed.png", "image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_pair_normal.png");
        }
        else
        {
            suit_poker_result_[current_suit_poker_type_].first->loadTextures("image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_other_pressed.png", "image/ofc_game_new_res/ofc_fantasy_res/btn_suit_pattern_sort_other_normal.png");
        }
    }
    
}


void newOfcPokerRoomFantasy::click_btn_suit_poker_sort(SUITPOKERTYPE  click_suit_sort_type)
{
    if (current_suit_poker_type_ != click_suit_sort_type)
    {
        current_suit_poker_type_ = click_suit_sort_type;
        current_suit_poker_rank_ = 0;
        //hand_card_suit_sort();
        std::vector<std::vector<uint32_t>> current_type_suit_result = suit_poker_result_[current_suit_poker_type_].second;
        
        if (current_type_suit_result.size() > 0)
        {
            int_sel_mark_.clear();
            std::vector<uint32_t> current_suit_pokers = current_type_suit_result[current_suit_poker_rank_];
            for (int i = 0; i < current_suit_pokers.size(); i++)
            {
                int_sel_mark_.push_back(current_suit_pokers[i]);
            }
        }
    }else
    {
        int_sel_mark_.clear();
        std::vector<std::vector<uint32_t>> current_type_suit_result = suit_poker_result_[current_suit_poker_type_].second;
        if (current_type_suit_result.size() > 0) {
            if (current_suit_poker_rank_ < current_type_suit_result.size()-1)
            {
                ++current_suit_poker_rank_;
            }else
            {
                current_suit_poker_rank_ = 0;
            }
            std::vector<uint32_t> current_suit_pokers = current_type_suit_result[current_suit_poker_rank_];
            for (int i = 0; i < current_suit_pokers.size(); i++)
            {
                int_sel_mark_.push_back(current_suit_pokers[i]);
            }
        }
    }
    update_hand_cards();
    update_put_row_btn();
}

#pragma mark RegisterEvents
void newOfcPokerRoomFantasy::register_events()
{
    
}

void newOfcPokerRoomFantasy::schedule_player_func(float dt)
{
//    cocos2d::log("Fantasy schedule_player_func");
    if (progress_player_)
    {
//        cocos2d::log("Fantasy schedule_player_func : all:%f current:%f", operator_time_, progress_player_->getPercentage());
        
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
        }
    }else{
        cocos2d::log("Error : Fantasy schedule_player_func progress_player is null!");
    }
}