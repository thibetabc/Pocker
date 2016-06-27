//
//  macro.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//
#ifndef pokerclient_macro_h
#define pokerclient_macro_h

template <class T>
class SINGLETON {
protected:
    SINGLETON(){}
    SINGLETON(const SINGLETON& __) {}
    SINGLETON& operator = (const SINGLETON& __){}
public:
    static T& shared_instence(){
        static T __instence;
        return __instence;
    }
};

#define LANGUAGE_EN 0
#define LANGUAGE_ZH 1
#define CLIENT_BUILD_CODE 10

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define PLATFORM_CODE 1
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define PLATFORM_CODE 2
#else
#define PLATFORM_CODE 255
#endif

#endif
