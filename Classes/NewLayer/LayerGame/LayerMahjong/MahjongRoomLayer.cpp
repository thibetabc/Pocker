//
//  MahjongLayer.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/5/30.
//
//

#include "MahjongRoomLayer.h"
#include "GameDataManager.h"
#include "TipView.h"
#include "PokerDataManager.h"
#include "FileManager.h"
#include "VoiceRecorderInterface.h"
#include "TipView.h"
#include "AudioManager.h"
#include "RecordAnimationLayer.h"
#include "Toast.h"
#include "LayerMahjongMenu.h"
#include "utils.h"
#include "LayerMahjongOnTheWater.h"
#include "NodeMahjong.h"
#include "LayerMahjongFinalResult.h"
#include "LayerMahjongReminder.h"
#include "LayerMahjongPullDown.h"
#include "LayerOfcPullDown.h"
#include "NodeGameInPause.h"
#include "NodeCommon.h"
#include "MatchScroeNode.h"

#define ANIMATION_LAYER_TAG 1
#define LAYER_ZORDER_MENU 1000
#define LAYER_ZORDER_MAHJONG_NODE 2000
#define LAYER_ZORDER_SUIT 3000
#define LAYER_ZORDER_PENG_GANG_HU 3000
#define LAYER_ZORDER_TRUSTEESHIP 4000

MahjongRoomLayer::MahjongRoomLayer():
mNodeMahjongDirection(nullptr)
{
    
}

MahjongRoomLayer::~MahjongRoomLayer()
{
    
}

bool MahjongRoomLayer::init()
{
    if ( !GameLayer::init() )
    {
        return false;
    }
    root_ = CSLoader::createNode("LayerMahjongRoom.csb");
    root_->setContentSize(DESIGN_SIZE);
    this->addChild(root_);
    
    image_mahjong_mark_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_mahjong_mark"));
    image_mahjong_mark_->setVisible(false);
    
    text_mahjong_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mahjong_num"));
    text_mahjong_num_->setVisible(false);
    
    btn_pull_down_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_pull_down"));
    btn_pull_down_->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_pull_down,this,std::placeholders::_1));
    btn_pull_down_->setLocalZOrder(LAYER_ZORDER_MENU);
    btn_start_ = Button::create();
    btn_start_->loadTextureNormal("image/ofc_game_res/btn_ofc_start_normal.png");
    btn_start_->loadTexturePressed("image/ofc_game_res/btn_ofc_start_pressed.png");
    btn_start_->loadTextureDisabled("image/ofc_game_res/btn_ofc_start_pressed.png");
    cocos2d::Size size_btn_start = btn_start_->getContentSize();
    
    btn_start_->setPosition(cocos2d::Point(DESIGN_WIDTH / 2, 815.0f));
    
    btn_start_->addClickEventListener(std::bind(&MahjongRoomLayer::beginGameAddClickListener, this, std::placeholders::_1));
    
    this->addChild(btn_start_, LAYER_ZORDER_MENU, "START_BTN_TAG");
    
    btn_start_->setVisible(false);
    mTextRoomId = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_id"));
    btn_menu_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_menu"));
    btn_menu_->addClickEventListener([&](Ref* sender){
        LayerMahjongMenu* menu = LayerMahjongMenu::create();
        this->addChild(menu, LAYER_ZORDER_MENU, "LayerMahjongMenu");
        menu->refresh_chat_btn();
    });
    btn_menu_->setLocalZOrder(LAYER_ZORDER_MENU);
    
    btn_voice_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_voice"));
    btn_voice_->addTouchEventListener(std::bind(&MahjongRoomLayer::click_btn_voice, this, std::placeholders::_1,std::placeholders::_2));
    btn_voice_->setLocalZOrder(LAYER_ZORDER_MENU);
    
    btn_reminder_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_reminder"));
    btn_reminder_->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_reminder, this, std::placeholders::_1));
    btn_reminder_->setLocalZOrder(LAYER_ZORDER_MENU);
    
    btn_on_the_water_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_on_the_water"));
    btn_on_the_water_->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_on_the_water, this, std::placeholders::_1));
    btn_on_the_water_->setLocalZOrder(LAYER_ZORDER_MENU);
    
    btn_auto_card_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_auto_card"));
    btn_auto_card_->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_auto_card, this, std::placeholders::_1));
    btn_auto_card_->setTitleText(tools::local_string("mahjong_auto_card","自动胡牌"));
    btn_auto_card_->setLocalZOrder(LAYER_ZORDER_MENU);
    
    btn_auto_play_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_auto_play"));
    btn_auto_play_->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_auto_play, this, std::placeholders::_1));
    btn_auto_play_->setTitleText(tools::local_string("mahjong_auto_play","自动打牌"));
    btn_auto_play_->setLocalZOrder(LAYER_ZORDER_MENU);
    
    //打牌倒计时
    mNodeMahjongDirection = NodeDirection::create();
    mNodeMahjongDirection->setVisible(false);
    root_->addChild(mNodeMahjongDirection);
    AlignNode(mNodeMahjongDirection, TOP, 0, 461 + 155 / 2);
    
    image_huanpai_bg_ = ImageView::create("image/Mahjong_game_res/image_action/image_dikuang.png");
    this->addChild(image_huanpai_bg_,1001);
    AlignNode(image_huanpai_bg_, TOP, 0, 473);
    image_huanpai_bg_->setVisible(false);
    
    text_switch_tip_ = Text::create("本局对家换牌", "Thonburi", 36);
    image_huanpai_bg_->addChild(text_switch_tip_,1001);
    AlignNode(text_switch_tip_, TOP, 0, 70);
    
    image_huanpai_tip_ = ImageView::create("image/Mahjong_game_res/image_action/image_hanpai_tip.png");
    image_huanpai_bg_->addChild(image_huanpai_tip_,1001);
    AlignNode(image_huanpai_tip_, TOP, 0, 15);
    
    
    
    //胡吃碰
    mButtonHu = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "button_hu"));
    mButtonHu->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_hu, this, std::placeholders::_1));
    mButtonHu->setVisible(false);
    mButtonHu->setLocalZOrder(LAYER_ZORDER_PENG_GANG_HU);
    
    mButtonPeng = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "button_peng"));
    mButtonPeng->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_peng, this, std::placeholders::_1));
    mButtonPeng->setVisible(false);
    mButtonPeng->setLocalZOrder(LAYER_ZORDER_PENG_GANG_HU);
    
    mButtonGang = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "button_gang"));
    mButtonGang->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_gang, this, std::placeholders::_1));
    mButtonGang->setVisible(false);
    mButtonGang->setLocalZOrder(LAYER_ZORDER_PENG_GANG_HU);
    
    mButtonPass = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "button_pass"));
    mButtonPass->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_pass, this, std::placeholders::_1));
    mButtonPass->setVisible(false);
    mButtonPass->setLocalZOrder(LAYER_ZORDER_PENG_GANG_HU);
    
    
    //选缺门
    mLayoutSuit = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "panel_suit"));
    mLayoutSuit->setVisible(false);
    mLayoutSuit->setLocalZOrder(LAYER_ZORDER_SUIT);
    mButtonTiao = dynamic_cast<Button*>(CSLoader::seekNodeByName(mLayoutSuit, "button_suit_tiao"));
    mButtonTiao->setTag(0);
    mButtonTiao->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_suit, this, std::placeholders::_1));
    mButtonTong = dynamic_cast<Button*>(CSLoader::seekNodeByName(mLayoutSuit, "button_suit_tong"));
    mButtonTong->setTag(1);
    mButtonTong->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_suit, this, std::placeholders::_1));
    mButtonWan = dynamic_cast<Button*>(CSLoader::seekNodeByName(mLayoutSuit, "button_suit_wan"));
    mButtonWan->setTag(2);
    mButtonWan->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_suit, this, std::placeholders::_1));
    
    //选同花色手牌
    mSpriteSwichCard = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_switch_card_bg"));
    mSpriteSwichCard->setVisible(false);
    mButtonSwichCard = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "button_switch_card"));
    mButtonSwichCard->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_switch_card, this, std::placeholders::_1));
    mTextAtlasSwitchCard = dynamic_cast<TextAtlas*>(CSLoader::seekNodeByName(mButtonSwichCard, "text_atlas_number"));//选了几张
    mSpriteLastDiscardTag = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_last_card_tag"));
    mSpriteLastDiscardTag->setVisible(false);
    mTextAtlasSwitchCard->setVisible(false);
    
    //托管
    mShowTrusteeship = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "button_trusteeship"));
    mShowTrusteeship->setVisible(false);
    mShowTrusteeship->setLocalZOrder(LAYER_ZORDER_TRUSTEESHIP);
    mShowTrusteeship->addClickEventListener(std::bind(&MahjongRoomLayer::click_btn_trusteeship, this, std::placeholders::_1));
    
    add_player_node(NodeMahjong::NODE_DIR::NODE_LEFT);
    add_player_node(NodeMahjong::NODE_DIR::NODE_RIGHT);
    add_player_node(NodeMahjong::NODE_DIR::NODE_UP);
    add_player_node(NodeMahjong::NODE_DIR::NODE_DOWN);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(MahjongRoomLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MahjongRoomLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MahjongRoomLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}
void MahjongRoomLayer::show_choose_switch_card()
{
    mSpriteSwichCard->setVisible(true);
}

void MahjongRoomLayer::add_player_node(NodeMahjong::NODE_DIR sitDir)
{
    auto nodeMahjong = NodeMahjong::create();
    nodeMahjong->setTag(MAHJONG_TAG + sitDir);
    root_->addChild(nodeMahjong, LAYER_ZORDER_MAHJONG_NODE);
    static_cast<NodeMahjong*>(nodeMahjong)->setDirectoion(sitDir);
    //createCard
    switch (sitDir) {
        case NodeMahjong::NODE_DIR::NODE_LEFT:
        {
            nodeMahjong->setContentSize(Size(273, 596));
            AlignNode(nodeMahjong, TOPLEFT, 0, 278);
            static_cast<NodeMahjong*>(nodeMahjong)->set_click_btn_sit_callback([&](){
                PDM->send_seat_info(RoomID, msg::SittingStatus::SITDOWN, 4);
            });
            break;
        }
        case NodeMahjong::NODE_DIR::NODE_UP:
        {
            nodeMahjong->setContentSize(Size(DESIGN_WIDTH, 450));
            AlignNode(nodeMahjong, TOPLEFT, 0, 0);
            static_cast<NodeMahjong*>(nodeMahjong)->set_click_btn_sit_callback([&](){
                PDM->send_seat_info(RoomID, msg::SittingStatus::SITDOWN, 3);
            });
            break;
        }
        case NodeMahjong::NODE_DIR::NODE_RIGHT:
        {
            nodeMahjong->setContentSize(Size(273, 596));
            AlignNode(nodeMahjong, TOPRIGHT, 0, 246);
            static_cast<NodeMahjong*>(nodeMahjong)->set_click_btn_sit_callback([&](){
                PDM->send_seat_info(RoomID, msg::SittingStatus::SITDOWN, 2);
            });
            break;
        }
        case NodeMahjong::NODE_DIR::NODE_DOWN:
        {
            nodeMahjong->setContentSize(Size(DESIGN_WIDTH, 524));
            AlignNode(nodeMahjong, BOTTOMLEFT, 0, 186);
            static_cast<NodeMahjong*>(nodeMahjong)->set_click_btn_sit_callback([&](){
                PDM->send_seat_info(RoomID, msg::SittingStatus::SITDOWN, 1);
            });
            break;
        }
    }
    static_cast<NodeMahjong*>(nodeMahjong)->set_switch_callback(std::bind(&MahjongRoomLayer::show_choose_switch_card, this));
    user_mahjong_node_.push_back(static_cast<NodeMahjong*>(nodeMahjong));
    
    static_cast<NodeMahjong*>(nodeMahjong)->update(nullptr, false);
}

void MahjongRoomLayer::beginGameAddClickListener(Ref *ref)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("MahjongRoomLayer::click_btn_start ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    uint32_t ownerid = ofc_room_data->get_owner_id();
    if (ownerid == GDM->get_user_id())
    {
        PDM->send_set_game_status(roomid, msg::GameStatus::RUNNING);
    }
    else
    {
        cocos2d::log("MahjongRoomLayer::click_btn_start not is owner but click button!");
    }
}

const std::shared_ptr<MahjongRoom> MahjongRoomLayer::GetRoom()
{
    return dynamic_pointer_cast<MahjongRoom>(PDM->get_room(RoomID));
}
void MahjongRoomLayer::update_game_status(const msg::GameStatus game_status)
{
    auto &room_data = this->GetRoom();
    if (room_data == nullptr)
    {
        cocos2d::log("MahjongRoomLayer::update_game_status ofc_room_data == nullptr");
        btn_start_->setVisible(false);
        return;
    }
    uint32_t ownerid = room_data->get_owner_id();
    
    
    NodeGameInPause* node_game_in_pause_ = dynamic_cast<NodeGameInPause*>(this->getChildByName("PAUSE_NODE_TAG"));
    
    if (game_status == msg::GameStatus::RUNNING )
    {
        
        btn_start_->setVisible(false);
        
        if (node_game_in_pause_ != nullptr)
        {
            node_game_in_pause_->setVisible(false);
        }
        
    }else if (game_status == msg::GameStatus::PAUSED)
    {
        if (ownerid == GDM->get_user_id())
        {
            btn_start_->setVisible(true);
            
            if (node_game_in_pause_ != nullptr)
            {
                node_game_in_pause_->setVisible(false);
            }
            
        }else
        {
            btn_start_->setVisible(false);
            
            if (node_game_in_pause_ != nullptr)
            {
                node_game_in_pause_->setVisible(true);
            }else
            {
                node_game_in_pause_ = NodeGameInPause::create();
                
                cocos2d::Size size_node_game_in_pause = node_game_in_pause_->getContentSize();
                
                node_game_in_pause_->setPosition(cocos2d::Point(
                                                                DESIGN_WIDTH / 2  - size_node_game_in_pause.width * (0.5 - node_game_in_pause_->getAnchorPoint().x),
                                                                DESIGN_HEIGHT / 2 - size_node_game_in_pause.height * (0.5 - node_game_in_pause_->getAnchorPoint().y)));
                
                //                node_game_in_pause_->setPositionY(sit_btn_[1]->getPositionY()- size_node_game_in_pause.height/2);
                
                this->addChild(node_game_in_pause_, LAYER_ZORDER_MENU, "PAUSE_NODE_TAG");
            }
        }
    }
}
//座位更新
void MahjongRoomLayer::PositionUpdate()
{
    
}

void MahjongRoomLayer::receive_game_start()
{
    remove_ting_layer();
    remove_layers();
    log("游戏开始 MahjongRoomLayer::receive_game_start()");
}

void MahjongRoomLayer::remove_layers()
{
    LayerMahjongOnTheWater* water = dynamic_cast<LayerMahjongOnTheWater*>(this->getChildByName("LayerMahjongOnTheWater"));
    if ( water )
        water->removeFromParent();
    LayerMahjongFinalResult* result = dynamic_cast<LayerMahjongFinalResult*>(this->getChildByName("LayerMahjongFinalResult"));
    if ( result )
        result->removeFromParent();
    MatchScroeNode* node = this->GetScoreLayer();
    if ( node )
        node->removeFromParent();
}

void MahjongRoomLayer::receive_game_end()
{
    remove_ting_layer();
    remove_layers();
    mNodeMahjongDirection->resetCountdown();
    log("游戏结束 MahjongRoomLayer::receive_game_end()");
}

void MahjongRoomLayer::receive_table_note(std::string note)
{
    mTextRoomId->setString(note);
}

void MahjongRoomLayer::init_auto_buttons(const msg::MahjongPlayerSetting& setting){
    setting_.CopyFrom(setting);
    if (setting_.discardall())
    {
        btn_auto_play_->loadTextures("image/Mahjong_game_res/mahjong_menu_res/btn_auto_pressed.png", "image/Mahjong_game_res/mahjong_menu_res/btn_auto_normal.png");
    }
    else
    {
        btn_auto_play_->loadTextures("image/Mahjong_game_res/mahjong_menu_res/btn_auto_normal.png", "image/Mahjong_game_res/mahjong_menu_res/btn_auto_pressed.png");
    }
    if (setting_.autohu())
    {
        btn_auto_card_->loadTextures("image/Mahjong_game_res/mahjong_menu_res/btn_auto_pressed.png", "image/Mahjong_game_res/mahjong_menu_res/btn_auto_normal.png");
    }
    else
    {
        btn_auto_card_->loadTextures("image/Mahjong_game_res/mahjong_menu_res/btn_auto_normal.png", "image/Mahjong_game_res/mahjong_menu_res/btn_auto_pressed.png");
    }
}

void MahjongRoomLayer::receive_player_action(const msg::MahjongAction& action){
    
    log("玩家action: userId = %d, scoreChange = %d, choice = %d", action.userid(), action.scorechange(), action.choice());
    for ( auto node : user_mahjong_node_ )
    {
        if ( action.userid() == node->getUserId() )
        {
            if ( action.has_choice() )
            {
                node->ShowAction(action.choice());
            }
            if ( action.has_scorechange() )
            {
                node->ShowAction(action.scorechange());
            }
        }
    }
    
}

void MahjongRoomLayer::receive_switch_type(const msg::MahjongCardSwitchType& switchtype){
//    log("玩家switchtype: switchtype = %d", switchtype);
    switch (switchtype) {
        case msg::MahjongCardSwitchType::SwitchClockwise: //顺时针
            image_huanpai_bg_->setVisible(true);
            text_switch_tip_->setString("本局顺时针换牌");
            break;
        case msg::MahjongCardSwitchType::SwitchWithOpposite:
            image_huanpai_bg_->setVisible(true);
            text_switch_tip_->setString("本局对家换牌");
            break;
        case msg::MahjongCardSwitchType::SwitchCounterClockwise:
            image_huanpai_bg_->setVisible(true);
            text_switch_tip_->setString("本局逆时针换牌");
            break;
        default:
            break;
    }
}
void MahjongRoomLayer::update_poker_menu_status()
{
    //剩余牌
    if ( GetRoom()->get_mahjong_prop_update().has_lastcards())
    {
        image_mahjong_mark_->setVisible(true);
        text_mahjong_num_->setVisible(true);
        text_mahjong_num_->setString(tools::to_string(GetRoom()->get_mahjong_prop_update().lastcards()));
    }
    else
    {
        image_mahjong_mark_->setVisible(false);
        text_mahjong_num_->setVisible(false);
    }
    
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("MahjongRoomLayer::click_btn_quit ofc_room_data == nullptr");
        return;
    }
    auto users = ofc_room_data->get_users();
    
    auto user_own = static_pointer_cast<user_mahjong>(GetRoom()->get_user(GDM->get_user_id()));
    if(user_own && (user_own->get_status() == msg::SittingStatus::PLAYING || user_own->get_status() == msg::SittingStatus::WANTTOLEAVE))
    {
        btn_auto_card_->setVisible(true);
        btn_auto_play_->setVisible(true);
        msg::MahjongPlayerInfo* mahjongPlayerInfo = user_own->get_properties();
        //听牌 begin
        ting_.clear();
        possible_ting_.clear();
        for ( int i = 0; i < mahjongPlayerInfo->ting_size(); i++ )
        {
            ting_.push_back(mahjongPlayerInfo->ting(i));
        }
        for ( int i = 0; i < mahjongPlayerInfo->actionrequest().possibleting_size() ; i++  )
        {
            possible_ting_.push_back(mahjongPlayerInfo->actionrequest().possibleting(i));
        }
        if ( ting_.size() > 0 )
        {
            btn_reminder_->setVisible(true);
        }
        else
        {
            btn_reminder_->setVisible(false);
        }
    }
    else
    {
        btn_auto_card_->setVisible(false);
        btn_auto_play_->setVisible(false);
        btn_reminder_->setVisible(false);
    }
    
//    if ( ofc_room_data->get_is_playing() )
//    {
//        btn_on_the_water_->setVisible(true);
//    }
//    else
//    {
//        if ( users.size() == 4 )
//        {
//            btn_on_the_water_->setVisible(true);
//        }
//        else
//        {
//            btn_on_the_water_->setVisible(false);
//        }
//    }
    
}


//协议流程：
//(进房间或断网收到700）
//400登录房间，410进入房间，411发坐下--412收座位更新----440开始--442接收游戏开始---接收到701更新牌
void MahjongRoomLayer::prop_update(){
    image_huanpai_bg_->setVisible(false);
    update_poker_menu_status();
    PositionUpdate();
    initData();
    auto users = GetRoom()->get_users();
    for ( auto  node : user_mahjong_node_ )
    {
        NodeMahjong::NODE_DIR dir = node->getDirection();
        bool is_refresh = false;
        for ( auto &user : users )
        {
            msg::MahjongPlayerInfo* mahjongPlayerInfo = static_pointer_cast<user_mahjong>(user.second)->get_properties();
            if(mahjongPlayerInfo)
            {
                NodeMahjong::NODE_DIR real_sit = getPlayerDir(user.second->get_seat_num());
                if ( real_sit == dir )
                {
                    is_refresh = true;
                    updateMahjongPlayerInfo(mahjongPlayerInfo, real_sit);
                    break;
                }
            }
        }
        if ( !is_refresh )
        {
            updateMahjongPlayerInfo(nullptr, dir);
        }
    }
}

#pragma mark - initData
void MahjongRoomLayer::initData()
{
    //initDirection
    auto eastSeat = GetRoom()->get_mahjong_prop_update().eastseatno();
    log("initData(), eastSeat = %d", eastSeat);
    mNodeMahjongDirection->setVisible(eastSeat != 0);
    if (eastSeat != 0)
        mNodeMahjongDirection->setEastDirection(getPlayerDir(eastSeat));
    
}

NodeMahjong::NODE_DIR MahjongRoomLayer::getPlayerDir(int sitNum)
{
    int basenum = 1;
    auto user = GetRoom()->get_user(GDM->get_user_id());
    if (user)
        basenum = user->get_seat_num();
    int distance = sitNum - basenum;
    if (distance < 0)
        distance += 4;
    switch (distance){
        case 0:
            return NodeMahjong::NODE_DIR::NODE_DOWN;
        case 1:
            return NodeMahjong::NODE_DIR::NODE_RIGHT;
        case 2:
            return NodeMahjong::NODE_DIR::NODE_UP;
        case 3:
            return NodeMahjong::NODE_DIR::NODE_LEFT;
        default:
            return NodeMahjong::NODE_DIR::NODE_DOWN;
    }
}

#pragma mark - UpdatePlayerInfo
void MahjongRoomLayer::updateMahjongPlayerInfo(msg::MahjongPlayerInfo *mahjongPlayerInfo, NodeMahjong::NODE_DIR sitDir)
{
    auto mahjongRoom = this->GetRoom();
    if (mahjongRoom == nullptr)
    {
        cocos2d::log("MahjongRoomLayer::updateMahjongPlayerInfo(...) called,  mahjongRoom == nullptr");
        return;
    }
    
    auto nodeMahjong = root_->getChildByTag(MAHJONG_TAG + sitDir);
    if ( nodeMahjong == nullptr )
    {
        return;
    }
    auto ownId = GDM->get_user_id();
    auto user_own = static_pointer_cast<user_mahjong>(GetRoom()->get_user(ownId));
    static_cast<NodeMahjong*>(nodeMahjong)->update(mahjongPlayerInfo, mahjongRoom->get_is_playing(), user_own == nullptr ? false : true );
    
    if ( mahjongPlayerInfo == nullptr )
    {
        return;
    }
    auto propUpdate = GetRoom()->get_mahjong_prop_update();
    auto currentUserId = mahjongPlayerInfo->playerinfo().userinfo().userid();
    this->ShowSwitchCard(false);
    if(mahjongPlayerInfo->has_actionrequest())
    {
        auto mahjongActionRequest = mahjongPlayerInfo->actionrequest();//麻将动作
        auto actionRequest = mahjongActionRequest.actionrequest();
        auto currentPlayerId =  actionRequest.actionplayerid();
        
        ShowChooseSuit((mahjongActionRequest.type() == msg::MahjongRequestType::MahjongChooseBlankSuit), mahjongActionRequest.defaultblanksuit());
        mButtonSwichCard->setVisible(mahjongActionRequest.type() == msg::MahjongRequestType::MahjongChooseSwitchCard);
        if (mButtonSwichCard->isVisible()){
            auto tNode = getNode(currentPlayerId);
            std::vector<msg::MahjongCard> defaultswitch;
            for (auto iter: mahjongActionRequest.defaultswitchcard())
                defaultswitch.push_back((msg::MahjongCard)iter);
            tNode->GetHandNode()->SetSelectedCard(defaultswitch);
            mButtonSwichCard->setEnabled(true);
            this->ShowSwitchCard(true);
            
        }
        ShowAction(mahjongActionRequest.canhu(), mahjongActionRequest.canpeng(), mahjongActionRequest.cangang());
        
        
        if(mahjongActionRequest.type() == msg::MahjongRequestType::MahjongChooseDiscard
           || ownId == currentPlayerId)
        {
            auto currentPlayingUser = static_pointer_cast<user_mahjong>(GetRoom()->get_user(currentPlayerId));
            auto seatNum = currentPlayingUser->get_seat_num();
            NodeMahjong::NODE_DIR real_sit = getPlayerDir(seatNum);
            mNodeMahjongDirection->setCountdown(real_sit, actionRequest.countdown(), actionRequest.isusingtimebank());
        }
    }
    else
    {
        if(currentUserId == ownId)
        {
            ShowChooseSuit(false, msg::MahjongSuit::SUITNONE);
            ShowAction(false, false, false);
            mButtonSwichCard->setVisible(false);
        }
    }
    static_cast<NodeMahjong*>(nodeMahjong)->SetLastCard(currentUserId == propUpdate.currentdiscarduserid(), propUpdate.currentdiscardzone());
    if(currentUserId == ownId)
    {
        ShowTrusteeship(mahjongPlayerInfo->playerinfo().autoaction());
    }
}

NodeMahjong *MahjongRoomLayer::getNode(uint32_t userId)
{
    for(int i = 0; i != MAX_PLAYER; ++i)
    {
        auto tNode = dynamic_cast<NodeMahjong*>(root_->getChildByTag(MAHJONG_TAG + i));
        if(tNode && tNode->getUserId() == userId)
        {
            return tNode;
        }
    }
    return nullptr;
}


#pragma mark - ClickEvent
void MahjongRoomLayer::handCardAddClickListener(Ref *ref)
{
    auto button = dynamic_cast<Button*>(ref);
    int tag = button->getTag();
    log("手牌tag ----> %d", tag);
    msg::MahjongCard disCard = (msg::MahjongCard)tag;
    PDM->send_mahjong_discard(RoomID, disCard);
}
void MahjongRoomLayer::click_btn_pull_down(cocos2d::Ref *sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_quit ofc_room_data == nullptr");
        return;
    }
    if ( ofc_room_data->get_owner_id() == GDM->get_user_id() )
    {
        LayerMahjongPullDown* menu = LayerMahjongPullDown::create(true);
        this->addChild(menu, 0, "LayerMahjongPullDown");
    }
    else
    {
        LayerMahjongPullDown* menu = LayerMahjongPullDown::create(false);
        this->addChild(menu, 0, "LayerMahjongPullDown");
    }
}

void MahjongRoomLayer::click_btn_voice(Ref* sender, Widget::TouchEventType event)
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
void MahjongRoomLayer::click_btn_on_the_water(Ref* sender)
{
    remove_ting_layer();
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("MahjongRoomLayer::click_btn_quit ofc_room_data == nullptr");
        return;
    }
    auto users = ofc_room_data->get_users();
    
    if ( ofc_room_data->get_is_playing() )
    {
        if ( users.size() > 0 )
        {
            //对拘流水
            auto self_user = ofc_room_data->get_user(GDM->get_user_id());
            if (self_user)
            {
                LayerMahjongOnTheWater* water = LayerMahjongOnTheWater::create();
                this->addChild(water,LAYER_ZORDER_MENU,"LayerMahjongOnTheWater");
                water->set_player_info(*self_user->get_properties());
            }
            else
            {
                msg::MahjongPlayerInfo info ;
                LayerMahjongOnTheWater* water = LayerMahjongOnTheWater::create();
                this->addChild(water,LAYER_ZORDER_MENU,"LayerMahjongOnTheWater");
                water->set_player_info(info);
            }
        }
        else
        {
            msg::MahjongPlayerInfo info ;
            LayerMahjongOnTheWater* water = LayerMahjongOnTheWater::create();
            this->addChild(water,LAYER_ZORDER_MENU,"LayerMahjongOnTheWater");
            water->set_player_info(info);
        }
    }
    else
    {
        LayerMahjongFinalResult* result = LayerMahjongFinalResult::create();
        this->addChild(result, 0, "LayerMahjongFinalResult");
        result->refresh_data();
    }
    
    
    
}
void MahjongRoomLayer::click_btn_reminder(Ref* sender)
{
    remove_ting_layer();
    LayerMahjongReminder* reminder = LayerMahjongReminder::create();
    reminder->set_data(ting_);
    this->addChild(reminder,1000,"LayerMahjongReminder");
    Size win_size = Director::getInstance()->getWinSize();
    int num = reminder->get_num();
    if ( num <= 5 )
    {
        reminder->setPosition(Vec2(win_size.width-reminder->getContentSize().width,356));
    }
    else
    {
        reminder->setPosition(Vec2(win_size.width-reminder->getContentSize().width,105));
    }
}
void MahjongRoomLayer::click_btn_auto_card(Ref* sender)
{
    remove_ting_layer();
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("MahjongRoomLayer::click_btn_quit ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    msg::MahjongPlayerSetting set;
    set.CopyFrom(setting_);
    set.set_autohu(setting_.autohu() == true ? false : true );
    PDM->send_mahjong_playersetting(set, roomid);
}
void MahjongRoomLayer::click_btn_auto_play(Ref* sender)
{
    remove_ting_layer();
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("MahjongRoomLayer::click_btn_quit ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    msg::MahjongPlayerSetting set;
    set.CopyFrom(setting_);
    set.set_discardall(setting_.discardall() == true ? false : true );
    PDM->send_mahjong_playersetting(set, roomid);
}


void MahjongRoomLayer::click_btn_hu(Ref *sender)
{
    PDM->send_mahjong_choice(RoomID, msg::MahjongActionChoice::HU);
}

void MahjongRoomLayer::click_btn_peng(Ref *sender)
{
    PDM->send_mahjong_choice(RoomID, msg::MahjongActionChoice::PENG);
}

void MahjongRoomLayer::click_btn_gang(Ref *sender)
{
    auto gangcards = getNode(GDM->get_user_id())->GetHandNode()->GetGangCard();
    if (gangcards.size() == 1)
        PDM->send_mahjong_choice(RoomID, msg::MahjongActionChoice::GANG, gangcards.at(0));
    else
        getNode(GDM->get_user_id())->GetHandNode()->setSelectMode(NodeHandCard::SELECT_GANG);
}

void MahjongRoomLayer::click_btn_pass(Ref *sender)
{
    PDM->send_mahjong_choice(RoomID, msg::MahjongActionChoice::PASS);
}

void MahjongRoomLayer::click_btn_suit(Ref *sender)
{
    int tag = dynamic_cast<Button*>(sender)->getTag();
    log("选缺门 tag = %d", tag);
    //发702
    msg::MahjongSuit suit;
    switch (tag) {
        case 0:
            suit = msg::MahjongSuit::TIAO;
            break;
        case 1:
            suit = msg::MahjongSuit::BING;
            break;
        case 2:
            suit = msg::MahjongSuit::WAN;
            break;
            
        default:
            break;
    }
    PDM->send_mahjong_blanksuit(RoomID, suit);
    
}

void MahjongRoomLayer::click_btn_switch_card(Ref *sender)
{
    SendSwitchCard(getNode(GDM->get_user_id())->GetHandNode()->GetSelectedCards());
}

void MahjongRoomLayer::click_hand_ting( msg::MahjongCard card)
{
    
    remove_ting_layer();
    auto user = static_pointer_cast<user_mahjong>(GetRoom()->get_user(GDM->get_user_id()));
    msg::MahjongPossibleTing possible_ting;
    for (int i = 0; i < user->get_properties()->actionrequest().possibleting_size(); i++)
    {
        if ( user->get_properties()->actionrequest().possibleting(i).card() ==  card)
        {
            possible_ting.CopyFrom(user->get_properties()->actionrequest().possibleting(i));
            break;
        }
    }
    if ( possible_ting.has_card() )
    {
        std::vector<msg::MahjongTing> ting;
        for ( int i = 0; i < possible_ting.ting_size() ; i++ )
        {
            ting.push_back(possible_ting.ting(i));
        }
        LayerMahjongReminder* reminder = LayerMahjongReminder::create();
        reminder->set_data(ting);
        this->addChild(reminder,0,"LayerMahjongReminder");
        
        Size win_size = Director::getInstance()->getWinSize();
        int num = reminder->get_num();
        if ( num <= 5 )
        {
            reminder->setPosition(Vec2(win_size.width-reminder->getContentSize().width,356));
        }
        else
        {
            reminder->setPosition(Vec2(win_size.width-reminder->getContentSize().width,105));
        }
        
    }
}

void MahjongRoomLayer::click_btn_trusteeship(Ref *ref)
{
    PDM->send_disable_auto_action(RoomID);
}

#pragma mark - showAction
void MahjongRoomLayer::remove_ting_layer()
{
    LayerMahjongReminder* reminder = dynamic_cast<LayerMahjongReminder*>(this->getChildByName("LayerMahjongReminder"));
    if ( reminder )
    {
        reminder->removeFromParent();
    }
}

void MahjongRoomLayer::SetSwitchEnabled(bool value)
{
    mButtonSwichCard->setEnabled(value);
}

//吃碰杠胡过
void MahjongRoomLayer::ShowAction(bool HasHu, bool HasPeng, bool HasGang)
{
    std::vector<Button*> vectorButton;
    
    if(HasHu || HasPeng || HasGang)
    {
        mButtonPass->setVisible(true);
        vectorButton.push_back(mButtonPass);
    }
    else
    {
        mButtonPass->setVisible(false);
    }
    
    mButtonHu->setVisible(HasHu);
    mButtonPeng->setVisible(HasPeng);
    mButtonGang->setVisible(HasGang);
    
    if(HasHu)
    {
        vectorButton.push_back(mButtonHu);
    }
    if(HasPeng)
    {
        vectorButton.push_back(mButtonPeng);
    }
    if(HasGang)
    {
        vectorButton.push_back(mButtonGang);
    }
    
    for(int i = 0; i < vectorButton.size(); ++i)
    {
        vectorButton[i]->setPositionX(645 - i * (vectorButton[i]->getContentSize().width + 5));
    }
    
}

void MahjongRoomLayer::DoAction(msg::MahjongActionChoice action)
{
    
}

//选缺门
void MahjongRoomLayer::ShowChooseSuit(bool IsShow, msg::MahjongSuit suit)
{
    float scale = IsShow ? 1.3f : 1.0f;
    mLayoutSuit->setVisible(IsShow);
    mButtonTiao->setScale(1.0f);
    mButtonTong->setScale(1.0f);
    mButtonWan->setScale(1.0f);
    switch (suit) {
        case msg::MahjongSuit::TIAO:
            mButtonTiao->setScale(scale);
            break;
        case msg::MahjongSuit::BING:
            mButtonTong->setScale(scale);
            break;
        case msg::MahjongSuit::WAN:
            mButtonWan->setScale(scale);
            break;
        default:
            break;
    }
    
}


//选牌
void MahjongRoomLayer::ShowSwitchCard(bool IsShow)
{
    mSpriteSwichCard->setVisible(IsShow);
}

void MahjongRoomLayer::setSwitchCard(int selectCount)
{
    mButtonSwichCard->setEnabled(selectCount == 3 ? true : false);
}

void MahjongRoomLayer::SendSwitchCard(std::vector<msg::MahjongCard> Cards)
{
    if ( Cards.size() != 3 )
    {
        TipView::showAlertView("请您选择三张同花色手牌");
        return;
    }
    PDM->send_mahjong_switchcard(RoomID, Cards);
}
//托管
void MahjongRoomLayer::ShowTrusteeship(bool isShow)
{
    mShowTrusteeship->setVisible(isShow);
}

#pragma mark touch
void MahjongRoomLayer::onTouchEnded(Touch* touch, Event* event)
{
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    LayerMahjongReminder* reminder = dynamic_cast<LayerMahjongReminder*>(this->getChildByName("LayerMahjongReminder"));
    if ( reminder )
    {
        if ( !reminder->getBoundingBox().containsPoint(touchLocation) )
        {
            this->removeChildByName("LayerMahjongReminder");
        }
    }
}







