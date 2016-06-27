//
//  time.cpp
//  Wesnoth_copy
//
//  Created by MirrorMiko on 13-9-12.
//  Copyright (c) 2013年 MirrorMiko. All rights reserved.
//

#include "time_utils.h"

namespace time_utils{
    boost::timer timer_;

    std::string get_timestamp(const time_t& t, const std::string& format) {
        char buf[100] = {0};
        tm* lt = localtime(&t);
        if (lt) {
            strftime(buf, 100, format.c_str(), lt);
        }
        return buf;
    }
    
    /*
     string to time_t
     时间格式 2009-3-24-0-00-08 或 2009-3-24
     */
    int stringToTimeEX(const std::string& strDateStr, time_t &timeData)
    {
        char *pBeginPos = (char*) strDateStr.c_str();
        char *pPos = strstr(pBeginPos,"-");
        if(pPos == NULL)
        {
            printf("strDateStr[%s] err \n", strDateStr.c_str());
            return -1;
        }
        int iYear = atoi(pBeginPos);
        int iMonth = atoi(pPos + 1);
        pPos = strstr(pPos + 1,"-");
        if(pPos == NULL)
        {
            printf("strDateStr[%s] err \n", strDateStr.c_str());
            return -1;
        }
        int iDay = atoi(pPos + 1);
        int iHour=0;
        int iMin=0;
        int iSec=0;
        pPos = strstr(pPos + 1,"-");
        //为了兼容有些没精确到时分秒的
        if(pPos != NULL)
        {
            iHour = atoi(pPos + 1);
            pPos = strstr(pPos + 1,"-");
            if(pPos != NULL)
            {
                iMin=atoi(pPos + 1);
                pPos = strstr(pPos + 1,"-");
                if(pPos != NULL)
                {
                    iSec=atoi(pPos + 1);
                }
            }
        }
        
        struct tm sourcedate;
        bzero((void*)&sourcedate,sizeof(sourcedate));
        sourcedate.tm_sec = iSec;
        sourcedate.tm_min = iMin;
        sourcedate.tm_hour = iHour;
        sourcedate.tm_mday = iDay;
        sourcedate.tm_mon = iMonth - 1;
        sourcedate.tm_year = iYear - 1900;
        timeData = mktime(&sourcedate);
        return 0;
    }
    
    /*
     time_t to string 时间格式 2009-3-24
     */
    int timeToString(std::string &strDateStr,const time_t &timeData)
    {
        char chTmp[100];
        bzero(chTmp,sizeof(chTmp));
        struct tm *p;
        p = localtime(&timeData);
        p->tm_year = p->tm_year + 1900;
        p->tm_mon = p->tm_mon + 1;
        
        snprintf(chTmp,sizeof(chTmp),"%04d-%02d-%02d",
                 p->tm_year, p->tm_mon, p->tm_mday);
        strDateStr = chTmp;
        return 0;
    }
    /*
     time_t to string 时间格式 2009-3-24-0-00-08
     */
    int timeToStringEX(std::string &strDateStr,const time_t &timeData)
    {
        char chTmp[100];
        bzero(chTmp,sizeof(chTmp));
        struct tm *p;
        p = localtime(&timeData);
        p->tm_year = p->tm_year + 1900;
        p->tm_mon = p->tm_mon + 1;
        
        snprintf(chTmp,sizeof(chTmp),"%04d-%02d-%02d-%02d-%02d-%02d",
                 p->tm_year, p->tm_mon, p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
        strDateStr = chTmp;
        return 0;
    }
    
    time_t get1970time(){
        tm t;
        memset(&t, 0, sizeof(tm));
        t.tm_year = 1970;
        time_t st = mktime(&t);
        return st;
    }
}