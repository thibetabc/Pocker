/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "SceneManager.h"
#import "PokerDataManager.h"
#import "PushManager.h"
#import "WXApi.h"
#import "WXApiObject.h"
#import "PokerDataManager.h"
#import "MobClick.h"
#import "UMstatistics.h"

#define WXAppId          @"wx2109735a7ec9925b"               //APPID
#define WXPartnerID      @"1355802302"
#define PDM PokerDataManager::getInstance()

const string app_eventid = "app_play";
@interface AppController ()<WXApiDelegate>

@end

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle


// cocos2d application instance
static AppDelegate s_sharedApplication;



- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    

    //[MobClick setLogEnabled:YES];
    
    /*Class cls = NSClassFromString(@"UMANUtil");
    SEL deviceIDSelector = @selector(openUDIDString);
    NSString *deviceID = nil;
    if(cls && [cls respondsToSelector:deviceIDSelector]){
        deviceID = [cls performSelector:deviceIDSelector];
    }
    NSData* jsonData = [NSJSONSerialization dataWithJSONObject:@{@"oid" : deviceID}
                                                       options:NSJSONWritingPrettyPrinted
                                                         error:nil];
    
    NSLog(@"%@", [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding]);*/
    
    
    [MobClick startWithAppkey:@"556d5eaa67e58e5fcd001173" reportPolicy:BATCH   channelId:@"App Store"];
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];

    // Init the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                         pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];

    // Use RootViewController manage CCEAGLView 
    _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    _viewController.wantsFullScreenLayout = YES;
    _viewController.view = eaglView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }

    [window makeKeyAndVisible];
    
    float cur = [[[UIDevice currentDevice] systemVersion] floatValue];
    NSLog(@"======%f",cur);
    //注册推送通知（注意iOS8注册方法发生了变化）
    if (cur >= 8) {
        [application registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert|UIUserNotificationTypeBadge|UIUserNotificationTypeSound categories:nil]];
        [application registerForRemoteNotifications];
    }
    else
    {
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes: UIRemoteNotificationTypeBadge |UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert];
    }
    application.applicationIconBadgeNumber = 0;
    //*******************微信支付**********************
    
    // 向微信终端注册appID
    [WXApi registerApp:WXAppId withDescription:@"demo 2.0"];
    /*! @brief WXApi的成员函数，在微信终端程序中注册第三方应用。
     *
     * 需要在每次启动第三方应用程序时调用。第一次调用后，会在微信的可用应用列表中出现。
     * @param appid 微信开发者ID
     * @param appdesc 应用附加信息，长度不超过1024字节
     * @return 成功返回YES，失败返回NO。
     */
    
    //************************************************
    
    //解析推送内容
    NSDictionary* remoteNotification = [launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey];
    if (remoteNotification != NULL)
    {
        if([remoteNotification objectForKey:@"type"] != NULL)
        {
            std::string pushType = [[remoteNotification objectForKey:@"type"] cStringUsingEncoding: NSUTF8StringEncoding];
            if (pushType == "1")
            {
                PushManager::getInstance()->m_bPushLogin = true;
                if([remoteNotification objectForKey:@"room_id"] != NULL)
                {
                    PushManager::getInstance()->room_id = [[remoteNotification objectForKey:@"room_id"] cStringUsingEncoding: NSUTF8StringEncoding];
                }
            }
        }
    }
    
    [self setSkipBackupFolder];
    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);

    app->run();

    return YES;
}


#pragma mark 注册推送通知之后
//在此接收设备令牌
/** 接收从苹果服务器返回的唯一的设备token，然后发送给自己的服务端*/
-(void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken{
    
    [self addDeviceToken:deviceToken];
    NSLog(@"device token:%@",deviceToken);
    
}


#pragma mark 获取device token失败后
-(void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error{
    NSLog(@"didFailToRegisterForRemoteNotificationsWithError:%@",error.localizedDescription);
    [self addDeviceToken:nil];
}

std::string pRoomID = "";
std::string pGroupID = "";
std::string room_ip = "";
std::string room_port ="";
#pragma mark 接收到推送通知之后
-(void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo{
    NSLog(@"receiveRemoteNotification,userInfo is %@",userInfo);
    //把icon上的标记数字设置为0,
    application.applicationIconBadgeNumber = 0;
    if ([userInfo objectForKey:@"type"] != NULL)
    {
        std::string pushType = [[userInfo objectForKey:@"type"] cStringUsingEncoding: NSUTF8StringEncoding];
        //进入房间
        if (pushType == "1")
        {
            PushManager::getInstance()->m_bPushLogin = false;
        }
        else
        {
            return;
        }
    }
    if ([userInfo objectForKey:@"room_id"] != NULL )
    {
//        NSDictionary *s = [userInfo objectForKey:@"aps"];
        PushManager::getInstance()->room_id = [[userInfo objectForKey:@"room_id"] cStringUsingEncoding: NSUTF8StringEncoding];
        if(application.applicationState == UIApplicationStateActive)
        {
            //push in game is useless
//            UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"新消息提示"
//                                                                message:[NSString stringWithFormat:@"%@",
//                                                                         [[s objectForKey:@"alert"]objectForKey:@"body"]]
//                                                               delegate:self
//                                                      cancelButtonTitle:@"取消"
//                                                      otherButtonTitles:@"确定", nil];
//            
//            
//            
//            [alertView show];
//            [alertView release];
            
        }
        
        else if(!PushManager::getInstance()->m_bPushLogin)
        {
            
            PushManager::getInstance()->GoToPokerRoom();
        }
    }
    
}
-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(buttonIndex == 1)
    {
        PushManager::getInstance()->GoToPokerRoom();
    }
    
}
#pragma mark - 私有方法
/**
 *  添加设备令牌到服务器端
 *
 *  @param deviceToken 设备令牌
 */
-(void)addDeviceToken:(NSData *)deviceToken{
    NSString *key=@"DeviceToken";
    NSData *oldToken=   [[NSUserDefaults standardUserDefaults]objectForKey:key];

    NSString *s = @"";
    if (deviceToken != NULL)
    {
        s = [NSString stringWithFormat:@"%@",deviceToken];
    }
    PushManager::getInstance()->m_strDeviceToken =[s UTF8String];
    PushManager::getInstance()->SendDeviceToken();
    //如果偏好设置中的已存储设备令牌和新获取的令牌不同则存储新令牌并且发送给服务器端

    if (![oldToken isEqualToData:deviceToken])
    {
        [[NSUserDefaults standardUserDefaults] setObject:deviceToken forKey:key];
    }
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
    UMstatistics::getInstance()->endApp(app_eventid);
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    UMstatistics::getInstance()->startApp(app_eventid);
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    application.applicationIconBadgeNumber = 0;
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}

- (void)dealloc {
    [window release];
    [super dealloc];
}

- (void)setSkipBackupFolder {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    [self addSkipBackupAttributeToItemAtPath:documentsDirectory];
}
- (BOOL)addSkipBackupAttributeToItemAtPath:(NSString *) filePathString
{
    NSURL* URL= [NSURL fileURLWithPath: filePathString];
    assert([[NSFileManager defaultManager] fileExistsAtPath: [URL path]]);
    
    NSError *error = nil;
    BOOL success = [URL setResourceValue: [NSNumber numberWithBool: YES]
                                  forKey: NSURLIsExcludedFromBackupKey error: &error];
    if(!success){
        NSLog(@"Error excluding %@ from backup %@", [URL lastPathComponent], error);
    }
    return success;
}


- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    return  [WXApi handleOpenURL:url delegate:self];
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    return [WXApi handleOpenURL:url delegate:self];
}


#pragma mark - WXApiDelegate
- (void)onResp:(BaseResp *)resp {
    if ([resp isKindOfClass:[SendMessageToWXResp class]]) {
            SendMessageToWXResp *messageResp = (SendMessageToWXResp *)resp;
        switch (resp.errCode) {
            case WXSuccess:
                NSLog(@"分享结果：成功！，retcode = %d", resp.errCode);
                break;
                
            default:
                NSLog(@"分享结果：失败！，retcode = %d", resp.errCode);
                break;
        }
        
        
    } else if ([resp isKindOfClass:[SendAuthResp class]]) {
        
            SendAuthResp *authResp = (SendAuthResp *)resp;
        
    }
//    else if ([resp isKindOfClass:[AddCardToWXCardPackageResp class]]) {
//        
//            AddCardToWXCardPackageResp *addCardResp = (AddCardToWXCardPackageResp *)resp;
//        
//        
//    }
    else if([resp isKindOfClass:[PayResp class]]){
        //支付返回结果，实际支付结果需要去微信服务器端查询
        NSString *strMsg,*strTitle = [NSString stringWithFormat:@"支付结果"];
        
        switch (resp.errCode) {
            case WXSuccess:
                strMsg = @"支付结果：成功！";
                NSLog(@"支付成功－PaySuccess，retcode = %d", resp.errCode);
                break;
                
            default:
                strMsg = [NSString stringWithFormat:@"支付结果：失败！retcode = %d, retstr = %@", resp.errCode,resp.errStr];
                NSLog(@"支付失败，retcode = %d, retstr = %@", resp.errCode,resp.errStr);
                break;
        }
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle message:strMsg delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [alert show];
        [alert release];
    }

}
- (void)onReq:(BaseReq *)req {
    if ([req isKindOfClass:[GetMessageFromWXReq class]]) {
        GetMessageFromWXReq *getMessageReq = (GetMessageFromWXReq *)req;
    } else if ([req isKindOfClass:[ShowMessageFromWXReq class]]) {
        ShowMessageFromWXReq *showMessageReq = (ShowMessageFromWXReq *)req;
    } else if ([req isKindOfClass:[LaunchFromWXReq class]]) {
        LaunchFromWXReq *launchReq = (LaunchFromWXReq *)req;
    }
}


@end
