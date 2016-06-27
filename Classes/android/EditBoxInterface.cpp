//
//  EditBoxInterface.cpp
//  pokerclient
//
//  Created by MirrorMiko on 6/13/15.
//
//

#include "EditBoxInterface.h"
#include "utils.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>
using namespace cocos2d;

//extern "C"{
//JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_changeHeadPic(JNIEnv* env, jobject thiz, jstring name)
//{
//    const char* pname = env->GetStringUTFChars(name, NULL);
//    CCLOGERROR("Java_org_cocos2dx_cpp_AppActivity_changeHeadPic:%s",pname);
//    std::string namec = pname;
//    namec = namec.substr(namec.rfind('/')+1, namec.npos);
//    CCLOGERROR("name:%s",namec.c_str());
//    b->change_head_pic(namec);
//}
//}

#endif

void a_log(const char *format, va_list args)
{
    char buf[cocos2d::MAX_LOG_LENGTH];
    
    vsnprintf(buf, cocos2d::MAX_LOG_LENGTH-3, format, args);
    strcat(buf, "\n");
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    __android_log_print(ANDROID_LOG_ERROR, "cpp",  "%s", buf);
#endif
}


void aa_log(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    a_log(format, args);
    va_end(args);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define ANDROIDLOG(format,...)  aa_log(format, ##__VA_ARGS__)
#else
#define ANDROIDLOG(format,...)  cocos2d::log(format, ##__VA_ARGS__)
#endif



EditBoxInterface::EditBoxInterface()
:control_id(-1)
{
    ANDROIDLOG("EditBoxInterface");
    //创建输入框，获取id
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "createEditBox", "()I")) {
        control_id = t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    ANDROIDLOG(("control id : " + tools::to_string(control_id)).c_str());
}

EditBoxInterface::~EditBoxInterface(){
    ANDROIDLOG("~EditBoxInterface");
    //根据id，移除输入框
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "removeEditBox", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

bool EditBoxInterface::is_focus(){
    ANDROIDLOG("is_focus");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "is_focus", "(I)Z")) {
        bool result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, control_id);
        t.env->DeleteLocalRef(t.classID);
        return result;
    }
#endif
    return false;
}

bool EditBoxInterface::is_password(){
    ANDROIDLOG("is_password");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "is_password", "(I)Z")) {
        bool result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, control_id);
        t.env->DeleteLocalRef(t.classID);
        return result;
    }
#endif
    return false;
}

bool EditBoxInterface::is_hidden(){
    ANDROIDLOG("is_hidden");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "is_hidden", "(I)Z")) {
        
        bool result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, control_id);
        t.env->DeleteLocalRef(t.classID);
        return result;
    }
#endif
    return false;
}

std::string EditBoxInterface::font_name(){
    return "";
}


void EditBoxInterface::set_text(const std::string& text){
    ANDROIDLOG("set_text:%s", text.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_text", "(ILjava/lang/String;)V")) {
        jstring stringArg1 = t.env->NewStringUTF(text.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, stringArg1);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_text_color(int red, int blue, int green){
    ANDROIDLOG("set_text_color:r-%d b-%d g-%d", red, blue, green);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_text_color", "(IIII)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, red, blue, green);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_background_color(int red, int blue, int green){
    ANDROIDLOG("set_background_color:r-%d b-%d g-%d", red, blue, green);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_background_color", "(IIII)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, red, blue, green);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_keyborad_type(int type){
    ANDROIDLOG("set_keyborad_type:%d", type);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_keyborad_type", "(II)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, type);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_input_flag(int flag){
    ANDROIDLOG("set_input_flag:%d", flag);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_input_flag", "(II)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, flag);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_return_type(int type){
    ANDROIDLOG("set_return_type:%d", type);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_return_type", "(II)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, type);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_keyborad_input_flag(int type)
{
    ANDROIDLOG("set_keyborad_input_flag:%d", type);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_keyboard_input_flag", "(II)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, type);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_mult_line(bool able){
    ANDROIDLOG("set_editable:%d", able);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_mult_line", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, able);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_editable(bool able){
    ANDROIDLOG("set_editable:%d", able);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_editable", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, able);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

bool EditBoxInterface::is_editable()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "is_editable", "(I)Z")) {
        bool result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, control_id);
        t.env->DeleteLocalRef(t.classID);
        return result;
    }
#endif
    return false;
}

void EditBoxInterface::set_visible(bool visible){
    ANDROIDLOG("set_visible:%d", visible);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_visible", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, visible);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_placeholder(const std::string& text){
    ANDROIDLOG("set_placeholder:%s", text.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_placeholder", "(ILjava/lang/String;)V")) {
        jstring stringArg1 = t.env->NewStringUTF(text.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, stringArg1);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_size(float w, float h){
    ANDROIDLOG("set_size:%f-%f", w, h);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_size", "(III)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, (int)w, (int)h);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::set_position(float x, float y){
    float height = cocos2d::Director::getInstance()->getWinSize().height;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "set_position", "(III)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id, (int)x, (int)(y));
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void setMultiLine(bool){
    
}

//std::string jstring2string(JNIEnv* env, jstring jstr) {
//    const char *nativeString = env->GetStringUTFChars(jstr, JNI_FALSE);
//    std::string s(nativeString);
//    env->ReleaseStringUTFChars(jstr, nativeString);
//    return std::move(s);
//}

std::string EditBoxInterface::text(){
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "text", "(I)Ljava/lang/String;")) {
        jstring string = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, control_id);
        if(string) {
            const char* s = t.env->GetStringUTFChars(string, JNI_FALSE);
            
            if (s) {
                ANDROIDLOG("text:%s", s);
                std::string result(s);
                t.env->ReleaseStringUTFChars(string, s);
                ANDROIDLOG("result:%s", result.c_str());
                t.env->DeleteLocalRef(t.classID);
                return result;
            }
            else {
            }
        }
    }
    else {
    }
#endif
    return "";
}


void EditBoxInterface::open_keyborad(){
    ANDROIDLOG("open_keyborad");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "open_keyborad", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void EditBoxInterface::close_keyborad(){
    ANDROIDLOG("close_keyborad");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/EditBoxPool", "close_keyborad", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, control_id);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void EditBoxInterface::doAnimationWhenKeyboardMoveWithDuration(float duration, float distence){
    
}