//
//  net_manager.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//

#ifndef __pokerclient__net_manager__
#define __pokerclient__net_manager__

#include <stdio.h>
#include "net_data.h"
#include "events.h"
#include "manager_interface.h"
#include "net-interface.h"
#include "net_data_send.h"
#include "google/protobuf/message.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    typedef ::google_public::protobuf::Message MyMessage;
#else //if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    typedef ::google_public::protobuf::Message MyMessage;
#endif

class server_data{
public:
public:
    server_data(uint32_t AID, std::string AIPin4, std::string AIPin6, uint32_t APort){
        id = AID;
        server_ip_v4 = AIPin4;
        server_ip_v6 = AIPin6;
        server_port = APort;
    }
    uint32_t current_peer_index = 0;
    std::string server_ip_v4 = "";
    std::string server_ip_v6 = "";
    int addr_type = AF_INET;
    uint32_t server_port = 0;
    uint32_t id;
    uint32_t send_code = 0;
    float signaldelay = 0;
    int signalCountToBreak = 0;
    int signalID = 0;
    int MaxReconnectTime = 5;
    void bind_timer();
    void end_timer();
    void connect_failed();
    void connect();
    void disconnect();
    void send_data(const Buffer& buffer, uint32_t code);
    void resend();
private:
    std::mutex receivelock;
    std::mutex sendlock;
    bool connecting = false;
    events::event_dispatch dispatch_;
    
    Buffer buffer;
    Buffer _send_buffer;
    int reconnect_times = 0;
    double signal_send, signal_recv;
    void on_timer(float dt);
    void handle_signal(cmd_data_pointer data);
    void send_signal();
    
    void connect_status(uint32_t, const boost::system::error_code&);
    void send_error(uint32_t, const boost::system::error_code&);
    void recv_error(uint32_t, const boost::system::error_code&);
    size_t recv_data(uint32_t, char*, size_t);
};

class net_manager :  public SINGLETON<net_manager>, public manager_interface {
public:
    static const uint32_t CLIENT_VERSION = 31;
    static const uint32_t SERVER_NONE = 0;
    static const uint32_t SERVER_LOGIN = 1;
    static const uint32_t SERVER_RESOURCE = 2;
    static const uint32_t SERVER_ROOM = 3;
    static int SERVER_IP_TYPE;
    std::string SERVER_IP_V4;
    std::string SERVER_IP_V6;
    std::string FileServerIPv4;
    std::string FileServerIPv6;
    const std::shared_ptr<server_data> get_server_data_with_request_type(const uint16_t request_type);
    uint32_t get_server_id_by_peer_index(const uint32_t peer_index);
    void disconnect_all();
    void connect_all();
    void GetLoginServer();
    void send_buffer(uint32_t serverid);
    bool init_network_type();
    
protected:
    bool _connecting;
    std::map<uint32_t, std::shared_ptr<server_data>> server_datas;
    std::map<uint16_t, uint32_t> _request_type;
protected:
    void handle_file(uint32_t peer_index, void* data);
public:
    virtual void init();
    virtual void update() {}
    void connect(uint32_t server_id);
    void disconnect(uint32_t server_id);
    void add_server(std::shared_ptr<server_data> data);
    void send_data(net_data_send_package& package);
    void send_data(const MyMessage& processor, int request_code, uint64_t timemark = 0,bool isNeedUserid=true);
    uint32_t user_id;
};

class server_status_data : public cmd_data {
public:
    uint32_t serverid;
    bool is_valid() {return true;}
};

class server_error_data : public cmd_data {
public:
    std::string error;
    bool is_valid() {return true;}
};


#endif /* defined(__pokerclient__net_manager__) */
