//
//  PlatformSdk.cpp
//  pokerclient
//
//  Created by qqronaldz on 16/4/18.
//
//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "PlatformSdk.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "jni.h"
#include "jni/JniHelper.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include "LayerFriendContactsList.h"
#include "json/rapidjson.h"
#include "json/document.h"

PlatformSdk *PlatformSdk::mPlatformSdk = nullptr;

PlatformSdk::PlatformSdk()
{
    
}

PlatformSdk::~PlatformSdk()
{
    
}

/*
 * java ------> c++
 */
extern "C"
{
    //返回android通讯录数据
    JNIEXPORT void JNICALL Java_com_insul_poker_AppActivity_allContents(JNIEnv *env, jobject thiz, jstring message)
    {
        auto layer = dynamic_cast<LayerFriendContactsList*>(SceneManager::get_layer_by_name(typeid(LayerFriendContactsList).name()));
        if(layer)
        {
            std::string tmpMessage = JniHelper::jstring2string(message);
            std::vector<LayerFriendContactsList::Contacts> mContacts;
            
            //json parse
            rapidjson::Document dJson;
            dJson.Parse<0>(tmpMessage.c_str());
            if (dJson.HasParseError())  //打印解析错误
            {
                CCLOG("GetParseError %s\n",dJson.GetParseError());
                return;
            }
            
            if (dJson.IsObject() && dJson.HasMember("all_contacts"))
            {
                const rapidjson::Value& allContactsJson = dJson["all_contacts"];
                for (rapidjson::SizeType i = 0; i < allContactsJson.Size(); ++i)
                {
                    
                    LayerFriendContactsList::Contacts tmpContacts;
                    const rapidjson::Value &nameJson = allContactsJson[i]["name"];
                    const rapidjson::Value &allTelephoneJson = allContactsJson[i]["all_telephone"];
                    
                    
                    tmpContacts.name = nameJson.GetString();
                    if(allTelephoneJson.IsArray())
                    {
                        for (rapidjson::SizeType j = 0; j < allTelephoneJson.Size(); ++j)
                        {
                            const rapidjson::Value &telephoneJson = allTelephoneJson[j]["telephone"];
                            const std::string telephone = telephoneJson.GetString();
                            
                            tmpContacts.allTelephone.push_back(telephone);
                        }
                    }
                    mContacts.push_back(tmpContacts);
                    
                }
            }
            layer->setContacts(mContacts);
            
        }
    }
}



/*
 * c++ ------> java
 */

//读取通讯录
bool PlatformSdk::getPhoneContacts()
{
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, "com/insul/poker/AppActivity", "GetAppActivity", "()Ljava/lang/Object;"))
    {
        
        jobject obj= t.env->CallStaticObjectMethod(t.classID,t.methodID);
        if (JniHelper::getMethodInfo(t, "com/insul/poker/AppActivity", "ReadAllContacts", "()V"))
        {
            t.env->CallVoidMethod(obj,t.methodID);
        }
        
    }
	return true;
}

//发送短信
void PlatformSdk::showMessageView(std::string phone, std::string title, std::string msg)
{
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, "com/insul/poker/AppActivity", "GetAppActivity", "()Ljava/lang/Object;"))
    {
        
        jobject obj= t.env->CallStaticObjectMethod(t.classID,t.methodID);
        if (JniHelper::getMethodInfo(t, "com/insul/poker/AppActivity", "sendMessageView", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
        {
            jstring jPhone = t.env->NewStringUTF(phone.c_str());
            jstring jTitle = t.env->NewStringUTF(title.c_str());
            jstring jMsg = t.env->NewStringUTF(msg.c_str());
            t.env->CallVoidMethod(obj, t.methodID, jPhone, jTitle, jMsg);
            t.env->DeleteLocalRef(jPhone);
            t.env->DeleteLocalRef(jTitle);
            t.env->DeleteLocalRef(jMsg);
            t.env->DeleteLocalRef(t.classID);
        }
        
    }
    
}


void PlatformSdk::shareWeixin(TargetShare targetShare,  ShareType shareType, std::string title, std::string description, std::string linkUrl, std::string image)
{
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, "com/insul/poker/AppActivity", "GetAppActivity", "()Ljava/lang/Object;"))
    {
        
        jobject obj= t.env->CallStaticObjectMethod(t.classID,t.methodID);
        if (JniHelper::getMethodInfo(t, "com/insul/poker/AppActivity", "shareWeixin", "(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
        {
            
            jstring jTitle = t.env->NewStringUTF(title.c_str());
            jstring jDescription = t.env->NewStringUTF(description.c_str());
            jstring jLinkUrl = t.env->NewStringUTF(linkUrl.c_str());
            jstring jImage = t.env->NewStringUTF(image.c_str());
            t.env->CallVoidMethod(obj, t.methodID, (int)targetShare, (int)shareType, jTitle, jDescription, jLinkUrl, jImage);
            t.env->DeleteLocalRef(jTitle);
            t.env->DeleteLocalRef(jDescription);
            t.env->DeleteLocalRef(jLinkUrl);
            t.env->DeleteLocalRef(jImage);
            t.env->DeleteLocalRef(t.classID);
        }
        
    }
}


PlatformSdk::LanguageType PlatformSdk::getCurrentLanguage()
{
    
    std::string country("");
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, "com/insul/poker/AppActivity", "GetAppActivity", "()Ljava/lang/Object;"))
    {
        
        jobject obj= t.env->CallStaticObjectMethod(t.classID,t.methodID);
        if (JniHelper::getMethodInfo(t, "com/insul/poker/AppActivity", "getCurrentCountry", "()Ljava/lang/String;"))
        {
            jstring str = (jstring)t.env->CallObjectMethod(obj, t.methodID);
            t.env->DeleteLocalRef(t.classID);
            country = JniHelper::jstring2string(str);
            t.env->DeleteLocalRef(str);
        }
        
    }
    
    std::string languageName = getCurrentLanguageJNI();
    const char* pLanguageName = languageName.c_str();
    const char* pCountry = country.c_str();
//    log("PlatformSdk =====> language = %s, country = %s", pLanguageName, pCountry);
    LanguageType ret = LanguageType::ENGLISH;
    
    if (0 == strcmp("zh", pLanguageName))
    {
        if(0 == strcmp("TW", pCountry) || 0 == strcmp("UK", pCountry))
        {
            ret = LanguageType::TRADITIONAL_CHINESE;
//            log("繁体中文");
        }
        else
        {
            ret = LanguageType::CHINESE;
//            log("简体中文");
        }
    }
    else if (0 == strcmp("en", pLanguageName))
    {
        ret = LanguageType::ENGLISH;
    }
    else if (0 == strcmp("fr", pLanguageName))
    {
        ret = LanguageType::FRENCH;
    }
    else if (0 == strcmp("it", pLanguageName))
    {
        ret = LanguageType::ITALIAN;
    }
    else if (0 == strcmp("de", pLanguageName))
    {
        ret = LanguageType::GERMAN;
    }
    else if (0 == strcmp("es", pLanguageName))
    {
        ret = LanguageType::SPANISH;
    }
    else if (0 == strcmp("ru", pLanguageName))
    {
        ret = LanguageType::RUSSIAN;
    }
    else if (0 == strcmp("nl", pLanguageName))
    {
        ret = LanguageType::DUTCH;
    }
    else if (0 == strcmp("ko", pLanguageName))
    {
        ret = LanguageType::KOREAN;
    }
    else if (0 == strcmp("ja", pLanguageName))
    {
        ret = LanguageType::JAPANESE;
    }
    else if (0 == strcmp("hu", pLanguageName))
    {
        ret = LanguageType::HUNGARIAN;
    }
    else if (0 == strcmp("pt", pLanguageName))
    {
        ret = LanguageType::PORTUGUESE;
    }
    else if (0 == strcmp("ar", pLanguageName))
    {
        ret = LanguageType::ARABIC;
    }
    else if (0 == strcmp("nb", pLanguageName))
    {
        ret = LanguageType::NORWEGIAN;
    }
    else if (0 == strcmp("pl", pLanguageName))
    {
        ret = LanguageType::POLISH;
    }
    else if (0 == strcmp("tr", pLanguageName))
    {
        ret = LanguageType::TURKISH;
    }
    else if (0 == strcmp("uk", pLanguageName))
    {
        ret = LanguageType::UKRAINIAN;
    }
    else if (0 == strcmp("ro", pLanguageName))
    {
        ret = LanguageType::ROMANIAN;
    }
    else if (0 == strcmp("bg", pLanguageName))
    {
        ret = LanguageType::BULGARIAN;
    }
    return ret;
}
#endif
