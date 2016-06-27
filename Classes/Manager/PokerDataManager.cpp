#include "PokerDataManager.h"
#include "GameDataManager.h"
#include "net_manager.h"
#include "TipView.h"
#include "utils.h"
#include "cmd_def.h"
#include "Room.h"
#include "TipView.h"
#include "SceneManager.h"
#include "user_data.h"
#include "FileManager.h"
#include "LoginManager.h"
#include "AudioManager.h"
#include "NET_CMD.h"
#include "RecordDetailLayer.h"
#include "msg.pb.h"
#include "net_data_recv.h"
#include "LayerScore.h"
#include "UMstatistics.h"
#include "LayerCreateGameRoom.h"

using namespace std;
const string game_eventid = "game_play";

PokerDataManager* PokerDataManager::__poker_data_manager__ = nullptr;
PokerDataManager::PokerDataManager()
: player_action_(0)
{
    init_card_images();
    register_events();
    
}

#pragma mark get
const std::string PokerDataManager::get_card_image(const uint32_t card_value)
{
    if(GDM->get_is_newcard()){
        auto iter = card_id_images_.find(card_value);
        assert(iter != card_id_images_.end());
        std::string image_name = "card/new_poker/" + iter->second;
        return image_name;
    }
    else{
        auto iter = card_id_images_.find(card_value);
        assert(iter != card_id_images_.end());
        std::string image_name = "card/" + iter->second;
        return image_name;
    }
}

#pragma mark request
void PokerDataManager::send_poker_add_chips(const uint32_t amount,uint32_t room_id)
{
    if(amount == 0) {
        return;
    }
    msg::Processor_510_PokerAddChips processor;
    processor.set_roomid(room_id);
    processor.set_amount(amount);
    net_manager::shared_instence().send_data(processor, POKER_ADD_CHIPS);
}

void PokerDataManager::send_poker_choice(uint32_t roomid, msg::ChoiceType type, bool result)
{
    msg::Processor_513_PokerReplyChoice processor;
    processor.set_choicetype(type);
    processor.set_roomid(roomid);
    processor.set_result(result);
    net_manager::shared_instence().send_data(processor, POKER_REQUEST_CHOICE);
}

void PokerDataManager::send_enter_room(uint32_t room_id)
{
    msg::Processor_410_EnterRoom processor;
    processor.set_roomid(room_id);
    net_manager::shared_instence().send_data(processor, REQ_ENTER_ROOM);
}

void PokerDataManager::leave_all_room()
{
    for (auto it = room_map.begin(); it != room_map.end();){
        auto ittemp = it;
        it++;
        send_seat_info(ittemp->second->get_id(), msg::SittingStatus::LEAVE, 0);
    }
    for (auto it = replay_room_map.begin(); it != replay_room_map.end();){
        auto ittemp = it;
        it++;
        ittemp->second->close();
    }
}

void PokerDataManager::send_seat_info(const uint32_t room_id,const msg::SittingStatus status, const uint32_t seat_id)
{
    msg::Processor_411_SendSeatInfo processor;
    processor.set_roomid(room_id);
    processor.set_sittingstatus(status);
    processor.set_sitnum(seat_id);
    net_manager::shared_instence().send_data(processor, REQ_SEND_SEAT_INFO);
    
    if (status == msg::SittingStatus::LEAVE){
        //        UMstatistics::getInstance()->endGame(game_eventid);// 游戏内友盟统计时间结束
        close_room(room_id);
    }
}

void PokerDataManager::send_poker_player_game_action(const uint32_t room_id , const msg::ActionStatus action_id, const uint32_t amount)
{
    player_action_ = action_id;
    msg::Processor_502_PokerSendAction processor;
    processor.set_roomid(room_id);
    processor.set_actionstatus(action_id);
    processor.set_amount(amount);
    net_manager::shared_instence().send_data(processor, POKER_SEND_PLAYER_ACTION);
}

void PokerDataManager::send_send_set_change(const msg::PokerPlayerSetting& properties, uint32_t room_id)
{
    msg::Processor_515_PokerSendSetting processor;
    processor.set_roomid(room_id);
    processor.mutable_settings()->MergeFrom(properties);
    net_manager::shared_instence().send_data(processor, POKER_SEND_SET_CHANGE);
}

void PokerDataManager::send_show_card(const std::vector<bool>& ShowCard,uint32_t room_id)
{
    msg::Processor_517_PokerSendShowCard processor;
    processor.set_roomid(room_id);
    for (bool value: ShowCard)
        processor.add_showcard(value);
    net_manager::shared_instence().send_data(processor, POKER_SEND_SHOW_CARD);
}

void PokerDataManager::send_ofc_set_change(const msg::OFCPlayerSetting& properties, uint32_t room_id)
{
    msg::Processor_615_OFCSendSetting processor;
    processor.set_roomid(room_id);
    processor.mutable_settings()->MergeFrom(properties);
    net_manager::shared_instence().send_data(processor, OFC_SEND_SET_CHANGE);
}

void PokerDataManager::send_poker_receive_prop_update()
{
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    net_manager::shared_instence().send_data(processor, POKER_RECEIVE_PROP_UPDATE);
}

void PokerDataManager::send_receive_seat_info()
{
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    net_manager::shared_instence().send_data(processor, RECEIVE_SEAT_INFO);
}

void PokerDataManager::send_player_choose_cards(const std::vector<msg::OFCCardLocation> card_location_send,const uint32_t room_id)
{
    msg::Processor_602_OFCSendPlayerCard processor;
    processor.set_roomid(room_id);
    for (int i = 0; i < card_location_send.size(); i++)
    {
        processor.add_cardlocation(card_location_send[i]);
    }
    net_manager::shared_instence().send_data(processor, OFC_SEND_PLAYER_CARDS);
}

void PokerDataManager::send_receive_player_action()
{
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    net_manager::shared_instence().send_data(processor, POKER_REQUEST_PLYAER_ACTION);
}

void PokerDataManager::send_get_score(const uint32_t room_id)
{
    msg::Processor_420_PokerGetScore_UP processor;
    
    processor.set_roomid(room_id);
    net_manager::shared_instence().send_data(processor, REQ_GET_SCORE);
}

void PokerDataManager::send_poker_receive_hand_result()
{
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    net_manager::shared_instence().send_data(processor, POKER_RECEIVE_HAND_RESULT);
}

void PokerDataManager::send_poker_get_hand_history(const uint32_t hand_id, bool isPoker)
{
    msg::Processor_300_PokerGetHandHistory_UP processor;
    processor.set_handid(hand_id);
    processor.set_ispoker(isPoker);
    net_manager::shared_instence().send_data(processor, POKER_GET_HAND_HISTORY);
}

void PokerDataManager::send_receive_game_start()
{
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    net_manager::shared_instence().send_data(processor, RECEIVE_GAME_START);
}

void PokerDataManager::send_receive_other_player_action()
{
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    net_manager::shared_instence().send_data(processor, RECEIVE_OTHER_PLAYER_ACTION);
}

void PokerDataManager::send_poker_receive_need_chip()
{
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    net_manager::shared_instence().send_data(processor, POKER_RECEIVE_NEED_CHIP);
}
void PokerDataManager::send_login_roomserver()
{
    msg::Processor_400_LoginRoom_UP processor;
    processor.set_key(GDM->get_session_key());
    net_manager::shared_instence().send_data(processor, REQ_LOGIN_ROOM);
}



void PokerDataManager::send_game_voice(const std::string& message, const uint32_t time, const uint32_t room_id)
{
    upload_filename = message + ".amr";
    room_id_ = room_id;
    time_ = time;
    FM.upload_file(upload_filename);
    
}

void PokerDataManager::send_receive_game_voice()
{
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    net_manager::shared_instence().send_data(processor, RECEIVE_GAME_VOICE);
}

void PokerDataManager::send_poker_get_tourmentstatus(const uint32_t roomID)
{
    msg::Processor_530_PokerGetTourmentStatus_UP processor;
    processor.set_roomid(roomID);
    
    net_manager::shared_instence().send_data(processor, POKER_TOURMENT_STATUS);
}

void PokerDataManager::send_poker_tournament_addon(const uint32_t roomID, bool is_addon)
{
    msg::Processor_533_PokerTournamentAddon processor;
    processor.set_roomid(roomID);
    processor.set_addon(is_addon);
    
    net_manager::shared_instence().send_data(processor, REQ_SEND_TOURMENT_ADDON);
}

void PokerDataManager::send_poker_tourment_change_table(const uint32_t roomID, const uint32_t tableID)
{
    msg::Processor_531_PokerTourmentChangeTable processor;
    processor.set_roomid(roomID);
    processor.set_tableid(tableID);
    
    net_manager::shared_instence().send_data(processor, REQ_SEND_TOURMENT_CHANGE_TABLE);
}

void PokerDataManager::send_set_game_status(uint32_t room_id, msg::GameStatus game_status)
{
    msg::Processor_440_SetGameStatus processor;
    processor.set_roomid(room_id);
    processor.set_status(game_status);
    
    net_manager::shared_instence().send_data(processor, REQ_SET_GAME_STATUS);
}

void PokerDataManager::send_kick_player(uint32_t room_id, uint32_t kick_user_id)
{
    msg::Processor_441_KickPlayer processor;
    processor.set_roomid(room_id);
    processor.set_kickuserid(kick_user_id);
    
    net_manager::shared_instence().send_data(processor, REQ_KICK_PLAYER);
}

#pragma mark handle
void PokerDataManager::handle_room_close(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    msg::Processor_490_ReceiveRoomClose processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    close_room(processor.roomid());
    LayerScore* layer = LayerScore::create(processor.roomid());
    SceneManager::add_layer(layer);
}

void PokerDataManager::handle_room_will_close(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    msg::Processor_491_ReceiveRoomMessage processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    
    // add by zhaoxuewei
    // 弹出message淡出
    auto runningscene = Director::getInstance()->getRunningScene();
    auto size = Director::getInstance()->getWinSize();
    
    Text* text_message = Text::create(processor.message(), "Thonburi", 36);
    text_message->setTextColor(Color4B(181, 181, 181, 255));
    text_message->setAnchorPoint(Vec2(0.5, 0.5));
    LayerColor* message_bg = LayerColor::create(Color4B(0, 0, 0, 125), text_message->getContentSize().width + 20, 50);
    //message_bg->setAnchorPoint(Vec2(0.5, 0.5));
    
    message_bg->addChild(text_message);
    text_message->setPosition(Point(message_bg->getContentSize().width/2, message_bg->getContentSize().height/2));
    
    runningscene->addChild(message_bg, 9999);
    message_bg->setPosition(Point(Director::getInstance()->getWinSize().width/2, message_bg->getContentSize().height/2 + 20) - Point(message_bg->getContentSize().width/2, message_bg->getContentSize().height/2));
    Action * message_action = Sequence::create(DelayTime::create(4.0f), FadeOut::create(2.0f), NULL);
    Action * message_action_cp = message_action->clone();
    text_message->runAction(message_action_cp);
    message_bg->runAction(Sequence::create(DelayTime::create(4.0f), FadeOut::create(2.0f), CallFunc::create([=](){message_bg->removeFromParent();}), NULL));
}

void PokerDataManager::handle_poker_add_chips(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Response processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        switch (processor.status()) {
            case msg::Status::SUCCESS: {
                break;
            }
            case msg::Status::FAILED:
                TipView::showAlertView(processor.message());
                
                break;
            default:
                break;
        }
        buy_chip_amount_ = 0;
    }
}

void PokerDataManager::handle_receive_choice(cmd_data_pointer data) //512
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    msg::Processor_512_PokerRequestChoice processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    auto iter = room_map.find(processor.roomid());
    if (iter == room_map.end()) return;
    auto room = static_pointer_cast<PokerRoom>(iter->second);
    room->receive_choice(processor.choicetype());
    
}

void PokerDataManager::handle_enter_room(cmd_data_pointer data) {
    msg::Processor_410_EnterRoom_DOWN processor;
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec)
    {
        processor.ParseFromArray(rec->body, rec->bodysize);
        switch (processor.response().status())
        {
            case msg::Status::SUCCESS:
            {
                GDM->set_last_game_info(processor.roominfo().roomtype().roomtype(), processor.roominfo().istournament(), processor.roominfo().groupinfo().groupid(), processor.roominfo().roomid());
                //                UMstatistics::getInstance()->startGame(game_eventid);// 游戏内友盟统计时间开始
                
                break;
            }
            case msg::Status::FAILED:
            {
                if(!processor.has_tournamentinfo())
                {
                    TipView::showAlertView(processor.response().message());
                }
                else
                {
                    LayerCreateGameRoom* layerCreateRoom = dynamic_cast<LayerCreateGameRoom*>(SceneManager::get_layer_by_name(typeid(LayerCreateGameRoom).name()));
                    if (layerCreateRoom && layerCreateRoom->get_room_info().roomid() == processor.roominfo().roomid())
                    {
                        layerCreateRoom->update_enter_room_info(processor, false);
                    }else
                    {
                        layerCreateRoom = LayerCreateGameRoom::create(processor, false);
                        SceneManager::add_layer(layerCreateRoom);
                    }
                    //dispatch_->raise_event(EVENT_GET_TOURNAMENTIFO_LIST);
                    //dispatch_->raise_event(EVENT_GET_TOURNAMENTIFO);
                }
            }
                
                break;
            default:
                break;
        }
    }
}

void PokerDataManager::handle_send_seat_info(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_411_SendSeatInfo_Down processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        switch (processor.response().status())
        {
            case msg::Status::SUCCESS:
                break;
            case msg::Status::FAILED:
                if ( processor.ishitnotenough() )
                {
                    TipView* tip = TipView::showAlertView(processor.response().message(),true);
                    tip->set_tipview_close_callback([=](){
                        dispatch_->raise_event(EVENT_HIT_NOT_ENOUGH_CALL);
                    });
                }
                else
                {
                    TipView::showAlertView(processor.response().message());
                }
                
                break;
            default:
                break;
                
        }
    }
}

void PokerDataManager::handle_send_player_game_action(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Response processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        switch (processor.status()) {
            case msg::Status::SUCCESS: {
                break;
            }
            case msg::Status::FAILED:
                break;
            default:
                break;
        }
    }
}

void PokerDataManager::handle_poker_send_set_change(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Response processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        switch (processor.status()) {
            case msg::Status::SUCCESS:
            {
                break;
            }
            case msg::Status::FAILED:
            {
                TipView::showAlertView(tools::local_string("operate_failed_retry","操作失败"));
            }
                break;
            default:
                break;
        }
    }
}

void PokerDataManager::handle_poker_receive_prop_update(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_503_PokerReceivePropUpdate processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<PokerRoom>(iter->second);
        room->mergeupdate(processor.propupdate());
    }
    send_poker_receive_prop_update();
}
//182
void PokerDataManager::handle_receive_enter_match(cmd_data_pointer data)
{
    
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec)
    {
        msg::Processor_182_TournamentStart processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        uint32_t room_id = processor.roomid();
        //TODO:
        //去掉－进入下一个房间时退出当前房间
        //        uint32_t seat_id = PDM->get_seat_num();
        //        if(SceneManager::get_scene_type() == SceneManager::SCENE_ROOM)
        //        {
        //            uint32_t old_room_id = PDM->get_room()->get_id();
        //            PDM->send_seat_info(msg::SittingStatus::LEAVE, seat_id, old_room_id);
        //        }else if (SceneManager::get_scene_type() == SceneManager::SCENE_FAN_ROOM ||
        //           SceneManager::get_scene_type() != SceneManager::SCENE_FAN) {
        //            uint32_t old_room_id = PDM->get_ofc_room()->get_id();
        //            PDM->send_seat_info(msg::SittingStatus::LEAVE, seat_id, old_room_id);
        //        }
        send_enter_room(room_id);
        
    }
}
void PokerDataManager::handle_receive_seat_info(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_412_ReceiveSeatInfo processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        auto room = room_map.find(processor.roomid());
        if (room != room_map.end())
            room->second->update_seat_info(processor);
        send_receive_seat_info();
    }
}

void PokerDataManager::handle_receive_player_action(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_501_PokerRequestAction processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<PokerRoom>(iter->second);
        room->handle_player_operation(processor.actionrequest());
    }
}

void PokerDataManager::handle_get_score(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_420_PokerGetScore_DOWN processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        if(room_map.end() != room_map.find(processor.roomid()))
        {
            std::shared_ptr<Room> room = room_map.find(processor.roomid())->second;
            room->ShowInGameScore(processor);
        }
    }
}

void PokerDataManager::handle_poker_receive_hand_result(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_520_PokerReceiveHandResult processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<PokerRoom>(iter->second);
        room->show_hand_result(processor.handresultinfo());
        send_poker_receive_hand_result();
    }
}

void PokerDataManager::AddRecord(const std::shared_ptr<msg::GameRecord> record){
    records_.insert(records_.begin(), record);
    //records_.push_back(record);
    save_records();
}

const std::shared_ptr<Room> PokerDataManager::get_room(uint32_t roomid)
{
    auto iter = room_map.find(roomid);
    if (iter == room_map.end())
        return nullptr;
    else
        return iter->second;
}

const std::shared_ptr<Room> PokerDataManager::get_replay_room(uint32_t handid)
{
    auto iter = replay_room_map.find(handid);
    if (iter == replay_room_map.end())
        return nullptr;
    else
        return iter->second;
}

void PokerDataManager::add_room(std::shared_ptr<Room> Room)
{
    room_map.insert(std::make_pair(Room->get_id(), Room));
}

void PokerDataManager::add_replay_room(std::shared_ptr<Room> Room)
{
    replay_room_map.insert(std::make_pair(Room->get_id(), Room));
}

void PokerDataManager::close_room(int roomid)
{
    auto iter = room_map.find(roomid);
    if (iter == room_map.end()) return;
    auto room = static_pointer_cast<Room>(iter->second);
    room->close();
    room_map.erase(roomid);
    if (replay_room_map.size() > 0)
        replay_room_map.rbegin()->second->InitRecord();
    else if (room_map.size() > 0)
        room_map.rbegin()->second->Init();
}

void PokerDataManager::close_replay_room(int handid)
{
    auto iter = replay_room_map.find(handid);
    if (iter == replay_room_map.end()) return;
    auto room = static_pointer_cast<Room>(iter->second);
    room->close();
    replay_room_map.erase(handid);
    if (replay_room_map.size() > 0)
        replay_room_map.rbegin()->second->InitRecord();
    else if (room_map.size() > 0)
        room_map.rbegin()->second->Init();
}

void PokerDataManager::handle_poker_refresh_all(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_500_PokerRefreshAll processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        auto iter = room_map.find(processor.commonroominfo().roomid());
        std::shared_ptr<PokerRoom> room_(NULL);
        if (iter != room_map.end())
            room_ = static_pointer_cast<PokerRoom>(iter->second);
        else{
            room_ = std::shared_ptr<PokerRoom>(new PokerRoom());
            room_->set_id(processor.commonroominfo().roomid());
            add_room(room_);
        }
        room_->Init(processor);
    }
}
void PokerDataManager::handle_ofc_refresh_all(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_600_OFCRefreshAll processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.commonroominfo().roomid());
        std::shared_ptr<OFCRoom> room_(NULL);
        if (iter != room_map.end())
            room_ = static_pointer_cast<OFCRoom>(iter->second);
        else{
            room_ = std::shared_ptr<OFCRoom>(new OFCRoom());
            room_->set_id(processor.commonroominfo().roomid());
            add_room(room_);
        }
        room_->Init(processor);
    }
}

void PokerDataManager::handle_receive_ofc_prop_update(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_601_OFCReceivePropUpdate processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end())
        {
            cocos2d::log("error: processor.roomid() != current_room_id");
            return;
        }
        auto room = static_pointer_cast<OFCRoom>(iter->second);
        room->update_ofc_prop(processor.update());
    }
}


void PokerDataManager::handle_ofc_receive_result(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_610_OFCReceiveResult processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end())
        {
            cocos2d::log("error: processor.roomid() != current_room_id");
            return;
        }
        auto room = static_pointer_cast<OFCRoom>(iter->second);
        room->update_ofc_result(processor.result());
    }
}

void PokerDataManager::handle_ofc_send_player_cards(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::RoomResponse processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end())
        {
            cocos2d::log("error: processor.roomid() != current_room_id");
            return;
        }
        auto room = static_pointer_cast<OFCRoom>(iter->second);
        
        switch (processor.response().status()) {
            case msg::Status::SUCCESS: {
                room->update_ofc_send_player_success(true);
            }
                break;
            case msg::Status::FAILED:{
                TipView::showAlertView(processor.response().message());
                room->update_ofc_send_player_success(false);;//选牌失败
            }
                break;
            default:
                break;
        }
    }
}


void PokerDataManager::handle_poker_get_hand_history(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_300_PokerGetHandHistory_DOWN processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        if (processor.handtype() == msg::HandType::Poker)
        {
            if (isDetail) {
                Scene* scene = Director::getInstance()->getRunningScene();
                if (scene) {
                    RecordDetailLayer * layer = RecordDetailLayer::create();
                    layer->update_data(processor);
                    SceneManager::add_layer(layer);
                }
            }
            else{
                std::shared_ptr<PokerRoom> room_(new PokerRoom());
                room_->set_id(processor.handid());
                add_replay_room(room_);
                room_->InitRecord(processor);
            }
            
            
        }else if (processor.handtype() == msg::HandType::OFC)
        {
            std::shared_ptr<OFCRoom> room_(new OFCRoom());
            room_->set_id(processor.handid());
            add_replay_room(room_);
            room_->InitRecord(processor);
        }
        
    }
}

void PokerDataManager::handle_receive_game_start(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_413_ReceiveGameStart processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<Room>(iter->second);
        room->clear();
        room->set_current_hand_id(processor.handid());
        room->set_dealer_position(processor.dealerpos());
        room->game_start();
    }
}

void PokerDataManager::handle_receive_other_player_action(cmd_data_pointer data) {
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_417_ReceiveOtherPlayerAction processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<Room>(iter->second);
        room->UpdatePlayerAction(processor.actionrequest().actionplayerid(), processor.actionrequest().countdown(), processor.actionrequest().isusingtimebank());
        send_receive_other_player_action();
    }
}

void PokerDataManager::handle_login_room(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_400_LoginRoom_DOWN processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        switch (processor.response().status())
        {
            case msg::Status::SUCCESS:
            {
                for (auto iterRoom = room_map.begin(); iterRoom != room_map.end();)
                {
                    bool oldRoomFind = false;
                    for (auto iterRoomID : processor.roomid())
                    {
                        if (iterRoom->first == iterRoomID)
                        {
                            oldRoomFind = true;
                        }
                    }
                    if (!oldRoomFind)
                    {
                        iterRoom = room_map.erase(iterRoom);
                    }else
                    {
                        iterRoom++;
                    }
                }
                break;
            }
            case msg::Status::FAILED:
            {
                TipView::showAlertView(processor.response().message());
                break;
            }
            default:
                break;
        }
        
    }
}

void PokerDataManager::handle_upload_file_success(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (s->get_value() != upload_filename) return;
    msg::Processor_430_SendGameVoice processor;
    processor.set_roomid(room_id_);
    processor.set_time(tools::to_string(time_));
    processor.set_voice(upload_filename);
    net_manager::shared_instence().send_data(processor, REQ_SEND_GAME_VOICE);
}

void PokerDataManager::handle_receive_game_voice(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_431_ReceiveGameVoice processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<Room>(iter->second);
        room->receive_voice(processor.userid(), processor.voice(),
                            tools::stoi(processor.time()));
        send_receive_game_voice();
    }
}

void PokerDataManager::handle_poker_receive_set_change(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_516_PokerReceiveSettingChange processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<PokerRoom>(iter->second);
        room->receive_auto_button_change(processor.settings());
    }
}

void PokerDataManager::handle_ofc_poker_receive_set_change(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_616_OFCReceiveSettingChange processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<OFCRoom>(iter->second);
        room->receive_auto_button_change(processor.settings());
    }
}

void PokerDataManager::handle_receive_game_end(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_414_ReceiveGameEnd processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<Room>(iter->second);
        room->set_dealer_position(0);
//        room->clear();
        room->game_end();
    }
}

void PokerDataManager::handle_receive_poker_get_tourmentstatus(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec)
    {
        msg::Processor_530_PokerGetTourmentStatus_DOWN processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<PokerRoom>(iter->second);
        room->receive_poker_tournamentstatus(processor);
    }
}

void PokerDataManager::handle_poker_tournament_request_addon(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec)
    {
        msg::Processor_532_PokerTournamentRequestAddon processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<PokerRoom>(iter->second);
        room->NoticeTournamentAddon(processor.buyin());
    }
}

void PokerDataManager::handle_poker_receive_table_note(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec)
    {
        msg::Processor_514_PokerReceiveTableNote processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto room = room_map.find(processor.roomid());
        if (room == room_map.end()) return;
        std::string tablenote = processor.tablenote();
        if (room != room_map.end())
            static_pointer_cast<PokerRoom>(room->second)->set_note(tablenote);
    }
}

void PokerDataManager::handle_receive_set_game_status(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Response processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        switch (processor.status()) {
            case msg::Status::SUCCESS: {
                break;
            }
            case msg::Status::FAILED:
                TipView::showAlertView(processor.message());
                
                break;
            default:
                break;
        }
    }
}

void PokerDataManager::handle_receive_kick_player(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Response processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        switch (processor.status()) {
            case msg::Status::SUCCESS: {
                break;
            }
            case msg::Status::FAILED:
                TipView::showAlertView(processor.message());
                
                break;
            default:
                break;
        }
    }
}

void PokerDataManager::handle_receive_game_status(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_442_ReceiveGameStatus processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<Room>(iter->second);
        room->update_game_status(processor.status());
    }
    
    
}

#pragma mark mahjong
void PokerDataManager::handle_mahjong_receive_set_change(cmd_data_pointer data){
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_716_MahjongReceiveSettingChange processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<MahjongRoom>(iter->second);
        room->receive_auto_button_change(processor.settings());
    }
}

void PokerDataManager::handle_mahjong_receive_action(cmd_data_pointer data){
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_703_MahjongReceivePlayerAction processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<MahjongRoom>(iter->second);
        if (processor.has_switchtype())
            room->receive_switch_type(processor.switchtype());
        else
            for (auto action: processor.action())
                room->receive_player_action(action);
    }
}

void PokerDataManager::handle_mahjong_refresh_all(cmd_data_pointer data){
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_700_MahjongRefreshAll processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        auto iter = room_map.find(processor.commonroominfo().roomid());
        std::shared_ptr<MahjongRoom> room_(NULL);
        if (iter != room_map.end())
            room_ = static_pointer_cast<MahjongRoom>(iter->second);
        else{
            room_ = std::shared_ptr<MahjongRoom>(new MahjongRoom());
            room_->set_id(processor.commonroominfo().roomid());
            add_room(room_);
        }
        room_->Init(processor);
    }
}

void PokerDataManager::handle_mahjong_receive_prop_update(cmd_data_pointer data){
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
        msg::Processor_701_MahjongReceivePropUpdate processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        auto iter = room_map.find(processor.roomid());
        if (iter == room_map.end()) return;
        auto room = static_pointer_cast<MahjongRoom>(iter->second);
        room->update_mahjong_prop(processor.update());
    }
}


void PokerDataManager::send_mahjong_playersetting(const msg::MahjongPlayerSetting& properties, uint32_t room_id){
    msg::Processor_715_MahjongSendSetting processor;
    processor.set_roomid(room_id);
    processor.mutable_settings()->CopyFrom(properties);
    net_manager::shared_instence().send_data(processor, MAHJONG_SEND_SET_CHANGE);
}

void PokerDataManager::send_mahjong_choice(const uint32_t& room_id, const msg::MahjongActionChoice& choice, const msg::MahjongCard  &operationcard){
    msg::Processor_702_MahjongSendPlayerChoice processor;
    processor.set_roomid(room_id);
    processor.set_choice(choice);
    if (operationcard != msg::MahjongCard::INVISIBLE)
        processor.set_gangcard(operationcard);
    net_manager::shared_instence().send_data(processor, MAHJONG_SEND_PLAYER_CHOICE);
}

void PokerDataManager::send_mahjong_discard(const uint32_t& room_id, const msg::MahjongCard& card){
    
    msg::Processor_702_MahjongSendPlayerChoice processor;
    processor.set_roomid(room_id);
    processor.set_card(card);
    net_manager::shared_instence().send_data(processor, MAHJONG_SEND_PLAYER_CHOICE);
}

void PokerDataManager::send_mahjong_blanksuit(const uint32_t& room_id, const msg::MahjongSuit& suit){
    
    msg::Processor_702_MahjongSendPlayerChoice processor;
    processor.set_roomid(room_id);
    processor.set_suit(suit);
    net_manager::shared_instence().send_data(processor, MAHJONG_SEND_PLAYER_CHOICE);
}

void PokerDataManager::send_mahjong_switchcard(const uint32_t& room_id, const std::vector<msg::MahjongCard>& cards){
    msg::Processor_702_MahjongSendPlayerChoice processor;
    processor.set_roomid(room_id);
    for (auto card: cards)
    processor.add_switchcard(card);
    net_manager::shared_instence().send_data(processor, MAHJONG_SEND_PLAYER_CHOICE);
}

void PokerDataManager::send_disable_auto_action(const uint32_t &room_id)
{
    msg::Processor_415_DisableAutoAction processor;
    processor.set_roomid(room_id);
    net_manager::shared_instence().send_data(processor, REQ_DISABLE_AUTO_ACTION);
}

#pragma mark register_events
void PokerDataManager::register_history_events()
{
    dispatch_ = std::shared_ptr<events::event_dispatch>(new events::event_dispatch());
    dispatch_->register_event(POKER_RECEIVE_HAND_RESULT,
                              BIND_FUNC(this, PokerDataManager::handle_poker_receive_hand_result));
}

void PokerDataManager::register_events()
{
    dispatch_ = std::shared_ptr<events::event_dispatch>(new events::event_dispatch());
    
    dispatch_->register_event(RECEIVE_ROOM_MESSAGE,
                              BIND_FUNC(this, PokerDataManager::handle_room_will_close));
    dispatch_->register_event(POKER_ADD_CHIPS,
                              BIND_FUNC(this, PokerDataManager::handle_poker_add_chips));
    dispatch_->register_event(POKER_REQUEST_CHOICE,
                              BIND_FUNC(this, PokerDataManager::handle_receive_choice));
    dispatch_->register_event(REQ_ENTER_ROOM,
                              BIND_FUNC(this, PokerDataManager::handle_enter_room));
    dispatch_->register_event(REQ_SEND_SEAT_INFO,
                              BIND_FUNC(this, PokerDataManager::handle_send_seat_info));
    dispatch_->register_event(POKER_SEND_PLAYER_ACTION,
                              BIND_FUNC(this, PokerDataManager::handle_send_player_game_action));
    dispatch_->register_event(POKER_SEND_SET_CHANGE,
                              BIND_FUNC(this, PokerDataManager::handle_poker_send_set_change));
    dispatch_->register_event(POKER_RECEIVE_PROP_UPDATE,
                              BIND_FUNC(this, PokerDataManager::handle_poker_receive_prop_update));
    dispatch_->register_event(RECEIVE_ENTER_MATCH,
                              BIND_FUNC(this, PokerDataManager::handle_receive_enter_match));
    dispatch_->register_event(RECEIVE_SEAT_INFO,
                              BIND_FUNC(this, PokerDataManager::handle_receive_seat_info));
    dispatch_->register_event(POKER_REQUEST_PLYAER_ACTION,
                              BIND_FUNC(this, PokerDataManager::handle_receive_player_action));
    dispatch_->register_event(REQ_GET_SCORE,
                              BIND_FUNC(this, PokerDataManager::handle_get_score));  //420
    dispatch_->register_event(POKER_RECEIVE_HAND_RESULT,
                              BIND_FUNC(this, PokerDataManager::handle_poker_receive_hand_result));
    dispatch_->register_event(POKER_REFRESH_ALL,
                              BIND_FUNC(this, PokerDataManager::handle_poker_refresh_all));
    dispatch_->register_event(POKER_GET_HAND_HISTORY,
                              BIND_FUNC(this, PokerDataManager::handle_poker_get_hand_history));
    
    dispatch_->register_event(RECEIVE_GAME_START,
                              BIND_FUNC(this, PokerDataManager::handle_receive_game_start));
    dispatch_->register_event(RECEIVE_OTHER_PLAYER_ACTION,
                              BIND_FUNC(this, PokerDataManager::handle_receive_other_player_action));
    dispatch_->register_event(REQ_LOGIN_ROOM,
                              BIND_FUNC(this, PokerDataManager::handle_login_room));
    dispatch_->register_event(POKER_RECEIVE_SET_CHANGE,
                              BIND_FUNC(this, PokerDataManager::handle_poker_receive_set_change));
    dispatch_->register_event(OFC_RECEIVE_SET_CHANGE,
                              BIND_FUNC(this, PokerDataManager::handle_ofc_poker_receive_set_change));
    dispatch_->register_event(RECEIVE_GAME_VOICE,
                              BIND_FUNC(this, PokerDataManager::handle_receive_game_voice));
    dispatch_->register_event(OFC_REFRESH_ALL,
                              BIND_FUNC(this, PokerDataManager::handle_ofc_refresh_all)); //69
    dispatch_->register_event(OFC_RECEIVE_PROP_UPDATE,
                              BIND_FUNC(this, PokerDataManager::handle_receive_ofc_prop_update));
    dispatch_->register_event(OFC_RECEIVE_RESULT,
                              BIND_FUNC(this, PokerDataManager::handle_ofc_receive_result)); //67
    dispatch_->register_event(OFC_SEND_PLAYER_CARDS,
                              BIND_FUNC(this, PokerDataManager::handle_ofc_send_player_cards)); //68
    dispatch_->register_event(RECEIVE_GAME_END,
                              BIND_FUNC(this, PokerDataManager::handle_receive_game_end));
    dispatch_->register_event(REQ_SET_GAME_STATUS,
                              BIND_FUNC(this, PokerDataManager::handle_receive_set_game_status));
    dispatch_->register_event(REQ_KICK_PLAYER,
                              BIND_FUNC(this, PokerDataManager::handle_receive_kick_player));
    dispatch_->register_event(REQ_RECEIVE_GAME_STATUS,
                              BIND_FUNC(this, PokerDataManager::handle_receive_game_status));
    dispatch_->register_event(RECEIVE_ROOM_CLOSE,
                              BIND_FUNC(this, PokerDataManager::handle_room_close)); //490
    dispatch_->register_event(POKER_TOURMENT_STATUS,
                              BIND_FUNC(this, PokerDataManager::handle_receive_poker_get_tourmentstatus)); //490
    dispatch_->register_event(REQ_RECEIVE_TOURMENT_ADDON,
                              BIND_FUNC(this, PokerDataManager::handle_poker_tournament_request_addon)); //532
    dispatch_->register_event(EVENT_UPLOAD_FILE_SUCCESS,
                              BIND_FUNC(this, PokerDataManager::handle_upload_file_success));
    dispatch_->register_event(POKER_RECEIVE_TABLE_NOTE,
                              BIND_FUNC(this, PokerDataManager::handle_poker_receive_table_note)); //514
    
    dispatch_->register_event(MAHJONG_REFRESH_ALL,
                              BIND_FUNC(this, PokerDataManager::handle_mahjong_refresh_all));
    dispatch_->register_event(MAHJONG_RECEIVE_PROP_UPDATE,
                              BIND_FUNC(this, PokerDataManager::handle_mahjong_receive_prop_update));
    dispatch_->register_event(MAHJONG_RECEIVE_SET_CHANGE,
                              BIND_FUNC(this, PokerDataManager::handle_mahjong_receive_set_change));
    dispatch_->register_event(MAHJONG_RECEIVE_PLAYER_ACTION,
                              BIND_FUNC(this, PokerDataManager::handle_mahjong_receive_action));
}

#pragma mark card
void PokerDataManager::init_card_images()
{
    card_id_images_[0] = "pokerback.png";
    card_id_images_[1] = "club_2.png";
    card_id_images_[2] = "club_3.png";
    card_id_images_[3] = "club_4.png";
    card_id_images_[4] = "club_5.png";
    card_id_images_[5] = "club_6.png";
    card_id_images_[6] = "club_7.png";
    card_id_images_[7] = "club_8.png";
    card_id_images_[8] = "club_9.png";
    card_id_images_[9] = "club_10.png";
    card_id_images_[10] = "club_j.png";
    card_id_images_[11] = "club_q.png";
    card_id_images_[12] = "club_k.png";
    card_id_images_[13] = "club_1.png";
    
    card_id_images_[14] = "diamond_2.png";
    card_id_images_[15] = "diamond_3.png";
    card_id_images_[16] = "diamond_4.png";
    card_id_images_[17] = "diamond_5.png";
    card_id_images_[18] = "diamond_6.png";
    card_id_images_[19] = "diamond_7.png";
    card_id_images_[20] = "diamond_8.png";
    card_id_images_[21] = "diamond_9.png";
    card_id_images_[22] = "diamond_10.png";
    card_id_images_[23] = "diamond_j.png";
    card_id_images_[24] = "diamond_q.png";
    card_id_images_[25] = "diamond_k.png";
    card_id_images_[26] = "diamond_1.png";
    
    card_id_images_[27] = "heart_2.png";
    card_id_images_[28] = "heart_3.png";
    card_id_images_[29] = "heart_4.png";
    card_id_images_[30] = "heart_5.png";
    card_id_images_[31] = "heart_6.png";
    card_id_images_[32] = "heart_7.png";
    card_id_images_[33] = "heart_8.png";
    card_id_images_[34] = "heart_9.png";
    card_id_images_[35] = "heart_10.png";
    card_id_images_[36] = "heart_j.png";
    card_id_images_[37] = "heart_q.png";
    card_id_images_[38] = "heart_k.png";
    card_id_images_[39] = "heart_1.png";
    
    
    card_id_images_[40] = "spade_2.png";
    card_id_images_[41] = "spade_3.png";
    card_id_images_[42] = "spade_4.png";
    card_id_images_[43] = "spade_5.png";
    card_id_images_[44] = "spade_6.png";
    card_id_images_[45] = "spade_7.png";
    card_id_images_[46] = "spade_8.png";
    card_id_images_[47] = "spade_9.png";
    card_id_images_[48] = "spade_10.png";
    card_id_images_[49] = "spade_j.png";
    card_id_images_[50] = "spade_q.png";
    card_id_images_[51] = "spade_k.png";
    card_id_images_[52] = "spade_1.png";
    
}

#pragma mark record
void PokerDataManager::read_records(int roomid)
{
    records_.clear();
    auto path = GDM->get_user_directory() + "match_record.a";
    if(file_utils::file_exists(path)) {
        std::string content = file_utils::read_file(path);
        msg::GameRecordForSave records;
        records.ParseFromString(content);
        
        for (int i = 0; i < records.record_size(); i++) {
            auto& obj = records.record(i);
            if(obj.roomid() == roomid) {
                std::shared_ptr<msg::GameRecord> ro(new msg::GameRecord);
                ro->CopyFrom(obj);
                records_.push_back(ro);
            }
        }
    }
}

void PokerDataManager::read_records_fri()
{
    records_.clear();
    auto path = GDM->get_user_directory() + "match_record.a";
    if(file_utils::file_exists(path)) {
        std::string content = file_utils::read_file(path);
        msg::GameRecordForSave records;
        records.ParseFromString(content);
        
        for (int i = 0; i < records.record_size(); i++) {
            auto& obj = records.record(i);
            
            std::shared_ptr<msg::GameRecord> ro(new msg::GameRecord);
            ro->CopyFrom(obj);
            records_.push_back(ro);
            
        }
    }
}
void PokerDataManager::save_records()
{
    msg::GameRecordForSave records;
    if(records_.empty())
        return;
    for (auto& iter : records_) {
        auto obj = records.add_record();
        obj->CopyFrom(*iter);
    }
    records.set_userid(GDM->get_user_id());
    std::string content = "";
    records.SerializePartialToString(&content);
    
    auto path = GDM->get_user_directory() + "match_record.a";
    file_utils::write_file(path, content);
}