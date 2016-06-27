//
//  versionCheck.cpp
//  pokerclient
//
//  Created by admin on 15-9-18.
//
//

#include "versionCheck.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "jni/JniHelper.h"
#include <android/log.h>
#endif

void versionCheck::update_version()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"itms://itunes.apple.com/cn/app/yue-ma-yue-ba/id1029725698?mt=8"]];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return;
    JniMethodInfo t;
    
    //获取java存储的推送数据，如果有
    if (JniHelper::getStaticMethodInfo(t, "com/insul/poker/MyApplication", "getPushData", "()Ljava/lang/String;"))
    {
        jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
        std::string str = JniHelper::jstring2string(jstr);
        cocos2d::log("jni-java函数执行完毕,得到的stsring：%s",str.c_str());
        room_id = str;
    }

#endif

}