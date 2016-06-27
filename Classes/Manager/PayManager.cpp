//
//  PayManager.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/8/29.
//
//
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "CCSwitchToOC.h"
#endif
#include "PayManager.h"
#include "msg.pb.h"
#include "GameDataManager.h"
#include "net_manager.h"
#include "TipView.h"
#include "NET_CMD.h"
#include "cmd_def.h"
#include "Md5.h"
#include <string.h>
#include "cocos2d.h"
#include "global_header.h"
#include "ProcessorManager.h"


PayManager* PayManager::__wechat_manager__;
PayManager::PayManager()
{
    FileUtils *fu=FileUtils::getInstance();
    this->map_trans_md5 = fu->getValueMapFromFile(fu->getWritablePath() + "failedtrans.plist");
    this->map_id_md5 = fu->getValueMapFromFile(fu->getWritablePath() + "failedid.plist");
    
    dispatch_.register_event(REQ_SEND_RECEIPT,BIND_FUNC(this, PayManager::HandleSendTransactions));
    cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&PayManager::on_timer, this, std::placeholders::_1),
                                                               this,
                                                               1.0/60,
                                                               kRepeatForever,
                                                               0,
                                                               false,
                                                               "PayManager::on_timer");
}
PayManager::~PayManager()
{
    
}
PayManager* PayManager::getInstance() {
    if(!__wechat_manager__) {
        __wechat_manager__ = new PayManager();
    }
    return __wechat_manager__;
}
void PayManager::SendToWeChat()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //        [AppController sendTextContent];
#endif
}
void PayManager::SetIsBegin(bool isBegin)
{
    this->isBegin = isBegin;
    if(!isBegin)
    {
        timer = 0;
    }
}

bool PayManager::GetTransing()
{
    return m_bTransing;
}

void PayManager::SetTransing(bool isTransing)
{
    Node *setting_layer = Director::getInstance()->getRunningScene();
    if(isTransing)
    {
        //show
        Layer *tip_layer = Layer::create();
        tip_layer->setTouchEnabled(true);
        tip_layer->setSwallowsTouches(true);
        tip_layer->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        ImageView *image = ImageView::create("res/login_res/shop_05.png");
        image->setPosition(Vec2(375,720));
        
        Text *text = Text::create(tools::local_string("charging","正在发起购买，请稍候"), "", 26);
        text->setPosition(Vec2(217, 100));
       // text->setAnchorPoint(Vec2(0.5,0.5));
        image->addChild(text);
        
        tip_layer->addChild(image);
        
        if(setting_layer)
        {
            tip_layer->setName("TipLayer");
            setting_layer->addChild(tip_layer,255);
        }
    }
    else
    {
        if(setting_layer)
        {
            Node *tip_layer = setting_layer->getChildByName("TipLayer");
            if(tip_layer)
            {
                tip_layer->removeFromParent();
            }
        }
    }
}

void PayManager::SendTransactions(std::string trans,std::string product_id,uint32_t userID)
{
    AddTransMd5(trans, userID == 0 ? mChargeForID : userID, product_id);
}

void PayManager::RemoveTransMd5(std::string transInfo)
{
    FileUtils *fu=FileUtils::getInstance();
    string path = FileUtils::getInstance()->getWritablePath() + "failedtrans.plist";
    string path1 = FileUtils::getInstance()->getWritablePath() + "failedid.plist";
    std::unordered_map<std::string, cocos2d::Value>::iterator it_tran;
    std::unordered_map<std::string, cocos2d::Value>::iterator it_id;
    it_tran = map_trans_md5.find(transInfo);
    it_id = map_id_md5.find(transInfo);
    if (it_tran != map_trans_md5.end())
    {
        map_trans_md5.erase(it_tran);
    }
    if (it_id != map_id_md5.end())
    {
        map_id_md5.erase(it_id);
    }
    
//    map_id_md5.erase(transInfo);
    fu->writeToFile(map_trans_md5, path);
    fu->writeToFile(map_id_md5, path1);
}

void PayManager::AddTransMd5(std::string transInfo,uint32_t userID,std::string product_id)
{
    FileUtils *fu=FileUtils::getInstance();
    string path = FileUtils::getInstance()->getWritablePath() + "failedtrans.plist";
    string path1 = FileUtils::getInstance()->getWritablePath() + "failedid.plist";
    myMD5::MD5 md5;
    md5.GenerateMD5((unsigned char *)transInfo.c_str(), (int)transInfo.length());
    std::string s = md5.ToString();
    if(map_trans_md5.find(s) == map_trans_md5.end())
    {
        map_trans_md5.insert(std::make_pair(s, cocos2d::Value(transInfo)));
        map_id_md5.insert(std::make_pair(s, cocos2d::Value((int)userID)));
        fu->writeToFile(map_trans_md5, path);
        fu->writeToFile(map_id_md5, path1);
    }
//    log("add----%s-%s",s.c_str(),transInfo.c_str());
    SendTransMd5(transInfo,userID,product_id);
}

void PayManager::SendTransMd5(std::string transInfo,uint32_t userID,std::string product_id)
{
    FileUtils *fu=FileUtils::getInstance();
    string path = FileUtils::getInstance()->getWritablePath() + "billInfo.plist";
    myMD5::MD5 md5;
    md5.GenerateMD5((unsigned char *)transInfo.c_str(), (int)transInfo.length());
    std::string s = md5.ToString();
    std::unordered_map<std::string,cocos2d::Value> bill = fu->getValueMapFromFile(path);
    if (bill.find(s) == bill.end())
    {
        bill.insert(std::make_pair(s,cocos2d::Value(tools::to_string(userID)+"+"+product_id)));
        fu->writeToFile(bill, path);
    }
    auto processor = PM->iospay_up(transInfo, userID);
    net_manager::shared_instence().send_data(processor,REQ_SEND_RECEIPT);
    events::global_dispatch.raise_event(IOS_PAY_LIST_REFRESH);
}

void PayManager::SendFaildTrans()
{
    for(auto iter : map_trans_md5)
    {
//        log("----%s-%s",iter.first.c_str(),iter.second.asString().c_str());
        SendTransMd5(iter.second.asString(), map_id_md5.find(iter.first)->second.asInt());
    }
}
void PayManager::RemovePayListItem(std::string s)
{
    RemoveTransMd5(s);
    FileUtils *fu=FileUtils::getInstance();
    string path = FileUtils::getInstance()->getWritablePath() + "billInfo.plist";
    std::unordered_map<std::string,cocos2d::Value> bill = fu->getValueMapFromFile(path);
    std::unordered_map<std::string,cocos2d::Value>::iterator it;
    it = bill.find(s);
    if ( it == bill.end() )
    {
        
    }
    else
    {
        bill.erase(it);
    }
    
    fu->writeToFile(bill, path);
}
void PayManager::HandleSendTransactions(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->iospay_down(rec);
    std::string s = processor.receiptmd5();
    
    switch (processor.response().status())
    {
        case msg::Status::SUCCESS:
        {
            RemovePayListItem(s);
            TipView::showAlertView(tools::local_string("buy_ok","购买成功"));
        }
            break;
        case msg::Status::FAILED:
        {
//            log("106-----%s",processor.response().message().c_str());
            if (processor.type() && processor.type() == msg::VerificationType::VERIFICATIONfAIL)
            {
                RemovePayListItem(s);
                TipView::showAlertView(processor.response().message());
            }
        }
            break;
        default:
            break;
    }
    events::global_dispatch.raise_event(IOS_PAY_LIST_REFRESH);
    //TODO callback layerusersetting send getusersetting
}

void PayManager::SendBuy(std::string id)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    product_id_ = id;
    CCSwitchToOC CtoOC;
    CtoOC.sendBuy(id);
#endif
}

void PayManager::SendWeChatPay(std::string appID,std::string partnerID,std::string prepayId , std::string nonceStr ,std::string timeStamp , std::string sign)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    
    //告诉MyApplication已启动
    
    if(JniHelper::getStaticMethodInfo(t, "com/insul/poker/AppActivity", "GetAppActivity", "()Ljava/lang/Object;"))
    {
        jobject obj= t.env->CallStaticObjectMethod(t.classID,t.methodID);
        
        jstring jAppID = t.env->NewStringUTF(appID.c_str());
        
        jstring jPartnerID = t.env->NewStringUTF(partnerID.c_str());
        
        jstring jPrepayId = t.env->NewStringUTF(prepayId.c_str());
        
        jstring jnonceStr = t.env->NewStringUTF(nonceStr.c_str());
        
        jstring jTimeStamp = t.env->NewStringUTF(timeStamp.c_str());
        
        jstring jSign = t.env->NewStringUTF(sign.c_str());
        
        if (JniHelper::getMethodInfo(t, "com/insul/poker/AppActivity", "SendPayWX", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
        {
            
            t.env->CallVoidMethod(obj,t.methodID,jAppID,jPartnerID,jPrepayId,jnonceStr,jTimeStamp,jSign);
            t.env->DeleteLocalRef(jAppID);
            t.env->DeleteLocalRef(jPartnerID);
            t.env->DeleteLocalRef(jPrepayId);
            t.env->DeleteLocalRef(jnonceStr);
            t.env->DeleteLocalRef(jTimeStamp);
            t.env->DeleteLocalRef(jSign);
            
        }
    }
    
#endif
}
void PayManager::on_timer(float dt)
{
    if(isBegin)
    {
        this->timer++;
        if(timer > 900)
        {
            //this->SetTransing(false);
            TipView::showAlertView(tools::local_string("request_timed_out","请求超时"));
            timer = 0;
            isBegin = false;
        }
    }
}
std::vector<std::pair<std::string,std::string>> PayManager::getiOSPayList()
{
    FileUtils *fu=FileUtils::getInstance();
    string path = FileUtils::getInstance()->getWritablePath() + "billInfo.plist";
    std::unordered_map<std::string,cocos2d::Value> bill = fu->getValueMapFromFile(path);
    std::vector<std::pair<std::string,std::string>> list;
    std::vector<std::string> product_id = {"com.chinarichinc.haopai.diamond_4_add","com.chinarichinc.haopai.diamond_3_add","com.chinarichinc.haopai.diamond_5_add","com.chinarichinc.haopai.diamond_2","com.chinarichinc.haopai.diamond_1"};
    std::vector<std::string> good_names = {"3280","600","6180","300","60"};
    std::vector<int> good_prices = {328,60,618,30,6};
    for (auto iter : bill)
    {
        std::pair<std::string,std::string> data;
        std::string info = iter.second.asString();
        long pos = info.find("+");
        std::string pro_id = info.substr(pos+1,info.length());
        for (int i = 0 ; i < 5; i++)
        {
            if (pro_id == product_id.at(i))
            {
                data.first = good_names.at(i)+tools::local_string("diamond",  "钻石");
                data.second = tools::to_string(good_prices.at(i));
                list.push_back(data);
                break;
            }
        }
    }
    return list;
}