//
//  NetManager.cpp
//  DK2.0
//
//  Created by mingt on 9/15/14.
//
//

#include "TcpManager.h"
#include <net-interface.h>
//#include "command-def.hpp"

using namespace cocos2d;

TcpManager::TcpManager():
m_server_port(0),
m_peer_index(0),
role_id_(0)
{
    net_interface::startup_io_service();
    
    Director::getInstance()->getScheduler()->schedule(std::bind(&TcpManager::handle_data_main_thread, this, std::placeholders::_1), this, 0, kRepeatForever, 0, false, "handle_data");
}

void TcpManager::connect(const std::string& server_ip, const uint32_t server_port)
{
    shutdown_ = false;
    m_server_ip = server_ip;
    m_server_port = server_port;
//    m_peer_index = net_interface2::connect(m_server_ip.c_str(), m_server_port, function_connect_status_, function_data_error_, function_data_);
}

void TcpManager::shutdown()
{
    shutdown_ = true;
    cocos2d::log("----------> TcpManager::shutdown(m_peer_index = %d)", m_peer_index);
    net_interface2::shutdown(m_peer_index);
}

void TcpManager::send(const char *data, const uint32_t length)
{
#define VIRTUAL_SERVER 1
#ifdef VIRTUAL_SERVER
    
#else
    net_interface2::send(m_peer_index, data, length);
#endif
}

void TcpManager::login()
{
//    uint64_t id = GameDataManager::getInstance().get_role()->get_id();
//    assert(id != 0);
//    const std::string session_key = "1";
//    HEADER_COMMAND_C2S_SYSTEM_LOGIN command = {
//        {
//            static_cast<uint32_t>(sizeof(HEADER_COMMAND_C2S_SYSTEM_LOGIN) + session_key.length()),
//            CHANNEL_SYSTEM,
//            COMMAND_C2S_SYSTEM_LOGIN,
//            0,
//        },
//        id,
//        0,
//    };
//    role_id_ = id;
//    CCLOG("$$$ role_id_ : %llu", role_id_);
//    
//    message_block mb(command.header.length + session_key.length());
//    mb << command << session_key;
//    
//    net_interface2::send(m_peer_index, mb.data(), mb.length());
}


void TcpManager::handle_connect_status(uint32_t peer_index, int result)
{
    assert(m_peer_index == peer_index);
    if(result == 0) {
        login();
    }
    else {
        Director::getInstance()->getScheduler()->schedule(std::bind(&TcpManager::handle_connect_faild, this, std::placeholders::_1), this, 0, 0, 0, false, "handle_data");
    }
}
void TcpManager::handle_connect_faild(float)
{
//    assert(0);
}

void TcpManager::handle_data_error(uint32_t peer_index, int result)
{
    assert(m_peer_index == peer_index);
//    if(!shutdown_)
//        assert(0);
}

void TcpManager::handle_virtual_data(char *pos)
{
    int peer = m_peer_index;
    handle_data(peer, pos);
}

void TcpManager::handle_data(uint32_t peer_index, char *pos)
{
    assert(m_peer_index == peer_index);
    
//    HEADER *data = (HEADER *) pos;
//    sp_message_block unhandle_data(new message_block(data->length));
//    unhandle_data->copy(pos, data->length);

//    mutex_.lock();
//    unhandle_datas_.push_back(unhandle_data);
//    mutex_.unlock();
}

void TcpManager::handle_data_main_thread(float)
{
    mutex_.lock();
    for(auto& iter : unhandle_datas_) {
//        HEADER *data = (HEADER *) iter->data();
//        CCLOG("****** receive command %d ****** errorcode : %d", data->command, data->result);
//        if(data->result == 0) {
//            switch (data->command) {
//              
//                default:
//                    break;
//            }
//        }
//        else
//        {
//            switch (data->result) {
//               
//                default:
//                {
//                    break;
//                }
//                    
//            }
//        } //end of else
    }
    unhandle_datas_.clear();
    mutex_.unlock();
}



