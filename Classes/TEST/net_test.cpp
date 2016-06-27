//
//  net_test.cpp
//  pokerclient
//
//  Created by MirrorMiko on 15/5/6.
//
//

#include "net_test.h"
#include "events.h"
#include "cmd_def.h"
#include "net_data_send.h"
#include "net_data_recv.h"
#include "net_manager.h"
#include "game_manager.h"
#include "NET_CMD.h"


net_test::net_test(){
    
//    _dispatch.register_event(EVENT_NET_CONNECT_SUCESS, BIND_FUNC(this, net_test::connect_sucess));
//    _dispatch.register_event(EVENT_NET_CONNECT_FAILED, BIND_FUNC(this, net_test::connect_failed));
//    _dispatch.register_event(EVENT_NET_SEND_SUCESS, BIND_FUNC(this, net_test::send_sucess));
//    _dispatch.register_event(EVENT_NET_SEND_FAILED, BIND_FUNC(this, net_test::send_failed));
//    _dispatch.register_event(EVENT_NET_RECV_FAILED, BIND_FUNC(this, net_test::recv_failed));
//    _dispatch.register_event(EVENT_NET_RECV_DATA, BIND_FUNC(net_test::recv_data));
}

void net_test::connect_sucess(cmd_data_pointer data){
    printf("connect_sucess\n");
    go_to_register();
}

void net_test::connect_failed(cmd_data_pointer data){
    cmd_data_int& code = dynamic_cast<cmd_data_int&>(*data);
    printf("connect_failed:%d\n", code.get_value());
}

void net_test::send_sucess(cmd_data_pointer data){
    printf("send_sucess\n");
}

void net_test::send_failed(cmd_data_pointer data){
    cmd_data_int& code = dynamic_cast<cmd_data_int&>(*data);
    printf("send_failed:%d\n", code.get_value());
}

void net_test::recv_failed(cmd_data_pointer data){
    cmd_data_int& code = dynamic_cast<cmd_data_int&>(*data);
    printf("recv_failed:%d\n", code.get_value());
}

void net_test::recv_data(cmd_data_pointer data){
//    net_data_recv_package& package = dynamic_cast<net_data_recv_package&>(*data);
//    printf("recv_data\n");
}

#pragma mark --
void net_test::go_to_connect(){
//    net_manager::shared_instence().connect("115.28.90.6", 20000);
}

void net_test::go_to_register(){
//    data_send_login l;
//    l.username = "mirrormiko";
//    l.password = "123456";
//    l.send();
}
