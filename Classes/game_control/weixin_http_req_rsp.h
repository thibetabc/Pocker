//
//  weixin_http_req_rsp.hpp
//  pokerclient
//
//  Created by zhongyu on 16/5/16.
//
//

#ifndef weixin_http_req_rsp_h
#define weixin_http_req_rsp_h
#include "network/HttpClient.h"
#include <stdio.h>
using namespace cocos2d::network;
using namespace std;
#define weixin_request_address1 "https://api.weixin.qq.com/sns/oauth2/access_token?appid="
#define weixin_request_address2 "https://api.weixin.qq.com/sns/oauth2/refresh_token?appid="
#define weixin_request_address3 "https://api.weixin.qq.com/sns/auth?access_token="
#define weixin_request_address4 "https://api.weixin.qq.com/sns/userinfo?access_token="
class weixin_http_req_rsp
{
public:
//    weixin_http_req_rsp();
//    ~weixin_http_req_rsp();
    void weixin_request(string code,string appid,string secret);
    void weixin_second_request(string appid, string refresh_token);
    void weixin_third_request(string access_token,string openid);
    void weixin_fourth_request(string access_token,string openid);
    void on_http_request_completed(HttpClient *sender, HttpResponse *response);
    void on_http_second_request_completed(HttpClient *sender, HttpResponse *response);
    void on_http_third_request_completed(HttpClient *sender, HttpResponse *response);
    void on_http_fourth_request_completed(HttpClient *sender, HttpResponse *response);
    std::string wxappid;
    std::string wxaccess_token;

};



#endif /* weixin_http_req_rsp_hpp */
