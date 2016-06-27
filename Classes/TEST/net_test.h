//
//  net_test.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/6.
//
//

#ifndef __pokerclient__net_test__
#define __pokerclient__net_test__

#include <stdio.h>
#include "events.h"

class net_test {
public:
    net_test();
    void go_to_connect();
    void go_to_register();
public:
    //call back
    void connect_sucess(cmd_data_pointer data);
    void connect_failed(cmd_data_pointer data);
    void send_sucess(cmd_data_pointer data);
    void send_failed(cmd_data_pointer data);
    void recv_failed(cmd_data_pointer data);
    void recv_data(cmd_data_pointer data);
protected:
    events::event_dispatch _dispatch;
};

#endif /* defined(__pokerclient__net_test__) */
