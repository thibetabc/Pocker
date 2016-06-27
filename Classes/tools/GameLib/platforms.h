//
//  platforms.h
//  DK2.0
//
//  Created by zhouxin on 14/11/29.
//
//

#ifndef __DK2_0__platforms__
#define __DK2_0__platforms__

#include <iostream>
#include <map>

namespace platforms {
    typedef enum {
        NotReachable = 0,
        ReachableViaWiFi, //wifi
        ReachableViaWWAN  //3g
    }NetworkStatus;
    
    NetworkStatus currentReachabilityStatus();
};

#endif /* defined(__DK2_0__platforms__) */
