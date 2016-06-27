//
//  friend_data.cpp
//  pokerclient
//
//  Created by ll on 15/8/18.
//
//
#include <vector>

#include "friend_data.h"

friend_data::friend_data(const uint32_t userid,
                         const std::string nick_name,
                         const std::string pic_name,
                         const bool isonline,
                         const uint32_t cell_type)
: userid_(userid)
, nick_name_(nick_name)
, isonline_(isonline)
, cell_type_(cell_type)
{
    std::string player_pic_name = pic_name;
    std::string::size_type idx = player_pic_name.rfind(".");
    if (idx == std::string::npos){
        player_pic_name += ".png";
    }
    set_pic_name(player_pic_name);
}

