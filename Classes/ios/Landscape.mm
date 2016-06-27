#include "Landscape.h"
#import "AppController.h"
#include "cocos2d.h"
#include "global_header.h"

using namespace cocos2d;

void landscape()
{
    AppController* controller = [[UIApplication sharedApplication] delegate];
    [controller.viewController supportedInterfaceOrientations];
    
    
    
    if ([[UIDevice currentDevice] respondsToSelector:@selector(setOrientation:)]) {
        SEL selector = NSSelectorFromString(@"setOrientation:");
        NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:[UIDevice instanceMethodSignatureForSelector:selector]];
        [invocation setSelector:selector];
        [invocation setTarget:[UIDevice currentDevice]];
        int val = UIInterfaceOrientationLandscapeRight;
        [invocation setArgument:&val atIndex:2];
        [invocation invoke];
        
        auto director = Director::getInstance();
        auto glview = director->getOpenGLView();
        
        float scalefactor = [[UIScreen mainScreen] scale];
        CGSize size = [[UIScreen mainScreen] bounds].size;
        cocos2d::Size s(std::max(size.width, size.height), std::min(size.width, size.height));
        glview->setFrameSize(s.width * scalefactor, s.height * scalefactor);
        glview->setDesignResolutionSize(1136, 640, ResolutionPolicy::EXACT_FIT);
    }
    
    [controller.viewController setNeedsStatusBarAppearanceUpdate];
//    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    
}

void portrait()
{
    AppController* controller = [[UIApplication sharedApplication] delegate];
    [controller.viewController supportedInterfaceOrientations];
    
    
    
    if ([[UIDevice currentDevice] respondsToSelector:@selector(setOrientation:)]) {
        SEL selector = NSSelectorFromString(@"setOrientation:");
        NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:[UIDevice instanceMethodSignatureForSelector:selector]];
        [invocation setSelector:selector];
        [invocation setTarget:[UIDevice currentDevice]];
        int val = UIDeviceOrientationPortrait;
        [invocation setArgument:&val atIndex:2];
        [invocation invoke];
        
        auto director = Director::getInstance();
        auto glview = director->getOpenGLView();
        
        float scalefactor = [[UIScreen mainScreen] scale];
        CGSize size = [[UIScreen mainScreen] bounds].size;
        cocos2d::Size s(std::min(size.width, size.height), std::max(size.width, size.height));
        glview->setFrameSize(s.width * scalefactor, s.height * scalefactor);
        glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, ResolutionPolicy::EXACT_FIT);
    }
    [controller.viewController setNeedsStatusBarAppearanceUpdate];
//    [[UIApplication sharedApplication] setStatusBarHidden:NO];
}