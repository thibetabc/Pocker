#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>
using namespace cocos2d;

#include "SoundPlayerInterface.h"

void SoundPlayerInterface::play_sound(const std::string& path)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/SoundPlayer", "play_sound", "(Ljava/lang/String;)V")) {
        jstring stringArg1 = t.env->NewStringUTF(path.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}
void SoundPlayerInterface::stop_play()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/SoundPlayer", "stop_play", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}
bool SoundPlayerInterface::is_playing()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/SoundPlayer", "is_playing", "()Z")) {
        bool result = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return result;
    }
    return false;
}

#endif