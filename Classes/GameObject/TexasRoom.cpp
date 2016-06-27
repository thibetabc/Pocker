#include "TexasRoom.h"
#include "cmd_def.h"

TexasRoom::TexasRoom(const uint32_t id)
: id_(id)
, buyin_min_(0)
, buyin_max_(0)
, seat_num_(0)
{
    hand_id_ = 0;
}
OFCRoom::OFCRoom(const uint32_t id)
: id_(id)
, seat_num_(0)
{
    hand_id_ = 0;
}

//获取庄家座位号
const uint32_t TexasRoom::get_dealer_position()
{
    return dealer_;
}
//底池计分牌
const uint32_t TexasRoom::get_chip_pool()
{
    return properties_.chips();
}

void TexasRoom::set_chip_pool(const int amount)
{
    properties_.set_chips(amount);
}

void TexasRoom::clear()
{
    properties_.Clear();
}