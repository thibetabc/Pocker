//
//  group_data.h
//  pokerclient
//
//  Created by duanmt on 15/5/11.
//
//

#ifndef __pokerclient__group_data__
#define __pokerclient__group_data__

#include <string>
#include "msg.pb.h"

class group_data
{
public:
    group_data(const uint32_t id,
               const uint32_t owner_id,
               const uint32_t player_counts,
               const std::string name,
               const std::string note,
               const std::string pic,
               const bool is_verify,
               const bool is_member,
               const msg::GroupType group_type
               );
    
    void set_id(const uint32_t id) { id_ = id;}
    void set_owner_id(const uint32_t owner_id) { owner_id_ = owner_id;}
    void set_player_counts(const uint32_t player_counts) { player_counts_ = player_counts;}
    //void set_password(const std::string& password) { password_ = password;}
    void set_name(const std::string& name) { name_ = name;}
    void set_note(const std::string& note) { note_ = note;}
    void set_pic(const std::string& pic) { pic_ = pic;}
    //void set_verify_every_time(const uint8_t verify_every_time) { verify_every_time_ = verify_every_time;}
    void set_hidden(const uint8_t hidden) { hidden_ = hidden;}
    void set_member_ids(const std::vector<uint32_t>& member_ids) { member_ids_ = member_ids;}
    void set_is_member(const bool is_member) { is_member_ = is_member;}
    void set_group_type(msg::GroupType group_type) {group_type_ = group_type;}
    const uint32_t get_id() { return id_;}
    const uint32_t get_owner_id() { return owner_id_;}
    //const std::string& get_password() { return password_;}
    const uint32_t get_player_counts();
    const std::string& get_name() { return name_;}
    const std::string& get_note() { return note_;}
    const std::string& get_pic() { return pic_;}
    //const uint8_t get_verify_every_time() { return verify_every_time_;}
    const uint8_t get_hidden() { return hidden_;}
    //const bool get_is_verify() { return is_verify_;}
    const bool get_is_member() { return is_member_;}
    const std::vector<uint32_t>& get_member_ids() { return member_ids_;}
    const msg::GroupType get_group_type() {return group_type_;}
    
    void add_one_to_member(const uint32_t member_id);
    void erase_one_member(const uint32_t member_id);
    
private:
    uint32_t id_;
    uint32_t owner_id_;
    uint32_t player_counts_;
    //std::string password_;
    std::string name_;
    std::string note_;
    std::string pic_;
    uint8_t verify_every_time_;
    uint8_t hidden_;
    bool is_verify_;
    bool is_member_;
    msg::GroupType group_type_;
    
    std::vector<uint32_t> member_ids_;
};

#endif /* defined(__pokerclient__group_data__) */
