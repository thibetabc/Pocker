//
//  net_manager.cpp
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//

#include "cocos2d.h"
#include "net_manager.h"
#include "Buffer.h"
#include "net-interface.h"
#include "events.h"
#include "cmd_def.h"
#include "net_data_recv.h"
#include "file_utils.h"
#include "LoginManager.h"
#include "utils.h"
#include "FileManager.h"
#include "ProcessorManager.h"
#include "SceneManager.h"
#include "GameDataManager.h"
#include "NET_CMD.h"
#include "GameTools.h"
#include "PlatformSdk.h"

const uint32_t net_manager::SERVER_LOGIN;
int net_manager::SERVER_IP_TYPE = AF_UNSPEC;

void net_manager::init() {
    CCLOGINFO("xixi");
    net_interface::startup_io_service();
    _request_type[REQ_VERSIONCHECK] = net_manager::SERVER_LOGIN;
    _request_type[REQ_LOGIN] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_REGISTER] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_SEND_DEVICE_ID] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_SEND_GET_GOODS_LIST] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_SEND_BUY_GOODS] = net_manager::SERVER_RESOURCE;
    
    _request_type[REQ_SEND_RECEIPT] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_SEND_ANDROID_CHARGE] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_SEND_VERIFICATION_BY_USER] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_RESET_PASSWORD] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_LOGIN_RESOURCES] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_CHANGE_USER_SETTING] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_GET_USER_SETTING] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_GET_USER_INFO] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_CREATE_GROUP] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_EDIT_GROUP] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_SEARCH_GROUP] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_GET_ROOM_INFO] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_ENTER_GROUP] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_GET_GROUP_INFO] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_KICK_FROM_GROUP] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_SEND_CHAT_MESSAGE] = net_manager::SERVER_RESOURCE;
    _request_type[RECEIVE_CHAT_MESSAGE] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_CREATE_ROOM] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_CANCEL_MATCH] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_GET_GROUP_SETTING] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_CHANGE_GROUP_SETTING] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_SEND_VERIFICATION] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_BIND_PHONE] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_CREATE_GROUP] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_SIGNAL_RESOURCE] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_OFFLINE] = net_manager::SERVER_RESOURCE;
    
    _request_type[REQ_LOGIN_ROOM] = net_manager::SERVER_ROOM;
    _request_type[REQ_ENTER_ROOM] = net_manager::SERVER_ROOM;
    _request_type[REQ_SEND_SEAT_INFO] = net_manager::SERVER_ROOM;
    _request_type[POKER_SEND_PLAYER_ACTION] = net_manager::SERVER_ROOM;  //--by lvzhangle
    _request_type[POKER_REQUEST_PLYAER_ACTION] = net_manager::SERVER_ROOM;
    _request_type[POKER_SEND_SET_CHANGE] = net_manager::SERVER_ROOM;
    _request_type[POKER_SEND_SHOW_CARD] = net_manager::SERVER_ROOM;
    _request_type[POKER_RECEIVE_SET_CHANGE] = net_manager::SERVER_ROOM;
    _request_type[OFC_SEND_SET_CHANGE] = net_manager::SERVER_ROOM;
    _request_type[OFC_RECEIVE_SET_CHANGE] = net_manager::SERVER_ROOM;
    _request_type[POKER_RECEIVE_PROP_UPDATE] = net_manager::SERVER_ROOM;
    _request_type[RECEIVE_SEAT_INFO] = net_manager::SERVER_ROOM;
    _request_type[POKER_REQUEST_PLYAER_ACTION] = net_manager::SERVER_ROOM;
    _request_type[REQ_GET_SCORE] = net_manager::SERVER_ROOM;
    _request_type[POKER_RECEIVE_HAND_RESULT] = net_manager::SERVER_ROOM;
    _request_type[POKER_TOURMENT_STATUS] = net_manager::SERVER_ROOM;
    _request_type[POKER_REFRESH_ALL] = net_manager::SERVER_ROOM;
    //    _request_type[POKER_GET_HAND_HISTORY] = net_manager::SERVER_ROOM;
    _request_type[POKER_GET_HAND_HISTORY] = net_manager::SERVER_RESOURCE;
    _request_type[POKER_ADD_CHIPS] = net_manager::SERVER_ROOM;
    _request_type[RECEIVE_GAME_START] = net_manager::SERVER_ROOM;
    _request_type[RECEIVE_OTHER_PLAYER_ACTION] = net_manager::SERVER_ROOM;
    _request_type[POKER_RECEIVE_NEED_CHIP] = net_manager::SERVER_ROOM;
    //    _request_type[RECEIVE_GAME_SET_INFO] = net_manager::SERVER_ROOM;
    _request_type[REQ_SIGNAL_ROOM] = net_manager::SERVER_ROOM;
    _request_type[REQ_SEND_GAME_VOICE] = net_manager::SERVER_ROOM;
    _request_type[RECEIVE_GAME_VOICE] = net_manager::SERVER_ROOM;
    _request_type[RECEIVE_ROOM_CLOSE] = net_manager::SERVER_ROOM;
    _request_type[RECEIVE_ROOM_MESSAGE] = net_manager::SERVER_ROOM;
    _request_type[RECEIVE_GAME_END] = net_manager::SERVER_ROOM;
    _request_type[REQ_DISABLE_AUTO_ACTION] = net_manager::SERVER_ROOM;
    
    //by liujia
    _request_type[OFC_RECEIVE_PROP_UPDATE] = net_manager::SERVER_ROOM;
    _request_type[OFC_RECEIVE_PLAYERCARDS] = net_manager::SERVER_ROOM;
    _request_type[OFC_RECEIVE_RESULT] = net_manager::SERVER_ROOM;
    _request_type[OFC_SEND_PLAYER_CARDS] = net_manager::SERVER_ROOM;
    _request_type[OFC_REFRESH_ALL] = net_manager::SERVER_ROOM;
    _request_type[OFC_RECEIVE_SCORE] = net_manager::SERVER_ROOM;
    
    //by lvzhangle
    _request_type[REQ_GET_FRIENDS_LIST] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_SEARCH_USER] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_ADD_FRIEND] = net_manager::SERVER_RESOURCE;
    _request_type[RECEIVE_REQUEST_LIST] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_DEAL_REQUEST] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_DELETE_FRIEND] = net_manager::SERVER_RESOURCE;
    
    //by zhaoxuewei
    _request_type[REQ_SHIELD_FRIEND] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_GET_PERMISSION] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_CHANGE_PERMISSION] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_190] = net_manager::SERVER_RESOURCE;
    
    _request_type[REQ_PUBLISH_MOMENT] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_GET_MOMENT] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_DELETE_MOMENT] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_PUBLISH_COMMENT] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_GET_COMMENT] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_DELETE_COMMENT] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_FEED_BACK] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_PLYAER_HISTORY_SOCRE] = net_manager::SERVER_RESOURCE;
    _request_type[REQ_GET_GROUP_USER_RANK] = net_manager::SERVER_RESOURCE;
    _request_type[POKER_REQUEST_CHOICE] = net_manager::SERVER_ROOM;
    
    _request_type[REQ_SEND_JOIN] = net_manager::SERVER_RESOURCE;
    
    _request_type[REQ_RECEIVE_TOURMENT_ADDON] = net_manager::SERVER_ROOM;
    _request_type[REQ_SEND_TOURMENT_ADDON] = net_manager::SERVER_ROOM;
    _request_type[REQ_SEND_TOURMENT_CHANGE_TABLE] = net_manager::SERVER_ROOM;
    _request_type[POKER_RECEIVE_TABLE_NOTE] = net_manager::SERVER_ROOM;
    _request_type[REQ_SET_GAME_STATUS] = net_manager::SERVER_ROOM;
    _request_type[REQ_KICK_PLAYER] = net_manager::SERVER_ROOM;
    _request_type[REQ_RECEIVE_GAME_STATUS] = net_manager::SERVER_ROOM;
    
    //by huangjianfeng
    _request_type[REQ_MOMENT_BLOCK_LIST] = SERVER_RESOURCE;
    _request_type[REQ_PHONE_CONTACTS_LIST] = SERVER_RESOURCE;
    _request_type[REQ_GET_GROUP_DETAIL_INFO] = SERVER_RESOURCE;
    _request_type[REQ_SET_GROUP_MANAGE_SETTING] = SERVER_RESOURCE;
    _request_type[REQ_SET_IS_ADMIN] = SERVER_RESOURCE;
    _request_type[REQ_GIFT_DIAMOUNT] = SERVER_RESOURCE;
    _request_type[REQ_CHANGE_LANGUAGE_ROOM] = SERVER_RESOURCE;
    _request_type[MAHJONG_RECEIVE_PROP_UPDATE] = net_manager::SERVER_ROOM;
    _request_type[MAHJONG_SEND_SET_CHANGE] = net_manager::SERVER_ROOM;
    _request_type[MAHJONG_SEND_PLAYER_CHOICE] = net_manager::SERVER_ROOM;

}

void net_manager::add_server(std::shared_ptr<server_data> data)
{
    for (auto iter: server_datas){
        if (iter.second->id == data->id)
            return;
    }
    server_datas.insert(std::make_pair(data->id, data));
}

const std::shared_ptr<server_data> net_manager::get_server_data_with_request_type(const uint16_t request_type)
{
    auto iter = _request_type.find(request_type);
    if(iter != _request_type.end())
    {
        auto it = server_datas.find(iter->second);
        if(it != server_datas.end())
        {
            return it->second;
        }
    }
    return nullptr;
}

uint32_t net_manager::get_server_id_by_peer_index(const uint32_t peer_index)
{
    for (auto& iter : server_datas) {
        if(iter.second->current_peer_index == peer_index) {
            return iter.second->id;
        }
    }
    return net_manager::SERVER_NONE;
}

bool net_manager::init_network_type()
{
    struct addrinfo *answer, hint, *curr;
    
    bzero(&hint, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_flags = AI_ADDRCONFIG;
    hint.ai_socktype = SOCK_STREAM;
    char ipstrv4[64];
    char ipstrv6[128];
    struct sockaddr_in  *sockaddr_ipv4;
    struct sockaddr_in6 *sockaddr_ipv6;
    
    int ret = getaddrinfo("pokerhaopai.chinarichinc.com", NULL, &hint, &answer);
    
    if (ret != 0) {
        cocos2d::log("ERROR: Getaddrinfo error info %s", gai_strerror(ret));
        
        return false;
    }else
    {
        bool isINET = false;
        bool isINET6 = false;
        for (curr = answer; curr != NULL; curr = curr->ai_next) {
            switch (curr->ai_family){
                case AF_UNSPEC:
                    //SERVER_IP_TYPE = AF_UNSPEC;
                    break;
                case AF_INET:
                    sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in *>( curr->ai_addr);
                    inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ipstrv4,sizeof(ipstrv4));
                    isINET = true;
                    break;
                case AF_INET6:
                    sockaddr_ipv6 = reinterpret_cast<struct sockaddr_in6 *>( curr->ai_addr);
                    inet_ntop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstrv6,sizeof(ipstrv6));
                    isINET6 = true;
                    break;
            }
        }
        
        SERVER_IP_V4 = ipstrv4;
        SERVER_IP_V6 = ipstrv6;
        
        if (!isINET && isINET6)
        {
            SERVER_IP_TYPE = AF_INET6;
        }else
        {
            SERVER_IP_TYPE = AF_INET;
        }
        //Don't forgot free addrinfo because memory leek   --by lvzhangle
        freeaddrinfo(answer);
    }
    
    return true;
}

void net_manager::GetLoginServer(){
    
    if (!init_network_type()) {
        
        SceneManager::addLoginLayer();
        auto layer = SceneManager::GetLoginLayer();
        layer->ShowError(tools::local_string("connect_failed", "当前网络不可用，请检查网络设置！"));
    }else
    {
        std::shared_ptr<server_data> loginserver(new server_data(net_manager::SERVER_LOGIN, SERVER_IP_V4, SERVER_IP_V6, 20000));
        add_server(loginserver);
        connect(net_manager::SERVER_LOGIN);
    }
}

void net_manager::send_data(net_data_send_package& package){
    Buffer tmp_buffer = Buffer();
    package.package(tmp_buffer);
    
    auto server = get_server_data_with_request_type(package.packagehead.request_code);
    if (server == nullptr) {
        auto iter = server_datas.find(net_manager::SERVER_LOGIN);
        if(iter == server_datas.end()){
            GetLoginServer();
            iter = server_datas.find(net_manager::SERVER_LOGIN);
            if (iter == server_datas.end())
                return;
            else
                server = iter->second;
        }
        else return;
    }
    if (server != nullptr)
        server->send_data(tmp_buffer, package.packagehead.request_code);
}

void net_manager::send_data(const MyMessage& processor, int request_code, uint64_t timemark,bool isNeedUserid){
    net_data_send_package package(request_code);
    msg::ClientRequestHead requesthead;
    requesthead.set_userid(net_manager::shared_instence().user_id);
    if(!isNeedUserid)
    {
        ::google_public::protobuf::int32  temp=0;
        requesthead.set_userid(temp);
    }
    auto language = GameTools::getLanguage();
    msg::Language msgLanguage = msg::Language::ZHCN;
    switch ((PlatformSdk::LanguageType)language) {
        case PlatformSdk::LanguageType::CHINESE:
            msgLanguage = msg::Language::ZHCN;
            break;
        case PlatformSdk::LanguageType::TRADITIONAL_CHINESE:
            msgLanguage = msg::Language::ZHTW;
            break;
        case PlatformSdk::LanguageType::ENGLISH:
            msgLanguage = msg::Language::ENUS;
            break;
            
        default:
            break;
    }
    requesthead.set_language(msgLanguage);
    requesthead.set_version(CLIENT_VERSION);
    requesthead.set_timemark(timemark);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    requesthead.set_platform(msg::Platform::ANDORID);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    requesthead.set_platform(msg::Platform::IOS);
#endif
    requesthead.set_testcode(0);
    
    package.headsize = requesthead.ByteSize();
    package.head = new char[package.headsize];
    requesthead.SerializeToArray(package.head, package.headsize);

    package.bodysize = processor.ByteSize();
    package.body = new char[package.bodysize];
    processor.SerializeToArray(package.body, package.bodysize);
    
    net_manager::shared_instence().send_data(package);
    delete[] package.body;
    delete[] package.head;
    package.body = nullptr;
    package.head = nullptr;
}

void net_manager::connect(uint32_t server_id)
{
    auto iter = server_datas.find(server_id);
    if (iter == server_datas.end()) return;
    iter->second->connect();
}

void net_manager::send_buffer(uint32_t serverid){
    auto server = server_datas.find(serverid);
    if (server != server_datas.end())
        server->second->resend();
}

void net_manager::disconnect(uint32_t server_id)
{
    auto iter = server_datas.find(server_id);
    if(iter != server_datas.end())
        iter->second->disconnect();
}

void net_manager::disconnect_all()
{
    cocos2d::log("net_manager::disconnect_all");
    for (auto& iter : server_datas) {
        disconnect(iter.second->id);
    }
}

void net_manager::connect_all()
{
    for (auto& iter : server_datas) {
        if ((iter.second->id == SERVER_RESOURCE) || (iter.second->id == SERVER_ROOM))
            connect(iter.second->id);
    }
}

void server_data::send_signal()
{
    auto processor = PM->null_up();
    net_manager::shared_instence().send_data(processor, signalID);
}

void server_data::on_timer(float dt)
{
    double date = tools::date_double();
    double passtime = date - signal_send;
    if(passtime > signaldelay) {
        signal_send = date;
        send_signal();
    }
    
    if(signal_recv == 0) {
        signal_recv = date;
    }
    
    double d2 = date - signal_recv;
    if(d2 > signaldelay * signalCountToBreak)
        connect_failed();
}

void server_data::disconnect()
{
    std::lock_guard<std::mutex> lock(receivelock);
    end_timer();
    net_interface2::shutdown(current_peer_index);
    buffer.Clear();
    current_peer_index = 0;
    
    std::shared_ptr<server_status_data> data(new server_status_data);
    data->serverid = id;
    events::global_dispatch.raise_event(EVENT_CONNECT_BREAK, data);
}


void server_data::send_error(uint32_t peer_index, const boost::system::error_code& status){
    if (current_peer_index != peer_index) return;
    printf("send_error idx:%d server_type:%d :%s",peer_index, id, status.message().c_str());
    connect_failed();
}

void server_data::connect_status(uint32_t peer_index, const boost::system::error_code& status){
    if (current_peer_index != peer_index) return;
    connecting = false;
    if (status != 0) {
        printf("connect_status idx:%d server_type:%d :%s\n",peer_index, id, status.message().c_str());
        current_peer_index = 0;
        connect_failed();
    }else{
        reconnect_times = 0;
        std::shared_ptr<server_status_data> data(new server_status_data);
        data->serverid = id;
        events::global_dispatch.raise_event(EVENT_CONNECT_SUCCESS, data);
    }
}

void server_data::send_data(const Buffer& buffer, uint32_t code)
{
    if ((current_peer_index == 0 || connecting) && code != REQ_LOGIN_ROOM && code != REQ_LOGIN_RESOURCES && code != REQ_VERSIONCHECK)
    {
        std::lock_guard<std::mutex> lock(sendlock);
        _send_buffer.Clear();
        _send_buffer.Append(buffer);
        send_code = code;
        connect();
    }
    else{
        double date = tools::date_double();
        if (code != signalID)
            cocos2d::log("send_data:%d, %f", code, date);
        net_interface2::send(current_peer_index, (const char*)buffer.Data(), (unsigned int)buffer.Length());
    }
}

void server_data::resend(){
    std::lock_guard<std::mutex> lock(sendlock);
    if (_send_buffer.Length() != 0){
        if (send_code != signalID)
            cocos2d::log("send_data:%d, %f", send_code, tools::date_double());
        net_interface2::send(current_peer_index, (const char*)_send_buffer.Data(), (unsigned int)_send_buffer.Length());
        _send_buffer.Clear();
    }
}

void server_data::recv_error(uint32_t peer_index, const boost::system::error_code& status)
{
    if (current_peer_index != peer_index) return;
    std::lock_guard<std::mutex> lock(receivelock);
    std::string servertext;
    switch (id) {
        case net_manager::SERVER_LOGIN:
            servertext = "登录服务器";
            break;
        case net_manager::SERVER_RESOURCE:
            servertext = "资源服务器";
            break;
        case net_manager::SERVER_ROOM:
            servertext = "房间服务器";
            break;
        default:
            break;
    }
    printf("recv_error idx:%s :%s\n", servertext.c_str(), status.message().c_str());
    if(status.value() != ENOENT && status.value() != ECANCELED &&status.value() != ENOTCONN ) {
        std::string message = tools::to_string((int)id) + "+" + status.message();
        
        std::shared_ptr<server_error_data> data(new server_error_data);
        data->error = message;
        cocos2d::log("message = %s",message.c_str());
        events::global_dispatch.raise_event(EVENT_RECV_ERROR, data);
    }
    buffer.Clear();
}

size_t server_data::recv_data(uint32_t peer, char* buf, size_t len){
    if (len == 0) {
        return 0;
    }
    
    if(peer != current_peer_index) {
        cocos2d::log("收到数据与已有索引不匹配，索引:%u", peer);
        return len;
    }
    
    buffer.Append(buf, len);
    Buffer tmp_buffer(buffer);
    size_t read_size = 0;
    while (true) {
        std::shared_ptr<net_data_recv_package> package = std::shared_ptr<net_data_recv_package>(new net_data_recv_package());
        if (package->parse(tmp_buffer)) {
            if (package->packagehead.request_code != signalID) {
                double date = tools::date_double();
                cocos2d::log("package_command_id:%u, %f", package->packagehead.request_code, date);
            }
        }else{
            break;
        }
        read_size += package->size();
        if (!GDM->get_IsInBackGround() || package->packagehead.request_code != RECEIVE_GAME_VOICE)
            events::global_dispatch.raise_event(package->packagehead.request_code, package);
    }
    buffer.Remove(0, read_size);
    
    return len;
}

void server_data::connect()
{
    if (current_peer_index != 0)
        disconnect();
    connecting = true;
    
    int connect_type;
    std::string result_addr;
    
    if (id == net_manager::SERVER_LOGIN)
    {
        connect_type = net_manager::SERVER_IP_TYPE;
        if (AF_INET == net_manager::SERVER_IP_TYPE)
        {
            result_addr = server_ip_v4;
        }else if (AF_INET6 == net_manager::SERVER_IP_TYPE)
        {
            result_addr = server_ip_v6;
        }
    }else
    {
        struct addrinfo *answer, hint, *curr;
        
        bzero(&hint, sizeof(hint));
        hint.ai_family = AF_UNSPEC;
        hint.ai_flags = AI_ADDRCONFIG;
        hint.ai_socktype = SOCK_STREAM;
        struct sockaddr_in  *sockaddr_ipv4;
        struct sockaddr_in6 *sockaddr_ipv6;
        char ipstrv6[128];
            
        int ret = getaddrinfo(server_ip_v4.c_str(), NULL, &hint, &answer);
        
        if (ret != 0) {
            cocos2d::log("ERROR: Getaddrinfo error info %s", gai_strerror(ret));
            
        }else
        {
            bool isINET = false;
            bool isINET6 = false;
            for (curr = answer; curr != NULL; curr = curr->ai_next) {
                switch (curr->ai_family){
                    case AF_UNSPEC:
                        break;
                    case AF_INET:
                        sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in *>( curr->ai_addr);
                        isINET = true;
                        break;
                    case AF_INET6:
                        sockaddr_ipv6 = reinterpret_cast<struct sockaddr_in6 *>( curr->ai_addr);
                        inet_ntop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstrv6, sizeof(ipstrv6));
                        isINET6 = true;
                        break;
                }
            }
            
            if (!isINET && isINET6)
            {
                connect_type = AF_INET6;
                result_addr = ipstrv6;
            }else
            {
                connect_type = AF_INET;
                result_addr = server_ip_v4;
            }
            
            //Don't forgot free addrinfo because memory leek   --by lvzhangle
            freeaddrinfo(answer);
        }
    }
    
    uint32_t peer = net_interface2::connect(result_addr.c_str(),
                                            server_port,
                                            connect_type,
                                            std::bind(&server_data::connect_status, this, std::placeholders::_1, std::placeholders::_2),
                                            std::bind(&server_data::send_error, this, std::placeholders::_1, std::placeholders::_2),
                                            std::bind(&server_data::recv_error, this, std::placeholders::_1, std::placeholders::_2),
                                            std::bind(&server_data::recv_data, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    current_peer_index = peer;
    cocos2d::log("连接索引：%u", current_peer_index);
    bind_timer();
}

void server_data::connect_failed()
{
    usleep(500000);
    end_timer();
    if(reconnect_times >= MaxReconnectTime) {
        reconnect_times = 0;
        std::shared_ptr<server_status_data> data(new server_status_data);
        data->serverid = id;
        events::global_dispatch.raise_event(EVENT_CONNECT_FAILED, data);
    }
    else {
        reconnect_times++;
        connect();
    }
}

void server_data::bind_timer()
{
    if (signalID != 0){
        dispatch_.register_event(signalID,
                                 BIND_FUNC(this, server_data::handle_signal));
        Director::getInstance()->getScheduler()->schedule(std::bind(&server_data::on_timer, this, std::placeholders::_1), this, 1.0f, kRepeatForever, 0, false, "on_timer");
        double date = tools::date_double();
        signal_send = date;
        signal_recv = date;
    }
}

void server_data::end_timer()
{
    if (signalID != 0)
        Director::getInstance()->getScheduler()->unschedule("on_timer", this);
}

void server_data::handle_signal(cmd_data_pointer data)
{
    signal_recv = tools::date_double();
}