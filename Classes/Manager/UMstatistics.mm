//
//  UMstatistics.cpp
//  pokerclient
//
//  Created by zhaoxuewei on 15/12/14.
//
//

#include "UMstatistics.h"
#include "MobClick.h"


UMstatistics* UMstatistics::__UMstatistics__ = nullptr;

UMstatistics* UMstatistics::getInstance()
{
    if(!__UMstatistics__){
        __UMstatistics__ = new UMstatistics;
    }
    return __UMstatistics__;
}

void UMstatistics::startApp(const string& eventid)
{
    NSString *ns = [NSString stringWithUTF8String:eventid.c_str()];
    [MobClick beginEvent:ns];
}

void UMstatistics::endApp(const string& eventid)
{
    NSString *ns = [NSString stringWithUTF8String:eventid.c_str()];
    [MobClick endEvent:ns];
}

void UMstatistics::startGame(const string& eventid)
{
    NSString *ns = [NSString stringWithUTF8String:eventid.c_str()];
    [MobClick beginEvent:ns];
}

void UMstatistics::endGame(const string& eventid)
{
    NSString *ns = [NSString stringWithUTF8String:eventid.c_str()];
    [MobClick endEvent:ns];
}