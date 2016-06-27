#pragma once
#include <map>
#include <inttypes.h>
#include <msg.pb.h>

class TexasRoom {
public:
    TexasRoom(const uint32_t id);
    
    //获取庄家座位号
    const uint32_t get_dealer_position();
    //底池计分牌
    const uint32_t get_chip_pool();
    
    const uint32_t get_hand_id() {return hand_id_;}
    const bool get_is_tournament() {return is_tournament_;};
    const uint32_t get_id(){ return id_;}
    const uint32_t get_buyin_min() { return buyin_min_;}
    const uint32_t get_buyin_max() { return buyin_max_;}
    const uint32_t get_buy_min_unit() { return buy_min_unit_;}
    const uint8_t get_seat_num() { return seat_num_;}
    const std::string& get_note(){ return note_;}
    msg::TableInfo& get_properties() { return properties_;}
    msg::GroupPermission get_user_group_permission() { return user_group_permission_;}
    
    void set_note(const std::string note){ note_ = note;}
    void set_dealer_position(const uint32_t dealer){ dealer_ = dealer;}
    void set_id(const uint32_t id) { id_ = id;}
    void set_chip_pool(const int amount);
    void set_buyin_min(const uint32_t buyin_min) { buyin_min_ = buyin_min;}
    void set_buyin_max(const uint32_t buyin_max) { buyin_max_ = buyin_max;}
    void set_buy_min_unit(const uint32_t buy_min_unit) { buy_min_unit_ = buy_min_unit;}
    void set_seat_num(const uint8_t seat_num) { seat_num_ = seat_num;}
    void set_is_tournament(const bool is_tournament) {is_tournament_ = is_tournament;};
    void set_user_group_permission(msg::GroupPermission & user_group_permission) {user_group_permission_ = user_group_permission;};
    void set_hand_id(const uint32_t hand_id) {hand_id_ = hand_id;}
    
    void clear();
    
private:
    uint32_t id_;
    uint8_t seat_num_;
    uint32_t buyin_min_;
    uint32_t buyin_max_;
    uint32_t buy_min_unit_;
    msg::TableInfo properties_;
    std::string note_;
    uint32_t dealer_;
    bool is_tournament_;
    msg::GroupPermission user_group_permission_;
    uint32_t hand_id_;
};

//liujia -- ofc
class OFCRoom {
public:
    OFCRoom(const uint32_t id);
    
    const uint32_t get_id(){ return id_;}
    const uint8_t get_seat_num() { return seat_num_;}
    const std::string& get_note(){ return note_;}
    const uint32_t get_hand_id() {return hand_id_;}
    
    void set_id(const uint32_t id) { id_ = id;}
    void set_seat_num(const uint8_t seat_num) { seat_num_ = seat_num;}
    void set_note(const std::string note) { note_ = note;}
    void set_hand_id(const uint32_t hand_id) { hand_id_ = hand_id;}

    //
    const std::map<uint32_t, int>& get_ofc_properties() { return ofc_properties_;}
    void update_ofc_property(const uint32_t id, const int32_t value) {ofc_properties_[id] = value;}
    
    //
    void set_operate_player_id(const uint32_t operate_player_id) { operate_player_id_ = operate_player_id;}
    const uint32_t get_operate_player_id(){ return operate_player_id_;}
    
    //
    void set_operate_time(const uint32_t operate_time) { operate_time_ = operate_time;}
    const uint32_t get_operate_time(){ return operate_time_;}
    
    void set_isfantasy(const bool fantasy_value){isfantasy = fantasy_value;}
    const bool get_isfantasy(){return isfantasy;}
    
    void clear();
    
    void set_dealer(uint32_t dealer) {dealer_ = dealer;}
    const uint32_t get_dealer() {return dealer_;}
    
    void set_max_seat_num(uint32_t max_seat_num) {max_seat_num_ = max_seat_num;}
    const uint32_t get_max_seat_num() {return max_seat_num_;}
    
private:
    uint32_t id_;
    uint8_t seat_num_;
    
    uint32_t operate_player_id_;
    uint32_t operate_time_;
    
    bool isfantasy;
    
    std::string note_;
    std::map<uint32_t, int> ofc_properties_;
    
    
     uint32_t dealer_;
    uint32_t max_seat_num_;
    
    uint32_t hand_id_;
};
