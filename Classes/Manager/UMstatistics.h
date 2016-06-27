//
//  UMstatistics.h
//  pokerclient
//
//  Created by zhaoxuewei on 15/12/14.
//
//

#ifndef __pokerclient__UMstatistics__
#define __pokerclient__UMstatistics__

#include <stdio.h>
//#include <string.h>

using namespace std;

class UMstatistics{
public:
    static UMstatistics* getInstance();
    void startApp(const string& eventid);
    void endApp(const string& eventid);
    void startGame(const string& eventid);
    void endGame(const string& eventid);
private:
    static UMstatistics* __UMstatistics__;
};

#endif /* defined(__pokerclient__UMstatistics__) */
