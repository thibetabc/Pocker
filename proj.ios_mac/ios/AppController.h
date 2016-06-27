#import <UIKit/UIKit.h>

#import <StoreKit/StoreKit.h>
@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    
}

@property(nonatomic, readonly) RootViewController* viewController;
+(void) sendTextContent;
+ (void)sendPay:prepayId c:(NSString*)nonceStr d:(UInt32)timeStamp f:(NSString*)sign;
@end

