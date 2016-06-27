//
//  game_net_data.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/7.
//
//

#ifndef __pokerclient__game_net_data__
#define __pokerclient__game_net_data__

#include <vector>
#include <stdio.h>
#include "net_data.h"
#include "NET_CMD.h"
#include "message_block.hpp"

#pragma mark game_net_dat --

#pragma pack(push, 1)

class game_net_data : public cmd_data {
public:
    game_net_data(uint16_t code):request_code(code){}
    inline uint16_t code() { return request_code; }
    bool is_valid() { return true; }
protected:
    uint16_t request_code;
};

#pragma pack(pop)

#endif /* defined(__pokerclient__game_net_data__) */
