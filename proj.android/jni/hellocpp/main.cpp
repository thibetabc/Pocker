#include "AppDelegate.h"
#include "cocos2d.h"
#include <jni.h>
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"
#include "PushManager.h"
#include "LoginManager.h"
//#include "HudLayer.h"
//#include <iostream>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

void cocos_android_app_init (JNIEnv* env, jobject thiz) {
    AppDelegate *pAppDelegate = new AppDelegate();
}
extern "C"
{
    void Java_org_cocos2dx_cpp_MyApplication_JChangeToScene(JNIEnv* env, jobject thiz)
	{
//		LOGD("Java_org_cocos2dx_cpp_ChangeToScene");
//		cocos2d::log("Java_org_cocos2dx_cpp_ChangeToScene");
//		LM->login("guojiabin", "123");
//		PushManager::getInstance()->GoToScene(1);
//		Director::getInstance()->end();//replaceScene(HudLayer::createScene(false,HudLayer::TAG_FRIENDS));
	}
    void Java_org_cocos2dx_cpp_MyApplication_DealWithPushMessage(JNIEnv* env, jobject thiz)
    	{
    		PushManager::getInstance()->HandleRecMsg();
    	}
}
