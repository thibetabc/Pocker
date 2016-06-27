//
//  PushManager.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/8/27.
//
//

#include "PushManager.h"
#include "PokerDataManager.h"
#include "GameDataManager.h"
#include "SceneManager.h"
#include "NET_CMD.h"
#include "msg.pb.h"
#include "net_manager.h"
#include "utils.h"
#include "ProcessorManager.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "jni/JniHelper.h"
#include <android/log.h>
#endif
USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>
extern "C"
{
//    char* jstringTostring(JNIEnv* env, jstring jstr)
//    {
//        char* rtn = NULL;
//        jclass clsstring = env->FindClass("java/lang/String");
//        jstring strencode = env->NewStringUTF("utf-8");
//        jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
//        jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
//        jsize alen = env->GetArrayLength(barr);
//        jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
//        if (alen > 0)
//        {
//            rtn = (char*)malloc(alen + 1);
//            memcpy(rtn, ba, alen);
//            rtn[alen] = 0;
//        }
//        env->ReleaseByteArrayElements(barr, ba, 0);
//        return rtn;
//    }

    JNIEXPORT void JNICALL Java_com_insul_poker_AppActivity_sendDeviceToken(JNIEnv* env, jobject thiz,jstring deviceToken)
    {
        CCLOGERROR("%s", "Java_com_insul_poker_AppActivity_sendDeviceToken");
        PushManager::getInstance()->m_strDeviceToken = tools::jstringTostring(env,deviceToken);
        CCLOGERROR("%s", PushManager::getInstance()->m_strDeviceToken.c_str());
        PushManager::getInstance()->SendDeviceToken();
    }
}
#endif
PushManager* PushManager::__push_manager__;

PushManager::PushManager()
{
    m_bIsLogin = false;
    m_bPushLogin = false;
}
PushManager::~PushManager()
{
    destroy();
}
void PushManager::destroy()
{

    delete __push_manager__;
    __push_manager__ = nullptr;

}
PushManager* PushManager::getInstance() {
    if(!__push_manager__) {
        __push_manager__ = new PushManager();
    }
    return __push_manager__;
}

void PushManager::GoToPokerRoom()
{
    if(room_id.empty())
    {
        return;
    }
//    auto it = GDM->get_groups().find(atoi(group_id.c_str()));
//    if (it != GDM->get_groups().end())
//    {
//        GDM->set_select_group(it->second);
//    }
    PDM->send_enter_room(atoi(room_id.c_str()));
    this->room_id = "";

}

void PushManager::HandleRecIOSMsg(uint32_t room_id,uint32_t group_id)
{
    //GDM->send_get_room_info();
}

void PushManager::SetAppStatusAndroid(int status)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    //告诉MyApplication已启动
    if (JniHelper::getStaticMethodInfo(t, "com/insul/poker/MyApplication", "setAppStatus", "(I)V"))
    {
        t.env->CallStaticVoidMethod(t.classID,t.methodID,status);
    }
#endif
}
void PushManager::HandleRecMsg()
{
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
     JniMethodInfo t;
    
    //获取java存储的推送数据，如果有
    if (JniHelper::getStaticMethodInfo(t, "com/insul/poker/MyApplication", "getPushData", "()Ljava/lang/String;"))
    {
        jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
        std::string str = JniHelper::jstring2string(jstr);
        cocos2d::log("jni-java函数执行完毕,得到的stsring：%s",str.c_str());
        room_id = str;
    }
    if (JniHelper::getStaticMethodInfo(t, "com/insul/poker/MyApplication", "getGroupID", "()Ljava/lang/String;"))
    {
        jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
        std::string str = JniHelper::jstring2string(jstr);
        cocos2d::log("jni-java函数执行完毕,得到的stsring：%s",str.c_str());
        
    }
    if (!room_id.empty() )
    {
        GoToPokerRoom();
//        GoToPokerRoom(atoi(room_id.c_str()), atoi(group_id.c_str()));
    }
#endif
}

void PushManager::GoToGroup()
{

}

void PushManager::SendDeviceToken()
{
    if (this->m_strDeviceToken.empty())
    {
        cocos2d::log("%s","read devicetoken");
        m_strDeviceToken = UserDefault::getInstance()->getStringForKey("DeviceTokenX");
    }
    else
    {
        UserDefault::getInstance()->setStringForKey("DeviceTokenX", m_strDeviceToken);
    }
    if (!this->m_strDeviceToken.empty() && m_bIsLogin)
    {
        while(m_strDeviceToken.find('<') != string::npos)
        {
            m_strDeviceToken.erase(m_strDeviceToken.find('<'),1);
        }
        while(m_strDeviceToken.find(' ') != string::npos)
        {
            m_strDeviceToken.erase(m_strDeviceToken.find(' '),1);
        }
        while(m_strDeviceToken.find('>') != string::npos)
        {
            m_strDeviceToken.erase(m_strDeviceToken.find('>'),1);
        }
        ::msg::Platform nPlatformID = ::msg::Platform::IOS;//默认ios
        if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        {
            nPlatformID = ::msg::Platform::ANDORID;//android
        }
        //TODO:send device token
        auto processor = PM->devicetoken_up(m_strDeviceToken);
        net_manager::shared_instence().send_data(processor,REQ_SEND_DEVICE_ID);
        this->m_strDeviceToken = "";
    }
}