//
//  PushManager.h
//  pokerclient
//
//  Created by qqronaldz on 15/8/27.
//
//

#ifndef __pokerclient__PushManager__
#define __pokerclient__PushManager__

#include <stdio.h>
#include <iostream>
class PushManager
{
public:
    PushManager();
    ~PushManager();
public:
    static PushManager* getInstance();
    
    static void destroy();
    void GoToPokerRoom();
    void GoToGroup();
    //send device token
    void SendDeviceToken();
    //handle recieve push
    void HandleRecMsg();
    void HandleRecIOSMsg(uint32_t room_id,uint32_t group_id);
    void SetAppStatusAndroid(int status);
public:
    std::string m_strDeviceToken;
    bool m_bIsLogin;
    bool m_bPushLogin;
    std::string room_id = "";
//    std::string group_id = "";
//    std::string room_ip = "";
//    std::string room_port = "";
private:
    static PushManager* __push_manager__;
};
#endif /* defined(__pokerclient__PushManager__) */
