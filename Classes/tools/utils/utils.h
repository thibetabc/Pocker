//
//  utils.h
//  DK2.0
//
//  Created by zhouxin on 14-9-17.
//
//

#ifndef __DK2_0__utils__
#define __DK2_0__utils__

#include <iostream>
#include <string>
#include <sstream>
#include <istream>
#include <map>
#include <unordered_map>
#include <sys/stat.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/any.hpp>

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "cocos2d.h"

#include "file_utils.h"
#include "Plist.hpp"

#include <boost/any.hpp>
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "jni/JniHelper.h"
#include <android/log.h>
#endif


std::string find_path(const std::string&  fName);
std::string event_net_notify(const std::string&  cmd);

/**
 *  基本工具操作
 */
namespace tools {

    double available_memory();
    double free_memory();

    bool is_zero(const double value);
    
    
    const std::string error_description(const int error_code);
    
    const std::string local_string(const std::string& key, const std::string = "");
    
    void write_file(const std::string& filename, const std::string& content);
    std::string read_file(const std::string& filename);
    
    const std::string md5(const std::string& input);
    
    const long date();
    const double date_double();
    const bool copyFile(const std::string& srcfilename, const std::string& desfilename);
    
    
    template <class T> std::string anything2string(T tmp) {
        std::stringstream ss;
        ss << tmp;
        return ss.str();
    }
    
    template <class T> std::string digital2string(T tmp)
    {
        std::stringstream ss;
        ss << tmp;
        return ss.str();
    }
    template <class T> T string2digital(const char* str) {
        if(!str) str = "0";
        std::istringstream iss(str);
        T num;
        iss >> num;
        return num;
    }
    //string2digital<int>(str)
    template <class T> T string2digital(const std::string& str)
    {
        std::istringstream iss(str);
        T num;
        iss >> num;
        return num;
    }
    
    std::string digital2time(const int second);
    
    //分数 x/x
    std::string digital2fractionstring(int molecular,int denominator);
    
    //int 转string 补全2位
    std::string intToSteLen2(int i);
    
    //服务器时间转本地
    std::string  serverTime2local(long long int timeSec);
    
    enum TIMEDISPLAYTYPE
    {
        NORMAL = 0x00,
        ONLYDATE = 0x01,
        ONLYTIME = 0x02,
        ALLTIME = 0x03,
        ONLYDAY = 0x04
    };
    
    //判断是不是同一天，如果是返回true，否则else
    bool time_is_same_day(long long int timeSec1, long long int timeSec2);
    
    //如果是今天，显示小时分秒，如果不是，显示年月日
    std::string time_display(long long int timeSec, TIMEDISPLAYTYPE time_type = TIMEDISPLAYTYPE::NORMAL);
    
    //本地时间显示
    const std::string time_string();
    
    
    
    static uint32_t generate_uint32_id() {
        static uint32_t uint32_id_begin = 100;
        return ++uint32_id_begin;
    }
    
    static uint64_t generate_uint64_id() {
        static uint64_t uint64_id_begin = 100000;
        return ++uint64_id_begin;
    }
    //获取文件后缀名
    std::string getFileExt(const std::string& file);
    std::string texture_name_from_plist(const std::string& );
    
    std::vector<std::string> split(const std::string& src, const char* token, const bool ignore_empty_str = true);
    
    const std::string boost_any_to_string(const boost::any& any);
    
    void set_current_language(const std::string&);
    void reset_current_language_file();
    
    /**
     *  判断两个矩形是否香蕉
     *
     *  @param r1
     *  @param r2
     *
     *  @return
     */
    bool crossLine(cocos2d::Rect r1,cocos2d::Rect r2);
    
    /**
     *  判断两个圆相交
     *
     *  @param pos1    圆心1
     *  @param radius1 半径1
     *  @param pos2    圆心2
     *  @param radius2 半径2
     *
     *  @return 是否相交
     */
    bool crossCircle(const cocos2d::Point pos1,float radius1,const cocos2d::Point pos2,const float radius2);
    
    /**
     *  求三角形面积
     *
     *  @param pos1
     *  @param pos2
     *  @param pos3
     *
     *  @return
     */
    float get_triangle_area(const cocos2d::Point pos1,const cocos2d::Point pos2,const cocos2d::Point pos3);
    
    /** <p>判断线段是否在矩形内 </p>
     * 先看线段所在直线是否与矩形相交，
     * 如果不相交则返回false，
     * 如果相交，
     * 则看线段的两个点是否在矩形的同一边（即两点的x(y)坐标都比矩形的小x(y)坐标小，或者大）,
     * 若在同一边则返回false，
     * 否则就是相交的情况。
     * @return 是否相交
     */
    bool isLineIntersectRectangle(const cocos2d::Point linePos1,const cocos2d::Point linePos2,const cocos2d::Rect rect);
    template <class T> std::string to_string(T tmp) {
        std::stringstream ss;
        ss << tmp;
        return ss.str();
    }
    
    bool IsNumberic(const std::string& str);
    int                stoi  (const std::string& str);
    long               stol  (const std::string& str);
    unsigned long      stoul (const std::string& str);
    long long          stoll (const std::string& str);
    unsigned long long stoull(const std::string& str);
    float       stof (const std::string& str);
    double      stod (const std::string& str);
    long double stold(const std::string& str);
    
    
    
    static std::map<std::string, std::string> local_strings;
    
    static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }
    
    // trim from end
    static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }
    
    static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
    }
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static jstring stoJstring(JNIEnv* env, const char* pat)
    {
        jclass strClass = env->FindClass("Ljava/lang/String;");
        jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
        jbyteArray bytes = env->NewByteArray(strlen(pat));
        env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
        jstring encoding = env->NewStringUTF("utf-8");
        return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
    }
    static char* jstringTostring(JNIEnv* env, jstring jstr)
    {
        char* rtn = NULL;
        jclass clsstring = env->FindClass("java/lang/String");
        jstring strencode = env->NewStringUTF("utf-8");
        jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
        jsize alen = env->GetArrayLength(barr);
        jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
        if (alen > 0)
        {
            rtn = (char*)malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
        }
        env->ReleaseByteArrayElements(barr, ba, 0);
        return rtn;
    }
#endif
    
};

#endif /* defined(__DK2_0__utils__) */
