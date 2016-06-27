//
//  VoiceRecorderAndroid.cpp
//  pokerclient
//
//  Created by MirrorMiko on 6/7/15.
//
//

#include "VoiceRecorderAndroid.h"
#include "cocos2d.h"
#include <memory>
#include "GameDataManager.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>
std::shared_ptr<VoiceRecorderInterface> __createRecorder()
{
    return std::shared_ptr<VoiceRecorderInterface>(new VoiceRecorderAndroid());
}
#endif

VoiceRecorderAndroid::VoiceRecorderAndroid()
{
}

VoiceRecorderAndroid::~VoiceRecorderAndroid()
{
}

void VoiceRecorderAndroid::beginRecordByFileName(const std::string& filename)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    using namespace cocos2d;
    std::string path = GDM->get_user_directory()+filename+".amr";
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/VoiceRecorder", "start_record", "(Ljava/lang/String;)V")) {
        jstring stringArg1 = t.env->NewStringUTF(path.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

int VoiceRecorderAndroid::stopRecord()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    using namespace cocos2d;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/VoiceRecorder", "stop_record", "()I")) {
        jint i = t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return i;
    }
#endif
    return 0;
}

int VoiceRecorderAndroid::amrToWav(const std::string& amrPath, const std::string& savePath)
{
    return 0;
}

int VoiceRecorderAndroid::wavToAmr(const std::string& wavPath, const std::string& savePath)
{
    return 0;
}