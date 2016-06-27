//
//  weixin_http_req_rsp.cpp
//  pokerclient
//
//  Created by zhongyu on 16/5/16.
//
//

#include "weixin_http_req_rsp.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "cocos2d.h"

void weixin_http_req_rsp::weixin_request(string code,string appid,string secret)
{
    std::string url = cocos2d::StringUtils::format("https://api.weixin.qq.com/sns/oauth2/access_token?appid=%s&secret=%s&code=%s&grant_type=authorization_code",appid.c_str(),secret.c_str(),code.c_str());
    printf("app secret is %s",secret.c_str());
    wxappid=appid;
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url.c_str());
    //request->setRequestType(HttpRequest::Type::DELETE);
    request->setRequestData(nullptr, 0);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(weixin_http_req_rsp::on_http_request_completed, this));//rep返回数据
    HttpClient * client = HttpClient::getInstance();
    client->setTimeoutForConnect(2);
    client->setTimeoutForRead(2);
    client->sendImmediate(request);
    request->release();
    
}

void weixin_http_req_rsp::on_http_request_completed(HttpClient *sender, HttpResponse *response)
{
    
    printf("request is down");
    bool isError = true;
    if (!response)
    {
        isError = false;
    }
    if (!response->isSucceed())
    {
        //log("error buffer: %s", response->getErrorBuffer());
        isError = false;
    }
    if (isError)
    {
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        std::string resultJson = "";
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            resultJson += (*buffer)[i];
        }
        printf("resultJson is %s",resultJson.c_str());
        std::string url = "";
        std::string updateInfo = "";
        //json
        std::string access_token="";
        std::string refresh_token="";
        std::string unionid="";
        rapidjson::Document dJson;
        dJson.Parse<0>(resultJson.c_str());
        if (dJson.HasParseError())  //打印解析错误
        {
            CCLOG("GetParseError %s\n",dJson.GetParseError());
            return;
        }
        if (dJson.IsObject() && dJson.HasMember("access_token"))
        {
            const rapidjson::Value& allContactsJson = dJson["access_token"];
            access_token = allContactsJson.GetString();
            wxaccess_token=access_token;
            
        }
        if (dJson.IsObject() && dJson.HasMember("refresh_token"))
        {
            const rapidjson::Value& allContactsJson = dJson["refresh_token"];
            refresh_token = allContactsJson.GetString();
            
            
        }
        if (dJson.IsObject() && dJson.HasMember("unionid"))
        {
            const rapidjson::Value& allContactsJson = dJson["unionid"];
            unionid = allContactsJson.GetString();
            
        }
        
        if((access_token!="")&&(refresh_token!="")&&(unionid!=""))
        {
            printf("");
            weixin_second_request(wxappid,refresh_token);
        }
        
    }
}


void weixin_http_req_rsp::weixin_second_request(string appid, string refresh_token)
{
    std::string url = cocos2d::StringUtils::format("https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=%s&grant_type=refresh_token&refresh_token=%s",appid.c_str(),refresh_token.c_str());
    printf("app secret is %s",refresh_token.c_str());
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestData(nullptr, 0);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(weixin_http_req_rsp::on_http_second_request_completed, this));//rep返回数据
    HttpClient * client = HttpClient::getInstance();
    client->setTimeoutForConnect(2);
    client->setTimeoutForRead(2);
    client->sendImmediate(request);
    request->release();
}

void weixin_http_req_rsp::weixin_third_request(string access_token,string openid)
{
    std::string url = cocos2d::StringUtils::format("https://api.weixin.qq.com/sns/userinfo?access_token=％s&openid=%s",access_token.c_str(),openid.c_str());
    printf("app secret is %s",access_token.c_str());
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestData(nullptr, 0);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(weixin_http_req_rsp::on_http_third_request_completed, this));//rep返回数据
    HttpClient * client = HttpClient::getInstance();
    client->setTimeoutForConnect(2);
    client->setTimeoutForRead(2);
    client->sendImmediate(request);
    request->release();
}
void weixin_http_req_rsp::weixin_fourth_request(string access_token,string openid)
{
    
}

void weixin_http_req_rsp::on_http_second_request_completed(HttpClient *sender, HttpResponse *response)
{
    printf("request is down");
    bool isError = true;
    if (!response)
    {
        isError = false;
    }
    if (!response->isSucceed())
    {
        //log("error buffer: %s", response->getErrorBuffer());
        isError = false;
    }
    if (isError)
    {
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        std::string resultJson = "";
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            resultJson += (*buffer)[i];
        }
        printf("resultJson is %s",resultJson.c_str());
        std::string url = "";
        std::string updateInfo = "";
        //json
        std::string refresh_token="";
        std::string openid="";
        std::string scope="";
        rapidjson::Document dJson;
        dJson.Parse<0>(resultJson.c_str());
        if (dJson.HasParseError())  //打印解析错误
        {
            CCLOG("GetParseError %s\n",dJson.GetParseError());
            return;
        }
        if (dJson.IsObject() && dJson.HasMember("refresh_token"))
        {
            const rapidjson::Value& allContactsJson = dJson["refresh_token"];
            refresh_token = allContactsJson.GetString();
            
        }
        if (dJson.IsObject() && dJson.HasMember("openid"))
        {
            const rapidjson::Value& allContactsJson = dJson["openid"];
            openid = allContactsJson.GetString();
            
            
        }
        if (dJson.IsObject() && dJson.HasMember("scope"))
        {
            const rapidjson::Value& allContactsJson = dJson["scope"];
            scope = allContactsJson.GetString();
            
        }
        
        if(openid!="")
        {
            weixin_third_request(wxaccess_token,openid);
        }
        
    }
}

void weixin_http_req_rsp::on_http_third_request_completed(HttpClient *sender, HttpResponse *response)
{
    printf("request is down");
    bool isError = true;
    if (!response)
    {
        isError = false;
    }
    if (!response->isSucceed())
    {
        //log("error buffer: %s", response->getErrorBuffer());
        isError = false;
    }
    if (isError)
    {
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        std::string resultJson = "";
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            resultJson += (*buffer)[i];
        }
        printf("resultJson is %s",resultJson.c_str());
        std::string url = "";
        std::string updateInfo = "";
        //json
        std::string nickname="";
        std::string sex="";
        std::string unionid="";
        rapidjson::Document dJson;
        dJson.Parse<0>(resultJson.c_str());
        if (dJson.HasParseError())  //打印解析错误
        {
            CCLOG("GetParseError %s\n",dJson.GetParseError());
            return;
        }
        if (dJson.IsObject() && dJson.HasMember("nickname"))
        {
            const rapidjson::Value& allContactsJson = dJson["nickname"];
            nickname = allContactsJson.GetString();
            
        }
        if (dJson.IsObject() && dJson.HasMember("sex"))
        {
            const rapidjson::Value& allContactsJson = dJson["sex"];
            sex = allContactsJson.GetString();
            
            
        }
        if (dJson.IsObject() && dJson.HasMember("unionid"))
        {
            const rapidjson::Value& allContactsJson = dJson["unionid"];
            unionid = allContactsJson.GetString();
            
        }
        
        
        
    }
}

void weixin_http_req_rsp::on_http_fourth_request_completed(HttpClient *sender, HttpResponse *response)
{
    
}