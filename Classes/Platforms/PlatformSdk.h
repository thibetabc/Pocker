//
//  PlatformSdk.hpp
//  pokerclient
//  平台相关接口
//  Created by qqronaldz on 16/4/18.
//
//

#ifndef PlatformSdk_hpp
#define PlatformSdk_hpp

#include <stdio.h>
#include "cocos2d.h"


#define PS PlatformSdk::getInstance()

class PlatformSdk
{
public:
    static PlatformSdk *getInstance()
    {
        if(!mPlatformSdk)
        {
            mPlatformSdk = new PlatformSdk();
        }
        return mPlatformSdk;
    }
    static void destory()
    {
        if(mPlatformSdk)
        {
            delete mPlatformSdk;
            mPlatformSdk = nullptr;
        }
    }
    
    //分享平台
    enum TargetShare
    {
        WeiXinWeChat,
        WeiXinCircle,
    };
    //分享类型
    enum ShareType
    {
        Text,  //文字类型
        Image, //图片类型
        Music, //音乐类型
        Video, //视频类型
        Web,   //网页类型
        
    };
    enum class LanguageType
    {
        ENGLISH = 0,
        CHINESE,
        TRADITIONAL_CHINESE,
        FRENCH,
        ITALIAN,
        GERMAN,
        SPANISH,
        DUTCH,
        RUSSIAN,
        KOREAN,
        JAPANESE,
        HUNGARIAN,
        PORTUGUESE,
        ARABIC,
        NORWEGIAN,
        POLISH,
        TURKISH,
        UKRAINIAN,
        ROMANIAN,
        BULGARIAN
    };
public:
    static bool getPhoneContacts();
    static void showMessageView(std::string phone, std::string title, std::string msg);
    static void shareWeixin(TargetShare targetShare,  ShareType shareType, std::string title, std::string description, std::string linkUrl, std::string image);
    LanguageType getCurrentLanguage();
private:
    PlatformSdk();
    ~PlatformSdk();
private:
    static PlatformSdk *mPlatformSdk;
    TargetShare mTargetShare;
    

    
};
#endif /* PlatformSdk_hpp */
