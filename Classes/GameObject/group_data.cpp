//
//  group_data.cpp
//  pokerclient
//
//  Created by duanmt on 15/5/11.
//
//
#include <vector>

#include "group_data.h"


group_data::group_data(const uint32_t id,
                       const uint32_t owner_id,
                       const uint32_t player_counts,
                       const std::string name,
                       const std::string note,
                       const std::string pic,
                       const bool is_verify,
                       const bool is_member,
                       const msg::GroupType group_type
                       )
: id_(id)
, owner_id_(owner_id)
, player_counts_(player_counts)
, name_(name)
, note_(note)
, is_verify_(is_verify)
, is_member_(is_member)
, group_type_(group_type)
{
    std::string group_pic = pic;
    std::string::size_type idx = group_pic.rfind(".");
    if (idx == std::string::npos) {
        group_pic += ".png";
    }
    set_pic(group_pic);
}

void group_data::add_one_to_member(const uint32_t member_id)
{
    bool exist = false;
    for (auto& iter : member_ids_) {
        if (iter == member_id) {
            exist = true;
            break;
        }
    }
    if(!exist) {
        member_ids_.push_back(member_id);
    }
}
void group_data::erase_one_member(const uint32_t member_id)
{
    for (auto iter = member_ids_.begin(); iter != member_ids_.end(); ++iter) {
        if(*iter == member_id) {
            member_ids_.erase(iter);
            break;
        }
    }
}

const uint32_t group_data::get_player_counts()
{
    if(!member_ids_.empty())
        player_counts_ = member_ids_.size();
    return player_counts_;
}
