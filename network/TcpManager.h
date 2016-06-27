#pragma once

#include <string>
#include <boost/noncopyable.hpp>
#include <memory>
#include <functional>
#include <map>
#include <list>
#include "message_block.hpp"
#include "cocos2d.h"

#define BUFFER_SIZE                     (500)
#define TM                            (TcpManager::getInstance())

class TcpManager
{
public:
    static TcpManager& getInstance() {
        static TcpManager __tcp__manager__;
        return __tcp__manager__;
    }
    void connect(const std::string& server_ip, const uint32_t server_port);
    void shutdown();
    
    void handle_virtual_data(char *pos);
private:
    TcpManager();
    TcpManager(const TcpManager&);
    TcpManager operator=(const TcpManager&);
    
    void login();
    
    
    void send(const char *data, const uint32_t length);
    
    std::string m_server_ip;
    int m_server_port;
    uint32_t m_peer_index;
    
    void handle_connect_status(uint32_t peer_index, int result);
    void handle_data_error(uint32_t peer_index, int result);
    void handle_data(uint32_t peer_index, char *pos);
    
    
    
    void handle_data_main_thread(float);
    void handle_connect_faild(float);
    
    
    std::list<std::shared_ptr<message_block>> unhandle_datas_;
    std::mutex mutex_;

    char buffer_[BUFFER_SIZE];
    
    bool shutdown_;
    
    uint64_t role_id_;
};