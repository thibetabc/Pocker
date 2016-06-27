//
//  newOfcPokerRoom.cpp
//  pokerclient
//
//  Created by ll on 15/10/29.
//
//

#include "GameDataManager.h"
#include "PokerDataManager.h"

#include "newOfcPokerRoom.h"
#include "newOfcPokerRoomFantasy.h"
#include "OfcBackGroundLayer.h"
#include "NodeOfcPlayer.h"
#include "NodeGameInPause.h"
#include "Room.h"
#include "user_data.h"
#include "msg.pb.h"

#define ZORDER_NODE_ACTION 10
#define ZORDER_PLAYER_NODE 100
#define ZORDER_PLAYER_OWN_NODE 200
#define ZORDER_FANTASY 900
#define ZORDER_PAUSE_NODE 999
#define ZORDER_START_BTN 999
#define ZORDER_BACKGROUND_LAYER 1000

#define FANTASY_LAYER_TAG 1
#define START_BTN_TAG 2
#define PAUSE_NODE_TAG 3
#define MOVEPLAYERACTION_TAG 800

bool newOfcPokerRoom::init()
{
    if (!Layer::init()) {
        return false;
    }
    
//    this->setSwallowsTouches(true);
//    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(newOfcPokerRoom::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(newOfcPokerRoom::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(newOfcPokerRoom::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    root_= CSLoader::createNode("LayerOfcPokerRoomPortrait.csb");
    this->addChild(root_);
    
    OfcBackGroundLayer* obgl = OfcBackGroundLayer::create();
    //obgl->setZOrder(1000);
    this->addChild(obgl, ZORDER_BACKGROUND_LAYER, "BACKGROUND_LAYER_TAG");
    
    player_own_ = nullptr;
    
    btn_start_ = Button::create();
    btn_start_->loadTextureNormal("image/ofc_game_res/btn_ofc_start_normal.png");
    btn_start_->loadTexturePressed("image/ofc_game_res/btn_ofc_start_pressed.png");
    btn_start_->loadTextureDisabled("image/ofc_game_res/btn_ofc_start_pressed.png");
    cocos2d::Size size_btn_start = btn_start_->getContentSize();
    
    btn_start_->setPosition(cocos2d::Point(
                            DESIGN_WIDTH / 2  - size_btn_start.width * (0.5 - btn_start_->getAnchorPoint().x),
                            DESIGN_HEIGHT / 2 - size_btn_start.height * (0.5 - btn_start_->getAnchorPoint().y)));
    
    btn_start_->setPositionY(btn_start_->getPositionY() + size_btn_start.height);
    
    btn_start_->addClickEventListener(std::bind(&newOfcPokerRoom::click_btn_start, this, std::placeholders::_1));
    
    this->addChild(btn_start_, ZORDER_START_BTN, "START_BTN_TAG");
    
    btn_start_->setVisible(false);

    uint32_t max_seat_num = GetRoom()->get_max_seat_num();
    for (int i = 0; i < 4; i++)
    {
        NodeOfcPlayer* ofcplayer = NodeOfcPlayer::create();
        ofcplayer->set_sitdown_callback([=](){if (!player_own_)PDM->send_seat_info(GetRoom()->get_room_id(), msg::SittingStatus::SITDOWN, i+1);});
        ofcplayer->set_nodeNum(i+1);
        
        if (i <= max_seat_num - 1)
        {
            ofcplayer->set_displayPositionIdx(4 - max_seat_num + i);
            ofcplayer->setPosition(player_node_normal_[4 - max_seat_num + i]);
            
        }else
        {
            ofcplayer->set_displayPositionIdx(player_node_normal_.size() - 1);
            ofcplayer->setPosition(player_node_normal_[player_node_normal_.size() - 1]);
        }

        player_vec_.push_back(ofcplayer);
        
        root_->addChild(ofcplayer);
    }
    
    register_events();
    
    //If you need to debug fantasy, open it or close.
    OpenTestFantasy(false);
    
    return true;
}

const std::shared_ptr<OFCRoom> newOfcPokerRoom::GetRoom()
{ 
    if (isRecord)
        return dynamic_pointer_cast<OFCRoom>(PDM->get_replay_room(RoomID));
    else
        return dynamic_pointer_cast<OFCRoom>(PDM->get_room(RoomID));
}

void newOfcPokerRoom::click_btn_start(Ref * sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("newOfcPokerRoom::click_btn_start ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_room_id();
    uint32_t ownerid = ofc_room_data->get_owner_id();
    if (ownerid == GDM->get_user_id())
    {
        PDM->send_set_game_status(roomid, msg::GameStatus::RUNNING);
    }else
    {
        cocos2d::log("newOfcPokerRoom::click_btn_start not is owner but click button!");
    }
}

#pragma mark handle
void newOfcPokerRoom::update_record_status()
{

    OfcBackGroundLayer* layer_background_ = dynamic_cast<OfcBackGroundLayer*>(this->getChildByName("BACKGROUND_LAYER_TAG"));
    if (layer_background_ != nullptr)
    {
        layer_background_->update_record_status();
    }
    if (player_own_)
    {
        player_own_->update_record_status();
    }
}

void newOfcPokerRoom::update_game_status(const msg::GameStatus game_status)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("newOfcPokerRoom::update_game_status ofc_room_data == nullptr");
        btn_start_->setVisible(false);
        return;
    }
    uint32_t roomid = ofc_room_data->get_room_id();
    uint32_t ownerid = ofc_room_data->get_owner_id();
    
    NodeGameInPause* node_game_in_pause_ = dynamic_cast<NodeGameInPause*>(this->getChildByName("PAUSE_NODE_TAG"));
    
    if (game_status == msg::GameStatus::RUNNING)
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
                
                node_game_in_pause_->setPositionY(node_game_in_pause_->getPositionY() - size_node_game_in_pause.height);
                
                this->addChild(node_game_in_pause_, ZORDER_PAUSE_NODE, "PAUSE_NODE_TAG");
            }
        }
    }
    
    OfcBackGroundLayer* layer_background_ = dynamic_cast<OfcBackGroundLayer*>(this->getChildByName("BACKGROUND_LAYER_TAG"));
    if (layer_background_ != nullptr)
    {
        layer_background_->update_game_status(game_status);
    }
}

void newOfcPokerRoom::PositionUpdate()
{
    this->PositionUpdate(false);
}

void newOfcPokerRoom::PositionUpdate(bool isRefreshAll)
{
    auto users = GetRoom()->get_valid_users();
    //先判断自己在不在座位上
    bool bCurrentUserExist = false;

    for (auto & user_iter:users)
    {
        if (user_iter.second->get_id() == GDM->get_user_id()){
            bCurrentUserExist = true;
            break;
        }
    }
    if (bCurrentUserExist){
        if (player_own_ == nullptr){
            auto currentuser = static_pointer_cast<user_ofc>(users.find(GDM->get_user_id())->second);
            //setViewOffset(true, currentuser->get_seat_num());
            player_own_ = NodeOfcPlayerOwn::create();
            root_->addChild(player_own_);
            player_own_->set_user(currentuser);
            if (isRecord)
                player_own_->update_record_status();
        }
        player_own_->update_seat_info();
    }
    else if (player_own_ && player_own_->getParent())
    {
        player_own_->removeFromParent();
        player_own_ = nullptr;
        //setViewOffset(false);
    }
    //其余位置
    for (int i = 0; i < player_vec_.size(); i++)
    {
        player_vec_[i]->set_user(nullptr);
        for (auto & user_iter:users){
            if (user_iter.second->get_seat_num() == player_vec_[i]->get_nodeNum())
            {
                player_vec_[i]->set_user(static_pointer_cast<user_ofc>(user_iter.second));
                break;
            }
        }
        if (player_vec_[i]->get_nodeNum() > GetRoom()->get_max_seat_num() ||    //不存在的座位号（3人中的4）和当前玩家座位号的不显示
            (player_own_ && player_own_->get_user()->get_seat_num() == player_vec_[i]->get_nodeNum()))
            player_vec_[i]->setVisible(false);
        else{
            player_vec_[i]->update_seat_info();
            if (player_own_ && player_vec_[i]->get_user() == nullptr)  //坐下后空位不显示
                player_vec_[i]->setVisible(false);
            else
                player_vec_[i]->setVisible(true);
        }
    }
    
    if (player_own_ == nullptr)
        cocos2d::log("OfcPokerRoom:PositionUpdate player_own is Null");
    else
        cocos2d::log("OfcPokerRoom:PositionUpdate player_own isVisiable %s", player_own_->isVisible() ? "true" : "false");
    for (int i = 0; i < player_vec_.size(); i++)
    {
        cocos2d::log("player_vec[%d], nodeNum:%u, isVisiable:%s", i, player_vec_[i]->get_nodeNum(), player_vec_[i]->isVisible() ? "true" : "false");
    }
    
    
    if (player_own_)
    {
        setViewOffset(true, isRefreshAll);
        cocos2d::log("player_own : setViewOffset(true);");
        //update_viewoffset();
    }else
    {
        setViewOffset(false, isRefreshAll);
        cocos2d::log("player_own : setViewOffset(false);");
    }
}

void newOfcPokerRoom::handle_update_dealer()
{
    if (player_own_)
        player_own_->update_dealer();
    for (int i = 0; i < player_vec_.size(); i++)
    {
        player_vec_[i]->update_dealer();
    }
}

void newOfcPokerRoom::receive_ofc_prop_update()
{
    newOfcPokerRoomFantasy* layer_fantasy_ = dynamic_cast<newOfcPokerRoomFantasy*>(this->getChildByName("FANTASY_LAYER_TAG"));
    if (layer_fantasy_ != nullptr)
    {
        layer_fantasy_->update_propupdate();
    }
    
    if (player_own_ &&
        layer_fantasy_ == nullptr &&
        player_own_->get_user()->get_ofc_action_status() == msg::OFCActionStatus::FANTASY)
    {
        layer_fantasy_ = newOfcPokerRoomFantasy::create();
        this->addChild(layer_fantasy_, 0, "FANTASY_LAYER_TAG");
        layer_fantasy_->set_user(player_own_->get_user());
    }
    
    if (player_own_)
        player_own_->update_propupdate();
    
    for (int i = 0; i < player_vec_.size(); i++)
    {
        player_vec_[i]->update_propupdate();
    }
    
}

void newOfcPokerRoom::receive_game_end()
{
    for (int i = 0; i < player_vec_.size(); i++)
    {
        player_vec_[i]->set_clear_result();
    }
    if (player_own_)
        player_own_->set_clear_result();
}

void newOfcPokerRoom::update_ofc_send_player_cards_success(bool isSuccess)
{
    if (isSuccess)
    {
        newOfcPokerRoomFantasy* layer_fantasy_ = dynamic_cast<newOfcPokerRoomFantasy*>(this->getChildByName("FANTASY_LAYER_TAG"));
        if (layer_fantasy_ != nullptr)
        {
            layer_fantasy_->update_ofcfantasy_player_choose_card_success();
        }
    }else
    {
        if (player_own_)
            player_own_->update_hand_cards();
    }
}

void newOfcPokerRoom::UpdatePlayerAction(int userid, int countdown, bool is_using_timebank)
{
    if (GDM->get_user_id() == userid)
    {
        if (player_own_)
            player_own_->set_ProgressTimer(true, countdown, is_using_timebank);
        else
            cocos2d::log("Error : player action player_own is null!");
        
        newOfcPokerRoomFantasy* layer_fantasy_ = dynamic_cast<newOfcPokerRoomFantasy*>(this->getChildByName("FANTASY_LAYER_TAG"));
        if (layer_fantasy_ != nullptr)
        {
            layer_fantasy_->set_ProgressTimer(true, countdown, is_using_timebank);
        }
        
    }else{
        for (int i = 0; i < player_vec_.size(); i++)
        {
            if (player_vec_[i]->get_user() != nullptr &&
                player_vec_[i]->get_user()->get_id() == userid)
            {
                player_vec_[i]->set_ProgressTimer(true, countdown, is_using_timebank);
            }
        }
    }
}

void newOfcPokerRoom::receive_game_start()
{
    cocos2d::log("newOfcPokerRoom : handle_receive_game_start");
    handle_update_dealer();
    
    if (isGameOver_)
    {
        if (player_own_)
            player_own_->set_clear_result();
        for (int i = 0; i < player_vec_.size(); i++)
        {
            player_vec_[i]->set_clear_result();
        }
        isGameOver_ = false;
    }
}

void newOfcPokerRoom::receive_ofc_result_score()
{
    for (int i = 0; i < player_vec_.size(); i++)
    {
        player_vec_[i]->update_user_score();
        player_vec_[i]->update_poker_result();
    }
    if (player_own_){
        player_own_->update_user_score();
        player_own_->update_poker_result();
    }
}

void newOfcPokerRoom::handle_ofc_receive_result_type()
{
    //close all progresstimer
    if (player_own_)
        player_own_->set_ProgressTimer(false);
    for (int i = 0; i < player_vec_.size(); i++)
    {
        player_vec_[i]->set_ProgressTimer(false);
    }
    
    
    if (player_own_)
    {
        isFantasy_ = player_own_->get_user()->get_fantasy();
        player_own_->update_poker_result();
    }
    
    for (int j = 0; j < player_vec_.size(); j++)
    {
        if (player_vec_[j]->get_user() != nullptr)
        {
            player_vec_[j]->update_poker_result();
        }
    }
}

void newOfcPokerRoom::handle_ofc_receive_result_compare()
{
    
}

void newOfcPokerRoom::receive_table_note(std::string note)
{
    
}

void newOfcPokerRoom::init_auto_buttons(msg::OFCPlayerSetting& setting)
{
    if (player_own_)
        player_own_->update_auto_buttons(setting);
}

void newOfcPokerRoom::show_history_layer()
{
    update_record_status();
    
    PositionUpdate(true);
    
    receive_ofc_prop_update();
    
    handle_update_dealer();

}

#pragma mark register_events
void newOfcPokerRoom::register_events()
{

}

#pragma mark ViewOffset
void newOfcPokerRoom::setViewOffset(bool isOn, bool isRefreshAll)
{
    uint32_t max_seat_num = GetRoom()->get_max_seat_num();
    uint32_t point_offset = 0;
    if (max_seat_num < 4)
    {
        point_offset = 1;
    }
    
    std::vector<NodeOfcPlayer*> in_player_vec;
    std::vector<NodeOfcPlayer*> un_player_vec;
    std::vector<NodeOfcPlayer*> out_player_vec;
    NodeOfcPlayer* player_own_node = nullptr;
    
    if (isOn)
    {
        for (int i = 0; i < player_vec_.size(); i++)
        {
            if (player_vec_[i]->get_user() != nullptr && player_vec_[i]->get_nodeNum() <= max_seat_num)
            {
                if (player_vec_[i]->get_user()->get_id() != GDM->get_user_id())
                    in_player_vec.push_back(player_vec_[i]);
                else
                {
                    player_own_node = player_vec_[i];
                }
            }else
            {
                un_player_vec.push_back(player_vec_[i]);
            }
        }
        
        int32_t cur_node_num = 0;
        if (player_own_node != nullptr)
            cur_node_num = player_own_node->get_nodeNum();
        auto player_vec_sort_func = [=](NodeOfcPlayer* node1, NodeOfcPlayer* node2){return node1->get_nodeNum() < node2->get_nodeNum();};
        
        if (in_player_vec.size() != 0)
        {
            for (int i = 0; i < max_seat_num; i++)
            {
                if (cur_node_num > max_seat_num)
                {
                    cur_node_num = 1;
                }
                
                for (auto & player_site_iter : in_player_vec)
                {
                    if (player_site_iter->get_nodeNum() == cur_node_num)
                    {
                        out_player_vec.push_back(player_site_iter);
                        cocos2d::log("out_player_vec push_back seat_num %u", player_site_iter->get_nodeNum());
                    }
                }

                cur_node_num++;
            }
        }
        if (un_player_vec.size() != 0)
        {
            if (un_player_vec.size() > 1)
            {
                std::sort(un_player_vec.begin(), un_player_vec.end(), player_vec_sort_func);
            }
            for (int i = 0; i < un_player_vec.size(); i++)
            {
                out_player_vec.push_back(un_player_vec[i]);
            }
        }
        for (int i = 0; i < out_player_vec.size(); i++)
        {
            out_player_vec[i]->stopActionByTag(MOVEPLAYERACTION_TAG);
            if (isRefreshAll)
            {
                out_player_vec[i]->setPosition(player_node_in_game_[i + point_offset]);
            }else
            {
                MoveTo* move_player = MoveTo::create(0.5f, player_node_in_game_[i + point_offset]);
                out_player_vec[i]->runAction(move_player);
                move_player->setTag(MOVEPLAYERACTION_TAG);
            }
            cocos2d::log("out_player_vec SetPos Node:%u, Pos:%d", out_player_vec[i]->get_nodeNum(), i + point_offset);
        }
        if (player_own_node != nullptr)
        {
            player_own_node->setPosition(player_node_in_game_[3]);
        }
        
    }else
    {
        for (int i = 0; i < player_vec_.size(); i++)
        {
            if (player_vec_[i]->get_nodeNum() <= max_seat_num)
            {
                player_vec_[i]->stopActionByTag(MOVEPLAYERACTION_TAG);
                if (isRefreshAll)
                {
                    player_vec_[i]->setPosition(player_node_normal_[player_vec_[i]->get_nodeNum() - 1 + point_offset]);
                    
                }else
                {
                    MoveTo* move_player = MoveTo::create(0.5f, player_node_normal_[player_vec_[i]->get_nodeNum() - 1 + point_offset]);
                    player_vec_[i]->runAction(move_player);
                    move_player->setTag(MOVEPLAYERACTION_TAG);
                }
            }
        }
    }
}

void newOfcPokerRoom::update_viewoffset()
{
    std::vector<NodeOfcPlayer*> in_player_vec;
    for (int i = 0; i < player_vec_.size(); i++)
    {
        if (player_vec_[i]->get_user() != nullptr &&
            player_vec_[i]->get_user()->get_id() != GDM->get_user_id())
        {
            in_player_vec.push_back(player_vec_[i]);
        }
    }
    auto player_vec_sort_func = [=](NodeOfcPlayer* node1, NodeOfcPlayer* node2){return node1->get_nodeNum() < node2->get_nodeNum();};
    std::sort(in_player_vec.begin(), in_player_vec.end(), player_vec_sort_func);
    
    uint32_t max_seat_num = GetRoom()->get_max_seat_num();
    uint32_t point_offset = 0;
    if (max_seat_num == 3)
    {
        point_offset = 1;
    }
    
    for (int i = 0; i < in_player_vec.size(); i++)
    {
        in_player_vec[i]->setPosition(player_node_in_game_[i + point_offset]);
    }
}

#pragma mark TouchEvent
bool newOfcPokerRoom::onTouchBegan(Touch* touch, Event* event)
{
    for (auto player_node_iter : player_vec_)
    {
        player_node_iter->set_dismiss();
    }
    
    return true;
}

void newOfcPokerRoom::onTouchMoved(Touch* touch, Event* event)
{

}

void newOfcPokerRoom::onTouchEnded(Touch* touch, Event* event)
{

    
}

#pragma TestFunc
void newOfcPokerRoom::OpenTestFantasy(bool isOpen)
{
    if (isOpen)
    {
        auto testuser = GetRoom()->get_user(GDM->get_user_id());
        testuser->set_hand_cards(std::vector<uint32_t>{3, 11, 12, 26, 40, 15, 32, 10, 9, 34, 20, 19, 31, 4});
        testuser->set_hand_cards_location(std::vector<msg::OFCCardLocation>{msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD});
        newOfcPokerRoomFantasy* layer_fantasy_ =newOfcPokerRoomFantasy::create();
        this->addChild(layer_fantasy_);
        layer_fantasy_->set_user(testuser);
    }
}