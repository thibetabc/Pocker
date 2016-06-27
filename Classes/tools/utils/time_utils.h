//
//  time.h
//  Wesnoth_copy
//
//  Created by MirrorMiko on 13-9-12.
//  Copyright (c) 2013年 MirrorMiko. All rights reserved.
//

#ifndef __Wesnoth_copy__time__
#define __Wesnoth_copy__time__

#include <iostream>
#include <boost/timer.hpp>
//#include "timer.h"

namespace time_utils {
    extern boost::timer timer_;
    std::string get_timestamp(const time_t& t, const std::string& format="%Y%m%d %H:%M:%S ");
    
    int stringToTimeEX(const std::string& strDateStr, time_t &timeData);
    int timeToString(std::string &strDateStr,const time_t &timeData);
    int timeToStringEX(std::string &strDateStr,const time_t &timeData);
    time_t get1970time();
}

#endif /* defined(__Wesnoth_copy__time__) */
