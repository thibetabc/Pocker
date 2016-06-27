#include "PokerRoomLayer.h"

#include "BuyChipLayer.h"
#include "TournamentBuyChipLayer.h"
#include "MatchScroeNode.h"
#include "utils.h"
#include "LayerChatOld.h"
#include "PokerDataManager.h"
#include "Room.h"
#include "PlayerInfoNode.h"
#include "RecordLayer.h"
#include "cmd_def.h"
#include "GameDataManager.h"
#include "UserSettingPropertyCell.h"
#include "ChipPoolNode.h"
#include "ChipNode.h"
#include "NodeGameInPause.h"
#include "AudioManager.h"
#include "ProcessorManager.h"
#include "FileManager.h"
#include "VoiceRecorderInterface.h"
#include "TipView.h"
#include "AudioManager.h"
#include "RecordAnimationLayer.h"
#include "GameChatLayer.h"
#include "SceneManager.h"
#include "RecordHudLayer.h"
#include "SpriteAction.h"
#include "PokerCardSprite.h"
#include "TouchablePoker.h"

#include "msg.pb.h"
#include "net_data_recv.h"

#include <math.h>
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
#define ZORDER_PLAYER_INFO 120
#define ZORDER_CARD 200
#define PUBLIC_CARD_TAG 300
#define RECORDLAYERTAG 1
#define ANIMATION_LAYER_TAG 2
#define GAME_CHAT_LAYER_TAG 3
#define RECORDHUDLAYERTAG 40

#define TAG_BUY_CHIP 10002

#define ZORDER_PAUSE_NODE 15000
#define ZORDER_START_BTN 15100

#define START_BTN_TAG 15100
#define PAUSE_NODE_TAG 15000

bool PokerRoomLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    

    //添加按钮
    player_seat_chip_card_positions_.push_back({Point(375, 195), Point(365, 315), Point(340, 48)});
    
    player_seat_chip_card_positions_.push_back({Point(80, 340), Point(185, 405), Point(120, 348)});
    player_seat_chip_card_positions_.push_back({Point(80, 580), Point(185, 645), Point(120, 588)});
    player_seat_chip_card_positions_.push_back({Point(80, 820), Point(185, 885), Point(120, 828)});
    player_seat_chip_card_positions_.push_back({Point(80, 1060), Point(185, 1125), Point(120, 1068)});
    
    player_seat_chip_card_positions_.push_back({Point(375, 1180), Point(365, 1060), Point(415, 1188)});
    
    player_seat_chip_card_positions_.push_back({Point(670, 1060), Point(560, 1125), Point(610, 1068)});
    player_seat_chip_card_positions_.push_back({Point(670, 820), Point(560, 885), Point(610, 828)});
    player_seat_chip_card_positions_.push_back({Point(670, 580), Point(560, 645), Point(610, 588)});
    player_seat_chip_card_positions_.push_back({Point(670, 340), Point(560, 405), Point(610, 348)});
    
    std::vector<uint32_t> vec2;
    vec2.push_back(0);
    vec2.push_back(5);
    
    std::vector<uint32_t> vec3;
    vec3.push_back(0);
    vec3.push_back(4);
    vec3.push_back(6);
    
    std::vector<uint32_t> vec4;
    vec4.push_back(0);
    vec4.push_back(3);
    vec4.push_back(5);
    vec4.push_back(7);
    
    std::vector<uint32_t> vec5;
    vec5.push_back(0);
    vec5.push_back(2);
    vec5.push_back(4);
    vec5.push_back(6);
    vec5.push_back(8);
    
    std::vector<uint32_t> vec6;
    vec6.push_back(0);
    vec6.push_back(2);
    vec6.push_back(4);
    vec6.push_back(5);
    vec6.push_back(6);
    vec6.push_back(8);
    
    std::vector<uint32_t> vec7;
    vec7.push_back(0);
    vec7.push_back(2);
    vec7.push_back(3);
    vec7.push_back(4);
    vec7.push_back(6);
    vec7.push_back(7);
    vec7.push_back(8);
    
    
    std::vector<uint32_t> vec8;
    vec8.push_back(0);
    vec8.push_back(2);
    vec8.push_back(3);
    vec8.push_back(4);
    vec8.push_back(5);
    vec8.push_back(6);
    vec8.push_back(7);
    vec8.push_back(8);
    
    std::vector<uint32_t> vec9;
    vec9.push_back(0);
    vec9.push_back(1);
    vec9.push_back(2);
    vec9.push_back(3);
    vec9.push_back(4);
    
    vec9.push_back(6);
    vec9.push_back(7);
    vec9.push_back(8);
    vec9.push_back(9);
    
    std::vector<uint32_t> vec10;
    vec10.push_back(0);
    vec10.push_back(1);
    vec10.push_back(2);
    vec10.push_back(3);
    vec10.push_back(4);
    vec10.push_back(5);
    vec10.push_back(6);
    vec10.push_back(7);
    vec10.push_back(8);
    vec10.push_back(9);
    
    seat_idx_with_person_amount_[2] = vec2;
    seat_idx_with_person_amount_[3] = vec3;
    seat_idx_with_person_amount_[4] = vec4;
    seat_idx_with_person_amount_[5] = vec5;
    seat_idx_with_person_amount_[6] = vec6;
    seat_idx_with_person_amount_[7] = vec7;
    seat_idx_with_person_amount_[8] = vec8;
    seat_idx_with_person_amount_[9] = vec9;
    seat_idx_with_person_amount_[10] = vec10;
    //    Sprite *bg = Sprite::create("login_res/game_res/table_bg_bg.png");
    //    Size winSize = Director::getInstance()->getWinSize();
    //    bg->setPosition(winSize.width/2,winSize.height/2);
    //    bg->setScale(1136.0/DESIGN_HEIGHT, 640.0/DESIGN_WIDTH);
    //    this->addChild(bg);
    root_ = CSLoader::createNode("LayerPokerRoom.csb");
    this->addChild(root_);
    //清除actionnode
    //    viewExitSprite = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "Button_clear"));
    //    viewExitSprite->addClickEventListener(std::bind(&PokerRoomLayer::clear_view, this, std::placeholders::_1));
    
    btn_action_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_action"));
    
    btn_action_->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_action, this, std::placeholders::_1));
    
    btn_record_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_record"));
    btn_record_->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_record, this, std::placeholders::_1));
    
    btn_match_score_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_match_score"));
    btn_match_score_->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_match_score, this, std::placeholders::_1));
    
    btn_buy_chip_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_buy_chip"));
    btn_buy_chip_->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_buy_chip, this, std::placeholders::_1));
    btn_tourment_info_ = Button::create();
    btn_tourment_info_->loadTextureNormal("image/tournament_info_portrait_res/btn_cominfo_normal.png");
    btn_tourment_info_->loadTexturePressed("image/tournament_info_portrait_res/btn_cominfo_pressed.png");
    btn_tourment_info_->loadTextureDisabled("image/tournament_info_portrait_res/btn_cominfo_pressed.png");
    btn_tourment_info_->setTitleText("");
    root_->addChild(btn_tourment_info_);
    btn_tourment_info_->setPosition(btn_buy_chip_->getPosition());
    btn_tourment_info_->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_tourment_info, this, std::placeholders::_1));
    
    bool is_tournament = GetRoom()->get_is_tournament();
    if (is_tournament)
    {
        btn_tourment_info_->setVisible(true);
    }
    else{
        btn_tourment_info_->setVisible(false);
    }
    
    
    btn_voice_ =  dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_voice"));
    btn_voice_->addTouchEventListener(std::bind(&PokerRoomLayer::click_btn_voice, this, std::placeholders::_1, std::placeholders::_2));
    
    btn_pause_ =  dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_pause"));
    btn_pause_->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_pause, this, std::placeholders::_1));
    btn_pause_->setVisible(false);
    
    btn_chat_ =  dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_chat"));
    btn_chat_->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_chat, this, std::placeholders::_1));
    
    btn_share_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_share"));
    btn_share_->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_share, this, std::placeholders::_1));
    
    btn_start_ = Button::create();
    btn_start_->loadTextureNormal("image/ofc_game_res/btn_ofc_start_normal.png");
    btn_start_->loadTexturePressed("image/ofc_game_res/btn_ofc_start_pressed.png");
    btn_start_->loadTextureDisabled("image/ofc_game_res/btn_ofc_start_pressed.png");
    cocos2d::Size size_btn_start = btn_start_->getContentSize();
    
    btn_start_->setPosition(cocos2d::Point(
                                           DESIGN_WIDTH / 2  - size_btn_start.width * (0.5 - btn_start_->getAnchorPoint().x),
                                           DESIGN_HEIGHT / 2 - size_btn_start.height * (0.5 - btn_start_->getAnchorPoint().y)));
    
    btn_start_->setPositionY(btn_start_->getPositionY() + size_btn_start.height);
    
    this->addChild(btn_start_, 0, "START_BTN_TAG");
    btn_start_->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_start, this, std::placeholders::_1));
    btn_start_->setVisible(false);
    
    //btn_action_->setPositionX(btn_chat_->getPositionX());
    //标示有新聊天信息的红色圆点
    image_red_ =  dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_red"));
    text_unread_message_count_ =  dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_unread_message_count"));
    
    if (!GetRoom()->get_is_record()) {
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
        
    }
    //底池
    sprite_chip_icon_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_chip_icon"));
    sprite_pool_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_pool"));
    text_pool_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_pool"));
    text_pool_amount_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_pool_amount"));
    nodes_of_chip_.push_back(sprite_chip_icon_);
    nodes_of_chip_.push_back(sprite_pool_);
    nodes_of_chip_.push_back(text_pool_);
    nodes_of_chip_.push_back(text_pool_amount_);
    text_pool_->setString(tools::local_string("pool", "底池"));
    if (!GetRoom()->get_is_record()) {
        uint32_t amount = GetRoom()->get_chip_pool();
        if(amount == 0)
            set_node_of_chip_visible(false);
    }
    

    
    text_table_note_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_table_note"));
    text_table_note_->setString(tools::local_string("texas_comment", "德州扑克注释备注"));
    text_table_note_->setFontSize(26);
    text_table_note_->setColor(Color3B(223,194,147));
    text_table_note_->setAnchorPoint(Vec2(0.5, 0.5));
    text_table_note_->setPosition(Point(375, 920));
    text_table_note_->setZOrder(ZORDER_UI);
    image_table_note_bg_ = ImageView::create( "login_res/game_res/dichi_05.png");
    
    root_->addChild(image_table_note_bg_, sprite_pool_->getZOrder());
    image_table_note_bg_->setAnchorPoint(Vec2(1, 0.5));
    image_table_note_bg_->setPosition(text_table_note_->getPosition());
    image_table_note_bg_->setVisible(false);
    
    image_sit_back_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_sit_back"));
    image_sit_back_->setVisible(false);
    image_sit_back_->addClickEventListener([=](Ref*){
        auto iter = GetRoom()->get_user(GDM->get_user_id());
        if(iter != NULL) {
            PDM->send_seat_info(GetRoom()->get_room_id(), msg::SittingStatus::SITDOWN, iter->get_seat_num());
        }
        else {
            assert(0);
        }
    });
    //新加按钮
    btn_twice = Button::create("res/login_res/game_res/chip_button_big.png","res/login_res/game_res/chip_button_big_press.png","res/login_res/game_res/chip_button_big_press.png");
    btn_twice->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_twice,this,std::placeholders::_1));
    btn_twice_2 = Button::create("res/login_res/game_res/chip_button_big.png","res/login_res/game_res/chip_button_big_press.png","res/login_res/game_res/chip_button_big_press.png");
    btn_twice_2->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_twice_2,this,std::placeholders::_1));
    btn_twice->setTitleColor(Color3B(26, 26, 26));
    btn_twice_2->setTitleColor(Color3B(26, 26, 26));
    btn_twice->setPosition(Point(375, 260));
    root_->addChild(btn_twice,255);
    btn_twice_2->setPosition(Vec2(375,120));
    root_->addChild(btn_twice_2,255);
    btn_twice->setVisible(false);
    btn_twice_2->setVisible(false);
    //按钮s
    btn_giveup_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_giveup"));
    auto btn_watch = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_watch"));
    auto btn_chip = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_chip"));
    auto btn_follow = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_follow"));
    
    text_btn_follow_amount_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_btn_follow_amount"));
    
    auto btn_add = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_add"));
    auto btn_all = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_all"));
    btn_all->setLocalZOrder(PUBLIC_CARD_TAG + 1);
    
    btn_giveup_->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_giveup, this, std::placeholders::_1));
    btn_watch->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_watch, this, std::placeholders::_1));
    btn_chip->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_chip, this, std::placeholders::_1));
    btn_follow->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_follow, this, std::placeholders::_1));
    btn_add->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_add, this, std::placeholders::_1));
    btn_all->addClickEventListener(std::bind(&PokerRoomLayer::click_btn_all, this, std::placeholders::_1));
    
    player_action_buttons_.insert(std::make_pair("fold", btn_giveup_));
    player_action_buttons_.insert(std::make_pair("check", btn_watch));
    player_action_buttons_.insert(std::make_pair("bet", btn_chip));
    player_action_buttons_.insert(std::make_pair("call", btn_follow));
    player_action_buttons_.insert(std::make_pair("raise", btn_add));
    player_action_buttons_.insert(std::make_pair("allin", btn_all));
    
    check_box_autocall = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root_, "check_box_autocall"));
    check_box_autocall_text = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "check_box_autocall_text"));
    
    check_box_autocall_amount = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "check_box_autocall_amount"));
    
    
    check_box_autocall->setTag(0);
    //    check_box_autocall->setSelected(property->boolvalue());
    //    check_box_autocall_text->setString(property->displayname());
    check_box_autocall->addEventListener(std::bind(&PokerRoomLayer::click_checkbox, this, std::placeholders::_1, std::placeholders::_2));
    check_box_autocall_text->addClickEventListener([=](Ref*){
        check_box_autocall->setSelected(!check_box_autocall->isSelected());
        CheckBox::EventType    event = check_box_autocall->isSelected() ? CheckBox::EventType::UNSELECTED : CheckBox::EventType::SELECTED;
        this->click_checkbox(check_box_autocall, event);
        
    });
    
    check_box_callorfold = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root_, "check_box_callorfold"));
    auto check_box_callorfold_text = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "check_box_callorfold_text"));
    check_box_callorfold->setTag(1);
    //    check_box_callorfold->setSelected(property->boolvalue());
    //    check_box_callorfold_text->setString(property->displayname());
    check_box_callorfold->addEventListener(std::bind(&PokerRoomLayer::click_checkbox, this, std::placeholders::_1, std::placeholders::_2));
    check_box_callorfold_text->addClickEventListener([=](Ref*){
        check_box_callorfold->setSelected(!check_box_callorfold->isSelected());
        CheckBox::EventType    event = check_box_callorfold->isSelected() ? CheckBox::EventType::UNSELECTED : CheckBox::EventType::SELECTED;
        this->click_checkbox(check_box_callorfold, event);
        
    });
    
    check_box_straddle = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root_, "check_box_straddle"));
    auto check_box_straddle_text = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "check_box_straddle_text"));
    check_box_straddle->setTag(2);
    //    check_box_straddle->setSelected(property->boolvalue());
    //    check_box_straddle_text->setString(property->displayname());
    check_box_straddle->addEventListener(std::bind(&PokerRoomLayer::click_checkbox, this, std::placeholders::_1, std::placeholders::_2));
    check_box_straddle_text->addClickEventListener([=](Ref*){
        check_box_straddle->setSelected(!check_box_straddle->isSelected());
        CheckBox::EventType    event = check_box_straddle->isSelected() ? CheckBox::EventType::UNSELECTED : CheckBox::EventType::SELECTED;
        this->click_checkbox(check_box_straddle, event);
        
    });
    
    check_box_straddle_text_number = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "check_box_straddle_text_number"));
    
    check_box_straddle_text_number->setString(tools::to_string(GetRoom()->get_big_blind()*2));
    
    check_box_sitout = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root_, "check_box_sitout"));
    auto check_box_sitout_text = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "check_box_sitout_text"));
    check_box_sitout->setTag(3);
    //    check_box_straddle->setSelected(property->boolvalue());
    //    check_box_sitout_text->setString(property->displayname());
    check_box_sitout->addEventListener(std::bind(&PokerRoomLayer::click_checkbox, this, std::placeholders::_1, std::placeholders::_2));
    check_box_sitout_text->addClickEventListener([=](Ref*){
        check_box_sitout->setSelected(!check_box_sitout->isSelected());
        CheckBox::EventType    event = check_box_sitout->isSelected() ? CheckBox::EventType::UNSELECTED : CheckBox::EventType::SELECTED;
        this->click_checkbox(check_box_sitout, event);
        
    });
    
    
    init_chip_pool();
    
    
    
    
    hidden_action_btns();
    
    register_events();
    if (!GetRoom()->get_is_record()) {
        init_seats();
        PositionUpdate();
    }
    else if (info_nodes_.size() == 0)
    {
        init_seats();
    }
    
    // AM->playBackgroundMusic("sound/bgm_dezhou.mp3",true);//取消背景音乐
    isExit = false;
    
    buy_chip_type_ = BUY_CHIP_TYPE::NONE;
    _last_chip_num = 0;
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(PokerRoomLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    

    return true;
}

void PokerRoomLayer::set_btn_choice_visible(msg::ChoiceType choice_type_)
{
    switch(choice_type_)
    {
            choicetype = choice_type_;
        case msg::ChoiceType::CloseChoice:
            btn_twice->setVisible(false);
            btn_twice_2->setVisible(false);
            break;
        case msg::ChoiceType::MuckCard:
            btn_twice->setVisible(true);
            btn_twice->setTitleText("盖牌");
            btn_twice->setTitleFontSize(30);
            
            break;
        case msg::ChoiceType::AskRunTwice:
            btn_twice->setVisible(true);
            btn_twice->setTitleText("发两次牌");
            btn_twice->setTitleFontSize(24);
            
            break;
        case msg::ChoiceType::ReplyRunTwice:
            btn_twice->setVisible(true);
            btn_twice->setTitleText("同意发两次牌");
            btn_twice_2->setVisible(true);
            btn_twice_2->setTitleText("不同意发两次牌");
            btn_twice->setTitleFontSize(18);
            btn_twice_2->setTitleFontSize(16);
            break;
    }
}

const std::shared_ptr<PokerRoom> PokerRoomLayer::GetRoom()
{
    if (isRecord)
        return dynamic_pointer_cast<PokerRoom>(PDM->get_replay_room(RoomID));
    else
        return dynamic_pointer_cast<PokerRoom>(PDM->get_room(RoomID));
}

void PokerRoomLayer::clear_view(Ref* sender)
{
    
}

void PokerRoomLayer::clear_view_new()
{
    Actionnode = (PlayerActionNode *)this->getChildByTag(TAG_ACTION_NODE);
    if(Actionnode != NULL)
    {
        Actionnode->setVisible(false);
        btn_action_->setVisible(true);
    }
}

void PokerRoomLayer::click_btn_action(Ref* sender)
{
    
    btn_action_->setVisible(false);
    Actionnode = (PlayerActionNode *)this->getChildByTag(TAG_ACTION_NODE);
    if (Actionnode)
    {
        Actionnode->setVisible(true);
        Actionnode->set_is_tournament(GetRoom()->get_is_tournament());
    }
    else
    {
        Actionnode = PlayerActionNode::create();
        Actionnode->set_is_tournament(GetRoom()->get_is_tournament());
        Actionnode->set_dismiss_callback([=]() {
            btn_action_->setVisible(true);
        });
        this->addChild(Actionnode, ZORDER_UI,TAG_ACTION_NODE);
    }
    
    
}

void PokerRoomLayer::click_btn_match_score(Ref* sender)
{
    PDM->send_get_score(this->GetRoom()->get_room_id());
}

void PokerRoomLayer::SetActionNodeVisible(bool b)
{
    if(this->Actionnode)
    {
        this->Actionnode->setVisible(b);
    }
}

void PokerRoomLayer::click_btn_record(Ref* sender)
{
    auto prl = RecordLayer::create();
    
    prl->set_isFriHandCard(false);
    prl->update_data();
    this->addChild(prl, 0, RECORDLAYERTAG);
}

// 分享按钮
void PokerRoomLayer::click_btn_share(Ref* sender)
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
    sdk->openShare ("我正在房间里玩德州扑克，你不来吗？", "icon108.png", nullptr);
#else
    sdk->openShare ("我正在房间里玩德州扑克，你不来吗？", "assets/Icon108.png", nullptr);
#endif

}

void PokerRoomLayer::click_btn_buy_chip(Ref* sender)
{
    //    IN_GAMEING = 0,
    //    SITOUT, //暂离
    //    STANDUP,    //站起
    //    QUIT_ROOM,  //离开房间
    //    WAITING_ENTER
    
    if (buy_chip_type_ == BUY_CHIP_TYPE::NONE)
    {
        auto processor = PM->GetUserPermission_up(GetRoom()->get_group_id(), GDM->get_user_id());
        net_manager::shared_instence().send_data(processor, REQ_GET_PERMISSION);
        return;
    }
    else
    {
        buy_chip_type_ = BUY_CHIP_TYPE::NONE;
        
    }
    
    auto user = static_pointer_cast<user_texas>(GetRoom()->get_user(GDM->get_user_id()));
    
    if(user) {
        int min_chip;
        uint32_t buyin_max = GetRoom()->get_buyin_max();
        uint32_t buyin_min = GetRoom()->get_buyin_min() - user->get_chip_amount();
        min_chip = buyin_max - user->get_chip_amount();
        if(min_chip <= 0) {
            TipView::showAlertView(tools::local_string("max_chip", "您的计分牌已经达到极限，不能添加"));
            return;
        }
        
        if(user->get_seat_num() != 0)
        {
            if(user->get_status() == msg::SittingStatus::PLAYING ||
               user->get_status() == msg::SittingStatus::SITOUT ||
               user->get_status() == msg::SittingStatus::SITDOWN)
            {
                auto old_buy_chip_layer = root_->getChildByTag(TAG_BUY_CHIP);
                if(old_buy_chip_layer)
                {
                    old_buy_chip_layer->removeFromParent();
                }
                BuyChipLayer* bcl = BuyChipLayer::create();
                this->addChild(bcl, ZORDER_UI,TAG_BUY_CHIP);
                
            }
        }
        else {
            TipView::showAlertView(tools::local_string("cannot_add_chip", "不能添加计分牌！"));
        }
    }
    else {
        TipView::showAlertView(tools::local_string("cannot_add_chip", "不能添加计分牌！"));
    }
}

void PokerRoomLayer::click_btn_twice(Ref *sender)
{
    PDM->send_poker_choice(GetRoom()->get_room_id(), choicetype, true);
    btn_twice->setVisible(false);
    btn_twice->setVisible(false);
}
void PokerRoomLayer::click_btn_twice_2(Ref *sender)
{
    PDM->send_poker_choice(GetRoom()->get_room_id(), choicetype, false);
    btn_twice_2->setVisible(false);
    btn_twice_2->setVisible(false);
}

void PokerRoomLayer::click_btn_giveup(Ref* sender)
{
    clear_view(sender);
    hidden_action_btns();
    node_chip_->setVisible(false);
    PDM->send_poker_player_game_action(GetRoom()->get_room_id(), msg::ActionStatus::FOLD, 0);
    
    auto user = GetRoom()->get_user( GDM->get_user_id());
    auto point = get_position_with_seat_num(user->get_seat_num());
    
    Size size = Director::getInstance()->getVisibleSize();
    SpriteAction::runCardAction(this, Vec2(point.card.x,point.card.y),Vec2(size.width*0.5f,size.height*0.55f),[ = ](){
    });
    
    SpriteAction::runCardAction(this, Vec2(point.card.x + 72,point.card.y),Vec2(size.width*0.5f,size.height*0.55f),[ = ](){
    });
    
}

void PokerRoomLayer::click_btn_watch(Ref* sender)
{
    clear_view(sender);
    hidden_action_btns();
    PDM->send_poker_player_game_action(GetRoom()->get_room_id(), msg::ActionStatus::CHECK, 0);
}

void PokerRoomLayer::click_btn_chip(Ref* sender)
{
    clear_view(sender);
    if(!node_chip_->isVisible()) {
        node_chip_->setVisible(true);
    }
    else {
        node_chip_->setVisible(false);
        hidden_action_btns();
        PDM->send_poker_player_game_action(GetRoom()->get_room_id(), msg::ActionStatus::BET, node_chip_->get_amount());
    }
}

void PokerRoomLayer::click_btn_follow(Ref* sender)
{
    clear_view(sender);
    hidden_action_btns();
    node_chip_->setVisible(false);
    PDM->send_poker_player_game_action(GetRoom()->get_room_id(), msg::ActionStatus::CALL, 0);
}

void PokerRoomLayer::click_btn_add(Ref* sender)
{
    clear_view(sender);
    if(!node_chip_->isVisible()) {
        node_chip_->setVisible(true);
    }
    else {
        node_chip_->setVisible(false);
        hidden_action_btns();
        PDM->send_poker_player_game_action(GetRoom()->get_room_id(), msg::ActionStatus::RAISE, node_chip_->get_amount());
    }
}
void PokerRoomLayer::click_btn_all(Ref* sender)
{
    clear_view(sender);
    auto user = GetRoom()->get_user(GDM->get_user_id());
    
    uint32_t amount = user->get_chip_amount();
    PDM->send_poker_player_game_action(GetRoom()->get_room_id(), msg::ActionStatus::ALLIN, amount);
    hidden_action_btns();
}


void PokerRoomLayer::click_btn_voice(Ref* sender, Widget::TouchEventType event)
{
    clear_view(sender);
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
            
            uint32_t record_time = FM.get_recorder()->stopRecord();
            if(record_time != 0) {
                PDM->send_game_voice(record_filename_, record_time, GetRoom()->get_room_id());
                
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
            btn_record_->setTitleText(tools::local_string("press_voice", "按住  说话"));
            
            AM->end_recording();
            break;
    }
}

void PokerRoomLayer::click_btn_start(Ref * sender)
{
    auto &poker_room_data = this->GetRoom();
    if (poker_room_data == nullptr)
    {
        cocos2d::log("PokerRoomLayer::click_btn_start poker_room_data == nullptr");
        return;
    }
    uint32_t roomid = poker_room_data->get_room_id();
    uint32_t ownerid = poker_room_data->get_owner_id();
    if (ownerid == GDM->get_user_id())
    {
        PDM->send_set_game_status(roomid, msg::GameStatus::RUNNING);
    }else
    {
        cocos2d::log("PokerRoomLayer::click_btn_start not is owner but click button!");
    }
}

void PokerRoomLayer::click_btn_pause(Ref* sender)
{
    auto &poker_room_data = this->GetRoom();
    if (poker_room_data == nullptr)
    {
        cocos2d::log("PokerRoomLayer::click_btn_pause poker_room_data == nullptr");
        return;
    }
    uint32_t roomid = poker_room_data->get_room_id();
    uint32_t ownerid = poker_room_data->get_owner_id();
    bool is_tournament = poker_room_data->get_is_tournament();
    if (ownerid == GDM->get_user_id() && !is_tournament)
    {
        PDM->send_set_game_status(roomid, msg::GameStatus::PAUSED);
    }else
    {
        cocos2d::log("PokerRoomLayer::click_btn_pause not is owner but click button!");
    }

}

void PokerRoomLayer::click_btn_chat(Ref* sender)
{
    clear_view(sender);
    GDM->clear_unreal_message_by_group_id(GetRoom()->get_group_id());
    
    handle_receive_chat(nullptr);
    
    GameChatLayer* gcl = GameChatLayer::create();
    gcl->set_not_touch();
    this->addChild(gcl, 0, GAME_CHAT_LAYER_TAG);
}

void PokerRoomLayer::click_btn_tourment_info(Ref* sender)
{
    uint32_t roomID = GetRoom()->get_room_id();
    PDM->send_poker_get_tourmentstatus(roomID);
    
    
}

void PokerRoomLayer::click_checkbox(Ref* sender, CheckBox::EventType event)
{
    auto cb = dynamic_cast<CheckBox*>(sender);
    
    if(cb) {
        switch (cb->getTag()) {
            case 0:
            {
                if(properties.callorfold()&&cb->isSelected())
                {
                    properties.set_callorfold(false);
                    check_box_callorfold->setSelected(false);
                }
                properties.set_autocall(cb->isSelected());
            }
                break;
            case 1:
            {
                if(properties.autocall()&&cb->isSelected())
                {
                    properties.set_autocall(false);
                    check_box_autocall->setSelected(false);
                }
                properties.set_callorfold(cb->isSelected());
            }
                break;
            case 2:
            {
                properties.set_straddle(cb->isSelected());
            }
                break;
            case 3:
            {
                properties.set_sitoutnexthand(cb->isSelected());
            }
                break;
                
            default:
                break;
        }
        
        PDM->send_send_set_change(properties,RoomID);
    }
    
}

void PokerRoomLayer::update_show_card()
{
    std::vector<bool> showCards;
    
    auto user = GetRoom()->get_user(GDM->get_user_id());
    if (user) {
        for (int i=0; i<user->get_properties()->holecards_size(); i++) {
            auto iter_user_card = user_cards_.find(user->get_id());
            if(iter_user_card != user_cards_.end()) {
                auto& this_user_card = iter_user_card->second;
                auto iter_card = this_user_card.find(i);
                if(iter_card != this_user_card.end())
                {
                    TouchablePoker * poker = dynamic_cast<TouchablePoker *>(iter_card->second);
                    if (poker) {
                        showCards.push_back(poker->getIsSelected());
                    }
                    
                }
            }
        }
        
        PDM->send_show_card(showCards, RoomID);
    }
    
}
#pragma mark handle events
void PokerRoomLayer::PositionUpdate()
{
    auto users = GetRoom()->get_valid_users();
    
    for (auto& iter : info_nodes_) {
        auto& sc = get_position_with_seat_num(iter.first);
        
        iter.second->setPosition(sc.seat);
        
        switch (iter.first) {

            case 2:
            case 10:
            {
                iter.second->setLocalZOrder(ZORDER_PLAYER_INFO + 4);
            }
                break;
            case 3:
            case 9:
            {
                iter.second->setLocalZOrder(ZORDER_PLAYER_INFO + 3);
            }
                break;
            case 4:
            case 8:
            {
                iter.second->setLocalZOrder(ZORDER_PLAYER_INFO + 2);
            }
                break;
            case 5:
            case 7:
            {
                iter.second->setLocalZOrder(ZORDER_PLAYER_INFO + 1);
            }
                break;
                
            default:
                break;
        }

        bool is_exist = false;
        auto info = info_nodes_.at(iter.first);
        
        for (auto& it : users)
        {
            if(it.second->get_seat_num() == iter.first && it.second->get_valid() &&
               (it.second->get_status() != msg::SittingStatus::OBSERVER && it.second->get_status() != msg::SittingStatus::LEAVE))
            {
                auto user = static_pointer_cast<user_texas>( it.second);
                info->update_data(user);
                is_exist = true;
                break;
            }
        }
        
        info->setVisible1(true);
        
        if(!is_exist) {
            auto it = users.find(GDM->get_user_id());
            if(it != users.end() && it->second->get_valid() && (it->second->get_status() != msg::SittingStatus::OBSERVER && it->second->get_status() != msg::SittingStatus::LEAVE)) {
                info->setVisible1(false);
            }
            else if(GetRoom()->get_is_tournament())
            {
                info->setVisible1(false);
            }
            info->update_data();
        }
    }
    uint32_t userid = GDM->get_user_id();
    auto iter_this_user = users.find(userid);
    if(iter_this_user != users.end()) {
        if(iter_this_user->second->get_status() == msg::SittingStatus::SITOUT) {
            hidden_action_btns();
            if(!GetRoom()->get_is_record() && !GetRoom()->get_is_tournament())
            {
                image_sit_back_->setVisible(true);
            }
            else
            {
                image_sit_back_->setVisible(false);
            }
            
        }
        else if (iter_this_user->second->get_status() == msg::SittingStatus::OBSERVER){
            auto iter = user_cards_.find(userid);
            if(iter != user_cards_.end()) {
                for (auto& it : iter->second) {
                    it.second->removeFromParent();
                    all_cards_.erase(it.first);
                }
                user_cards_.erase(iter);
            }
            
            auto info_node = info_nodes_.find(iter_this_user->second->get_seat_num());
            if(info_node != info_nodes_.end()) {
                info_node->second->recreate();
                info_node->second->update_data();
            }
            
            if(userid == GDM->get_user_id()) {
                node_chip_->setVisible(false);
                check_box_straddle->setVisible(false);
                check_box_sitout->setVisible(false);
                hidden_action_btns();
            }
        }
        else {
            image_sit_back_->setVisible(false);
        }
    }
    else {
        image_sit_back_->setVisible(false);
    }
    
    for (auto iter = chip_nodes_.begin(); iter != chip_nodes_.end(); ) {
        if(users.find(iter->first) == users.end()) {
            iter->second->removeFromParent();
            iter = chip_nodes_.erase(iter);
        }
        else {
            iter++;
        }
    }
    
    
    
    for (auto iter = chip_nodes_flags.begin(); iter != chip_nodes_flags.end(); ) {
        if(users.find(iter->first) == users.end()) {
            iter->second = 0;
            iter = chip_nodes_flags.erase(iter);
        }
        else {
            iter++;
        }
    }//yxd
    
    
    
    
    std::vector<uint32_t> clear_ids;
    for (auto& iter : user_cards_) {
        if(users.find(iter.first) == users.end()) {
            clear_ids.push_back(iter.first);
        }
    }
    for (auto& iter : clear_ids) {
        auto it = user_cards_.find(iter);
        if(it != user_cards_.end()) {
            for (auto& card : it->second) {
                card.second->removeFromParent();
            }
            user_cards_.erase(iter);
        }
    }
    
    handle_update_property();
}

void PokerRoomLayer::update_auto_buttons()
{
    auto user = GetRoom()->get_user(GDM->get_user_id());
    bool buser = !isRecord && user && user->get_valid();
    bool bstraddle = buser && !GetRoom()->get_is_tournament() && user->get_status() != msg::SittingStatus::OBSERVER;
    bool bsitout = buser && !GetRoom()->get_is_tournament() && user->get_status() == msg::SittingStatus::PLAYING;
    check_box_straddle->setVisible(bstraddle);
    check_box_sitout->setVisible(bsitout);
    bool bAutoButton = buser && user->get_status() == msg::SittingStatus::PLAYING && user->get_properties()->actionstatus() != msg::ActionStatus::ALLIN && user->get_properties()->actionstatus() != msg::ActionStatus::FOLD && !bCurrentUserAction && bPlaying;
    set_listview_visible(bAutoButton);
}

void PokerRoomLayer::handle_update_property()
{
    update_auto_buttons();
    
    set_node_of_chip_visible(true);
    std::map<uint32_t, std::shared_ptr<user_base>> users;
    
    users = GetRoom()->get_valid_users();
    if (GetRoom()->get_is_record())
    {
        SetActionNodeVisible(false);
        
    }
    
    for (auto& iter : users) {
        
        auto user = static_pointer_cast<user_texas>(iter.second);
        if(user->get_id() == 0) {
            continue;
        }
        update_user_property(user);
    }
    
    
    //yxd动画
    if (GetRoom()->get_chip_pool() == 0) {
        _poolNum = 0;
    }
    if (GetRoom()->get_chip_pool() != 0 && GetRoom()->get_chip_pool()>_poolNum) {
        
        for ( auto iter = chip_nodes_.begin(); iter != chip_nodes_.end(); iter++) {
            SpriteAction::runChipAction(this, iter->second ->getPosition(), text_pool_amount_->getPosition(), _last_chip_nodes_num, [ = ](){
                
            });
        }
        
        _poolNum = GetRoom()-> get_chip_pool();
        _last_chip_nodes_num = 0;
        
    }
    //yxd动画
    
    text_pool_amount_->setString(tools::to_string(GetRoom()->get_chip_pool()));
    
    text_table_note_->setString(GetRoom()->get_note());
    image_table_note_bg_->setContentSize(cocos2d::Size(text_table_note_->getContentSize().width, image_table_note_bg_->getContentSize().height));

    for (auto& node : info_nodes_) {
        if(node.first == GetRoom()->get_dealer_position()) {
            node.second->set_dealer(true);
        }
        else {
            node.second->set_dealer(false);
        }
    }
    
    
//    for(auto& iter : public_cards1_) {
//        iter->removeFromParent();
//    }
//    public_cards1_.clear();
//    
//    for(auto& iter : public_cards2_) {
//        iter->removeFromParent();
//    }
//    public_cards2_.clear();
    
    size_t publicCardSize1 = public_cards1_.size();
    int size = GetRoom()->get_properties().holecards_size();
    if (size > publicCardSize1) {
        for(int i = 0; i < size;i++)
        {
            if (i >= publicCardSize1) {
                int index = GetRoom()->get_properties().holecards(i);
                Sprite* card = PokerCardSprite::createCardSprite(index);
                all_cards_[index] = card;
                card->setTag(index);
                public_cards1_.push_back(card);
            }
        }
    }
    else if (size < publicCardSize1)
    {
        while (publicCardSize1 > size) {
            Sprite * card = public_cards1_.back();
            if (card) {
                card->removeFromParent();
            }
            public_cards1_.pop_back();
            publicCardSize1 = public_cards1_.size();
        }
    }

    
    
    size_t publicCardSize2 = public_cards2_.size();
    int size2 = GetRoom()->get_properties().secondholecards_size();
    
    if (size2 > publicCardSize2) {
        for(int i = 0; i < size2;i++)
        {
            if (i >= publicCardSize2) {
                int index = GetRoom()->get_properties().secondholecards(i);
                Sprite* card = PokerCardSprite::createCardSprite(index);
                all_cards_[index] = card;
                card->setTag(index);
                public_cards2_.push_back(card);
            }
        }
    }
    else if (size2 < publicCardSize2)
    {
        while (publicCardSize2 > size2) {
            Sprite * card = public_cards2_.back();
            if (card) {
                card->removeFromParent();
            }
            public_cards2_.pop_back();
            publicCardSize2 = public_cards2_.size();
        }
    }
    

    
//    for (int i : GetRoom()->get_properties().holecards()) {
//        
//        //const std::string image_name = PDM->get_card_image(i);
//        Sprite* card = PokerCardSprite::createCardSprite(i);
//        all_cards_[i] = card;
//        card->setTag(i);
//        public_cards1_.push_back(card);
//    }
//    
//    
//    for (int i : GetRoom()->get_properties().secondholecards()) {
//        const std::string image_name = PDM->get_card_image(i);
//        Sprite* card = PokerCardSprite::createCardSprite(i);
//        public_cards2_.push_back(card);
//        all_cards_[i] = card;
//        
//    }
    float pos_x = 230;
    float pos_y1 = 720;
    float pos_y2 = 580;
    float scale = 1.0f;
    if(public_cards2_.empty()) {
        pos_y1 = 720;
        scale = 1.0f;
    }
    else {
        pos_y1 = 720;
    }
    
    int i = 0;
    
    for (auto& iter : public_cards1_) {
        i++;

        Sprite * card = (Sprite *)root_->getChildByName("public_cards1_"+tools::to_string(i));
        if(card == nullptr)
        {
            root_->addChild(iter, PUBLIC_CARD_TAG,"public_cards1_"+tools::to_string(i));

            if (scale > 1.3)
            {
                pos_x = pos_x / (scale-0.12);
            }
            iter->setPosition(pos_x + (i-1) * 70 * scale, pos_y1);
            iter->setScale(scale);
            
            if(show_cards_num1_ <= i)
            {
                
                PokerCardSprite::setCardSprite(0, iter);
                
                iter->runAction(Sequence::create(
                                                 OrbitCamera::create(0.2f,1, 0, 0, 90, 0, 0)
                                                 ,
                                                 CallFunc::create([this,iter](){
                    PokerCardSprite::setCardSprite(iter->getTag(), iter);
                    
                }),
                                                 OrbitCamera::create(0.2f,1, 0, -90, 90, 0, 0),

                                                 
                                                 NULL));
                show_cards_num1_++;
            }
            
            has_show_cards1_.push_back(iter);
        }
        
        
        
        
    }
    
    int j = 0;
    for (auto& iter : public_cards2_)
    {
        j++;
        
        Sprite * card = (Sprite *)root_->getChildByName("public_cards2_"+tools::to_string(j));
        if(card == nullptr)
        {
            root_->addChild(iter, PUBLIC_CARD_TAG,"public_cards2_"+tools::to_string(j));

            if (scale > 1.3)
            {
                pos_x = pos_x / (scale-0.12);
            }
            iter->setPosition(pos_x + (j-1) * 70 * scale, pos_y2);
            iter->setScale(scale);
            
            if(show_cards_num2_ <= j)
            {
                
                PokerCardSprite::setCardSprite(0, iter);
                
                iter->runAction(Sequence::create(
                                                 OrbitCamera::create(0.2f,1, 0, 0, 90, 0, 0)
                                                 ,
                                                 CallFunc::create([this,iter](){
                    PokerCardSprite::setCardSprite(iter->getTag(), iter);
                    
                }),
                                                 OrbitCamera::create(0.2f,1, 0, -90, 90, 0, 0),
                                                 
                                                 
                                                 NULL));
                show_cards_num2_++;
            }
            
            has_show_cards2_.push_back(iter);
        }
    }
    if(publicCardSize2 > 0)
    {
        for(int i = 1; i <= 5;i++)
        {
            Sprite * card1 = (Sprite *)root_->getChildByName("public_cards1_"+tools::to_string(i));
            
            Sprite * card2 = (Sprite *)root_->getChildByName("public_cards2_"+tools::to_string(i));
            
            if (card1&&card2&&(card1->getTag() == card2->getTag())) {
                card1->setPosition(pos_x + (i-1) * 70 * scale, (pos_y1+pos_y2)*0.5f);
                card2->setVisible(false);
            }
        }
    }
}
void PokerRoomLayer::UpdatePlayerAction(int userid, int countdown, bool is_using_timebank)
{
    //by lvzhangle 如果不是自己，隐藏动作按钮
    if (userid != GDM->get_user_id())
    {
        hidden_action_btns();
    }
    
    auto info_node = get_player_info_node(userid);
    for (auto& it : info_nodes_) {
        it.second->show_green_box(false);
    }
    if (userid == GDM->get_user_id() && countdown > 0)
    {
        AM->playEffect("sound/alarm_active.mp3");
    }
    if(info_node)
    {
        info_node->show_progress_box(countdown,0, is_using_timebank);
    }

}

void PokerRoomLayer::update_game_status(const msg::GameStatus game_status)
{
    auto &poker_room_data = this->GetRoom();
    if (poker_room_data == nullptr)
    {
        cocos2d::log("PokerRoomLayer::update_game_status poker_room_data == nullptr");
        btn_start_->setVisible(false);
        btn_pause_->setVisible(false);
        return;
    }
    uint32_t roomid = poker_room_data->get_room_id();
    uint32_t ownerid = poker_room_data->get_owner_id();
    bool is_tournament = poker_room_data->get_is_tournament();
    NodeGameInPause* node_game_in_pause_ = dynamic_cast<NodeGameInPause*>(this->getChildByName("PAUSE_NODE_TAG"));
    
    if (!is_tournament)
    {
        
        if (game_status == msg::GameStatus::RUNNING)
        {
            
            btn_start_->setVisible(false);
            
            if (node_game_in_pause_ != nullptr)
            {
                node_game_in_pause_->setVisible(false);
            }
            
            if (ownerid == GDM->get_user_id())
            {
                btn_pause_->setVisible(true);
            }else
            {
                btn_pause_->setVisible(false);
            }
            
        }else if (game_status == msg::GameStatus::PAUSED)
        {
            if (ownerid == GDM->get_user_id())
            {
                btn_start_->setVisible(true);
                btn_pause_->setVisible(false);
                
                if (node_game_in_pause_ != nullptr)
                {
                    node_game_in_pause_->setVisible(false);
                }
                
            }else
            {
                btn_start_->setVisible(false);
                btn_pause_->setVisible(false);
                
                if (node_game_in_pause_ != nullptr)
                {
                    node_game_in_pause_->setVisible(true);
                }else
                {
                    node_game_in_pause_ = NodeGameInPause::create();
                    
                    Size size = Director::getInstance()->getVisibleSize();
                    
                    node_game_in_pause_->setPosition(size.width*0.5f- node_game_in_pause_->getContentSize().width*0.5f,size.height*0.5f);
                    
                    
                    this->addChild(node_game_in_pause_, 0, "PAUSE_NODE_TAG");
                }
            }
        }
    }else
    {
        btn_start_->setVisible(false);
        btn_pause_->setVisible(false);
        if (node_game_in_pause_ != nullptr)
        {
            node_game_in_pause_->setVisible(false);
        }
    }
    
}

void PokerRoomLayer::handle_showcard(const std::vector<bool> showcard)
{
    auto user = GetRoom()->get_user(GDM->get_user_id());
    if (user) {
        for (int i=0; i<user->get_properties()->holecards_size(); i++) {
            auto  iter_user_card = user_cards_.find(user->get_id());
            if(iter_user_card != user_cards_.end()) {
                auto & this_user_card = iter_user_card->second;
                auto iter_card = this_user_card.find(i);
                if(iter_card != this_user_card.end())
                {
                    TouchablePoker * poker = dynamic_cast<TouchablePoker*>(iter_card->second);
                    if (poker) {
                        poker->setIsSelected(showcard.at(i));
                    }
                }
            }
        }
    }
    
}

void PokerRoomLayer::handle_count_down(const msg::Processor_500_PokerRefreshAll& processor)
{
    //msg::PokerActionRequest
    uint32_t user_id = processor.actionrequest().actionrequest().actionplayerid();
    std::string roomnote = GetRoom()->get_note();
    text_table_note_->setString(roomnote);
    image_table_note_bg_->setContentSize(cocos2d::Size(text_table_note_->getContentSize().width, image_table_note_bg_->getContentSize().height));
    
    auto info_node = get_player_info_node(user_id);
    for (auto& it : info_nodes_) {
        it.second->show_green_box(false);
    }
    if(info_node) {
        info_node->show_progress_box(processor.actionrequest().actionrequest().countdown(),0, processor.actionrequest().actionrequest().isusingtimebank());
    }
    
    bool is_tournament = GetRoom()->get_is_tournament();
    if (is_tournament)
    {
        btn_tourment_info_->setVisible(true);
        btn_match_score_->setVisible(false);
        btn_buy_chip_->setVisible(false);
        //btn_record_->setVisible(false);
    }else
    {
        btn_tourment_info_->setVisible(false);
        btn_match_score_->setVisible(true);
        btn_buy_chip_->setVisible(true);
        //btn_record_->setVisible(true);
    }
}


//1.选择fold, check, bet
//2.选择fold, check, allin
//3.选择fold, call, raise
//4.选择fold, call, allin
//5.选择fold, allin
//6.选择fold, call
//7.选择fold，check，raise

//fold:弃牌
//check：看牌
//bet:下注
//call:跟注
//raise:加注
//allin：全下
void PokerRoomLayer::handle_player_operation(const msg::PokerActionRequest& data)
{
    hidden_action_btns();
    bCurrentUserAction = true;
    update_auto_buttons();
    
    std::vector<Button*> btns;
    player_operator_action_.CopyFrom(data);
    switch (data.actionchoice()) {
        case 1:
            btns.push_back(player_action_buttons_.at("fold"));
            btns.push_back(player_action_buttons_.at("check"));
            //btns.push_back(player_action_buttons_.at("bet"));
            node_chip_->setVisible(true);
            break;
        case 2:
            btns.push_back(player_action_buttons_.at("fold"));
            btns.push_back(player_action_buttons_.at("check"));
            btns.push_back(player_action_buttons_.at("allin"));
            break;
        case 3:
            btns.push_back(player_action_buttons_.at("fold"));
            btns.push_back(player_action_buttons_.at("call"));
            //btns.push_back(player_action_buttons_.at("raise"));
            node_chip_->setVisible(true);
            break;
        case 4:
            btns.push_back(player_action_buttons_.at("fold"));
            btns.push_back(player_action_buttons_.at("call"));
            btns.push_back(player_action_buttons_.at("allin"));
            break;
        case 5:
            btns.push_back(player_action_buttons_.at("fold"));
            btns.push_back(player_action_buttons_.at("allin"));
            break;
        case 6:
            btns.push_back(player_action_buttons_.at("fold"));
            btns.push_back(player_action_buttons_.at("call"));
            break;
        case 7:
            btns.push_back(player_action_buttons_.at("fold"));
            btns.push_back(player_action_buttons_.at("check"));
            //btns.push_back(player_action_buttons_.at("raise"));
            node_chip_->setVisible(true);
            break;
            
        default:
            break;
    }
    
    for (size_t i = 0; i < btns.size(); i++) {
        auto& btn = btns.at(i);
        if (i == 0) {
            btn->setPosition(Point(240, 150));
        }
        else if (i == 1)
        {
            if (data.actionchoice() == 5) {
                btn->setPosition(Point(375, 195));
            }
            else
            {
                btn->setPosition(Point(510, 150));
            }
            
        }
        else if (i == 2)
        {
            btn->setPosition(Point(375, 195));
        }
        
        btn->setVisible(true);
        
    }
    
    auto iter = player_action_buttons_.find("call");
    if(iter != player_action_buttons_.end()) {
        
        int tableBetMax = GetRoom()->get_bet_max();
        int amount = tableBetMax - GetRoom()->get_user(GDM->get_user_id())->get_properties()->betchips();
        if (amount == 0) {
            auto btn_watch = player_action_buttons_.at("check");
            btn_watch->setVisible(true);
            auto btn_call = player_action_buttons_.at("call");
            btn_call->setVisible(false);
            btn_watch->setPosition(btn_call->getPosition());
        }
        text_btn_follow_amount_->setString(tools::to_string(amount));
    }
    
    node_chip_->update_data(data);
}

void PokerRoomLayer::handle_game_hand_info(const msg::HandResultInfo& data)
{
    int32_t idx = 0;
    hidden_action_btns();
    set_node_of_chip_visible(false);
    bPlaying = false;
    update_auto_buttons();
    for (auto& iter : table_pot_chips_) {
        iter->removeFromParent();
    }
    table_pot_chips_.clear();
    
    for (auto& it : info_nodes_) {
        it.second->show_green_box(false);
    }// stop all countdowns. // by ronaldz.
    
    float width = Director::getInstance()->getWinSize().width;
    for (auto& iter : data.pot()) {
        ChipNode* cn = ChipNode::create();
        cn->update_data(iter.potamount(), true);
        cn->setPosition(Point(width/2 - 100 + 100 * idx++, 860));
        table_pot_chips_.push_back(cn);
        root_->addChild(cn);
    }
    
    for (int i = 0; i < data.pot_size(); i++) {
        idx = 0;
        for (int j = 0; j < data.pot(i).playerwin_size(); j++)
        {
            int32_t chip = data.pot(i).playerwin(j).playerwinchips();
            if(chip != 0)
            {
                PlayerInfoNode* info = get_player_info_node(data.pot(i).playerwin(j).userid());
                if(info)
                {
                    std::string s = "res/login_res/game_res/chips/";
                    if (chip > 1000)
                    {
                        s += "chip_1000_.png";
                    }
                    else if(chip > 500)
                    {
                        s += "chip_500_999.png";
                    }
                    else if(chip > 100)
                    {
                        s += "chip_100_499.png";
                    }
                    else if(chip > 50)
                    {
                        s += "chip_50_99.png";
                    }
                    else
                    {
                        s += "chip_25_49.png";
                    }
                    
                    //筹码尾巴
                    
                    Animation* animation = Animation::create();
                    animation->setDelayPerUnit(0.05f);
                    for (int i = 1; i <= 10; i++) {
                        std::string filename = StringUtils::format("chip_trailing_%02d.png",i);
                        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename));
                    }
                    
                    
                    Animate *animate = Animate::create(animation);
                    auto node = Sprite::create(s);
                    node->setPosition(table_pot_chips_[i]->getPosition());
                    int x = info->getPositionX() - node->getPositionX();
                    int y = info->getPositionY() - node->getPositionY();
                    double rotation = acos(abs(x)/sqrt(x*x + y*y)) / 3.14 * 180;
                    if (x >0)
                    {
                        if(y > 0)
                        {
                            rotation = 180 - rotation;
                        }
                        else
                        {
                            rotation = 180 + rotation;
                        }
                    }
                    else
                    {
                        if(y < 0)
                        {
                            rotation = 360 - rotation;
                        }
                        
                        
                    }
                    
                    auto sprite = Sprite::createWithSpriteFrameName("chip_trailing_01.png");
                    sprite->setRotation(rotation);
                    sprite->setAnchorPoint(Vec2(0,0.5));
                    sprite->setPosition(Vec2(node->getContentSize().width/2,node->getContentSize().height/2));
                    sprite->runAction(RepeatForever::create(animate));
                    node->addChild(sprite);
                    node->setScale(1.7);
                    
                    table_pot_chips_[i]->getParent()->addChild(node,256);
                    MoveTo* move = MoveTo::create(1.5f, info->getPosition() +  info->get_text_point());
                    
                    Sequence* seq = Sequence::create(DelayTime::create(0.5) ,move, CallFunc::create([=](){node->removeFromParent();
                        info->show_win_amount(chip);
                    }), NULL);
                    node->runAction(seq);
                    
                    AM->playEffect("sound/chip.mp3");
                    
                }
            }
        }
        
    }
    
    /*  auto call = [=](const uint32_t card_id, const bool low) {
     auto iter = all_cards_.find(card_id);
     if(iter != all_cards_.end()) {
     std::string image = "card/card_box_red.png";
     if(low) {
     image = "card/card_box_yellow.png";
     }
     auto bg = Sprite::create(image);
     bg->setPosition(Point(iter->second->getContentSize()) * 0.5f);
     iter->second->addChild(bg, -1);
     }
     };*/
    
    
    //    for (int i = 0; i < data->playerresult_size(); i++) {
    //        int32_t user_id = data->playerresult(i).userdetailinfo().userid();
    //        if(user_id == GDM->get_user_id()) {
    //            for (int j=0; j<data->playerresult(i).hicards_size(); j++)
    //                call(data->playerresult(i).hicards(j), false);
    //            for (int j=0; j<data->playerresult(i).locards_size(); j++)
    //                call(data->playerresult(i).locards(j), false);
    //        }
    //    }
    clear_room_ = false;
}
void PokerRoomLayer::handle_poker_receive_need_chip(cmd_data_pointer)
{
    if (!GetRoom()->get_is_record() && !GetRoom()->get_is_tournament()){
        auto processor = PM->GetUserPermission_up(GetRoom()->get_group_id(), GDM->get_user_id());
        net_manager::shared_instence().send_data(processor, REQ_GET_PERMISSION);
        PDM->send_poker_receive_need_chip();
        //        if (buy_chip_type_ == BUY_CHIP_TYPE::NONE)
        //        {
        //            GDM->send_get_playercredit(GDM->get_select_group_id(), GDM->get_user_id());
        //            buy_chip_type_ = BUY_CHIP_TYPE::HANDLE_BUY_CHIP;
        //            PDM->send_poker_receive_need_chip();
        //        }
        //        else
        //        {
        //            buy_chip_type_ = BUY_CHIP_TYPE::NONE;
        //            BuyChipLayer* bcl = BuyChipLayer::create();
        //            bcl->setPosition(Point(100,0));
        //            Size winSize = Director::getInstance()->getWinSize();
        //            bcl->setPosition(Point(0,winSize.height));
        //            root_->addChild(bcl, ZORDER_UI);
        //            MoveBy *mb = MoveBy::create(0.5, Point(0,-winSize.height));
        //            bcl->runAction(mb);
        //        }
        
    }
}

void PokerRoomLayer::handle_download_events(cmd_data_pointer data)
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
                auto info_node = get_player_info_node(user_id);
                if(info_node) {
                    info_node->show_voice_animation(duration);
                }
                AM->play_game_voice(amr_path);
            }
        }
    }
    else {  //刷新头像
        auto users = GetRoom()->get_valid_users();
        for (auto& iter : users) {
            auto info_node = info_nodes_.find(iter.second->get_seat_num());
            if(info_node != info_nodes_.end()) {
                auto user = static_pointer_cast<user_texas>(iter.second);
                info_node->second->update_data(user);
            }
        }
    }
}

void PokerRoomLayer::play_history_finished()
{
    RecordHudLayer* rhl = (RecordHudLayer*)getChildByTag(RECORDHUDLAYERTAG);
    rhl->handle_play_history_finished();
}

void PokerRoomLayer::show_history_layer()
{
    RecordHudLayer* rhl = RecordHudLayer::create();
    rhl->set_clear_callback([this](){
        clear(0);
        
        for (auto& iter : info_nodes_) {
            iter.second->set_description("");
        }
        
        for (auto& iter : chip_nodes_) {
            iter.second->removeFromParent();
        }
        chip_nodes_.clear();
        chip_nodes_flags.clear();//yxd
    });
    rhl->set_dismiss_callback([this](){
        clear(0);
        btn_action_->setVisible(true);
        btn_record_->setVisible(true);
        btn_match_score_->setVisible(true);
        bool is_tournament = GetRoom()->get_is_tournament();
        if(!is_tournament)
        {
            btn_buy_chip_->setVisible(true);
        }
        btn_voice_->setVisible(true);
        btn_chat_->setVisible(true);
        btn_share_->setVisible(true);
    });
    this->addChild(rhl,1, RECORDHUDLAYERTAG);
    clear(0);
    set_listview_visible(false);
    hidden_action_btns();
    node_chip_->setVisible(false);
    
    btn_action_->setVisible(false);
    btn_record_->setVisible(false);
    btn_match_score_->setVisible(false);
    btn_buy_chip_->setVisible(false);
    btn_voice_->setVisible(false);
    btn_chat_->setVisible(false);
    btn_share_->setVisible(false);
    clear_room_ = false;
    set_node_of_chip_visible(true);
    
    RecordLayer* prl = (RecordLayer*)getChildByTag(RECORDLAYERTAG);
    if((prl != NULL)&&prl->isShow)
    {
        prl->handle_get_history();
    }
    
    PositionUpdate();
}

void PokerRoomLayer::receive_game_start()
{
    set_node_of_chip_visible(true);
    bPlaying = true;
    clear(0);
    
    for (auto& iter : info_nodes_) {
        if(iter.first == GetRoom()->get_dealer_position()) {
            iter.second->set_dealer(true);
        }
        else {
            iter.second->set_dealer(false);
        }
    }
}

void PokerRoomLayer::handle_receive_chat(cmd_data_pointer)
{
    uint32_t unread_message_count = GDM->get_unread_message_count(GetRoom()->get_group_id());
    GameChatLayer* gcl = (GameChatLayer*) getChildByTag(GAME_CHAT_LAYER_TAG);
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

void PokerRoomLayer::receive_game_end()
{
    clear(0);
    show_cards_num1_ = 1;
    show_cards_num2_ = 1;
    has_show_cards1_.clear();
    has_show_cards2_.clear();
}

void PokerRoomLayer::receive_poker_tournamentstatus(const msg::Processor_530_PokerGetTourmentStatus_DOWN& processor)
{
    LayerCompetitionInfo* layer_com_info = LayerCompetitionInfo::create();
    
    this->addChild(layer_com_info, ZORDER_UI+100, "LayerComInfo");
    
    layer_com_info->update_data(processor);
}

void PokerRoomLayer::handle_receive_poker_tournament_request_addon(uint32_t buyin)
{
    auto user = static_pointer_cast<user_texas>(GetRoom()->get_user(GDM->get_user_id()));
    
    if(user) {
        if (GetRoom()->get_is_tournament())
        {
            if(user->get_seat_num() != 0) {
                if(user->get_status() == msg::SittingStatus::PLAYING ||
                   user->get_status() == msg::SittingStatus::SITOUT ||
                   user->get_status() == msg::SittingStatus::SITDOWN) {
                    
                    TournamentBuyChipLayer* tbcl = TournamentBuyChipLayer::create();
                    tbcl->set_text_string(tools::local_string("to_continue_addon", "继续参加需要报名费：")+ tools::to_string(buyin));
                    
                    Size winSize = Director::getInstance()->getWinSize();
                    tbcl->setPosition(Point(0,winSize.height));
                    root_->addChild(tbcl, ZORDER_UI);
                    MoveBy *mb = MoveBy::create(0.5, Point(0,-winSize.height));
                    tbcl->runAction(mb);
                }
            }
        }
    }
}

void PokerRoomLayer::receive_table_note(std::string note)
{
    text_table_note_->setString(note);
    image_table_note_bg_->setContentSize(cocos2d::Size(text_table_note_->getContentSize().width, image_table_note_bg_->getContentSize().height));
}

void PokerRoomLayer::handle_get_user_permission(cmd_data_pointer)
{
    if (!isRecord){
        buy_chip_type_ = BUY_CHIP_TYPE::BUY_CHIP;
        click_btn_buy_chip(btn_buy_chip_);
    }
}


#pragma mark private
void PokerRoomLayer::update_user_property(const std::shared_ptr<user_texas>& user)
{
    
    if(user->get_seat_num() == 0 || user->get_seat_num() == -1)
    {
        auto it = chip_nodes_.find(user->get_id());
        if(it != chip_nodes_.end()) {
            it->second->removeFromParent();
            chip_nodes_.erase(it);
        }
    }
    
    auto point = get_position_with_seat_num(user->get_seat_num());
    bool isBottom = false;
    if (point.card.x == 340) {
        isBottom = true;
    }
    
    if (!isRecord&&user->get_id() == GDM->get_user_id()&&user->get_status() == msg::SittingStatus::PLAYING)
    {
        if(user->get_properties()->holecards_size() == 4)
        {
            point.card = point.card + Point(-73,0);
        }
    }
    else
    {
        if (isBottom) {
            point.card = point.card + Point(75,155);
        }
    }
    
    
    float distance = 22.0f;
    
    bool isShowCard = false;
    for (int i=0; i<user->get_properties()->holecards_size(); i++) {
        int cardnum = user->get_properties()->holecards(i);
        if (cardnum > 0) {
            isShowCard = true;
            break;
        }
    }
    
    for (int i=0; i<user->get_properties()->holecards_size(); i++) {
        int cardnum = user->get_properties()->holecards(i);
        if ((user->get_status() == msg::SittingStatus::PLAYING) && (cardnum != -1)) {
            const std::string image_name = PDM->get_card_image(cardnum);
            auto iter_user_card = user_cards_.find(user->get_id());
            if(iter_user_card != user_cards_.end()) {
                auto& this_user_card = iter_user_card->second;
                auto iter_card = this_user_card.find(i);
                if(iter_card != this_user_card.end())
                {
                    
                    
                    if (!isRecord&&user->get_id() == GDM->get_user_id()&&user->get_status() == msg::SittingStatus::PLAYING)
                    {
                        iter_card->second->setPosition(point.card + Point(distance * i + i*50, 0));
                    }
                    else
                    {
                        
                        if(cardnum == 0&&!isShowCard)
                        {
                            
                            iter_card->second->setPosition(point.card + Point(distance * i, 0));
                            iter_card->second->setScale(0.6f);
                        }
                        else
                        {
                            
                            iter_card->second->setScale(1.0f);
                            if (iter_card->second->getPositionX() < 500||isBottom) {
                                iter_card->second->setPosition(point.card + Point(distance * i -40, 0));
                            }
                            else
                            {
                                iter_card->second->setPosition(point.card + Point(distance * i + 40, 0));
                            }
                            ssize_t c = getActionManager()->getNumberOfRunningActionsInTarget(iter_card->second);
                            if (cardnum != 0 && c == 0 && iter_card->second->getTag() != 1) {
                                iter_card->second->runAction(Sequence::create(
                                                                              OrbitCamera::create(0.2f,1, 0, 0, 90, 0, 0)
                                                                              ,
                                                                              CallFunc::create([=](){
                                    iter_card->second->setTexture(image_name);
                                    iter_card->second->setTag(1);
                                    
                                }),
                                                                              OrbitCamera::create(0.2f,1, 0, -90, 90, 0, 0),
                                                                              
                                                                              
                                                                              NULL));

                            }
                            
                        }
                    }
                    all_cards_[cardnum] = iter_card->second;
                    
                    
                }
                else {
                    Sprite* card = nullptr;
                    
                    
                    if (!isRecord&&user->get_id() == GDM->get_user_id()&&user->get_status() == msg::SittingStatus::PLAYING)
                    {
                        card = TouchablePoker::create(image_name);
                        card->setPosition(point.card + Point((distance+50) * this_user_card.size() , 0));
                        
                        
                        
                    }
                    else
                    {
                        
                        card = Sprite::create(image_name);
                        
                        if (cardnum == 0&&!isShowCard) {
                            card->setPosition(point.card + Point(distance * this_user_card.size() , 0));
                            card->setScale(0.6f);
                        }
                        else
                        {
                            if (point.card.x < 500||isBottom) {
                                card->setPosition(point.card + Point(distance * this_user_card.size()-40, 0));
                            }
                            else
                            {
                                card->setPosition(point.card + Point(distance * this_user_card.size() +40, 0));
                            }
                            
                            
                        }
                        
                    }
                    
                    this_user_card.insert(std::make_pair(i, card));
                    all_cards_[cardnum] = card;
                    
                    log("card_value:%u, %lu", cardnum, all_cards_.size());
                    card -> setVisible(false);//yxd动画
                    
                    root_->addChild(card, ZORDER_CARD);
                    card->retain();
                    
                    
                    //yxd动画
                    Size wins = Director::getInstance()->getWinSize();
                    
                    SpriteAction::runCardAction(this, Vec2(wins.width*0.5,wins.height*0.5)
                                                ,card->getPosition(),
                                                [ = ](){
                                                    
                                                    card -> setVisible(true);
                                                    TouchablePoker * poker = dynamic_cast<TouchablePoker *>(card);
                                                    if(poker)
                                                    {
                                                        poker->flip();
                                                    }
                                                    card->release();
                                                });//yxd动画
                    
                }
            }
            else {
                std::map<uint8_t, Sprite*> this_user_card;
                
                
                Sprite* card = nullptr;
                
                if (!isRecord&&user->get_id() == GDM->get_user_id()&&user->get_status() == msg::SittingStatus::PLAYING)
                {
                    card = TouchablePoker::create(image_name);
                    card->setPosition(point.card + Point((distance+50) * this_user_card.size() , 0));
                    
                }
                else
                {
                    
                    card = Sprite::create(image_name);
                    
                    if (cardnum == 0&&!isShowCard) {
                        
                        card->setPosition(point.card + Point(distance * this_user_card.size() , 0));
                        card->setScale(0.6f);
                    }
                    else
                    {
                        if (point.card.x < 375||isBottom) {
                            card->setPosition(point.card + Point(distance * this_user_card.size()-40, 0));
                        }
                        else
                        {
                            card->setPosition(point.card + Point(distance * this_user_card.size() +40, 0));
                        }
                        
                        
                    }
                    
                    
                }
                
                
                this_user_card.insert(std::make_pair(i, card));
                user_cards_.insert(std::make_pair(user->get_id(), this_user_card));
                all_cards_[cardnum] = card;
                //                        log("card_value:%u", iter.second->value);
                
                
                log("card_value:%u, %lu", cardnum, all_cards_.size());
                card -> setVisible(false);//yxd动画
                root_->addChild(card, ZORDER_CARD);
                card->retain();
                
                
                
                //yxd动画
                Size wins = Director::getInstance()->getWinSize();
                SpriteAction::runCardAction(this, Vec2(wins.width*0.5,wins.height*0.5)
                                            ,card->getPosition(),
                                            [ = ](){
                                                
                                                card -> setVisible(true);
                                                TouchablePoker * poker = dynamic_cast<TouchablePoker *>(card);
                                                if(poker)
                                                {
                                                    poker->flip();
                                                }
                                                card->release();
                                            });//yxd动画
                
                
            }
        }
        else{
            auto iter_user_card = user_cards_.find(user->get_id());
            if(iter_user_card != user_cards_.end()) {
                auto& this_user_card = iter_user_card->second;
                
                auto iter_card = this_user_card.find(i);
                if(iter_card != this_user_card.end()){
                    iter_card->second->removeFromParent();
                    this_user_card.erase(i);
                    if (this_user_card.size() == 0)
                        user_cards_.erase(user->get_id());
                }
            }
        }
        
        //by lvzhangle 20150808
        for (auto& iter : user_cards_gray){
            if (NULL != iter.second && iter.second->getParent())
                iter.second->removeFromParent();
        }
        user_cards_gray.clear();
        
    }
    //手里筹码
    
    int tmp_last_chip_nodes_num = _last_chip_nodes_num;


        
        auto it = info_nodes_.find(user->get_seat_num());
        if(it != info_nodes_.end()) {
            it->second->update_data(user);
        }
    
    if(user->get_status() == msg::SittingStatus::PLAYING)
    {
        int betchips = user->get_properties()->betchips();
        
        auto iter_chip_node = chip_nodes_.find(user->get_id());
        if(iter_chip_node != chip_nodes_.end()) {
            iter_chip_node->second->update_data(betchips);
            //yxd动画
            auto pos = get_position_with_seat_num(user->get_seat_num());
            if (chip_nodes_flags.find(user->get_id())->second != (betchips)
                &&iter_chip_node->second->isVisible()&&isNeedAction())
            {
                SpriteAction::runChipAction(this, Vec2(pos.seat.x,pos.seat.y-70),Vec2(pos.chip.x+18.5,pos.chip.y+15), betchips,[ = ](){
                });
                _last_chip_nodes_num = betchips;
            }
            chip_nodes_flags.find(user->get_id())->second = betchips;//yxd动画
            iter_chip_node->second->setPosition(pos.chip);
        }
        else {
            ChipNode* cn = ChipNode::create();
            cn->update_data(betchips);
            auto pos = get_position_with_seat_num(user->get_seat_num());
            cn->setPosition(pos.chip);
            
            if (user->get_id() == GDM->get_user_id()||pos.chip.x == 365) {
                cn->setHorizontal();
            }
            
            if (cn->getPositionY() == 0)
            {
                cn->setVisible(false);
            }
            root_->addChild(cn);
            chip_nodes_.insert(std::make_pair(user->get_id(), cn));
            
            chip_nodes_flags.insert(std::make_pair(user->get_id(), betchips));//yxd动画
        }

        
        
    }
    
    
    std::string action = "";
    switch (user->get_properties()->actionstatus()) {
        case msg::ActionStatus::NORMAL:
            action = "";
            break;
        case msg::ActionStatus::FOLD:
        {
            //by lvzhangle 20150808
            action = "弃牌";
            if (user->get_properties()->sittingstatus() == msg::SittingStatus::PLAYING){
                
                
                auto iter_user_card = user_cards_.find(user->get_id());
                if(iter_user_card != user_cards_.end())
                {
                    int i = 0;
                    //auto& this_user_card = iter_user_card->second;
                    for (auto & iter_card : iter_user_card->second)
                    {
                        i++;
                        if (NULL != iter_card.second && iter_card.second->getParent())
                        {
                            if (user->get_id() == GDM->get_user_id())
                            {
                                Sprite* cardmask = Sprite::create("card/card_fold.png");
                                cardmask->setPosition(iter_card.second->getPosition());
                                iter_card.second->setLocalZOrder(ZORDER_CARD+i);
                                root_->addChild(cardmask, ZORDER_CARD+i);
                                user_cards_gray.insert(std::make_pair(iter_card.first, cardmask));
                            }else {
                                iter_card.second->setVisible(false);  // by ronaldz 20160215. 隐藏， fold之后隐藏。
                            }
                        }
                    }
                }
            }
        }
            
            break;
        case msg::ActionStatus::CHECK:
            action = "让牌";
            
            break;
        case msg::ActionStatus::BET:
            action = "下注";
            
            break;
        case msg::ActionStatus::CALL:
            action = "跟注";
            
            break;
        case msg::ActionStatus::RAISE:
            action = "加注";
            
            break;
        case msg::ActionStatus::ALLIN:
        {
            action = "全下";
            
            if (user->get_id() == GDM->get_user_id()) {
                
                int betchips = user->get_properties()->betchips();
                int haschip = user->get_chip_amount();
                
                auto pos = get_position_with_seat_num(user->get_seat_num());
                if ((betchips < tmp_last_chip_nodes_num) && (haschip > _last_chip_num)) {
                    SpriteAction::runChipAction(this, Vec2(pos.chip.x+18.5,pos.chip.y+15),Vec2(pos.seat.x,pos.seat.y-70), betchips,[ = ](){
                    });
                }
        
            }
        }
            
            
            break;
        default:
            break;
    }
    
    _last_chip_num = user->get_chip_amount();
    
    auto info_node = get_player_info_node(user->get_id());
    if (info_node && !action.empty() && user->get_status() == msg::PLAYING)
    {
        
        info_node->set_description(action);
        
        
        if (GetRoom()->get_is_record())
        {
            
            if(GetRoom()->get_current_step() != GetRoom()->GetReplaySteps().replaystep_size())
            {
                info_node->hideWinAnimation();
            }
        }
        
        
    }
}


const PokerRoomLayer::POINTS PokerRoomLayer::get_position_with_seat_num(const uint32_t seat_num)
{
    clear_view_new();
    uint32_t seat_count = GetRoom()->get_max_seat_num();
    auto seats = seat_idx_with_person_amount_.at(seat_count);
    std::vector<POINTS> points;
    for(auto& iter : seats) {
        points.push_back(player_seat_chip_card_positions_.at(iter));
    }
    
    std::map<uint32_t, std::shared_ptr<user_base> > users;
    users = GetRoom()->get_valid_users();
    
    auto iter_this_user = users.find(GDM->get_user_id());
    if(!isRecord && iter_this_user != users.end() && iter_this_user->second->get_status() != msg::SittingStatus::OBSERVER) {
        std::vector<PlayerInfoNode*> sort_seats;
        for (auto& iter : info_nodes_) {
            sort_seats.push_back(iter.second);
        }
        
        std::sort(sort_seats.begin(), sort_seats.end(), [](PlayerInfoNode* obj1, PlayerInfoNode* obj2)->bool {
            return obj1->get_seat_id() < obj2->get_seat_id();
        });
        
        
        while (true) {
            auto front = sort_seats.front();
            if(front->get_seat_id() == iter_this_user->second->get_seat_num()) {
                break;
            }
            else {
                sort_seats.erase(sort_seats.begin());
                sort_seats.push_back(front);
            }
        }
        
        PokerRoomLayer::POINTS result;
        int32_t idx = 0;
        for (auto& iter : sort_seats) {
            if(iter->get_seat_id() == seat_num) {
                result = points.at(idx);
                break;
            }
            idx++;
        }
        return result;
    }
    else {
        return points.at(seat_num - 1);
    }
}

PlayerInfoNode* PokerRoomLayer::get_player_info_node(const uint32_t user_id)
{
    auto users = GetRoom()->get_valid_users();
    auto iter = users.find(user_id);
    if(iter != users.end()) {
        auto it = info_nodes_.find(iter->second->get_seat_num());
        if(it != info_nodes_.end()) {
            return it->second;
        }
    }
    return nullptr;
}


void PokerRoomLayer::init_seats()
{
    uint8_t seat_count = GetRoom()->get_max_seat_num();
    auto seats = seat_idx_with_person_amount_.at(seat_count);
    std::vector<POINTS> points;
    for(auto& iter : seats) {
        points.push_back(player_seat_chip_card_positions_.at(iter));
    }
    
    int idx = 1;
    auto users = GetRoom()->get_valid_users();
    for (auto& iter : points) {
        PlayerInfoNode* info = PlayerInfoNode::create();
        info->setPosition(iter.seat);
        info->set_giveup_callback(std::bind(&PokerRoomLayer::click_btn_giveup, this, btn_giveup_));
        root_->addChild(info, ZORDER_PLAYER_INFO);
        info->set_seat_id(idx);
        info_nodes_.insert(std::make_pair(idx++, info));
        
        bool is_exist = false;
        for (auto& it : users) {
            if(it.second->get_seat_num() == idx) {
                
                auto user = static_pointer_cast<user_texas>( it.second);
                
                info->update_data(user);
                is_exist = true;
                break;
            }
        }
        
        if(!is_exist) {
            info->update_data();
        }
    }
    
}

void PokerRoomLayer::init_auto_buttons(const msg::PokerPlayerSetting& setting)
{
    properties = setting;
    check_box_autocall->setSelected(setting.autocall());
    check_box_callorfold->setSelected(setting.callorfold());
    check_box_straddle->setSelected(setting.straddle());
    check_box_sitout->setSelected(setting.sitoutnexthand());
}


void PokerRoomLayer::register_events()
{
    dispatch_.register_event(POKER_RECEIVE_NEED_CHIP,
                             BIND_FUNC(this, PokerRoomLayer::handle_poker_receive_need_chip));
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, PokerRoomLayer::handle_download_events));
    
    dispatch_.register_event(EVENT_DOWLOAD_SOUND,
                             BIND_FUNC(this, PokerRoomLayer::handle_download_events));
    dispatch_.register_event(EVENT_HANDLE_REFRESH_CHAT_RECORD,
                             BIND_FUNC(this, PokerRoomLayer::handle_receive_chat));
    
    dispatch_.register_event(EVENT_REQ_GET_USER_PERMISSION,
                             BIND_FUNC(this, PokerRoomLayer::handle_get_user_permission));
    
}

void PokerRoomLayer::hidden_action_btns()
{
    bCurrentUserAction = false;
    for (auto& iter : player_action_buttons_) {
        iter.second->setVisible(false);
    }
    
    node_chip_->setVisible(false);
    
    for (auto& iter : info_nodes_) {
        iter.second->show_green_box(false);
    }
}

void PokerRoomLayer::set_listview_visible(const bool visible)
{
    check_box_autocall->setVisible(visible);
    check_box_callorfold->setVisible(visible);
    if (visible) {
        
        int tableBetMax = GetRoom()->get_bet_max();
        int amount = tableBetMax - GetRoom()->get_user(GDM->get_user_id())->get_properties()->betchips();
        ;
        if (amount == 0) {
            check_box_autocall_text->setString("自动看牌");
            check_box_autocall_text->setPositionY(60);
            check_box_autocall_amount->setVisible(false);
        }
        else
        {
            check_box_autocall_text->setString("自动跟注");
            check_box_autocall_text->setPositionY(75.60f);
            check_box_autocall_amount->setString(tools::to_string(amount));
            check_box_autocall_amount->setVisible(true);
        }
        
        
    }
    
}


void PokerRoomLayer::init_chip_pool()
{
    node_chip_ = ChipPoolNode::create();
    node_chip_->setPosition(Point(375, 240));
    node_chip_->setPokerRoomLayer(this);
    node_chip_->setVisible(false);
    root_->addChild(node_chip_, PUBLIC_CARD_TAG + 1);
}

void PokerRoomLayer::set_node_of_chip_visible(const bool visible)
{
    for (auto& iter : nodes_of_chip_) {
        iter->setVisible(visible);
    }
}

void PokerRoomLayer::clear(float)
{
//    if(clear_room_) return;
//    clear_room_ = true;
    for (auto& iter : user_cards_) {
        for (auto& it : iter.second) {
            it.second->removeFromParent();
        }
    }
    
    //by lvzhangle 20150808
    for (auto& iter : user_cards_gray){
        if (NULL != iter.second && iter.second->getParent())
            iter.second->removeFromParent();
    }
    

    hidden_action_btns();
    update_auto_buttons();
    node_chip_->setVisible(false);
    check_box_autocall->setSelected(false);
    check_box_callorfold->setSelected(false);
    check_box_straddle->setVisible(false);
    check_box_sitout->setVisible(false);
    check_box_straddle->setSelected(false);
    check_box_sitout->setSelected(false);
    properties.set_autocall(false);
    properties.set_callorfold(false);
    properties.set_straddle(false);
    properties.set_sitoutnexthand(false);
    
    
    text_table_note_->setString("");
    image_table_note_bg_->setContentSize(cocos2d::Size(text_table_note_->getContentSize().width, image_table_note_bg_->getContentSize().height));
    
    user_cards_.clear();
    user_cards_gray.clear();
    
    for (auto& iter : info_nodes_) {
        iter.second->clear();
    }
    
    for (auto& iter : table_pot_chips_) {
        iter->removeFromParent();
    }
    table_pot_chips_.clear();
    
    for (auto& iter : public_cards1_) {
        iter->removeFromParent();
    }
    for (auto& iter : public_cards2_) {
        iter->removeFromParent();
    }
    
    for (auto& iter : chip_nodes_) {
        iter.second->removeFromParent();
    }
    chip_nodes_.clear();
    chip_nodes_flags.clear();//yxd
    
    
    public_cards1_.clear();
    public_cards2_.clear();
    
    all_cards_.clear();
}

#pragma mark onEnter
void PokerRoomLayer::onEnter()
{
    Layer::onEnter();
    text_table_note_->setString(GetRoom()->get_note());
    image_table_note_bg_->setContentSize(cocos2d::Size(text_table_note_->getContentSize().width, image_table_note_bg_->getContentSize().height));
    handle_update_property();
}

//yxd
bool PokerRoomLayer::isNeedAction(){
    int32_t num;
    auto iter = chip_nodes_.begin();
    num = iter->second->_amount;
    for (; iter != chip_nodes_.end(); iter++) {
        if(iter->second->_amount != num) {
            return true;
            
        }
    }
    return false;
}//yxd

bool PokerRoomLayer::onTouchBegan(Touch *touch, Event *event)
{
    for (auto & info_iter : info_nodes_)
    {
        if (info_iter.second)
        {
            info_iter.second->set_dismiss();
        }
    }
    
    return true;
}
