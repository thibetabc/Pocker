//
//  ImagePickerAndroid.cpp
//  pokerclient
//
//  Created by MirrorMiko on 6/8/15.
//
//

#include "ImagePickerAndroid.h"
#include "cocos2d.h"
#include "utils.h"
#include "GameDataManager.h"
#include "utils.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>
static ImagePickerBridge* b = NULL;
//std::shared_ptr<ImagePickerBridge> __createImagePicker(const ImagePickerType type)
//{
//    b = new ImagePickerAndroid(type);
//    return std::shared_ptr<ImagePickerBridge>(b);
//}

ImagePickerBridge *__createImagePicker(const ImagePickerType type)
{
    b = new ImagePickerAndroid(type);
    return b;
}
extern "C"{
JNIEXPORT void JNICALL Java_com_insul_poker_AppActivity_changeHeadPic(JNIEnv* env, jobject thiz, jstring name)
{
    const char* pname = env->GetStringUTFChars(name, NULL);
    CCLOGERROR("Java_com_insul_poker_AppActivity_changeHeadPic:%s",pname);
    std::string namec = pname;
    namec = namec.substr(namec.rfind('/')+1, namec.npos);
    CCLOGERROR("name:%s",namec.c_str());
    b->change_head_pic(namec);
}
}
#endif

ImagePickerAndroid::ImagePickerAndroid(const ImagePickerType type){
    std::string head_pic = "head_pic.png";
    long t_ = tools::date();
    head_pic = tools::to_string(GDM->get_user_id()) + "_" + tools::to_string(t_) + "_" + head_pic;
    head_pic = GDM->get_user_directory() + head_pic;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    using namespace cocos2d;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/insul/poker/AppActivity", "start_capture", "(Ljava/lang/String;I)V")) {
        jstring stringArg1 = t.env->NewStringUTF(head_pic.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, type);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}