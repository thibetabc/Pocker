//
//  LanscapeAndroid.cpp
//  pokerclient
//
//  Created by MirrorMiko on 6/29/15.
//
//
#include "global_header.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>
#include "cocos2d.h"
#endif
using namespace cocos2d;

void landscape(){
    //创建输入框，获取id
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "landscape", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
    
    auto director = cocos2d::Director::getInstance();
    auto glview = director->getOpenGLView();
    
    glview->setDesignResolutionSize(1136, 640, ResolutionPolicy::EXACT_FIT);
    
#endif
}

void portrait(){
    //创建输入框，获取id
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "portrait", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }
    
    auto director = cocos2d::Director::getInstance();
    auto glview = director->getOpenGLView();
    
    glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, ResolutionPolicy::EXACT_FIT);
#endif
}