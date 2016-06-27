#ifndef __pokerclient__LoginManager__
#define __pokerclient__LoginManager__

#include <string>
#include <vector>
#include <map>
#include "events.h"
#include "cocos2d.h"

#define LM LoginManager::getInstance()

class account_data {
public:
    enum LOGIN_MODE{
        USERNAME = 0,
        PHONENUM = 1
    };
    const std::string& get_username() { return username_;}
    const std::string& get_phonenum() { return phonenum_;}
    const std::string& get_password() { return password_;}
    const bool get_status_login() { return status_login_;}
    const LOGIN_MODE get_login_mode() { return login_mode_;}
    
    void set_username(const std::string& username) { username_ = username;}
    void set_phonenum(const std::string& phonenum) { phonenum_ = phonenum;}
    void set_password(const std::string& password) { password_ = password;}
    void set_status_login(const bool login) { status_login_ = login;}
    void set_login_mode(LOGIN_MODE login_mode) { login_mode_ = login_mode;}
    
private:
    std::string username_;
    std::string phonenum_;
    std::string password_;
    uint32_t user_id_;
    bool status_login_ = false;
    LOGIN_MODE login_mode_ = LOGIN_MODE::USERNAME;
};

class LoginManager {
public:
    static LoginManager* getInstance() {
        if(!__login_manager__) {
            __login_manager__ = new LoginManager();
        }
        return __login_manager__;
    }
    
    static void destroy() {
        cocos2d::log("destroy--LoginManager");
        delete __login_manager__;
        __login_manager__ = nullptr;
    }
    
    void save_account();
    void read_account();
    std::shared_ptr<account_data> get_default_account() { return default_account_;}
    
    void connect_login_server();
    void login();
    void check_version(){}
    void login(const std::string& account, const std::string& password, account_data::LOGIN_MODE login_mode);
    void send_versioncheck();
private:
    
#pragma mark tcp callback --
    void handle_versioncheck(cmd_data_pointer data);
private:
    std::shared_ptr<account_data> default_account_ = std::shared_ptr<account_data>(new account_data);
    events::event_dispatch dispatch_;
    bool IsFinishVersionCheck = false;
private:
    LoginManager();
    LoginManager(const LoginManager&);
    LoginManager operator=(const LoginManager&);
    
    static LoginManager* __login_manager__;
};

#endif /* defined(__pokerclient__LoginManager__) */
