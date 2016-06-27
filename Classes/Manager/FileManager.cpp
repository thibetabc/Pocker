#include "FileManager.h"
#include "cmd_def.h"
#include "net_manager.h"
#include "file_utils.h"
#include "message_block.hpp"
#include "utils.h"
#include "VoiceRecorderInterface.h"
#include "GameDataManager.h"
#include "HttpClient.h"
#include "utils.h"
#include "curl/curl.h"
#include <thread>

/* read callback function, fread() look alike */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    ssize_t retcode;
    curl_off_t nread;
    
    int *fdp = (int *)stream;
    int fd = *fdp;
    
    retcode = read(fd, ptr, size * nmemb);
    
    nread = (curl_off_t)retcode;
    
    fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
            " bytes from file\n", nread);
    
    return retcode;
}

/* ioctl callback function */
static curlioerr my_ioctl(CURL *handle, curliocmd cmd, void *userp)
{
    int *fdp = (int *)userp;
    int fd = *fdp;
    
    (void)handle; /* not used in here */
    cocos2d::log("********my_ioctl******");
    switch(cmd) {
        case CURLIOCMD_RESTARTREAD:
            /* mr libcurl kindly asks as to rewind the read data stream to start */
            if(-1 == lseek(fd, 0, SEEK_SET))
            /* couldn't rewind */
                return CURLIOE_FAILRESTART;
            
            break;
            
        default: /* ignore unknown commands */
            return CURLIOE_UNKNOWNCMD;
    }
    cocos2d::log("********my_ioctl******");
    return CURLIOE_OK; /* success! */ 
}

//response callback
size_t FileManager::Response(char *dataPtr, size_t size, size_t nmemb, void *user_p)
{
    if(!dataPtr)
    {
        return 0;
    }
    
    cocos2d::log("FileManager::Response %s", dataPtr);
    cocos2d::log("already upload %ld",size*nmemb);
    
    //    log("dataPtr = %s",*dataPtr[size * nmemb]);
    return size * nmemb;
}

#define STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES         6000
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     3

struct myprogress {
    double lastruntime;
    CURL *curl;
};

/* this is how the CURLOPT_XFERINFOFUNCTION callback works */
static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow)
{
    struct myprogress *myp = (struct myprogress *)p;
    CURL *curl = myp->curl;
    double curtime = 0;
    
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);
    
    /* under certain circumstances it may be desirable for certain functionality
     to only run every N seconds, in order to do this the transaction time can
     be used */
    if((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL) {
        myp->lastruntime = curtime;
        printf("TOTAL TIME: %f \r\n", curtime);
    }
    
    printf( "UP: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
            "  DOWN: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
            "\r\n",
            ulnow, ultotal, dlnow, dltotal);
    
    if(dlnow > STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES)
        return 1;
    return 0;
}

static int older_progress(void *p,
                          double dltotal, double dlnow,
                          double ultotal, double ulnow)
{
    return xferinfo(p,
                    (curl_off_t)dltotal,
                    (curl_off_t)dlnow,
                    (curl_off_t)ultotal,
                    (curl_off_t)ulnow);
}

static int seek_callback(void *userp, curl_off_t offset, int origin)
{
    
}

bool FileManager::init_file_server_network()
{
    struct addrinfo *answer, hint, *curr;
    
    bzero(&hint, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_flags = AI_ADDRCONFIG;
    hint.ai_socktype = SOCK_STREAM;
    struct sockaddr_in  *sockaddr_ipv4;
    struct sockaddr_in6 *sockaddr_ipv6;
    char ipstrv6[128];
    
    int ret = getaddrinfo(net_manager::shared_instence().FileServerIPv4.c_str(), NULL, &hint, &answer);
    
    if (ret != 0) {
        cocos2d::log("ERROR: Getaddrinfo error info %s", gai_strerror(ret));
        return false;
    }else
    {
        bool isINET = false;
        bool isINET6 = false;
        for (curr = answer; curr != NULL; curr = curr->ai_next) {
            switch (curr->ai_family){
                case AF_UNSPEC:
                    break;
                case AF_INET:
                    sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in *>( curr->ai_addr);
                    isINET = true;
                    break;
                case AF_INET6:
                    sockaddr_ipv6 = reinterpret_cast<struct sockaddr_in6 *>( curr->ai_addr);
                    inet_ntop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstrv6, sizeof(ipstrv6));
                    isINET6 = true;
                    break;
            }
        }
        
        if (!isINET && isINET6)
        {
            file_server_addr_type_ = AF_INET6;
            file_server_addr_ = ipstrv6;
        }else
        {
            file_server_addr_type_ = AF_INET;
            file_server_addr_ = net_manager::shared_instence().FileServerIPv4;
        }
        
        //Don't forgot free addrinfo because memory leek   --by lvzhangle
        freeaddrinfo(answer);
    }
    return true;
}

void FileManager::start_upload_file(const std::string& filename)
{
    cocos2d::log("start upload file %s",filename.c_str());
    std::string file = filename;
    if(!cocos2d::FileUtils::getInstance()->isAbsolutePath(filename)) {
        file = cocos2d::FileUtils::getInstance()->getWritablePath() + filename;
    }
    if(!file_utils::file_exists(file)) {
        file = GDM->get_user_directory() + filename;
    }
    
    struct stat file_info;
    int hd = open(file.c_str(), O_RDONLY);
    fstat(hd, &file_info);
        char buf[512] = {0};
    std::string url = "http://" ;
    init_file_server_network();
    if (AF_INET == file_server_addr_type_)
    {
        sprintf(buf, "http://%s:8080/poker/headpic/%s", file_server_addr_.c_str(), filename.c_str());
    }else if (AF_INET6 == file_server_addr_type_)
    {
        sprintf(buf, "http://[%s]:8080/poker/headpic/%s", file_server_addr_.c_str(), filename.c_str());
    }
    
    CURLcode m_CURLcode;
    CURL *m_CURL;
    
    //Init Global
    m_CURLcode = curl_global_init(CURL_GLOBAL_ALL);
    if(CURLE_OK != m_CURLcode)
    {
        curl_global_cleanup();
        return ;
    }
    
    m_CURL = curl_easy_init();
    if(!m_CURL)
    {
        curl_easy_cleanup(m_CURL);
        curl_global_cleanup();
        return ;
    }
    
    struct myprogress prog;
    
    prog.lastruntime = 0;
    prog.curl = m_CURL;
    
    curl_httppost *post = NULL;
    curl_httppost *last = NULL;
    
    std::string contentType = "Content-Type: multipart/form-data; charset=UTF-8";
    std::string name = "filename=" + filename;
    std::string filePath = GDM->get_user_directory() + filename;
    
    curl_easy_setopt(m_CURL, CURLOPT_READFUNCTION, read_callback);
    
    curl_easy_setopt(m_CURL, CURLOPT_READDATA, (void*)&hd);
    
    curl_easy_setopt(m_CURL, CURLOPT_IOCTLFUNCTION, my_ioctl);
    
   
    curl_easy_setopt(m_CURL, CURLOPT_IOCTLDATA, (void*)&hd);
    
    curl_easy_setopt(m_CURL, CURLOPT_UPLOAD, 1L);
   
    curl_easy_setopt(m_CURL, CURLOPT_PUT, 1L);
    
    curl_easy_setopt(m_CURL, CURLOPT_NOPROGRESS, 0L);
    
    curl_formadd(&post, &last,
                 CURLFORM_COPYNAME, "filename",
                 CURLFORM_END);
    
    curl_easy_setopt(m_CURL, CURLOPT_URL,buf);
    
    curl_easy_setopt(m_CURL, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)file_info.st_size);
    
    curl_easy_setopt(m_CURL, CURLOPT_VERBOSE,1);
    //curl_easy_setopt(m_CURL, CURLOPT_WRITEFUNCTION, Response);
    
    curl_easy_setopt(m_CURL, CURLOPT_SEEKFUNCTION, seek_callback);
    
    curl_easy_setopt(m_CURL, CURLOPT_XFERINFOFUNCTION, xferinfo);
    curl_easy_setopt(m_CURL, CURLOPT_XFERINFODATA, &prog);
    
    
    
    
    
    CURLcode res;
    res = curl_easy_perform(m_CURL);
    if (res==CURLE_COULDNT_CONNECT) {
        printf("连接服务器失败！");
    }
    printf("");
    curl_easy_cleanup(m_CURL);
    close(hd);
    curl_global_cleanup();
    
    if (res!=CURLE_OK) {
        //如果返回错误，通过此方法输出错误信息
        fprintf(stderr, "curl_easy_perform() failed :%s",curl_easy_strerror(res));
    
       
        if (upload_files_timestamp_.find(filename) != upload_files_timestamp_.end())
        {
            if (++(upload_files_timestamp_.find(filename)->second) > 5)
            {
                upload_files_timestamp_.erase(filename);
                m_pUploadFaidFiles.push_back(filename);  // ronaldz. we put it into the failed upload files.
                cmd_data_pointer p(new cmd_data_string(filename));
                events::global_dispatch.raise_event(EVENT_UPLOAD_FILE_FAILED, p);
            }else{
                start_upload_file(filename);
            }
        }
    }else
    {
        upload_files_timestamp_.erase(filename);
        
        
        m_pUploadedFiles.push_back(filename);
        
        cmd_data_pointer p(new cmd_data_string(filename));
        events::global_dispatch.raise_event(EVENT_UPLOAD_FILE_SUCCESS, p);

    }
}

void FileManager::start_download_file(const std::string& filename)
{
    

    cocos2d::log("下载文件：%s", filename.c_str());
    cocos2d::network::HttpRequest* pRequest = new cocos2d::network::HttpRequest();
    
    char buf[512] = {0};
    init_file_server_network();
    if (AF_INET == file_server_addr_type_)
    {
        sprintf(buf, "http://%s:8080/poker/headpic/%s", file_server_addr_.c_str(), filename.c_str());
    }else if (AF_INET6 == file_server_addr_type_)
    {
        sprintf(buf, "http://[%s]:8080/poker/headpic/%s", file_server_addr_.c_str(), filename.c_str());
    }

    pRequest->setUrl(buf);
    
    pRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    pRequest->setResponseCallback(CC_CALLBACK_2(FileManager::handle_downloadfile,this,filename));
    cocos2d::network::HttpClient::getInstance()->send(pRequest);
    pRequest->release();

    
}

void FileManager::upload_file(const std::string& filename)
{
    if(!GDM->is_file_exist(filename))
    {
        return;
    }
    if (upload_files_timestamp_.find(filename) != upload_files_timestamp_.end())
    {
        return;
    }else
    {
        upload_files_timestamp_.insert(std::make_pair(filename, 0));
        auto t = std::thread(&FileManager::start_upload_file, this, filename);
        t.detach();
    }
}

bool FileManager::is_file_in_upload_queue(const std::string& filename)
{
    if(upload_files_timestamp_.empty()) {
        return false;
    }
    else {
        if (upload_files_timestamp_.find(filename) != upload_files_timestamp_.end())
            return true;
        else
            return false;
    }
    return false;
}

bool FileManager::is_file_in_download_queue(const std::string& filename){
    if(download_files_timestamp_.empty()) {
        return false;
    }
    else {
        if (download_files_timestamp_.find(filename) != download_files_timestamp_.end())
            return true;
        else
            return false;
    }
    return false;
}

void FileManager::download_file(const std::string& filename)
{
    if(GDM->is_file_exist(filename))
    {
        return;
    }
    if (download_files_timestamp_.find(filename) != download_files_timestamp_.end())
    {
        return;
    }else
    {
        download_files_timestamp_.insert(std::make_pair(filename, 0));
        auto t = std::thread(&FileManager::start_download_file, this, filename);
        t.detach();
    }
}

void FileManager::handle_uploadfile(cocos2d::network::HttpClient * pSender, cocos2d::network::HttpResponse * pResponse, std::string filename)
{
    if (!pResponse->isSucceed())
    {
        cocos2d::log("Upload File Failed! Error:%s", pResponse->getErrorBuffer());
    }
    if (pResponse->getResponseCode() != 200)
    {
        cocos2d::log("Upload File Failed! ResponseCode :%ld", pResponse->getResponseCode());
        if (!filename.empty())
        {
            m_pUploadFaidFiles.push_back(filename);
        }
    }else
    {
        if (!filename.empty())
        {
            m_pUploadFaidFiles.push_back(filename);
        }
        //dispatch_.raise_event(EVENT_UPLOAD_FILE_SUCCESS);
        cocos2d::log("Upload Success!");
    }
    cocos2d::log("handle_uploadfile : Upload filename %s", filename.c_str());

}

void FileManager::init(){
    recorder_ = __createRecorder();
    timer = 0;
    cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&FileManager::on_timer, this, std::placeholders::_1),
                                                               this,
                                                               5.0,
                                                               kRepeatForever,
                                                               0,
                                                               false,
                                                               "FileManager::on_timer");
}

const std::string FileManager::get_file_name(const std::string& filename)
{
    ssize_t index = filename.find_last_of("/");
    std::string file_name = filename.substr(index+1,filename.length());
    return file_name;
}

void FileManager::handle_downloadfile(cocos2d::network::HttpClient *pSender,cocos2d::network::HttpResponse *pResponse,std::string filename)
{
    if (!pResponse->isSucceed())
    {
        cocos2d::log("Receive Error! %s\n",pResponse->getErrorBuffer());
        return;
    }
    if(pResponse->getResponseCode() != 200 )
    {
        return;
    }

    std::vector<char> *buffer = pResponse->getResponseData();
    char* buf = (char*)malloc(buffer->size());
    std::copy(buffer->begin(), buffer->end(), buf);
    std::string fullpath;
    std::string new_file_name = get_file_name(filename);
    if (GDM->get_user_directory() == "")
    {
        return;
    }else
    {
        fullpath = GDM->get_user_directory() + new_file_name;
    }

    FILE *fp = fopen(fullpath.c_str(),"wb+");
    if(!fp)
    {
        return;
    }
    fwrite(buf,1,buffer->size(),fp);
    fclose(fp);
    
    download_files_timestamp_.erase(filename);
    
    if(tools::getFileExt(new_file_name) == "amr")
    {
        auto pos = new_file_name.find(".");
        std::string file = new_file_name.substr(0, pos);
        
        std::string amr_path = GDM->get_user_directory() + new_file_name;
        std::string wav_path = GDM->get_user_directory() + file + ".wav";
        recorder_->amrToWav(amr_path, wav_path);
        cmd_data_pointer p(new cmd_data_string(file + ".wav"));
        events::global_dispatch.raise_event(EVENT_DOWLOAD_FILE, p);
    }
    else
    {
        cocos2d::log("下载文件成功%s",new_file_name.c_str());
        cmd_data_pointer p(new cmd_data_string(new_file_name));
        events::global_dispatch.raise_event(EVENT_DOWLOAD_FILE, p);
    }
}

void FileManager::on_timer(float dt)
{
    for (auto it = download_files_timestamp_.begin(); it != download_files_timestamp_.end();)
    {
        start_download_file(it->first);
        it->second++;
        if (it->second > 5)
            it = download_files_timestamp_.erase(it);
        else
            it++;
    }
}

void FileManager::end_timer()
{
    cocos2d::Director::getInstance()->getScheduler()->unschedule("FileManager::on_timer", this);
}
