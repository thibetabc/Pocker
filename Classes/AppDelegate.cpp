#include "AppDelegate.h"
#include "SceneManager.h"
#include "game_manager.h"
#include "LoginManager.h"
#include "net_manager.h"
#include "ImagePickerBridge.h"
#include "PushManager.h"
#include "NET_CMD.h"
#include "global_header.h"
#include "AudioManager.h"
#include "GameDataManager.h"

//#include "CCUMSocialSDK.h"
//#include "CCUMTypeDef.h" //share
//#include "Header.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "ImagePickerBridge.h"
#include "jni/JniHelper.h"
#include <android/log.h>
#include "../cocos2d/external/libmobclickcpp/include/MobClickCpp.h"

#else
#include "UMstatistics.h"
#endif

#define LAUNCHED 1
#define UNLANCHED 0

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    
    // 判断app是否为第一次运行
    if(UserDefault::getInstance()->getBoolForKey("first_time", true))
    {
        UserDefault::getInstance()->setBoolForKey("first_time", false);
        UserDefault::getInstance()->setBoolForKey("is_newcard", false);
        
        UserDefault::getInstance()->setBoolForKey("show_group_tutor", true);
        UserDefault::getInstance()->setBoolForKey("show_friend_tutor" , true);
        UserDefault::getInstance()->setBoolForKey("show_game_tutor", true);
        UserDefault::getInstance()->setBoolForKey("show_moment_tutor", true);
        UserDefault::getInstance()->setBoolForKey("show_setting_tutor", true);
        
        UserDefault::getInstance()->setFloatForKey("music_volume", 0.5);
        UserDefault::getInstance()->setFloatForKey("effects_volume", 0.5);
        
        // 第一次运行时如果XML文件不存在就会新建
        UserDefault::getInstance()->flush();
    }
    

    
    
//    PushManager::getInstance()->HandleRecMsg();
//    //app启动，设置状态
//    PushManager::getInstance()->SetAppStatusAndroid(LAUNCHED);
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 30);  // from 60-30 by ronaldz. to reduce power usage. 

    register_all_packages();

    glview->setDesignResolutionSize(750, 1334, ResolutionPolicy::EXACT_FIT);
    
    std::vector<std::string> vecSearchPaths;
    vecSearchPaths.push_back("res");
    vecSearchPaths.push_back("res/login_res");
    vecSearchPaths.push_back("res/login_res/game_res/effect");
    vecSearchPaths.push_back("res/image");
    vecSearchPaths.push_back("res/csb");
    vecSearchPaths.push_back("res/Default");
    vecSearchPaths.push_back("res/mahjong");
    vecSearchPaths.push_back("res/image/Mahjong_game_res");
    cocos2d::FileUtils::getInstance()->setSearchPaths(vecSearchPaths);
    
    game_manager::shared_instence().init();
    
    auto default_account = LM->get_default_account();
    bool online = false;
    if(default_account) {
        online = default_account->get_status_login();
    }
    auto s = Scene::create();
    Director::getInstance()->runWithScene(s);
    net_manager::shared_instence().GetLoginServer();
    if (online && net_manager::shared_instence().get_server_data_with_request_type(REQ_VERSIONCHECK) != nullptr){
        SceneManager::addWelcomeLayer();
    }
    else {
        SceneManager::addLoginLayer();
    }
    //友盟统计
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("557f7d5767e58e9cc9001d65", "test_chanel");
#endif
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    GDM->set_IsInBackGround(true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    umeng::MobClickCpp::applicationDidEnterBackground();
#endif
    Director::getInstance()->stopAnimation();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    net_manager::shared_instence().disconnect_all();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //app后台，设置状态
    PushManager::getInstance()->SetAppStatusAndroid(UNLANCHED);
//    if(!ImagePickerBridge::take_photo) {
//        exit(0);
//    }
#endif
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    GDM->set_IsInBackGround(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    umeng::MobClickCpp::applicationWillEnterForeground();
#endif
    Director::getInstance()->startAnimation();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    net_manager::shared_instence().connect_all();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //app启动，设置状态
    PushManager::getInstance()->SetAppStatusAndroid(LAUNCHED);
#endif    
    PushManager::getInstance()->HandleRecMsg();

    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::applicationScreenSizeChanged(int newWidth, int newHeight) {
    cocos2d::log("AppDelegate::applicationScreenSizeChanged:%d, %d", newWidth, newHeight);
    cocos2d::Application::applicationScreenSizeChanged(newWidth, newHeight);
    
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    auto designResolutionSize = glview->getDesignResolutionSize();
    if (glview != nullptr) {
        glview->setFrameSize(newWidth, newHeight);
        // Set the design resolution
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, glview->getResolutionPolicy());
    }
}

