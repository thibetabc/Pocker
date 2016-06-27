//
//  LoginManager.cpp
//  pokerclient
//
//  Created by duanmt on 15/4/30.
//
//

#include "LoginManager.h"
#include "utils.h"
#include "cmd_def.h"
#include "net_manager.h"
#include "TipView.h"
#include "SceneManager.h"
#include "GameDataManager.h"
#include "PokerDataManager.h"
#include "FileManager.h"
#include "msg.pb.h"
#include "net_data_recv.h"
#include "PushManager.h"
#include "versionCheck.h"
#include "NET_CMD.h"
#include "ProcessorManager.h"
#include "ReconnectView.h"

//#define BIND_FUNC(func) std::bind(&func, this, std::placeholders::_1)

LoginManager* LoginManager::__login_manager__ = nullptr;

LoginManager::LoginManager()
{
    read_account();
    dispatch_.register_event(REQ_VERSIONCHECK, BIND_FUNC(this, LoginManager::handle_versioncheck));
}

void LoginManager::login()
{
    if (default_account_->get_login_mode() == account_data::LOGIN_MODE::USERNAME)
    {
        login(default_account_->get_username(), default_account_->get_password(), account_data::LOGIN_MODE::USERNAME);
    }else if (default_account_->get_login_mode() == account_data::LOGIN_MODE::PHONENUM)
    {
        login(default_account_->get_phonenum(), default_account_->get_password(), account_data::LOGIN_MODE::PHONENUM);
    }
    
}

void LoginManager::login(const std::string& account, const std::string& password, account_data::LOGIN_MODE login_mode)
{
    
    default_account_->set_password(password);
    auto resserver = net_manager::shared_instence().get_server_data_with_request_type(REQ_LOGIN);
    if (login_mode == account_data::LOGIN_MODE::USERNAME)
    {
        default_account_->set_username(account);
        if (resserver){
            auto processor= PM->login_up(account, password, false);
            net_manager::shared_instence().send_data(processor, REQ_LOGIN);
        }
        else
            send_versioncheck();
        
    }else if (login_mode == account_data::LOGIN_MODE::PHONENUM)
    {
        if (resserver){
            default_account_->set_phonenum(account);
            auto processor= PM->login_up(account, password, true);
            net_manager::shared_instence().send_data(processor, REQ_LOGIN);
        }
        else
            send_versioncheck();
    }
}

void LoginManager::send_versioncheck()
{
    auto server = net_manager::shared_instence().get_server_data_with_request_type(REQ_VERSIONCHECK);
    if (server){
        auto processor = PM->null_up();
        net_manager::shared_instence().send_data(processor, REQ_VERSIONCHECK);
    }
    else
        net_manager::shared_instence().GetLoginServer();
}

void LoginManager::handle_versioncheck(cmd_data_pointer data){
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(rec) {
        msg::Processor_1_VersionCheck processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        std::shared_ptr<server_data> resourceserver(new server_data(net_manager::SERVER_RESOURCE, processor.resserverinfo().ip(), processor.resserverinfo().ipv6(), processor.resserverinfo().port()));
        net_manager::shared_instence().add_server(resourceserver);
        resourceserver->signaldelay = 3.0f;
        resourceserver->signalCountToBreak = 3;
        resourceserver->signalID = REQ_SIGNAL_RESOURCE;
        IsFinishVersionCheck = true;
        GDM->set_is_check_version(processor.ischeckversion());
        net_manager::shared_instence().disconnect(net_manager::SERVER_LOGIN);
        if (processor.response().status() == msg::Status::SUCCESS)
        {
            auto default_account = get_default_account();
            if(default_account && default_account->get_status_login())
                login();
        }
        else
        {
            SceneManager::addLoginLayer(false,processor.response().message());
            SceneManager::GetLoginLayer()->ShowError(processor.response().message());
        }
        if (processor.versionstatus() == msg::VersionStatus::HasNewVersion || processor.versionstatus() == msg::VersionStatus::NeedUpdate)
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            auto tip = TipView::showAlertView(processor.response().message());
            versionCheck *v = new versionCheck();
            tip->show_cancel(true);
            tip->set_tipview_close_callback([v](){
                v->update_version();
            });
#else
            JniMethodInfo t;
            //告诉MyApplication已启动
            if(JniHelper::getStaticMethodInfo(t, "com/insul/poker/AppActivity", "GetAppActivity", "()Ljava/lang/Object;"))
            {
                jobject obj= t.env->CallStaticObjectMethod(t.classID,t.methodID);
                jstring jFullpath = t.env->NewStringUTF("");
                if (JniHelper::getMethodInfo(t, "com/insul/poker/AppActivity", "installAPK", "(Ljava/lang/String;)V"))
                    t.env->CallVoidMethod(obj,t.methodID,jFullpath);
            }
#endif
        }
    }
}

void LoginManager::save_account()
{
    std::shared_ptr<rapidjson::Document> json(new rapidjson::Document());
    json->SetObject();
    rapidjson::Document::AllocatorType& allocator = json->GetAllocator();
    json->AddMember("acc_name", default_account_->get_username().c_str(), allocator);
    json->AddMember("password", default_account_->get_password().c_str(), allocator);
    json->AddMember("status", default_account_->get_status_login(), allocator);
    
    json->AddMember("phonenum", default_account_->get_phonenum().c_str(), allocator);
    
    json->AddMember("loginmode", (int)(default_account_->get_login_mode()), allocator);
    
    std::string descrption = json->description();
    std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "account.json";
    file_utils::write_file(path, descrption);
    
    
    
}

void LoginManager::read_account()
{
    std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "account.json";
    const std::string content = tools::read_file(path);
    if(!content.empty()) {
        default_account_ = std::shared_ptr<account_data>(new account_data);
        rapidjson::Document doc;
        doc.Parse<0>(content.c_str());
        if(!doc.HasParseError()) {
            std::string acc_name = doc["acc_name"].GetString();
            std::string password = doc["password"].GetString();
            
            std::string phonenum;
            if (doc.HasMember("phonenum"))
            {
                phonenum = doc["phonenum"].GetString();
            }else
            {
                phonenum = "";
            }
            
            int loginmode;
            if (doc.HasMember("loginmode"))
            {
                loginmode = doc["loginmode"].GetInt();
            }else
            {
                loginmode = (int)account_data::LOGIN_MODE::USERNAME;
            }
            
            bool login = doc["status"].GetBool();
            
            default_account_->set_username(acc_name);
            default_account_->set_password(password);
            default_account_->set_status_login(login);
            default_account_->set_phonenum(phonenum);
            default_account_->set_login_mode((account_data::LOGIN_MODE)loginmode);
        }else{
            default_account_ = nullptr;
        }
    }
}

