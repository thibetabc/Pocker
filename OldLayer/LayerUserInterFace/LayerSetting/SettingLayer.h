#ifndef __pokerclient__SettingLayer__
#define __pokerclient__SettingLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "events.h"
#include "ShaderSprite.h"

void shareCallback1(int platform,int stCode,std::string& errorMsg);

using namespace cocos2d;
using namespace cocos2d::ui;

class SettingLayer : public BaseLayer {
public:
    CREATE_FUNC(SettingLayer);
    virtual bool init();
    
    void update_data(cmd_data_pointer = nullptr);
    
    void update_ui(cmd_data_pointer);
    
    void click_image_account(Ref* sender);
    void click_image_record(Ref* sender);
    void click_image_setting(Ref* sender);
    void click_image_share(Ref* sender);
    void click_image_report(Ref* sender);
    void click_image_store(Ref* sender);
    void clickLicense(Ref* sender);
    
    void click_btn_logout(Ref* sender);
    void send_get_goods_list();
    void handle_get_goods_list(cmd_data_pointer);
private:
    ImageView* image_icon_;
    ShaderSprite* sprite_icon_;
    Text* text_nickname_;
    Text* text_account_name_;
    Text* text_diamond_;
    Text* text_diamond_number_;
    Text* text_hitpoint_;
    Text* text_hitpoint_number_;
    events::event_dispatch dispatch_;
    Node* root_;
    cocos2d::ui::Text *text_title;
    cocos2d::ui::Text *text_account;
    Text* text_record_;;
    cocos2d::ui::Text *text_setting;
    cocos2d::ui::Text *text_logout;
    cocos2d::ui::Text *text_share;
    cocos2d::ui::Text *text_report;
    cocos2d::ui::Text *text_store;
    std::vector<std::pair<std::string, std::pair<std::string, std::string> > > languages_;
    int32_t hitpoint;
    int32_t diamond;
    msg::UserDetailInfo userinfo;
    
};

#endif /* defined(__pokerclient__SettingLayer__) */
