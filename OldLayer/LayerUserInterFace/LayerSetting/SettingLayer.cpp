#include "SettingLayer.h"
#include "SceneManager.h"
#include "SettingLayerChild.h"
#include "GameDataManager.h"
#include "utils.h"
#include "UserInfoLayer.h"
#include "FileManager.h"
#include "cmd_def.h"
#include "TipView.h"
#include "ReportType.h"
#include "LayerRegister.h"
#include "LayerLicense.h"
#include "ShopLayer.h"
#include "PokerCardSprite.h"
#include "LayerUserMatchInfo.h"
#include "global_header.h"
#include "msg.pb.h"
#include "CCUMSocialSDK.h"
#include "UMShareButton.h"
#include "PayManager.h"
// 使用友盟命令空间
USING_NS_UM_SOCIAL;
#include "net_manager.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif
#define STORE  1001
#define DIAMONDSTORE 1002
bool SettingLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    this->setTag(TAG_SETTING_LAYER);
    sprite_icon_ = nullptr;
    
    auto root = CSLoader::createNode("LayerSetting.csb");
    this->addChild(root);
    
    root_ = root;
    text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    text_title->setString(tools::local_string("setting", "设置"));
    
    text_account = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_account"));
    text_account->setString(tools::local_string("account", "账号"));
    
    text_diamond_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_diamond"));
    text_diamond_->setString("钻石");
    
    text_hitpoint_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_hitpoint"));
    text_hitpoint_->setString("体力");
    
    text_record_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "record"));
    
    text_setting = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_setting"));
    text_setting->setString(tools::local_string("setting", "设置"));
    
    //text_logout = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_logout"));
    //text_logout->setString(tools::local_string("logout", "注销登录"));
    
    text_share = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_shared"));
    text_share->setString(tools::local_string("share", "分享"));
    text_share->setPositionX(text_share->getPositionX());

    text_report = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_report"));
    text_report->setString(tools::local_string("report_back", "反馈并举报"));
    
    text_store = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_store"));
    text_store->setString(tools::local_string("charge", "充值"));
    
    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon"));
    //image_icon_->setVisible(false);
    
    text_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_nickname"));
    text_account_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_account_name"));
    text_diamond_number_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_diamond_number"));
    text_hitpoint_number_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_hitpoint_number"));
    
    auto image_record = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_record"));
    auto image_setting = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_setting"));
    auto image_account = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_account"));
    auto image_share = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_shared"));//
    auto image_report = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_report"));
    auto image_store = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_store"));
    auto image_diamonde_store = dynamic_cast<ImageView *>(CSLoader::seekNodeByName(root, "image_diamond_store"));
    image_record->addClickEventListener(std::bind(&SettingLayer::click_image_record, this, std::placeholders::_1));
    
    image_setting->addClickEventListener(std::bind(&SettingLayer::click_image_setting, this, std::placeholders::_1));

    image_account->addClickEventListener(std::bind(&SettingLayer::click_image_account, this, std::placeholders::_1));
    //
    image_share->addClickEventListener(std::bind(&SettingLayer::click_image_share, this, std::placeholders::_1));
    image_report->addClickEventListener(std::bind(&SettingLayer::click_image_report, this, std::placeholders::_1));
    
    image_store->addClickEventListener(std::bind(&SettingLayer::click_image_store, this, std::placeholders::_1));
    image_diamonde_store->addClickEventListener(std::bind(&SettingLayer::send_get_goods_list, this));
    image_diamonde_store->setTag(DIAMONDSTORE);
    image_store->setTag(STORE);

    auto text_license_1 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_license_1"));
    text_license_1->setString(tools::local_string("license3", "使用本程序，即表示你同意"));
    
    auto text_license_2 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_license_2"));
    text_license_2->setString(tools::local_string("license4", "《好牌许可及服务协议》"));
    
    text_license_1->addClickEventListener(std::bind(&SettingLayer::clickLicense, this, std::placeholders::_1));
    text_license_2->addClickEventListener(std::bind(&SettingLayer::clickLicense, this, std::placeholders::_1));
    
    //by lvzhangle
    text_license_1->setVisible(false);
    text_license_2->setVisible(false);
    
    auto btn_logout = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_logout"));
    btn_logout->setTitleText(tools::local_string("logout", "注销登录"));
    btn_logout->addClickEventListener(std::bind(&SettingLayer::click_btn_logout, this, std::placeholders::_1));
    
    //auto text_jifen_name = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_score"));
    //auto text_jifen = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_score_name"));
    //text_jifen->setVisible(false);
    //text_jifen_name->setVisible(false);
    
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, SettingLayer::update_data));
    
    update_data();
    
    dispatch_.register_event(EVENT_CHANGE_LANGUAGE,
                             BIND_FUNC(this, SettingLayer::update_ui));
    dispatch_.register_event(EVENT_REQ_GET_GOODS_LIST,
                             BIND_FUNC(this, SettingLayer::handle_get_goods_list));
    dispatch_.register_event(EVENT_REFRESH_USER_INFO,
                             BIND_FUNC(this, SettingLayer::update_ui));
    
    return true;
}


void SettingLayer::send_get_goods_list()
{

    PayManager::getInstance()->SendGetGoodsList();

}

void SettingLayer::handle_get_goods_list(cmd_data_pointer)
{

    ShopLayer *pShopLayer = ShopLayer::create();
    
    this->getParent()->addChild(pShopLayer,255);
    pShopLayer->update_data(PayManager::getInstance()->GetShopGoods(),true);

}
void SettingLayer::clickLicense(Ref* sender)
{
    
    LayerLicense* ll = LayerLicense::create();
    ll->set_dismiss_callback([this]() {
        
    });
    this->addChild(ll);
}

void SettingLayer::update_ui(cmd_data_pointer)
{
    text_title->setString(tools::local_string("setting", "设置"));
    text_account->setString(tools::local_string("account", "账号"));
    text_setting->setString(tools::local_string("setting", "设置"));
    //text_logout->setString(tools::local_string("logout", "注销登录"));
    text_share->setString(tools::local_string("share", "分享给每66人送10元现金"));
    text_diamond_number_->setString(tools::anything2string(diamond));
    text_hitpoint_number_->setString(tools::anything2string(hitpoint));
}


void SettingLayer::update_data(cmd_data_pointer)
{
        if(GDM->is_file_exist(userinfo.picname())) {
            image_icon_->loadTexture(userinfo.picname());
            if (sprite_icon_ && sprite_icon_->getParent()) {
                sprite_icon_->removeFromParent();
            }
            sprite_icon_ = ShaderSprite::create(userinfo.picname(), "image/head_mask.png");
            sprite_icon_->setPosition(image_icon_->getPosition());
            sprite_icon_->setScale(image_icon_->getContentSize().width / sprite_icon_->getContentSize().width,
                                   image_icon_->getContentSize().height / sprite_icon_->getContentSize().height);
            root_->addChild(sprite_icon_, image_icon_->getZOrder());
        }
        else {
            FM.download_file(userinfo.picname());
        }
    
    
        text_nickname_->setString(userinfo.nickname());
        text_account_name_->setString(GDM->get_username());
        text_diamond_number_->setString(StringUtils::format("%d", diamond));
        text_hitpoint_number_->setString(StringUtils::format("%d", hitpoint));
    
}

void SettingLayer::click_image_account(Ref* sender)
{
    UserInfoLayer* uil = UserInfoLayer::create();
    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
    uil->set_update_head_pic_callback([this](){
        update_data(nullptr);
    });
    parent->addChild(uil);
}

void SettingLayer::click_image_record(Ref* sender)
{
    //TipView::showAlertView("施工中……");
    LayerUserMatchInfo* lumi = LayerUserMatchInfo::create();
    lumi->set_is_group_match(false, 0);
    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
    parent->addChild(lumi);
}

void SettingLayer::click_image_setting(Ref* sender)
{
    
    SettingLayerChild* slc = SettingLayerChild::create();
    this->addChild(slc);
    
    //TipView::showAlertView("此功能暂未开通");
    
}
void SettingLayer::click_btn_logout(Ref* sender)
{
    GDM->send_offline();
}

void SettingLayer::click_image_report(Ref* sender)
{
    ReportType* ryl = ReportType::create();
    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
    parent->addChild(ryl);
}

void SettingLayer::click_image_share(Ref* sender)
{
    CCUMSocialSDK *sdk;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    sdk = CCUMSocialSDK::create("556d5eaa67e58e5fcd001173");//友盟APP key
   
#else
    
    sdk = CCUMSocialSDK::create("55d7cc9c67e58eed9100790a");//友盟APP key
#endif
    
    
    sdk->setWeiXinAppInfo("wx659249d10a35848b","5b5b7b4b90c656a426d71c4f1e2bca32");//微信id，key（secret）

    vector<int>* platforms = new vector<int>();
    platforms->push_back(umeng::social::WEIXIN);
    platforms->push_back(umeng::social::WEIXIN_CIRCLE);

    sdk->setPlatforms(platforms);

    char  a[256];
    sprintf(a,"http://%s:8080/poker/share.jsp?userid=%s",
    net_manager::shared_instence().FileServerIP.c_str(),text_account_name_->getString().c_str());
    sdk->setTargetUrl(a);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    sdk->setPlatformShareContent(umeng::social::Platform::WEIXIN, "来看看这个朋友圈的约局神器，约德州大菠萝，比赛也可以", "assets/Icon108.png","好牌",a);
    sdk->setPlatformShareContent(umeng::social::Platform::WEIXIN_CIRCLE, "来看看这个朋友圈的约局神器，约德州大菠萝，比赛也可以", "assets/Icon108.png","好牌",a);
    
    sdk->openShare ("来看看这个朋友圈的约局神器，约德州大菠萝，比赛也可以", "assets/Icon108.png", share_selector(shareCallback1));
    
#else
    sdk->openShare ("来看看这个朋友圈的约局神器，约德州大菠萝，比赛也可以", "icon108.png", share_selector(shareCallback1));
    sdk->setPlatformShareContent(umeng::social::Platform::WEIXIN, "来看看这个朋友圈的约局神器，约德州大菠萝，比赛也可以", "icon108.png","好牌",a);
    sdk->setPlatformShareContent(umeng::social::Platform::WEIXIN_CIRCLE, "来看看这个朋友圈的约局神器，约德州大菠萝，比赛也可以", "icon108.png","好牌",a);
#endif
 
}
void SettingLayer::click_image_store(Ref *sender)
{

    ShopLayer *pShopLayer = ShopLayer::create();
    
    this->getParent()->addChild(pShopLayer,255);
//    //充值
//    Text * text_des = Text::create("钻石可以在游戏商城内消费", "", 32);
//    text_des->setColor(Color3B::BLACK);
//    text_des->cocos2d::Node::setPosition(375, 400);
//    pShopLayer->addChild(text_des);
    std::vector<std::string> description;
    description.push_back("花费6元获得60钻石");
    description.push_back("花费30元获得300钻石");
    description.push_back("花费60元获得600钻石");
    description.push_back("花费328元获得3280钻石");
    description.push_back("花费618元获得6180钻石");
    std::vector<std::string> name;
    name.push_back("60钻石");
    name.push_back("300钻石");
    name.push_back("600钻石");
    name.push_back("3280钻石");
    name.push_back("6180钻石");
    std::vector<std::string> id;
    id.push_back("com.chinarichinc.haopai.diamond_1");
    id.push_back("com.chinarichinc.haopai.diamond_2");
    id.push_back("com.chinarichinc.haopai.diamond_3_add");
    id.push_back("com.chinarichinc.haopai.diamond_4_add");
    id.push_back("com.chinarichinc.haopai.diamond_5_add");
    std::vector<int> price;
    price.push_back(6);
    price.push_back(30);
    price.push_back(60);
    price.push_back(328);
    price.push_back(618);
    
    std::vector<std::shared_ptr<msg::GoodInfo>> shop_data;
    for (int  i = 0; i < 5; i++)
    {
        std::shared_ptr<msg::GoodInfo> good_info(new msg::GoodInfo);
        good_info->set_id(id[i]);
        good_info->set_name(name[i]);
        good_info->set_price(price[i]);
        good_info->set_description(description[i]);
        shop_data.push_back(good_info);
    }
    pShopLayer->update_data(shop_data);

 
}
void shareCallback1(int platform,int stCode,std::string& errorMsg)
{
    
}
