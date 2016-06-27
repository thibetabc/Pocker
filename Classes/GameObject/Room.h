//
//  Room.h
//  pokerclient
//
//  Created by admin on 15-12-22.
//
//

#ifndef __pokerclient__Room__
#define __pokerclient__Room__

#include <stdio.h>
#include "msg.pb.h"
#include "PokerRoomLayer.h"
#include "newOfcPokerRoom.h"
#include "MahjongRoomLayer.h"
class user_base;
class user_texas;
class user_ofc;
class PokerRoomLayer;
class newOfcPokerRoom;
class MahjongRoomLayer;

class Room{
public:
    const bool get_is_record() { return is_record_; }   // if this is replaying the games.
    void InitRecord(const msg::Processor_300_PokerGetHandHistory_DOWN& processor);
    void InitRecord();
    virtual void Init() = 0;
    const msg::RoomType get_room_type() {return roominfo_.roomtype();}
    const uint32_t get_current_step() { return current_step_;}
    const msg::ReplaySteps& GetReplaySteps() {return steps_;};
    const bool get_is_tournament() {return is_tournament_;};
    const uint32_t get_id(){ return roominfo_.roomid();}
    const msg::GameStatus get_gamestatus(){ return roominfo_.status();}
    const uint32_t get_current_hand_id(){ return roominfo_.handid();}
    const uint32_t get_group_id(){ return roominfo_.groupid();}
    const uint32_t get_currentuser_seat_num();
    const std::map<uint32_t, std::shared_ptr<user_base>>& get_users(){return users_;}
    const std::string& get_note(){ return roominfo_.tablenote();}
    const uint32_t get_owner_id() {return roominfo_.ownerid();};
    const uint32_t get_dealer_position() {return roominfo_.dealerpos();}
    const uint32_t get_max_seat_num() {return roominfo_.maxseatnum();}
    const bool get_is_playing(){return roominfo_.isplaying();}

    void set_id(uint32_t value){ roominfo_.set_roomid(value);}
    void set_dealer_position(uint32_t value){ roominfo_.set_dealerpos(value);}
    void set_max_seat_num(uint32_t value){ roominfo_.set_maxseatnum(value);}
    void set_note(const std::string& value);
    void set_current_hand_id(uint32_t value) {roominfo_.set_handid(value);}
    void set_is_tournament(const bool is_tournament) {is_tournament_ = is_tournament;}
    void set_common_roominfo(const msg::CommonRoomInfo& value) {roominfo_.CopyFrom(value);}
    virtual void clear() = 0;
    virtual void UpdatePlayerAction(int userid, int countdown, bool is_using_timebank = false);
    void update_seat_info(const msg::Processor_412_ReceiveSeatInfo& processor);
    void update_game_status(const msg::GameStatus game_status);
    void ShowInGameScore(const msg::Processor_420_PokerGetScore_DOWN& processor);
    virtual GameLayer* CreateLayer() = 0;
#pragma mark replay
    void stop_play_record();
    void pre_step();
    void next_step();
    void play_record();
    void set_current_step(const uint32_t step) { current_step_ = step;}
    std::shared_ptr<user_base> get_user(uint32_t user_id)
    {
        if(users_.find(user_id) != users_.end())
            return users_.find(user_id)->second;
        else return NULL;
    }
    void remove_exist_user(uint32_t user_id)
    {
        if(users_.find(user_id) != users_.end())
        {
            std::map<uint32_t, std::shared_ptr<user_base>>::iterator iter_remove;
            for ( iter_remove = users_.begin() ; iter_remove != users_.end(); ++iter_remove)
            {
                if ( iter_remove->first == user_id)
                {
                    break;
                }
                
            }
            users_.erase(iter_remove);
        }
    }
    void receive_voice(uint32_t userid, std::string FileName, uint32_t time);
    void add_owner_last_voice(uint32_t userid, std::string FileName, uint32_t time);
    
    const uint32_t get_voice_user_id(const std::string& filename, int& length);
    const std::string get_last_voice_name(uint32_t userid);
    virtual const std::shared_ptr<user_base>& CreateUser(uint32_t userid) = 0;
    void game_start();
    virtual void game_end();
    void close();
protected:
    void CheckUser(const std::vector<msg::CommonPlayerInfo>& infos);
    virtual void ApplyStep(const msg::ReplayStep& step) = 0;
    void NotifyLayerPositionUpdate();
    GameLayer* GetLayer();
    void CheckUserHeadpic(std::shared_ptr<user_base> user);
    void RequestFile(std::string FileName);
    std::map<std::string, std::pair<uint32_t, uint32_t> > receive_voice_ids_;
    std::map<uint32_t, std::pair<std::string,uint32_t> > receive_last_voice_ids_;
    std::vector<std::string> RequestFiles;
    msg::Processor_300_PokerGetHandHistory_DOWN history_;
    msg::CommonRoomInfo roominfo_;
    uint32_t current_step_;
    bool is_tournament_;
    msg::ReplaySteps steps_;//牌局步骤记录
    bool is_record_ = false;  //是否播放牌局记录
    std::map<uint32_t, std::shared_ptr<user_base>> users_;
};

class PokerRoom : public Room{
public:
    
    //底池计分牌
    const uint32_t get_chip_pool() {return properties_.chips();}
    const uint32_t get_bet_max();
    const uint32_t get_big_blind() {return properties_.bigblind();}
    const uint32_t get_buyin_min() { return buyin_min_;}
    const uint32_t get_buyin_max() { return buyin_max_;}
    const uint32_t get_buy_min_unit() { return buy_min_unit_;}
    msg::TableInfo& get_properties() { return properties_;}
    msg::GroupPermission get_user_group_permission() { return user_group_permission_;}
    
    void set_chip_pool(const int amount) {properties_.set_chips(amount);}
    void set_buyin_min(const uint32_t buyin_min) { buyin_min_ = buyin_min;}
    void set_buyin_max(const uint32_t buyin_max) { buyin_max_ = buyin_max;}
    void set_buy_min_unit(const uint32_t buy_min_unit) { buy_min_unit_ = buy_min_unit;}
    void set_user_group_permission(msg::GroupPermission & user_group_permission) {user_group_permission_ = user_group_permission;};
    
    void clear();
    const std::shared_ptr<user_texas> get_user(const uint32_t id);
    void show_hand_result(const msg::HandResultInfo& data);
    const std::shared_ptr<user_base>& CreateUser(uint32_t userid);
    void handle_player_operation(const msg::PokerActionRequest& data);
    void set_show_card(std::vector<bool> value) {showcard = value;}
    void mergeupdate(const msg::TableAndUserUpdate& update);
    void refresh();
    void Init(const msg::Processor_500_PokerRefreshAll& processor);
    void Init();
    void NoticeTournamentAddon(uint32_t buyin);
    void receive_choice(msg::ChoiceType type);
    void receive_auto_button_change(const msg::PokerPlayerSetting& setting);
    void UpdatePlayerAction(int userid, int countdown, bool is_using_timebank = false);
    void receive_poker_tournamentstatus(const msg::Processor_530_PokerGetTourmentStatus_DOWN& processor);
    GameLayer* CreateLayer();
    void game_end();
protected:
    void ApplyStep(const msg::ReplayStep& step);
private:
    PokerRoomLayer* GetPokerLayer();
    msg::PokerActionRequest actionrequest;
    std::vector<bool> showcard;
    uint32_t buyin_min_ = 0;
    uint32_t buyin_max_ = 0;
    uint32_t buy_min_unit_;
    msg::TableInfo properties_;
    msg::GroupPermission user_group_permission_;
};

class OFCRoom : public Room{
public:
    void set_last_hand_id(uint32_t last_hand_id){last_hand_id_ = last_hand_id;};
    uint32_t get_last_hand_id(){return  last_hand_id_;};
    void set_isfantasy(const bool fantasy_value){isfantasy = fantasy_value;}
    const bool get_isfantasy(){return isfantasy;}
    void clear();
    const std::shared_ptr<user_ofc> get_user(const uint32_t id);
    const std::shared_ptr<user_base>& CreateUser(uint32_t userid);
    
    void UpdatePlayerAction(int userid, int countdown, bool is_using_timebank = false);
    void Init(const msg::Processor_600_OFCRefreshAll& processor);
    void Init();
    void update_ofc_prop(const msg::OFCPropUpdate & update);
    void update_ofc_result(const msg::OFCResult & update_result);
    void update_ofc_send_player_success(bool isSuccess);
    GameLayer* CreateLayer();
    void receive_auto_button_change(const msg::OFCPlayerSetting& setting);
    void game_end();
protected:
    void ApplyStep(const msg::ReplayStep& step);
private:
    newOfcPokerRoom* GetOFCLayer();
    std::map<int, std::shared_ptr<msg::ActionRequest>> action_count_down_map;
    bool isfantasy;
    uint32_t last_hand_id_ = 0;
};

class MahjongRoom : public Room{
public:
    void clear();
    const std::shared_ptr<user_mahjong> get_user(const uint32_t id);
    const std::shared_ptr<user_base>& CreateUser(uint32_t userid);
    void Init(const msg::Processor_700_MahjongRefreshAll& processor);
    void Init();
    void update_mahjong_prop(const msg::MahjongPropUpdate & update);
    void update_mahjong_update(const msg::MahjongPropUpdate & update);
    msg::MahjongPropUpdate& get_mahjong_prop_update(){ return prop_update_;};
    GameLayer* CreateLayer();
    void receive_auto_button_change(const msg::MahjongPlayerSetting& setting);
    
    void receive_player_action(const msg::MahjongAction& action);
    void receive_switch_type(const msg::MahjongCardSwitchType& switchtype);
    void game_end();
protected:
    void ApplyStep(const msg::ReplayStep& step);
private:
    MahjongRoomLayer* GetMahjongLayer();
    msg::MahjongPropUpdate prop_update_;
};

#endif /* defined(__pokerclient__Room__) */
