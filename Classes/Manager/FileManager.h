#pragma once

#include "cmd_data.h"
#include "events.h"
#include "HttpRequest.h"
#include "HttpClient.h"
#include <vector>

#define FM FileManager::shared_instence()

class VoiceRecorderInterface;

class FileManager: public SINGLETON<FileManager>, public manager_interface
{
public:
    void handle_uploadfile(cocos2d::network::HttpClient * pSender, cocos2d::network::HttpResponse * pResponse, std::string filename);
    void handle_downloadfile(cocos2d::network::HttpClient *pSender,cocos2d::network::HttpResponse *pResponse,std::string filename);
    const std::shared_ptr<VoiceRecorderInterface>& get_recorder() { return recorder_;}
    const std::string get_file_name(const std::string& filename);
    void upload_file(const std::string& filename);
    
    void download_file(const std::string& filename);
    bool is_file_in_upload_queue(const std::string& filename);
    bool is_file_in_download_queue(const std::string& filename);
    void start_download_file(const std::string& filename);
    void start_upload_file(const std::string& filename);
    static size_t Response(char *dataPtr, size_t size, size_t nmemb, void *user_p);
    std::vector<std::string> m_pUploadedFiles;
    std::vector<std::string> m_pUploadFaidFiles;
    void init();
    void update(){}
private:
    bool init_file_server_network();
    void on_timer(float dt);
    void end_timer();
    events::event_dispatch dispatch_;
    
    std::map<std::string, uint32_t> download_files_timestamp_;
    std::map<std::string, uint32_t> upload_files_timestamp_;
    
    std::shared_ptr<VoiceRecorderInterface> recorder_;
    
    std::string m_pUploadingFile = "";
    
    std::list<std::string> download_files_;
    
    std::string file_server_addr_;
    int file_server_addr_type_;
    
    std::mutex mutex_;
    long timer;
};

extern std::shared_ptr<VoiceRecorderInterface> __createRecorder();