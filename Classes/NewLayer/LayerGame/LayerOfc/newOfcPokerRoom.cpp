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
#include "cmd_def.h"
#include "LayerOfcClickPlayerPhoto.h"
#include "LayerShop.h"


#define ZORDER_NODE_ACTION 10
#define ZORDER_PLAYER_NODE 100
#define ZORDER_PLAYER_OWN_NODE 200
#define ZORDER_FANTASY 100
#define ZORDER_PAUSE_NODE 999
#define ZORDER_START_BTN 999
#define ZORDER_BACKGROUND_LAYER 1000

//#define ZORDER_FANTASY_LAYER 10001

#define FANTASY_LAYER_TAG 1
#define START_BTN_TAG 2
#define PAUSE_NODE_TAG 3
#define MOVEPLAYERACTION_TAG 800

bool newOfcPokerRoom::init()
{
    if (!GameLayer::init()) {
        return false;
    }
    
//    this->setSwallowsTouches(true);
//    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(newOfcPokerRoom::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(newOfcPokerRoom::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(newOfcPokerRoom::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    root_= CSLoader::createNode("LayerOfcPokerRoomPortrait.csb");
    this->addChild(root_);
    
    OfcBackGroundLayer* obgl = OfcBackGroundLayer::create();
    obgl->setPlayVoiceCallback(std::bind(&newOfcPokerRoom::functionVideoCallPlayer, this, std::placeholders::_1, std::placeholders::_2));
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
        cocos2d::Point pos;
        bool is_large = false;
        if ( i == 2 &&  max_seat_num == 3)
        {
            is_large = true;
        }
        else if (i == 3 && max_seat_num == 4)
        {
            is_large = true;
        }
        else if (max_seat_num == 2)
        {
            pos = Tow_player_pos_[i];
            is_large = true;
        }
        NodeOfcPlayer* ofcplayer = NodeOfcPlayer::create(is_large);
        ofcplayer->set_sitdown_callback([=](){if (!player_own_)PDM->send_seat_info(GetRoom()->get_id(), msg::SittingStatus::SITDOWN, i+1);});
        ofcplayer->set_click_photo_callback(std::bind(&newOfcPokerRoom::add_player_info_layer,this,std::placeholders::_1));
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
        if (max_seat_num == 2)
        {
            ofcplayer->setPosition(pos);
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
    uint32_t roomid = ofc_room_data->get_id();
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
    
//    newOfcPokerRoomFantasy* layer_fantasy_ = dynamic_cast<newOfcPokerRoomFantasy*>(this->getChildByName("FANTASY_LAYER_TAG"));
//    if (layer_fantasy_ != nullptr)
//    {
//        layer_fantasy_->update_bcakground_record_status();
//    }
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
    uint32_t roomid = ofc_room_data->get_id();
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
//    newOfcPokerRoomFantasy* layer_fantasy_ = dynamic_cast<newOfcPokerRoomFantasy*>(this->getChildByName("FANTASY_LAYER_TAG"));
//    if (layer_fantasy_ != nullptr)
//    {
//        layer_fantasy_->update_bcakground_game_status(game_status);
//    }
}

void newOfcPokerRoom::PositionUpdate()
{
    this->PositionUpdate(false);
}

size_t newOfcPokerRoom::get_current_room_member_num()
{
    int member_num = 0;
    auto users = GetRoom()->get_users();
    for (auto iterUser : users)
    {
        if (iterUser.second != nullptr && iterUser.second->get_status() != msg::SittingStatus::OBSERVER && iterUser.second->get_status() != msg::SittingStatus::LEAVE && iterUser.second->get_seat_num() > 0)
            member_num++;
    }
    return member_num;
}
void newOfcPokerRoom::PositionUpdate(bool isRefreshAll)
{
    auto users = GetRoom()->get_users();
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
            player_own_->setPosition(player_node_in_game_[3]);
            player_own_->set_click_photo_callback(std::bind(&newOfcPokerRoom::add_player_info_layer,this,std::placeholders::_1));
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
    std::vector<int> need_change_index;
    std::vector<int> need_change_index_second;
    uint32_t max_seat_num = GetRoom()->get_max_seat_num();
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
        {
            player_vec_[i]->setVisible(false);
        }
        else{
            player_vec_[i]->update_seat_info();
            player_vec_[i]->update_record_status();
            if (player_own_ && player_vec_[i]->get_user() == nullptr)  //坐下后空位不显示
            {
                player_vec_[i]->setVisible(false);
            }
            else
            {
                player_vec_[i]->setVisible(true);
                if ( player_vec_[i]->get_is_large_cell() && this->get_current_room_member_num() != 2 && player_vec_[i]->get_user() && player_own_)
                {
                    need_change_index.push_back(i);
                }
                else if (!player_vec_[i]->get_is_large_cell() && this->get_current_room_member_num() == 2  && player_vec_[i]->get_user()&& player_own_)
                {
                    need_change_index.push_back(i);
                }
                
                if ( !player_own_ )
                {
                    
                    if ( i == 2 &&  max_seat_num == 3)
                    {
                        if (!player_vec_[i]->get_is_large_cell())
                        {
                            need_change_index.push_back(i);
                        }
                    }
                    else if (i == 2 && max_seat_num == 4)
                    {
                        if (player_vec_[i]->get_is_large_cell())
                        {
                            need_change_index.push_back(i);
                        }
                    }
                    else if (i == 3 && max_seat_num == 4)
                    {
                        if (!player_vec_[i]->get_is_large_cell())
                        {
                            need_change_index.push_back(i);
                        }
                    }
                    else if (i == 3 && max_seat_num == 3)
                    {
                        if (player_vec_[i]->get_is_large_cell())
                        {
                            need_change_index.push_back(i);
                        }
                    }
                    else if (i == 1 && max_seat_num != 2)
                    {
                        if (player_vec_[i]->get_is_large_cell())
                        {
                            need_change_index.push_back(i);
                        }
                    }
                    else if (i == 0 && max_seat_num != 2)
                    {
                        if (player_vec_[i]->get_is_large_cell())
                        {
                            need_change_index.push_back(i);
                        }
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < need_change_index.size(); i++)
    {
        bool is_big_cell = false;
        if (player_vec_[need_change_index[i]]->get_is_large_cell())
        {
            is_big_cell = false;
        }
        else
        {
            is_big_cell = true;
        }
        std::vector<NodeOfcPlayer*>::iterator it;
        for (it = player_vec_.begin(); it != player_vec_.end(); ++it)
        {
            if ((*it)->get_nodeNum() == player_vec_[need_change_index[i]]->get_nodeNum())
            {
                break;
            }
        }
        if (it != player_vec_.end())
        {
            NodeOfcPlayer* ofcplayer = NodeOfcPlayer::create(is_big_cell);
            ofcplayer->set_sitdown_callback([=](){if (!player_own_)PDM->send_seat_info(GetRoom()->get_id(), msg::SittingStatus::SITDOWN, player_vec_[need_change_index[i]]->get_nodeNum());});
            ofcplayer->set_nodeNum( player_vec_[need_change_index[i]]->get_nodeNum() );
            ofcplayer->set_displayPositionIdx(player_vec_[need_change_index[i]]->get_displayPositionIdx());
            ofcplayer->setPosition(player_vec_[need_change_index[i]]->getPosition());
            ofcplayer->set_user(player_vec_[need_change_index[i]]->get_user());
            ofcplayer->update_seat_info();
//            ofcplayer->update_propupdate();
            ofcplayer->set_click_photo_callback(std::bind(&newOfcPokerRoom::add_player_info_layer,this,std::placeholders::_1));
            player_vec_.push_back(ofcplayer);
            root_->addChild(ofcplayer);
            (player_vec_[need_change_index[i]])->removeFromParent();
            swap(ofcplayer,player_vec_[need_change_index[i]]);
            player_vec_.pop_back();
        }
    }
    
    
    
    for (int i = 0;i < need_change_index.size(); i++)
    {
        player_vec_[need_change_index[i]]->update_record_status();
        player_vec_[need_change_index[i]]->update_data(isRefreshAll);
        
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

void newOfcPokerRoom::receive_ofc_prop_update(bool isRefreshAll)
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
        this->addChild(layer_fantasy_, ZORDER_FANTASY, "FANTASY_LAYER_TAG");
        layer_fantasy_->set_user(player_own_->get_user());
        OfcBackGroundLayer* layer_background_ = dynamic_cast<OfcBackGroundLayer*>(this->getChildByName("BACKGROUND_LAYER_TAG"));
        if (layer_background_ != nullptr)
        {
            layer_background_->update_menu_status(true);
        }
    }
    
    if (player_own_)
        player_own_->update_propupdate(isRefreshAll);
    
    for (int i = 0; i < player_vec_.size(); i++)
    {
        player_vec_[i]->update_propupdate(isRefreshAll);
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
    OfcBackGroundLayer* layer_background_ = dynamic_cast<OfcBackGroundLayer*>(this->getChildByName("BACKGROUND_LAYER_TAG"));
    if (layer_background_ != nullptr)
    {
        layer_background_->update_room_note(note);
        layer_background_->update_chat_message();
    }
}

void newOfcPokerRoom::init_auto_buttons(const msg::OFCPlayerSetting& setting)
{
    if (player_own_)
        player_own_->update_auto_buttons(setting);
}

void newOfcPokerRoom::show_history_layer()
{
    update_record_status();
    
    PositionUpdate(true);
    
    receive_ofc_prop_update(true);
    
    handle_update_dealer();

}

void newOfcPokerRoom::refresh_user_poker_cards()
{
    if (player_own_)
    {
        player_own_->refresh_poker_card();
    }
    
    for (int j = 0; j < player_vec_.size(); j++)
    {
        if (player_vec_[j]->get_user() != nullptr)
        {
            player_vec_[j]->refresh_poker_card();
        }
    }
}

#pragma mark register_events
void newOfcPokerRoom::register_events()
{
    dispatch_.register_event(EVENT_HIT_NOT_ENOUGH_CALL,BIND_FUNC(this, newOfcPokerRoom::add_Shop_Layer));
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
            if (player_vec_[i]->get_user() != nullptr)
            {
                if (player_vec_[i]->get_user()->get_id() != GDM->get_user_id())
                {
                    in_player_vec.push_back(player_vec_[i]);
                }
                else
                {
                    player_own_node = player_vec_[i];
                }
            }else
            {
                un_player_vec.push_back(player_vec_[i]);
            }

        }
        if (this->get_current_room_member_num() == 2)
        {
            in_player_vec[0]->stopActionByTag(MOVEPLAYERACTION_TAG);
            if (isRefreshAll)
            {
                in_player_vec[0]->setPosition(player_two_player_[0]);
            }
            else
            {
                MoveTo* move_player = MoveTo::create(0.5f, player_two_player_[0]);
                in_player_vec[0]->runAction(move_player);
                move_player->setTag(MOVEPLAYERACTION_TAG);
            }
        }
        else if (this->get_current_room_member_num() == 3)
        {
            for ( int i = 0 ; i < this->get_current_room_member_num()-1 ; i++ )
            {
                in_player_vec[i]->stopActionByTag(MOVEPLAYERACTION_TAG);
                if (isRefreshAll)
                {
                    in_player_vec[i]->setPosition(player_three_player_[i]);
                }
                else
                {
                    MoveTo* move_player = MoveTo::create(0.5f, player_three_player_[i]);
                    in_player_vec[i]->runAction(move_player);
                    move_player->setTag(MOVEPLAYERACTION_TAG);
                }
            }
        }
        else if (this->get_current_room_member_num() == 4)
        {
            for ( int i = 0 ; i < this->get_current_room_member_num()-1 ; i++ )
            {
                in_player_vec[i]->stopActionByTag(MOVEPLAYERACTION_TAG);
                if (isRefreshAll)
                {
                    in_player_vec[i]->setPosition(player_four_player_[i]);
                }
                else
                {
                    MoveTo* move_player = MoveTo::create(0.5f, player_four_player_[i]);
                    in_player_vec[i]->runAction(move_player);
                    move_player->setTag(MOVEPLAYERACTION_TAG);
                }
            }
        }
  
        if (player_own_node != nullptr)
        {
            player_own_node->setPosition(cocos2d::Point(56,0));
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
                    if (max_seat_num == 2 )
                    {
                        player_vec_[i]->setPosition(Tow_player_pos_[i]);
                    }
                    else
                    {
                        player_vec_[i]->setPosition(player_node_normal_[player_vec_[i]->get_nodeNum() - 1 + point_offset]);
                    }
                }else
                {
                    if (max_seat_num == 2 )
                    {
                        MoveTo* move_player = MoveTo::create(0.5f,Tow_player_pos_[i]);
                        player_vec_[i]->runAction(move_player);
                        move_player->setTag(MOVEPLAYERACTION_TAG);
                    }
                    else
                    {
                        MoveTo* move_player = MoveTo::create(0.5f, player_node_normal_[player_vec_[i]->get_nodeNum() - 1 + point_offset]);
                        player_vec_[i]->runAction(move_player);
                        move_player->setTag(MOVEPLAYERACTION_TAG);
                    }
                    
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
void newOfcPokerRoom::add_player_info_layer(std::shared_ptr<user_ofc> user)
{
    msg::UserDetailInfo info;
    info.set_sex(user->get_sex());
    info.set_nickname(user->get_nickname());
    info.set_picname(user->get_head_pic_name());
    info.set_userid(user->get_id());
    
    uint32_t group_id = GetRoom()->get_group_id();
    uint32_t owner_id = GetRoom()->get_owner_id();
    uint32_t room_id = GetRoom()->get_id();
    
    LayerOfcClickPlayerPhoto* user_info = LayerOfcClickPlayerPhoto::create();
    this->addChild(user_info, 1001, "LayerOfcClickPlayerPhoto");
    user_info->setScale(0);
    CCScaleTo* scaleTo = CCScaleTo::create(0.15, 1.05);
    CCScaleTo* scaleTo2 = CCScaleTo::create(0.1, 1);
    user_info->runAction(CCSequence::createWithTwoActions(scaleTo,scaleTo2));
    
    user_info->set_player_info(info, group_id,owner_id,room_id,true);
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
        testuser->set_hand_cards(std::vector<uint32_t>{12, 11, 10, 9, 5, 25, 24, 23, 22, 20, 18, 17, 16, 14});
        testuser->set_hand_cards_location(std::vector<msg::OFCCardLocation>{msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD, msg::OFCCardLocation::DISCARD});
        newOfcPokerRoomFantasy* layer_fantasy_ =newOfcPokerRoomFantasy::create();
        this->addChild(layer_fantasy_,ZORDER_FANTASY);
        layer_fantasy_->set_user(testuser);
        OfcBackGroundLayer* layer_background_ = dynamic_cast<OfcBackGroundLayer*>(this->getChildByName("BACKGROUND_LAYER_TAG"));
        if (layer_background_ != nullptr)
        {
            layer_background_->update_menu_status(true);
        }
        
    }
}

void newOfcPokerRoom::functionVideoCallPlayer(uint32_t userId, int duration)
{
    ImageView *imgHeadIcon = nullptr;
    for(auto iter : player_vec_)
    {
        auto user = iter->get_user();
        if (user && user->get_id() == userId)
        {
            imgHeadIcon = iter->getImageView();
            break;
        }
    }
    if(imgHeadIcon)
    {
        auto s = Sprite::create("login_res/record_voice/user_voice_1.png");
        s->setPosition(imgHeadIcon->getPosition());
//        s->setPosition(imgHeadIcon, imgHeadIcon->getContentSize().height/2);
        imgHeadIcon->getParent()->addChild(s, 1001);
        
        auto animation = Animation::create();
        for (int i = 1; i != 4; ++i)
        {
            animation->addSpriteFrameWithFile("login_res/record_voice/user_voice_" + tools::to_string(i) + ".png");
        }
        animation->setDelayPerUnit(0.1f);
        
        s->runAction(RepeatForever::create(Animate::create(animation)));
        
        static int idx = 0;
        std::string remove_key = "remove_voice" + tools::to_string(idx++);
        scheduleOnce([=](float)
        {
            s->removeFromParent();
        }, duration, remove_key);
    }
}

void newOfcPokerRoom::onEnter()
{
    
//    log("进入newOfcPokerRoom游戏---删除图片资源前: %s", Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
    Director::getInstance()->purgeCachedData();
    Director::getInstance()->getTextureCache()->removeAllTextures();
//    log("进入newOfcPokerRoom游戏---删除图片资源后: %s", Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
    BaseLayer::onEnter();
}


void newOfcPokerRoom::add_Shop_Layer(cmd_data_pointer data)
{
    LayerShop* shop = LayerShop::create(false);
    this->addChild(shop,ZORDER_BACKGROUND_LAYER+1,"LayerShop");
}
