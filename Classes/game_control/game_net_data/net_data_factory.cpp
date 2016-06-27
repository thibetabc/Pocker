//
//  net_data_factory.cpp
//  pokerclient
//
//  Created by MirrorMiko on 15/5/7.
//
//

#include "net_data_factory.h"
#include "events.h"
#include "NET_CMD.h"
#include "utils.h"

#define BIND_FUNC_ONE(func) std::bind(&func, this, std::placeholders::_1)
void net_data_factory::recv_data(cmd_data_pointer data){
    net_data_recv_package& package = dynamic_cast<net_data_recv_package&>(*data);
    try {
        events::global_dispatch.raise_event(package.packagehead.request_code, data);
    } catch (std::exception ex) {
    
    }
}
