//
//  CCSwitchToOC.cpp
//  pokerclient
//
//  Created by qqronaldz on 16/3/8.
//
//

#include "CCSwitchToOC.h"
#include "IosPay.h"
#include "AppController.h"

void CCSwitchToOC::sendBuy(std::string id)
{
    NSString *goods_id = [NSString stringWithCString:id.c_str() encoding:[NSString defaultCStringEncoding]];
    //@"com.chinarichinc.yuemayueba.gametime_1";
    [[PaymentViewController alloc] sendBuy : goods_id];
}
