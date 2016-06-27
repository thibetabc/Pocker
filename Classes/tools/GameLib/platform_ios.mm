//
//  platform_ios.cpp
//  DK2.0
//
//  Created by zhouxin on 14/11/29.
//
//

#include "platforms.h"
#import "Reachability.h"

platforms::NetworkStatus platforms::currentReachabilityStatus()
{
    static Reachability *reacher = nullptr;
    if(reacher == nullptr){
        reacher = [Reachability reachabilityForInternetConnection];
        [reacher startNotifier];
    }
    return [reacher currentReachabilityStatus];
}
