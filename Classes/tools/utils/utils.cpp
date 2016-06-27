//
//  utils.cpp
//  DK2.0
//
//  Created by zhouxin on 14-9-17.
//
//

#include "utils.h"
#include <fstream>
//#include <mach/mach.h>
#include <boost/array.hpp>
#include <boost/format.hpp>
#include "GameTools.h"
#include "PlatformSdk.h"



#pragma mark tools
const std::string tools::error_description(const int error_code)
{
    
    static std::map<int, std::string> error_map;
    if(error_map.empty()) {
        cocos2d::ValueMap vm = cocos2d::FileUtils::getInstance()->getValueMapFromFile("client_errors_codes.plist");
        for (auto& iter : vm) {
            error_map.insert(std::make_pair(tools::stoi(iter.first), iter.second.asString()));
        }
    }
    
    auto iter = error_map.find(error_code);
    if(iter != error_map.end()) {
        return iter->second;
    }
    
    return to_string(error_code);
}

void tools::set_current_language(const std::string& language)
{
    cocos2d::log("set_current_language");
    local_strings.clear();
    if(local_strings.empty()) {
        rapidjson::Document doc;
        doc.SetObject();
        std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename(language);
        const std::string content = read_file(path);
        doc.Parse<0>(content.c_str());
        for (auto iter = doc.MemberonBegin(); iter != doc.MemberonEnd(); ++iter) {
            local_strings.insert(std::make_pair(iter->name.GetString(), iter->value.GetString()));
        }
    }
    
    const std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "languagex.txt";
    file_utils::write_file(path, language);
}

void tools::reset_current_language_file()
{
    
    local_strings.clear();
    rapidjson::Document doc;
    doc.SetObject();
    const std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "languagex.txt";
    PlatformSdk::LanguageType currentLanguageType = (PlatformSdk::LanguageType)GameTools::getLanguage(); //app language by huangfeng
    // to do : ronaldz.  应该同时把客户端的语言发送至服务器上去。
    
    std::string language_path = cocos2d::FileUtils::getInstance()->fullPathForFilename("client_language_en.json");
    
    switch(currentLanguageType){
        case PlatformSdk::LanguageType::ENGLISH:
            language_path = cocos2d::FileUtils::getInstance()->fullPathForFilename("client_language_en.json");
            break;
        case PlatformSdk::LanguageType::CHINESE:
            language_path =cocos2d::FileUtils::getInstance()->fullPathForFilename("client_language_zh.json");
            break;
        case PlatformSdk::LanguageType::TRADITIONAL_CHINESE:
            language_path =cocos2d::FileUtils::getInstance()->fullPathForFilename("client_language_zh_fan.json");
            break;
        default:
            language_path =cocos2d::FileUtils::getInstance()->fullPathForFilename("client_language_en.json");
            break;
    }
    
    
    cocos2d::log("language_path.......:%s", language_path.c_str());
    if(file_utils::file_exists(path)) {
        std::string filename = tools::read_file(path);
        language_path = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);
    }
    cocos2d::log("language_path:%s", language_path.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    const std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(language_path);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    const std::string content = read_file(language_path);
#endif
    if(!content.empty()) {
        doc.Parse<0>(content.c_str());
        for (auto iter = doc.MemberonBegin(); iter != doc.MemberonEnd(); ++iter) {
            if (iter->value.IsArray())
            {
                for (rapidjson::SizeType i = 0; i != iter->value.Size(); ++i)
                {
                    local_strings.insert(std::make_pair(iter->name.GetString() + to_string(i), iter->value[i].GetString()));
                }
            }
            else
            {
                local_strings.insert(std::make_pair(iter->name.GetString(), iter->value.GetString()));
            }
        }
    }
}


const std::string tools::local_string(const std::string& key, const std::string)
{
    //cocos2d::log("local_string");
    if(local_strings.empty()) {
        reset_current_language_file();
    }
    auto iter = local_strings.find(key);
    if(iter != local_strings.end()) {
        return iter->second;
    }
    else {
        cocos2d::log("error:local %s description is empty!!", key.c_str());
    }
    return key;
    
}

const std::string tools::md5(const std::string& input)
{
    std::string key = "_$fGFPv8ZySm{G$j28!#PuJAfIJglx}uEK@Ge&0pS}xIqHqDlbtJgvV9agTavX1t";
    std::string iput = input + key;
    return md5(iput);
}

void tools::write_file(const std::string& filename, const std::string& content)
{
    std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + filename;
    cocos2d::log("path:%s", path.c_str());
    std::ofstream file(path.c_str(), std::ios::binary);
    file << content;
    file.close();
}

std::string tools::read_file(const std::string& filename)
{
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    return cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
//#endif
    std::string content;
    std::ifstream file(filename.c_str(), std::ios::binary);
    
    if (!file.is_open()) {
//        std::cerr << "file " << filename << " open failed!" << std::endl;
        cocos2d::log("open file %s failed", filename.c_str());
        return content;
    }
    
    for(std::string s; getline(file,s);) {
        content += s;
    }
    
    
    file.close();
    
    return content;
    
}

const bool tools::copyFile(const std::string& srcfilename, const std::string& desfilename)
{
    // 资源路径
    std::string sourcePath = cocos2d::FileUtils::getInstance()->fullPathForFilename(srcfilename);
    cocos2d::Data data = cocos2d::FileUtils::getInstance()->getDataFromFile(sourcePath);
    
    // 可写路径
    std::string destPath = desfilename;
    FILE *fp = fopen(destPath.c_str(), "w+");
    if (fp)
    {
        size_t size = fwrite(data.getBytes(), sizeof(unsigned char), data.getSize(), fp);
        fclose(fp);
        
        if (size > 0)
        {
            return true;
        }
    }
    CCLOG("copy file %s failed.", srcfilename.c_str());
    return false;
}

const long tools::date()
{
    struct timeval time;
    gettimeofday(&time, 0);
    return time.tv_sec;
}
const double tools::date_double()
{
    struct timeval time;
    gettimeofday(&time, 0);
    return time.tv_sec + time.tv_usec / 1000000.0;
}

std::string tools::digital2time(const int second)
{
    int day = second / (24*60*60);
    int hour =second / (60*60)%24;
    int min = second/60%60;
    int s = second%60;
    if(day > 0)
    {
        hour = hour+day*24;//天数换算成小时
    }
//    assert(day == 0);
    
    std::string timeStr =  intToSteLen2(hour) + ":"+ intToSteLen2(min) + ":"+ intToSteLen2(s);
    return timeStr;
}

std::string  tools::intToSteLen2(int i)
{
    std::stringstream ss;
    std::string s;
    char t[10];
    sprintf(t, "%02d",i);
    s = t;
    return s;
}

std::string  tools::serverTime2local(long long int timeSec)
{
    char str[64] = {0};
    time_t timep = timeSec;
    tm* timeStuct = localtime(&timep);
    sprintf(str, "%02d/%02d/%02d",
            timeStuct->tm_year + 1900,
            timeStuct->tm_mon + 1,
            timeStuct->tm_mday
            );
    return str;
    
}

bool tools::time_is_same_day(long long int timeSec1, long long int timeSec2)
{
    time_t timep = timeSec1;
    tm* timeStuct1 = localtime(&timep);
    
    int year1 = timeStuct1->tm_year;
    int month1 = timeStuct1->tm_mon;
    int day1 = timeStuct1->tm_mday;

    
    timep = timeSec2;
    tm* timeStuct2 = localtime(&timep);
    int year2 = timeStuct2->tm_year;
    int month2 = timeStuct2->tm_mon;
    int day2 = timeStuct2->tm_mday;
    
    if (year1 == year2 &&
        month1 == month2 &&
        day1 == day2)
        return true;
    else
        return false;
    
    return false;
}

std::string tools::time_display(long long int timeSec, TIMEDISPLAYTYPE time_type)
{
    char str[64] = {0};
    time_t timep = timeSec;
    tm* timeStuct = localtime(&timep);
    
    int year = timeStuct->tm_year;
    int month = timeStuct->tm_mon;
    int day = timeStuct->tm_mday;
    int hour = timeStuct->tm_hour;
    int min = timeStuct->tm_min;
    int sec = timeStuct->tm_sec;
    
    long time = date();
    tm* time_struct = localtime(&time);
    
    switch (time_type) {
        case TIMEDISPLAYTYPE::NORMAL :{
            if(time_struct->tm_year == year
               && time_struct->tm_mon == month
               && time_struct->tm_mday == day) {
                sprintf(str, "%02d:%02d",
                        hour,
                        min
                        );
            }
            else {
                sprintf(str, "%02d/%02d/%02d",
                        year + 1900,
                        month + 1,
                        day
                        );
            }
            break;
        }
        case TIMEDISPLAYTYPE::ONLYDATE :{
            sprintf(str, "%02d/%02d/%02d",
                    year + 1900,
                    month + 1,
                    day
                    );
            break;
        }
        case TIMEDISPLAYTYPE::ONLYDAY :{
            sprintf(str, "%02d/%02d",
                    month + 1,
                    day
                    );
            break;
        }
        case TIMEDISPLAYTYPE::ONLYTIME :{
            sprintf(str, "%02d:%02d",
                    hour,
                    min
                    );
            break;
        }
        case TIMEDISPLAYTYPE::ALLTIME :{
            sprintf(str, "%02d/%02d/%02d %02d:%02d:%02d",
                    year + 1900,
                    month + 1,
                    day,
                    hour,
                    min,
                    sec
                    );
            break;
        }
        default:{
            break;
        }
    }

    return str;
}

const std::string tools::time_string()
{
    long time = date();
    tm* time_struct = localtime(&time);
    char str[64] = {0};
    sprintf(str, "%02d/%02d/%02d %02d:%02d:%02d",
            time_struct->tm_year + 1900,
            time_struct->tm_mon + 1,
            time_struct->tm_mday,
            time_struct->tm_hour,
            time_struct->tm_min,
            time_struct->tm_sec
            );
    return str;
}

std::string tools::digital2fractionstring(int molecular,int denominator)
{
    std::string fraction = "";
    fraction+=digital2string(molecular) + "/" + digital2string(denominator);
    return fraction;
}


std::string tools::getFileExt(const std::string& file)
{
    std::string::size_type idx = file.rfind(".");
    std::string ext = "";
    if (idx != std::string::npos) {
        ext = file.substr(idx+1, std::string::npos);
    }
    return std::move(ext);
}

std::string tools::texture_name_from_plist(const std::string& plist){
    cocos2d::ValueMap dict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(plist);
    CCAssert(!dict.empty(), ("error, can not found the plist:" + plist).c_str());
    std::string textureName;
    if(dict.find("metadata") != dict.end()) {
        auto& metadata1 = dict["metadata"];
        auto& metadata = metadata1.asValueMap();
        int format = string2digital<int>(metadata["format"].asString());
        
        switch (format) {
            case 2:
            {
                textureName = metadata["realTextureFileName"].asString();
                break;
            }
            case 3:
            {
                auto& theDict = metadata["target"].asValueMap();
                std::string name = theDict["realTextureFileName"].asString();
                std::string extension = theDict["textureFileExtension"].asString();
                textureName = name + extension;
                break;
            }
            default:
                break;
        }
        if (textureName.empty()) {
            textureName = plist;
            std::string::size_type pos = textureName.rfind(".");
            if (pos != std::string::npos) {
                textureName.erase(textureName.begin() + pos, textureName.end());
            }
            textureName += ".png";
        }
    }
    else {
        textureName = dict.begin()->first;
        textureName = textureName.substr(textureName.find("."));
        textureName += ".pvr.gz";
    }
	return  textureName;
}

std::vector<std::string> tools::split(const std::string& src, const char* token, const bool ignore_empty_str)
{
    std::vector<std::string> vect;
    int nend=0;
    int nbegin=0;
    while(nend != -1)
    {
        nend = src.find(token, nbegin);
        if(nend == -1) {
            std::string str = src.substr(nbegin, src.length()-nbegin);
            
            if(str.length() || !ignore_empty_str)
                vect.push_back(str);
        }
        else {
            std::string str = src.substr(nbegin, nend-nbegin);
            if(str.length() || !ignore_empty_str)
                vect.push_back(str);
        }
        nbegin = nend + strlen(token);
    };
    return vect;
}


double tools::available_memory() {
    double mem = 0;
#ifdef COCOS2D_DEBUG
//    vm_statistics_data_t vmStats;
//    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
//    kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
//    
//    if(kernReturn != KERN_SUCCESS) {
//        return LONG_MAX;
//    }
//    mem = (double) ((vm_page_size * vmStats.free_count) / 1024.0) / 1024.0;
//    CCLOG("A Current Available Memory: %f", mem);
#endif
    return mem;
}

double tools::free_memory() {
    double mem = 0;
#ifdef COCOS2D_DEBUG
//    
//    mach_port_t host_port;
//    mach_msg_type_number_t host_size;
//    vm_size_t pagesize;
//    host_port = mach_host_self();
//    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
//    host_page_size(host_port, &pagesize);
//    vm_statistics_data_t vm_stat;
//    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS) {
//        CCLOG("Failed to fetch vm statistics");
//        return 0;
//    }
//    /* Stats in bytes */
//    natural_t mem_free = vm_stat.free_count * pagesize;
//    mem = mem_free/1024.0/1024.0;
//    CCLOG("B Current Available Memory: %f", mem);
#endif
    return mem;
}

std::string find_path(const std::string& fName){
    std::string path = fName;
//    path = cache_path(fName);
//    if (file_utils::file_exists(path)) return std::move(path);
//    path = write_path(fName);
//    if (file_utils::file_exists(path)) return std::move(path);
//    path = sdcard_path(fName);
//    if (file_utils::file_exists(path)) return std::move(path);
    return std::move(cocos2d::FileUtils::getInstance()->fullPathForFilename(fName.c_str()));
}


bool tools::is_zero(const double value)
{
    return std::abs(value) < 0.00001;
}

std::string event_net_notify(const std::string& cmd)
{
    return std::move(cmd + "_notif");
}

const std::string tools::boost_any_to_string(const boost::any& value)
{
    std::string tempStr;
    if(value.type() == typeid(int)) {
        int v = boost::any_cast<int>(value);
        tempStr = to_string(v);
    }
    else if(value.type() == typeid(float)) {
        int v = boost::any_cast<float>(value);
        tempStr = to_string(v);
    }
    else if(value.type() == typeid(float)) {
        int v = boost::any_cast<float>(value);
        tempStr = to_string(v);
    }
    else if(value.type() == typeid(double)) {
        int v = boost::any_cast<double>(value);
        tempStr = to_string(v);
    }
    else if(value.type() == typeid(uint32_t)) {
        uint32_t v = boost::any_cast<uint32_t>(value);
        tempStr = to_string(v);
    }
    else if(value.type() == typeid(uint64_t)) {
        uint64_t v = boost::any_cast<uint64_t>(value);
        tempStr = to_string(v);
    }
    else if(value.type() == typeid(std::string)) {
        tempStr = boost::any_cast<std::string>(value);
    }
    else {
        assert(0);
    }
    return std::move(tempStr);
}



bool tools::crossLine(cocos2d::Rect r1, cocos2d::Rect r2)
{
    float minx1 = r1.origin.x;
    float miny1 = r1.origin.y;
    float maxx1 = minx1 + r1.size.width;
    float maxy1 = miny1 + r1.size.height;
    
    float minx2 = r2.origin.x;
    float miny2 = r2.origin.y;
    float maxx2 = minx2 + r2.size.width;
    float maxy2 = miny2 + r2.size.height;
    
    
    float minx   =   fmax(minx1,   minx2);
    float miny   =   fmax(miny1,   miny2);
    float maxx   =   fmin(maxx1,   maxx2);
    float maxy   =   fmin(maxy1,   maxy2);
    
    if(minx  >  maxx ||  miny  >  maxy)
    {
        return false;
    }
    
    return true;
}

bool tools::crossCircle(const cocos2d::Point pos1,float radius1,const cocos2d::Point pos2,const float radius2)
{
    if(pos1.distance(pos2) <= radius1 + radius2)
    {
        return true;
    }
    return false;
}



float tools::get_triangle_area(const cocos2d::Point pos1,const cocos2d::Point pos2,const cocos2d::Point pos3)
{
    float a = pos1.distance(pos2);
    float b = pos2.distance(pos3);
    float c = pos3.distance(pos1);
    float p = (a + b + c) / 2;
    
    
    return sqrt(p*(p-a)*(p-b)*(p-c));
    
}


bool tools::isLineIntersectRectangle(const cocos2d::Point linePos1,const cocos2d::Point linePos2,const cocos2d::Rect rect)
{
    float  lineHeight = linePos2.y - linePos1.y;
    float  lineWidth = linePos2.x - linePos1.x;  // 计算叉乘
    float  c = linePos1.x * linePos2.y - linePos2.x * linePos1.y;
    float  rectangleLeftTopX = rect.origin.x;
    float  rectangleLeftTopY = rect.origin.y + rect.size.height;
    float  rectangleRightBottomX = rect.origin.x + rect.size.width;
    float  rectangleRightBottomY = rect.origin.y;
    if ((lineHeight * rectangleLeftTopX + lineWidth * rectangleLeftTopY + c >= 0 && lineHeight * rectangleRightBottomX + lineWidth * rectangleRightBottomY + c <= 0)
        || (lineHeight * rectangleLeftTopX + lineWidth * rectangleLeftTopY + c <= 0 && lineHeight * rectangleRightBottomX + lineWidth * rectangleRightBottomY + c >= 0)
        || (lineHeight * rectangleLeftTopX + lineWidth * rectangleRightBottomY + c >= 0 && lineHeight * rectangleRightBottomX + lineWidth * rectangleLeftTopY + c <= 0)
        || (lineHeight * rectangleLeftTopX + lineWidth * rectangleRightBottomY + c <= 0 && lineHeight * rectangleRightBottomX + lineWidth * rectangleLeftTopY + c >= 0))
    {
        
        if (rectangleLeftTopX > rectangleRightBottomX)
        {
            float temp = rectangleLeftTopX;
            rectangleLeftTopX = rectangleRightBottomX;
            rectangleRightBottomX = temp;
        }
        if (rectangleLeftTopY < rectangleRightBottomY)
        {
            float temp1 = rectangleLeftTopY;
            rectangleLeftTopY = rectangleRightBottomY;
            rectangleRightBottomY = temp1;
        }
        if ((linePos1.x < rectangleLeftTopX && linePos2.x < rectangleLeftTopX)
            || (linePos1.x > rectangleRightBottomX && linePos2.x > rectangleRightBottomX)
            || (linePos1.y > rectangleLeftTopY && linePos2.y > rectangleLeftTopY)
            || (linePos1.y < rectangleRightBottomY && linePos2.y < rectangleRightBottomY))
        {
            return false;
        } else
        {
            return true;
        }
    } else
    {
        return false;
    }
}

bool tools::IsNumberic(const std::string& str)
{
    for (auto num: str){
        if (num < '0' || num > '9')
            return false;
    }
    return true;
}

int                tools::stoi  (const std::string& str)
{
    return string2digital<int>(str.c_str());
}
long               tools::stol  (const std::string& str)
{
    return string2digital<long>(str.c_str());
}
unsigned long      tools::stoul (const std::string& str)
{
    return string2digital<unsigned long>(str.c_str());
}
long long          tools::stoll (const std::string& str)
{
    return string2digital<long long>(str.c_str());
}
unsigned long long tools::stoull(const std::string& str)
{
    return string2digital<unsigned long long>(str.c_str());
}
float       tools::stof (const std::string& str)
{
    return string2digital<float>(str.c_str());
}
double      tools::stod (const std::string& str)
{
    return string2digital<double>(str.c_str());
}
long double tools::stold(const std::string& str)
{
    return string2digital<long double>(str.c_str());
}
