//
//  PayManager.h
//  pokerclient
//
//  Created by qqronaldz on 15/8/29.
//
//

#ifndef __pokerclient__PayManager__
#define __pokerclient__PayManager__

#include <stdio.h>
#include "string.h"
#include <iostream>
#include "events.h"
#include "cmd_data.h"
#include "msg.pb.h"
#include "cocos2d.h"
#include "TipView.h"
#include "events.h"
#include "utils.h"
class Bill
{
public:
    std::string trans;
    std::string product_id;
    uint32_t user_id;
};
class PayManager
{
public:
    PayManager();
    ~PayManager();
    static PayManager* getInstance();
    static void SendToWeChat();
    void HandleSendTransactions(cmd_data_pointer data);
    void SendWeChatPay(std::string appID,std::string partnerID,std::string prepayId , std::string nonceStr ,std::string timeStamp , std::string sign);
    void SendTransactions(std::string trans,std::string product_id,uint32_t userID = 0);
    void SetIsBegin(bool isBegin);
    void SendBuy(std::string id);
    bool GetTransing();
    void SetTransing(bool isTransing);
    void SendTransMd5(std::string transInfo,uint32_t userID,std::string product_id="");
    void AddTransMd5(std::string transInfo,uint32_t userID,std::string product_id);
    void RemoveTransMd5(std::string transInfo);
    
    void SendFaildTrans();
    std::unordered_map<std::string, cocos2d::Value> GetFaiedTransList(){return map_trans_md5;}
    void on_timer(float dt);
    void HandleTransFail(){TipView::showAlertView(tools::local_string("charge_failed","交易失败，请重新发送"));}
    void SetChargeForID(uint32_t id){mChargeForID = id;}
    uint32_t GetChargeForID(){return mChargeForID;}
    std::vector<std::pair<std::string,std::string>> getiOSPayList();
    void RemovePayListItem(std::string s);
    
private:
    static PayManager* __wechat_manager__;
    bool m_bTransing = false;
    events::event_dispatch dispatch_;
    std::unordered_map<std::string, cocos2d::Value> map_trans_md5;
    std::unordered_map<std::string, cocos2d::Value> map_id_md5;
    int timer = 0;
    bool isBegin = false;
    uint32_t mChargeForID = 0;
    std::string product_id_ = "";
   
};
#endif /* defined(__pokerclient__PayManager__) */
