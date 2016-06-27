//
//  Room.cpp
//  pokerclient
//
//  Created by admin on 15-12-22.
//
//

#include "Room.h"
#include "AudioManager.h"
#include "GameDataManager.h"
#include "PokerDataManager.h"
#include "user_data.h"
#include "SceneManager.h"
#include "FileManager.h"

const uint32_t Room::get_currentuser_seat_num()
{
    auto iter = users_.find(GDM->get_user_id());
    if(iter != users_.end())
    {
        return iter->second->get_seat_num();
    }
    return 0;
}

void Room::close()
{
    auto layer = GetLayer();
    if(layer == nullptr)
        return;
    layer->removeFromParent();
    
    if (!is_record_)
    {
        AM->stopAll();
    }
}

void Room::CheckUser(const std::vector<msg::CommonPlayerInfo>& infos){
    std::set<uint32_t> exist_users;
    for (auto& it: infos)
    {
        exist_users.insert(it.userinfo().userid());
        auto iter = get_user(it.userinfo().userid());
        if (iter == nullptr)
        {
            auto base = CreateUser(it.userinfo().userid());
            base->set_common_info(it);
            CheckUserHeadpic(base);
        }else{
            iter->set_common_info(it);
        }
    }
    
    for (auto iter = users_.begin(); iter != users_.end();) {
        if(exist_users.find(iter->first) == exist_users.end())
            iter = users_.erase(iter);
        else
            iter++;
    }
}

void Room::InitRecord(const msg::Processor_300_PokerGetHandHistory_DOWN& processor)
{
    history_.CopyFrom(processor);
    is_record_ = true;
    set_max_seat_num(processor.baseinfo().totalseatnum());
    set_dealer_position(processor.baseinfo().dealerpos());
    set_note(processor.baseinfo().tableremark());
    set_current_hand_id(processor.handid());
    InitRecord();
}

void Room::InitRecord()
{
    auto gamelayer = CreateLayer();
    SceneManager::add_layer(gamelayer);
    play_record();
}

void Room::add_owner_last_voice(uint32_t userid, std::string FileName, uint32_t time)
{
    auto iter = receive_last_voice_ids_.find(userid);
    if ( iter != receive_last_voice_ids_.end())
    {
        iter->second.first = FileName;
        iter->second.second = time;
    }
    else
    {
        receive_last_voice_ids_[userid] = std::make_pair(FileName, time);
    }
}
const std::string Room::get_last_voice_name(uint32_t userid)
{
    std::string voice_name = "";
    auto iter = receive_last_voice_ids_.find(userid);
    if(iter != receive_last_voice_ids_.end()) {
        voice_name = iter->second.first;
    }
    return voice_name;
}
const uint32_t Room::get_voice_user_id(const std::string& filename, int& length)
{
    uint32_t result = 0;
    auto iter = receive_voice_ids_.find(filename);
    if(iter != receive_voice_ids_.end()) {
        result = iter->second.first;
        length = iter->second.second;
        receive_voice_ids_.erase(filename);
    }
    
    return result;
}

GameLayer* Room::GetLayer(){
    GameLayer* layer = dynamic_cast<GameLayer*>(SceneManager::GetRoomLayer());
    if (layer && layer->get_roomid() == get_id() && layer->get_isrecord() == is_record_)
        return layer;
    else
        return nullptr;
}

void Room::ShowInGameScore(const msg::Processor_420_PokerGetScore_DOWN& processor)
{
    auto layer = GetLayer();
    if(layer)
        layer->ShowInGameScore(processor);
}

void Room::NotifyLayerPositionUpdate()
{
    auto layer = GetLayer();
    if(layer)
        layer->PositionUpdate();
}

void Room::receive_voice(uint32_t userid, std::string FileName, uint32_t time)
{
    receive_voice_ids_[FileName] = std::make_pair(userid, time);
    auto iter = receive_last_voice_ids_.find(userid);
    if ( iter != receive_last_voice_ids_.end())
    {
        iter->second.first = FileName;
        iter->second.second = time;
    }
    else
    {
        receive_last_voice_ids_[userid] = std::make_pair(FileName, time);
    }
    FM.download_file(FileName);
}

void Room::CheckUserHeadpic(std::shared_ptr<user_base> user)
{
    if(!GDM->is_file_exist(user->get_head_pic_name())) {
        RequestFile(user->get_head_pic_name());
    }
}

void Room::set_note(const std::string& value)
{
    roominfo_.set_tablenote(value);
    auto layer = GetLayer();
    if (layer)
        layer->receive_table_note(value);
}

void Room::game_start()
{
    roominfo_.set_isplaying(true);
    auto layer = GetLayer();
    if(layer)
        layer->receive_game_start();
}

void Room::game_end()
{
    roominfo_.set_isplaying(false);
    auto layer = GetLayer();
    if(layer)
        layer->receive_game_end();
}

void Room::update_seat_info(const msg::Processor_412_ReceiveSeatInfo& processor)
{
    for (int i = 0; i < processor.playersitstatus().size(); i++)
    {
        uint32_t user_id = processor.playersitstatus(i).info().userid();
        msg::SittingStatus status = processor.playersitstatus(i).status();
        int seat_number = processor.playersitstatus(i).sitnum();
        
        auto user = get_user(user_id);
        
        if(user)
        {
            user->set_status(status);
            user->set_seat_num(seat_number);
        }
        else
        {
            user = CreateUser(user_id);
            user->set_seat_num(seat_number);
            user->set_status(status);
            user->set_userinfo(processor.playersitstatus(i).info());
            CheckUserHeadpic(user);
        }
        
        switch (status) {
            case msg::SittingStatus::SITOUT:{
                break;
            }
            case msg::SittingStatus::OBSERVER:
                if ( user != NULL){
                    cocos2d::log("Room::update_seat_info case msg::SittingStatus::OBSERVER");
                    cocos2d::log("userid:%u usernickname:%s", user->get_id(), user->get_nickname().c_str());
                    user->clear();
                    remove_exist_user(user_id);
                    
                }
                break;
            case msg::SittingStatus::PLAYING:
            {
                break;
            }
            case msg::SittingStatus::SITDOWN:{
                break;
            }
            case msg::SittingStatus::WANTTOLEAVE:{//大菠萝 即将离开
                break;
            }
            default:
                break;
        }
    }
    NotifyLayerPositionUpdate();
}

void Room::update_game_status(const msg::GameStatus game_status)
{
    roominfo_.set_status(game_status);
    auto layer = GetLayer();
    if(layer)
        layer->update_game_status(game_status);
}

void Room::RequestFile(std::string FileName)
{
    RequestFiles.push_back(FileName);
}

void Room::UpdatePlayerAction(int userid, int countdown, bool is_using_timebank)
{
    auto layer = GetLayer();
    if(layer)
        layer->UpdatePlayerAction(userid, countdown, is_using_timebank);
}

#pragma mark replay
void Room::pre_step()
{
    if(current_step_ > 0) {
        current_step_--;
        ApplyStep(steps_.replaystep(current_step_-1));
    }
}

void Room::next_step()
{
    if(current_step_ < steps_.replaystep_size()) {
        current_step_++;
        ApplyStep(steps_.replaystep(current_step_-1));
    }
}

void Room::play_record()
{
    auto& recv = history_;
    
    is_record_ = true;
    uint32_t idx = 0;
    
    if (recv.handtype() == msg::HandType::Poker)
    {
        for (int i=0; i<recv.players().replayplayer_size(); i++) {
            std::shared_ptr<user_texas> user(new user_texas(recv.players().replayplayer(i).userdetailinfo().userid()));
            user->set_userinfo(recv.players().replayplayer(i).userdetailinfo());
            user->set_seat_num(recv.players().replayplayer(i).sitnum());
            user->set_status(msg::SittingStatus::PLAYING);
            users_[recv.players().replayplayer(i).userdetailinfo().userid()] = user;
            CheckUserHeadpic(user);
            idx++;
        }
    }else if (recv.handtype() == msg::HandType::OFC)
    {
        for (int i=0; i<recv.players().replayplayer_size(); i++) {
            std::shared_ptr<user_ofc> user(new user_ofc(recv.players().replayplayer(i).userdetailinfo().userid()));
            user->set_userinfo(recv.players().replayplayer(i).userdetailinfo());
            user->set_seat_num(recv.players().replayplayer(i).sitnum());
            user->set_status(msg::SittingStatus::PLAYING);
            user->get_properties()->set_actionstatus(msg::OFCActionStatus::WAITING);
            users_[recv.players().replayplayer(i).userdetailinfo().userid()] = user;
            CheckUserHeadpic(user);
            idx++;
        }
    }
    
    steps_.Clear();
    steps_.MergeFrom(recv.steps());
    
    current_step_ = 0;
    auto layer = GetLayer();
    if(layer)
        layer->show_history_layer();
    
    if (recv.handtype() == msg::HandType::OFC)
    {
        current_step_ = steps_.replaystep().size();
        ApplyStep(steps_.replaystep(steps_.replaystep().size()-1));
    }
}

void Room::stop_play_record()
{
    current_step_ = 0;
}

void PokerRoom::ApplyStep(const msg::ReplayStep& step)
{
    mergeupdate(step.propupdate());
    auto layer = GetPokerLayer();
    if (layer)
        layer->handle_update_property();
    
    if(current_step_ == steps_.replaystep_size()) {
        show_hand_result(history_.handresultinfo());
        auto layer = GetPokerLayer();
        if (layer)
            layer->play_history_finished();
    }
}

void PokerRoom::NoticeTournamentAddon(uint32_t buyin)
{
    auto layer = GetPokerLayer();
    if (layer)
        layer->handle_receive_poker_tournament_request_addon(buyin);
}

void PokerRoom::receive_choice(msg::ChoiceType type)
{
    auto layer = GetPokerLayer();
    if (layer)
        layer->set_btn_choice_visible(type);
}

void PokerRoom::show_hand_result(const msg::HandResultInfo& data)
{
    std::shared_ptr<msg::GameRecord> obj(new msg::GameRecord());
    
    obj->set_roomid(get_id());
    auto user_this_player = get_user(GDM->get_user_id());
    if(user_this_player != nullptr)  //这里有问题回放的时候取得是回放的player 不是游戏中的
    {
        for (int i : user_this_player->get_properties()->holecards())
            obj->add_holecard(i);
    }
    
    obj->set_handid(get_current_hand_id());
    
    uint32_t result = 0;
    for (auto& iter : data.pot()) {
        result += iter.potamount();
    }
    
    obj->set_poolsize(result);
    
    for (int i = 0; i < data.playerresult_size(); i++) {
        
        if(data.playerresult(i).playerwinchips() > 0){
            obj->add_winnername(data.playerresult(i).userdetailinfo().nickname());
            
        }
    }
    
    auto layer = GetPokerLayer();
    if (layer)
        layer->handle_game_hand_info(data);
    if (!get_is_record()) {
        PDM->AddRecord(obj);
    }
}

void PokerRoom::receive_auto_button_change(const msg::PokerPlayerSetting& setting)
{
    auto layer = GetPokerLayer();
    if (layer)
        layer->init_auto_buttons(setting);
}

const std::shared_ptr<user_texas> PokerRoom::get_user(const uint32_t id)
{
    auto iter = users_.find(id);
    if(iter != users_.end()) {
        return dynamic_pointer_cast<user_texas> (iter->second);
    }
    cocos2d::log("Warnning:user not found! The id is: %u", id);
    return nullptr;
}

void PokerRoom::UpdatePlayerAction(int userid, int countdown, bool is_using_timebank)
{
    actionrequest.mutable_actionrequest()->set_actionplayerid(userid);
    actionrequest.mutable_actionrequest()->set_countdown(countdown);
    actionrequest.mutable_actionrequest()->set_isusingtimebank(is_using_timebank);
    Room::UpdatePlayerAction(userid, countdown, is_using_timebank);
}

void PokerRoom::receive_poker_tournamentstatus(const msg::Processor_530_PokerGetTourmentStatus_DOWN& processor)
{
    auto layer = GetPokerLayer();
    if (layer)
        layer->receive_poker_tournamentstatus(processor);
}

void PokerRoom::mergeupdate(const msg::TableAndUserUpdate& update){
    const msg::TableInfo& TableInfo = update.tableinfo();
    get_properties().CopyFrom(TableInfo);
    for(auto& it: update.pokerplayerinfo()){
        auto user = get_user(it.playerinfo().userinfo().userid());
        
        cocos2d::log("PokerRoom::mergeupdate: user %u, %s, chip:%u", it.playerinfo().userinfo().userid(), it.playerinfo().userinfo().nickname().c_str(), it.chips());
        
        if(user != nullptr && user->get_properties()->actionstatus() != it.actionstatus())
        {
            switch (it.actionstatus())
            {
                case msg::ActionStatus::NORMAL:
                    break;
                case msg::ActionStatus::FOLD:
                {
                    if (user->get_properties()->playerinfo().sittingstatus() == msg::SittingStatus::PLAYING)
                    {
                        //AM->playEffect("sound/fold.mp3");
                        
                    }
                }
                    
                    break;
                case msg::ActionStatus::CHECK:
                    
                    AM->playEffect("sound/click.mp3");
                    break;
                case msg::ActionStatus::BET:
                    AM->playEffect("sound/bet.mp3");
                    break;
                case msg::ActionStatus::CALL:
                    
                    AM->playEffect("sound/bet.mp3");
                    break;
                case msg::ActionStatus::RAISE:
                    
                    AM->playEffect("sound/bet.mp3");
                    break;
                case msg::ActionStatus::ALLIN:
                    
                    //                    AM->playEffect("sound/all_in.mp3");
                    break;
                default:
                    break;
            }
        }
        if (user != nullptr)
        {
            user->get_properties()->CopyFrom(it);
        }
    }
    refresh();
}

const uint32_t PokerRoom::get_bet_max()
{
    uint32_t tableBetMax = 0;
    std::map<uint32_t, std::shared_ptr<user_base>> users = get_users();
    for (auto& iter : users) {
        auto user = static_pointer_cast<user_texas>(iter.second);
        if (user && user->get_status() == msg::SittingStatus::PLAYING) {
            int bet = user->get_properties()->betchips();
            if (bet > tableBetMax) {
                tableBetMax = bet;
            }
        }
    }
    return tableBetMax;
}

void PokerRoom::clear()
{
    properties_.Clear();
    for (auto& iter : users_)
    {
        int chips = static_pointer_cast<user_texas>(iter.second)->get_properties()->chips();
        iter.second->clear();
        static_pointer_cast<user_texas>(iter.second)->get_properties()->set_chips(chips);
        static_pointer_cast<user_texas>(iter.second)->get_properties()->set_actionstatus(msg::ActionStatus::NORMAL);
    }
}

GameLayer* PokerRoom::CreateLayer()
{
    auto layer = SceneManager::GetRoomLayer();
    if (layer)
        layer->removeFromParent();
    return PokerRoomLayer::Create(get_id(), is_record_);
}

PokerRoomLayer* PokerRoom::GetPokerLayer(){
    auto layer = GetLayer();
    if (layer)
        return static_cast<PokerRoomLayer*>(layer);
    else
        return nullptr;
}

void PokerRoom::handle_player_operation(const msg::PokerActionRequest& data){
    actionrequest.CopyFrom(data);
    auto layer = GetPokerLayer();
    if (layer)
        layer->handle_player_operation(data);
}

void PokerRoom::game_end()
{
    actionrequest.Clear();
    Room::game_end();
}

void PokerRoom::refresh()
{
    auto layer = GetPokerLayer();
    if (layer)
        layer->handle_update_property();
}

void PokerRoom::Init(const msg::Processor_500_PokerRefreshAll& processor)
{
    is_record_ = false;
    set_common_roominfo(processor.commonroominfo());
    set_buyin_max(processor.maxbuy());
    set_buyin_min(processor.minbuy());
    set_buy_min_unit(processor.buyunit());
    set_is_tournament(processor.istournament());
    
    PDM->read_records(get_id());
    
    std::vector<msg::CommonPlayerInfo> infos;
    for (auto& it:processor.propupdate().pokerplayerinfo())
        infos.push_back(it.playerinfo());
    CheckUser(infos);
    
    get_properties().CopyFrom(processor.propupdate().tableinfo());
    for (auto& it:processor.propupdate().pokerplayerinfo()){
        auto iter = users_.find(it.playerinfo().userinfo().userid());
        if(iter != users_.end()) {
            static_pointer_cast<user_texas>(iter->second)->get_properties()->CopyFrom(it);
        }
    }
    if (processor.has_actionrequest())
        actionrequest.CopyFrom(processor.actionrequest());
    else
        actionrequest.Clear();
    showcard.clear();
    for (auto & value: processor.showcard())
        showcard.push_back(value);
    Init();
}

void PokerRoom::Init()
{
    auto gamelayer = CreateLayer();
    SceneManager::add_layer(gamelayer);
    
    auto pokerlayer = GetPokerLayer();
    if (pokerlayer){
        pokerlayer->clear(0);
        
        if (actionrequest.has_actionrequest()){
            pokerlayer->set_is_playing(true);
            pokerlayer->handle_player_operation(actionrequest);
        }
        
        pokerlayer->handle_update_property();
        pokerlayer->update_game_status(get_gamestatus());
        
        
        //by lvzhangle --20160317
        //this code have problem!
        //And will make dont't run next code
        //result : you can not find success log, but app do not breakdown!
        auto user = get_user(GDM->get_user_id());
        if (user)
            pokerlayer->handle_showcard(showcard);
    }
}

const std::shared_ptr<user_base>& PokerRoom::CreateUser(uint32_t userid)
{
    std::shared_ptr<user_texas> user(new user_texas(userid));
    users_.insert(std::make_pair(userid, user));
    return users_.find(userid)->second;
}

const std::shared_ptr<user_base>& OFCRoom::CreateUser(uint32_t userid)
{
    std::shared_ptr<user_ofc> user(new user_ofc(userid));
    users_.insert(std::make_pair(userid, user));
    return users_.find(userid)->second;
}

void OFCRoom::receive_auto_button_change(const msg::OFCPlayerSetting& setting)
{
    auto layer = GetOFCLayer();
    if (layer)
        layer->init_auto_buttons(setting);
}

newOfcPokerRoom* OFCRoom::GetOFCLayer(){
    auto layer = GetLayer();
    if (layer)
        return static_cast<newOfcPokerRoom*>(layer);
    else
        return nullptr;
}

void OFCRoom::Init(const msg::Processor_600_OFCRefreshAll& processor)
{
    is_record_ = false;
    set_common_roominfo(processor.commonroominfo());
    
    std::vector<msg::CommonPlayerInfo> infos;
    for (auto& it:processor.propupdate().playerinfo())
        infos.push_back(it.playerinfo());
    CheckUser(infos);
    
    action_count_down_map.clear();
    for (int i = 0; i < processor.propupdate().playerinfo().size(); i++)
    {
        uint32_t iter_player_id = processor.propupdate().playerinfo(i).playerinfo().userinfo().userid();
        auto iter = users_.find(iter_player_id);
        auto user = static_pointer_cast<user_ofc>(iter->second);
        user->set_ofc_playerinfo(processor.propupdate().playerinfo(i));
        user->set_is_ofc_hand_card_change(true);
        
        if (processor.propupdate().playerinfo(i).has_actionrequest())
        {
            std::shared_ptr<msg::ActionRequest> request(new msg::ActionRequest);
            request->CopyFrom(processor.propupdate().playerinfo(i).actionrequest());
            action_count_down_map[iter_player_id] = request;
        }
    }
    Init();
}

void OFCRoom::Init()
{
    SceneManager::add_layer(CreateLayer());
    auto ofc_room_layer = GetOFCLayer();
    
    ofc_room_layer->update_record_status();
    
    ofc_room_layer->PositionUpdate(true);
    
    ofc_room_layer->receive_ofc_prop_update(true);
    
    ofc_room_layer->handle_update_dealer();
    
    for (auto & action_iter : action_count_down_map) {
        ofc_room_layer->UpdatePlayerAction(action_iter.first, action_iter.second->countdown(), action_iter.second->isusingtimebank());
    }
    
    ofc_room_layer->update_game_status(get_gamestatus());
    
    ofc_room_layer->receive_table_note(get_note());
}

void OFCRoom::UpdatePlayerAction(int userid, int countdown, bool is_using_timebank)
{
    std::shared_ptr<msg::ActionRequest> request(new msg::ActionRequest);
    request->set_actionplayerid(userid);
    request->set_countdown(countdown);
    request->set_isusingtimebank(is_using_timebank);
    action_count_down_map[userid] = request;
    Room::UpdatePlayerAction(userid, countdown, is_using_timebank);
}

void OFCRoom::update_ofc_prop(const msg::OFCPropUpdate & update)
{
    for (int i = 0; i < update.playerinfo().size(); i++)
    {
        auto user = get_user(update.playerinfo(i).playerinfo().userinfo().userid());
        if (user != nullptr)
            user->set_ofc_playerinfo(update.playerinfo(i));
    }
    
    auto layer = GetOFCLayer();
    if (layer)
        layer->receive_ofc_prop_update(false);
}

void OFCRoom::update_ofc_result(const msg::OFCResult & update_result)
{
    for (int i = 0; i < update_result.ofcplayerhandresult().size(); i++)
    {
        uint32_t player_id = update_result.ofcplayerhandresult(i).playerid();
        uint32_t step_score = update_result.ofcplayerhandresult(i).score();
        
        auto user = get_user(player_id);
        if(user != nullptr) {
            user->set_step_score(step_score);
        }
    }
    auto layer = GetOFCLayer();
    if (layer)
        layer->receive_ofc_result_score();
}

void OFCRoom::update_ofc_send_player_success(bool isSuccess)
{
    auto layer = GetOFCLayer();
    if (layer)
        layer->update_ofc_send_player_cards_success(isSuccess);
}

void OFCRoom::game_end()
{
    last_hand_id_ = get_current_hand_id();
    Room::game_end();
    auto layer = GetOFCLayer();
    if (layer){
        layer->receive_game_end();
        layer->update_record_status();
    }
}

void OFCRoom::clear()
{
    for (auto& iter : users_) {
        iter.second->clear();
    }
}

GameLayer* OFCRoom::CreateLayer()
{
    auto layer = SceneManager::GetRoomLayer();
    if (layer)
        layer->removeFromParent();
    return newOfcPokerRoom::Create(get_id(), is_record_);
}

void OFCRoom::ApplyStep(const msg::ReplayStep& step)
{
    update_ofc_prop(step.ofcpropupdate());
    
    if(current_step_ == steps_.replaystep_size()) {
        update_ofc_result(history_.ofcresult());
        auto layer = GetOFCLayer();
        if (layer)
            layer->receive_ofc_result_score();
    }
}

const std::shared_ptr<user_ofc> OFCRoom::get_user(const uint32_t id)
{
    auto iter = users_.find(id);
    if(iter != users_.end()) {
        return dynamic_pointer_cast<user_ofc> (iter->second);
    }
    cocos2d::log("Warnning:user not found! The id is: %u", id);
    return nullptr;
}

void MahjongRoom::clear(){
    for (auto& iter : users_) {
        iter.second->clear();
    }
}

const std::shared_ptr<user_mahjong> MahjongRoom::get_user(const uint32_t id){
    
    auto iter = users_.find(id);
    if(iter != users_.end()) {
        return dynamic_pointer_cast<user_mahjong> (iter->second);
    }
    return nullptr;
}

const std::shared_ptr<user_base>& MahjongRoom::CreateUser(uint32_t userid){
    std::shared_ptr<user_mahjong> user(new user_mahjong(userid));
    users_.insert(std::make_pair(userid, user));
    return users_.find(userid)->second;
}

void MahjongRoom::Init(const msg::Processor_700_MahjongRefreshAll& processor){
    is_record_ = false;
    set_common_roominfo(processor.commonroominfo());
    update_mahjong_prop(processor.propupdate());
    std::vector<msg::CommonPlayerInfo> infos;
    for (auto& it:processor.propupdate().playerinfo())
        infos.push_back(it.playerinfo());
    CheckUser(infos);
    
    for (int i = 0; i < processor.propupdate().playerinfo().size(); i++)
    {
        uint32_t iter_player_id = processor.propupdate().playerinfo(i).playerinfo().userinfo().userid();
        auto iter = users_.find(iter_player_id);
        auto user = static_pointer_cast<user_mahjong>(iter->second);
        user->get_properties()->CopyFrom(processor.propupdate().playerinfo(i));
    }
    Init();
}

void MahjongRoom::Init(){
    SceneManager::add_layer(CreateLayer());
    auto layer = GetMahjongLayer();
    layer->PositionUpdate();
    layer->prop_update();
    layer->update_game_status(get_gamestatus());
    layer->receive_table_note(get_note());
}

void MahjongRoom::update_mahjong_prop(const msg::MahjongPropUpdate & update){
    if(!users_.empty())
    {
        for (int i = 0; i < update.playerinfo().size(); i++)
        {
            uint32_t iter_player_id = update.playerinfo(i).playerinfo().userinfo().userid();
            auto iter = users_.find(iter_player_id);
            auto user = static_pointer_cast<user_mahjong>(iter->second);
            user->get_properties()->CopyFrom(update.playerinfo(i));
        }
    }
    this->update_mahjong_update(update);
    auto layer = GetMahjongLayer();
    if (layer)
        layer->prop_update();
}

void MahjongRoom::update_mahjong_update(const msg::MahjongPropUpdate & update)
{
    prop_update_.MergeFrom(update);
    prop_update_.clear_playerinfo();
}

GameLayer* MahjongRoom::CreateLayer(){
    auto layer = SceneManager::GetRoomLayer();
    if (layer)
        layer->removeFromParent();
    return MahjongRoomLayer::Create(get_id(), is_record_);
}

void MahjongRoom::receive_auto_button_change(const msg::MahjongPlayerSetting& setting){
    auto layer = GetMahjongLayer();
    if (layer)
        layer->init_auto_buttons(setting);
}

void MahjongRoom::ApplyStep(const msg::ReplayStep& step)
{
    update_mahjong_prop(step.mahjongupdate());
}

MahjongRoomLayer* MahjongRoom::GetMahjongLayer(){
    auto layer = GetLayer();
    if (layer)
        return static_cast<MahjongRoomLayer*>(layer);
    else
        return nullptr;
}

void MahjongRoom::receive_player_action(const msg::MahjongAction& action){
    auto layer = GetMahjongLayer();
    if (layer)
        layer->receive_player_action(action);
}

void MahjongRoom::receive_switch_type(const msg::MahjongCardSwitchType& switchtype){
    auto layer = GetMahjongLayer();
    if (layer)
        layer->receive_switch_type(switchtype);
}
void MahjongRoom::game_end()
{
    Room::game_end();
//    last_hand_id_ = get_current_hand_id();
    auto layer = GetMahjongLayer();
    if (layer){
        layer->receive_game_end();
//        layer->update_record_status();
    }
}
