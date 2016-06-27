#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "TextViewInterface.h"
#include "utils.h"
#include <jni.h>
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>
using namespace cocos2d;


TextViewInterface::TextViewInterface()
{
    cocos2d::log("TextViewInterface");
    //创建输入框，获取id
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/insul/poker/TextViewAndroid", "createTextView", "()I")) {
        control_id = t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
//    cocos2d::log(("control id : " + tools::to_string(control_id)).c_str());
}

TextViewInterface::~TextViewInterface()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/insul/poker/TextViewAndroid", "removeTextView", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void TextViewInterface::setString(const std::string& text)
{
    cocos2d::log("setString:%s", text.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/insul/poker/TextViewAndroid", "set_string", "(ILjava/lang/String;)V")) {
        jstring stringArg1 = t.env->NewStringUTF(text.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, stringArg1);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void TextViewInterface::setPosition(const cocos2d::Point& pos)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    float height = cocos2d::Director::getInstance()->getWinSize().height;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/insul/poker/TextViewAndroid", "set_position", "(III)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, (int)pos.x, (int)(pos.y));
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void TextViewInterface::setContentSize(const cocos2d::Size& size)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/insul/poker/TextViewAndroid", "set_size", "(III)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, (int)size.width, (int)size.height);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}


#endif
