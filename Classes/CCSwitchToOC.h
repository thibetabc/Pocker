//
//  CCSwitchToOC.hpp
//  pokerclient
//
//  Created by qqronaldz on 16/3/8.
//
//

#ifndef CCSwitchToOC_hpp
#define CCSwitchToOC_hpp

#include <stdio.h>
#include <string.h>
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
class CCSwitchToOC
{
public:

    void sendBuy(std::string id);

};
#endif
#endif /* CCSwitchToOC_h */
