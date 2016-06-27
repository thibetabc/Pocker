//
//  LayerUserSetting.cpp
//  pokerclient
//
//  Created by zhongyu on 16/3/31.
//
//

#include "LayerUserSetting.h"

#include "SceneManager.h"
#include "GameDataManager.h"
#include "LoginManager.h"
#include "FileManager.h"
#include "ProcessorManager.h"
#include "utils.h"
#include "GameTools.h"
#include "NET_CMD.h"
#include "LayerBindPhone.h"
#include "TipView.h"
#include "ImagePickerBridge.h"
#include "LayerChangePassword.h"
#include "LayerScoreList.h"
#include "LayerGameSetting.h"
#include "LayerReportType.h"
#include "LayerShop.h"
#include "NodeScoreCell.h"
#include "LayerChoosePhoto.h"
#include "LayerShare.h"

#define USER_PHOTO_IMAGE_WIDTH 120
#define USER_PHOTO_IMAGE_HEIGHT 120

bool LayerUserSetting::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    
    this->set_show_dir(DIRECTION_NONE);
    root_ = CSLoader::createNode("LayerUserSetting.csb");
    this->addChild(root_);
    
    other_func_layer_ = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "panel_layer_color"));
    other_func_layer_->setVisible(false);
    other_func_layer_->addClickEventListener([this](Ref*){
        is_show_other_func_part(false);
    });
    
    sprite_icon_ = nullptr;
    Sex_ = msg::Sex::MALE;
    
    panel_other_function_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_other_func"));
    panel_other_function_->setVisible(false);
    panel_other_function_->setTouchEnabled(true);
    node_people_image_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_people_image"));
    
    text_title_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("usersetting_title","个人设置"));
    
    text_user_account_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_user_account"));
    text_user_account_->setString("");
    
    text_name_tip_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_name_tip"));
    text_name_tip_->setString(tools::local_string("name_tip","昵称"));
    
    const int EDITBOX_HEIGHT = 40;
    auto tf_user_name = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_user_name"));
    tf_user_name->setContentSize(Size(tf_user_name->getContentSize().width, EDITBOX_HEIGHT));
    tf_user_name_ = GameTools::createEditBoxFromTextField(root_,tf_user_name);
    tf_user_name_->setFontColor(Color3B(255, 255, 255));
    tf_user_name_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_user_name_->setInputMode(cocos2d::ui::EditBox::InputMode::ANY);
    tf_user_name_->setTouchEnabled(false);
    tf_user_name_->setFontSize(20);
    panel_other_function_->setLocalZOrder(tf_user_name_->getLocalZOrder() + 1);
    
    text_diamond_tip_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_diamond_tip"));
    text_diamond_tip_->setString(tools::local_string("diamond_tip","钻石"));
    
    text_diamond_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_diamond"));
    text_diamond_->setString("");
    
    text_stamina_tip_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_stamina_tip"));
    text_stamina_tip_->setString(tools::local_string("stamina_tip","体力"));
    
    text_stamina_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_stamina"));
    text_stamina_->setString("");
    
    text_phone_tip_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_phone_tip"));
    text_phone_tip_->setString(tools::local_string("bind_phone_tip","手机号"));
    
    text_bind_phone_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_bind_phone"));
    text_bind_phone_->setString(tools::local_string("not_bind_phone","未绑定"));
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_back,this,std::placeholders::_1));
    btn_back_->setTitleText("");
    
    btn_bind_phone_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_bind_phone"));
    btn_bind_phone_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_bind_phone, this,std::placeholders::_1));
    btn_bind_phone_->setTitleText(tools::local_string("usersetting_bind_phone","绑定手机号"));
    
    btn_buy_diamond_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_buy_diamond"));
    btn_buy_diamond_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_buy_diamond, this,std::placeholders::_1));
    btn_buy_diamond_->setTitleText(tools::local_string("usersetting_buy_diamond","充值"));
    
    btn_charge_stamina_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_charge_stamina"));
    btn_charge_stamina_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_charge_stamina, this,std::placeholders::_1));
    btn_charge_stamina_->setTitleText(tools::local_string("usersetting_charge_stamina","兑换"));
    
    btn_enter_record_layer_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_enter_record_layer"));
    btn_enter_record_layer_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_enter_record_layer, this,std::placeholders::_1));
    
    btn_enter_share_layer_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_enter_share_layer"));
    btn_enter_share_layer_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_enter_share_layer, this,std::placeholders::_1));
    btn_enter_share_layer_->setString(tools::local_string("usersetting_share_layer","分享"));
    
    btn_game_setting_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_game_setting"));
    btn_game_setting_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_game_setting, this,std::placeholders::_1));
    btn_game_setting_->setTitleText(tools::local_string("usersetting_game_setting","游戏设置"));
    
    btn_change_password_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_change_password"));
    btn_change_password_->setTitleText(tools::local_string("change_password","修改密码"));
    btn_change_password_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_change_password, this,std::placeholders::_1));
    
    btn_exit_game_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_exit_game"));
    btn_exit_game_->setString(tools::local_string("exit_game","注销"));
    btn_exit_game_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_exit_game, this,std::placeholders::_1));
    
    btn_user_response_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_user_response"));
    btn_user_response_->setString(tools::local_string("user_response","反馈"));
    btn_user_response_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_user_response, this,std::placeholders::_1));
    
    btn_other_function_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_other_function"));
    btn_other_function_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_other_function, this,std::placeholders::_1));
    
    btn_nickname_change_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_nickname_change"));
    btn_nickname_change_->setTitleText(tools::local_string("nickname_change","修改"));
    btn_nickname_change_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_nickname_change, this,std::placeholders::_1));
    
    image_nick_name_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_nick_name_bg"));
    image_nick_name_bg_->setVisible(false);
    
    btn_female_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_female"));
    btn_female_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_change_sex, this,std::placeholders::_1));
    btn_female_->setTitleText("");
    btn_female_->setTouchEnabled(false);
    
    btn_male_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_male"));
    btn_male_->addClickEventListener(std::bind(&LayerUserSetting::click_btn_change_sex, this,std::placeholders::_1));
    btn_male_->setVisible(false);
    btn_male_->setTitleText("");
    btn_male_->setTouchEnabled(false);
    
    this->set_game_type();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerUserSetting::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerUserSetting::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerUserSetting::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->setSwallowTouches(false);
    layout->addClickEventListener([this](Ref*){
        GameTools::editBoxLoseInputFocus();
    });
    
    
    this->addChild(layout, 10);
    
    RefreshLayer();
    return true;
}

void LayerUserSetting::RefreshLayer()
{
    if (!is_choose_photo_)
    {
        auto processor = PM->null_up();
        send_data(processor, REQ_GET_USER_SETTING);
    }
    else
    {
        is_choose_photo_ = false;
    }
}

void LayerUserSetting::set_game_type()
{
    text_score_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score_title"));
    text_score_title_->setString(tools::local_string("my_result", "个人战绩"));
    
    text_score_time_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score_time"));
    text_score_time_->setString(tools::local_string("Time range", "时间范围：7天之内"));
    
    text_score_first_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score_first"));
    
    text_score_third_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score_third"));
    
    text_score_second_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score_second"));
    
    text_score_first_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score_first_num"));
    text_score_first_num_->setString(tools::local_string("NA", "暂无"));
    
    text_score_game_type_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score_game_type"));
    
    text_score_third_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score_third_num"));
    text_score_third_num_->setString("");
    
    text_score_second_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score_second_num"));
    text_score_second_num_->setString(tools::local_string("NA", "暂无"));
    msg::RoomType room_type = GDM->get_last_game_room_type();
    bool is_tournament = GDM->get_last_game_is_tournament();
    is_tournament_ = is_tournament;
    switch (room_type) {
        case msg::RoomType::NLHOLDEM:
            text_score_game_type_->setString(tools::local_string("holdempoker", "德州扑克"));
            break;
        case msg::RoomType::OFC3:
            text_score_game_type_->setString(tools::local_string("pineapplepoker", "大菠萝"));
            break;
        case msg::RoomType::PLO:
            text_score_game_type_->setString(tools::local_string("omaha", "底池限注奥马哈"));
            break;
        case msg::RoomType::PLOHILO:
            text_score_game_type_->setString(tools::local_string("omaha_hl", "底池限注奥马哈 高/低"));
            break;
        case msg::RoomType::OFC4:
            text_score_game_type_->setString(tools::local_string("openfacechinesepoker", "明牌十三张"));
            break;
        case msg::RoomType::SIChUANMahjong:
            text_score_game_type_->setString(tools::local_string("sichuan_mahjong", "四川麻将"));
            break;
        default:
            break;
    }
    if (is_tournament)
    {
        text_score_first_->setString(tools::local_string("total_profit","总盈利"));
        text_score_first_num_->setString(tools::local_string("NA","暂无"));
        text_score_second_num_->setString("");
        text_score_second_->setString("");
        text_score_third_->setString(tools::local_string("average_game_profit","平均场次盈利"));
        text_score_third_num_->setString(tools::local_string("NA","暂无"));
    }
    else
    {
        text_score_first_->setString(tools::local_string("total_profit","总盈利"));
        text_score_first_num_->setString(tools::local_string("NA","暂无"));
        text_score_second_num_->setString(tools::local_string("NA","暂无"));
        text_score_second_->setString(tools::local_string("per_hour_profit","每小时盈利"));
        text_score_third_->setString(tools::local_string("per_100_profit","每100手盈利"));
        text_score_third_num_->setString(tools::local_string("NA","暂无"));
    }
    long endtime = tools::date();
    long time=7*24*60*60;
    long startTime=endtime-time;
    auto processor_score= PM->GetHistoryScore_Up(room_type,is_tournament_,tools::to_string(startTime),tools::to_string(endtime),0);
    send_data(processor_score, REQ_PLYAER_HISTORY_SOCRE);
}

void LayerUserSetting::is_show_other_func_part(bool show)
{
    if (show)
    {
        panel_other_function_->setVisible(true);
        other_func_layer_->setVisible(true);
        other_func_layer_->setSwallowTouches(true);
    }
    else
    {
        panel_other_function_->setVisible(false);
        other_func_layer_->setVisible(false);
    }
}

void LayerUserSetting::register_events()
{
    dispatch_.register_event(REQ_CHANGE_USER_SETTING,
                             BIND_FUNC(this, LayerUserSetting::handle_change_user_nickname));
    dispatch_.register_event(REQ_GET_USER_SETTING,
                             BIND_FUNC(this, LayerUserSetting::handle_get_user_setting));
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, LayerUserSetting::handle_download_event));
    dispatch_.register_event(EVENT_UPLOAD_FILE_SUCCESS,
                             BIND_FUNC(this, LayerUserSetting::handle_upload_event));
    dispatch_.register_event(REQ_PLYAER_HISTORY_SOCRE,
                             BIND_FUNC(this, LayerUserSetting::handle_get_score));
}

#pragma mark click_event
void LayerUserSetting::click_btn_back(cocos2d::Ref *sender)
{
    this->removeFromParent();
}

void LayerUserSetting::click_btn_bind_phone(cocos2d::Ref *sender)
{
    log("绑定手机号");
    is_show_other_func_part(false);
    LayerBindPhone* bindPhone=LayerBindPhone::create(true);
    bindPhone->set_phone_number_callback = std::bind(&LayerUserSetting::setPhoneNumber, this, std::placeholders::_1);
    this->addChild(bindPhone, 0, "bindPhone");
}

void LayerUserSetting::setPhoneNumber(const std::string& phone_number)
{
    refreshPhoneNumber(phone_number);
}

void LayerUserSetting::click_btn_buy_diamond(cocos2d::Ref *sender)
{
    is_show_other_func_part(false);
    LayerShop* shop = LayerShop::create();
    this->addChild(shop, 0, "LayerShop");
}

void LayerUserSetting::click_btn_charge_stamina(cocos2d::Ref *sender)
{
    is_show_other_func_part(false);
    LayerShop* shop = LayerShop::create(false);
    this->addChild(shop,0,"LayerShop");
}

void LayerUserSetting::click_btn_enter_record_layer(cocos2d::Ref *sender)
{
    is_show_other_func_part(false);
    LayerScoreList* layerScore=LayerScoreList::create();
    this->addChild(layerScore,0,"LayerScoreList");
}
void LayerUserSetting::click_btn_enter_share_layer(cocos2d::Ref *sender)
{
    auto shareLayer = LayerShare::create();
    shareLayer->setShareUserInfo(GDM->get_user_id());
    AddGlobalLayer(shareLayer);
}

void LayerUserSetting::click_btn_game_setting(cocos2d::Ref *sender)
{
    is_show_other_func_part(false);
    LayerGameSetting* gameSetting=LayerGameSetting::create(true);
    this->addChild(gameSetting,0,"LayerGameSetting");
}

void LayerUserSetting::click_btn_nickname_change(cocos2d::Ref *sender)
{
    is_show_other_func_part(false);
    if (btn_nickname_change_->getTitleText()==tools::local_string("nickname_change","修改"))
    {
        btn_nickname_change_->setTitleText(tools::local_string("nickname_save","保存"));
        tf_user_name_->setTouchEnabled(true);
        image_nick_name_bg_->setVisible(true);
        if (Sex_==msg::Sex::MALE )
        {
            btn_male_->setTouchEnabled(true);
        }
        else
        {
            btn_female_->setTouchEnabled(true);
        }
    }
    else
    {
        msg::UserDetailInfo info;
        info.set_nickname(tf_user_name_->getText());
        info.set_sex(Sex_);
        auto processor = PM->changeUserSetting_up(info);
        send_data(processor, REQ_CHANGE_USER_SETTING);
        btn_nickname_change_->setTitleText(tools::local_string("nickname_change","修改"));
        tf_user_name_->setTouchEnabled(false);
        image_nick_name_bg_->setVisible(false);
        btn_female_->setTouchEnabled(false);
        btn_male_->setTouchEnabled(false);
    }
    
}

void LayerUserSetting::click_btn_other_function(cocos2d::Ref *sender)
{
    is_show_other_func_part(true);
}

void LayerUserSetting::click_btn_user_response(cocos2d::Ref *sender)
{
    is_show_other_func_part(false);
    LayerReportType* report=LayerReportType::create();
    this->addChild(report,0,"LayerReportType");
}

void LayerUserSetting::click_btn_change_password(cocos2d::Ref *sender)
{
    is_show_other_func_part(false);
    LayerChangePassword* changePW=LayerChangePassword::create();
    this->addChild(changePW,0,"LayerChangePassword");
}

void LayerUserSetting::click_btn_change_sex(cocos2d::Ref *sender)
{
    is_show_other_func_part(false);
    if(btn_female_->isVisible())
    {
        btn_female_->setVisible(false);
        btn_male_->setVisible(true);
        Sex_=msg::Sex::MALE;
        btn_female_->setTouchEnabled(false);
        btn_male_->setTouchEnabled(true);
    }
    else
    {
        btn_male_->setVisible(false);
        btn_female_->setVisible(true);
        Sex_=msg::Sex::FEMALE;
        btn_female_->setTouchEnabled(true);
        btn_male_->setTouchEnabled(false);
    }
}

void LayerUserSetting::click_btn_exit_game(cocos2d::Ref *sender)
{
    is_show_other_func_part(false);
    std::string message=tools::local_string("logout_confirm","您确定要注销吗");
    auto tipviewWin = TipView::showAlertView(message, true);
    tipviewWin->set_tipview_close_callback(
        [=](){
            auto processor = PM->null_up();
            send_data(processor, REQ_OFFLINE);
        }
    );

}

void LayerUserSetting::after_choose_pic(const std::string& filename)
{
    change_picname_=filename;
    FM.upload_file(filename);
}

void LayerUserSetting::handle_get_user_setting(cmd_data_pointer data)
{
    ClearError();
    
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec));
    auto processor = PM->getUserSetting_down(rec);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS:
        {
            refreshUserInfo(GDM->get_userinfo()->nickname(), GDM->get_userinfo()->picname(), processor.diamond(), processor.hitpoint(), processor.phonenumber(), GDM->get_userinfo()->sex());
            break;
        }
        case msg::Status::FAILED:
            ShowError(tools::local_string("recv_player_data_failed", "获取玩家信息失败"));
            break;
        default:
            break;
    }
}

void LayerUserSetting::handle_get_score(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec))
    {
        return;
    }
    auto processor = PM->GetHistoryScore_Down(rec);
    std::vector<float> score = GDM->get_self_score(processor);
    if(is_tournament_)
    {
        text_score_first_num_->setString(tools::to_string(score.at(0)));
        long game_size = processor.score_size();
        text_score_third_num_->setString(tools::to_string(score.at(0)/ game_size));
    }
    else
    {
        if (processor.score_size()==0)
        {
             text_score_first_num_->setString(tools::local_string("NA", "暂无"));
        }
        else
        {
             text_score_first_num_->setString(tools::to_string(score.at(0)));
        }
        if (score.at(3) != 0)
        {
            char str_per_hour_profit[64] = {0};
            sprintf(str_per_hour_profit, "%.2f", score.at(4)/ score.at(3));
            
            text_score_second_num_->setString(str_per_hour_profit);
        }else
        {
            text_score_second_num_->setString(tools::local_string("NA", "暂无"));
        }
        
        if (score.at(1) == 0)
        {
            text_score_third_num_->setString(tools::local_string("NA", "暂无"));
        }else
        {
            char str_per_100_profit[64];
            sprintf(str_per_100_profit, "%.2f", score.at(2) / score.at(1) * 100);
            text_score_third_num_->setString(str_per_100_profit);
        }
    }
}

#pragma mark refreshdata
void LayerUserSetting::refreshUserInfo(std::string nickName,std::string picName,int diamond,int hit,std::string phoneNumber,msg::Sex sex)
{
    is_show_other_func_part(false);
    tf_user_name_->setText(nickName.c_str());
    user_nickname_=nickName;
    auto account = LM->get_default_account();
    std::string userAccount=account->get_username();
    text_user_account_->setString(tools::local_string("account", "账号：")+userAccount);
    Sex_=sex;
    btn_male_->setVisible(sex==msg::Sex::MALE);
    btn_female_->setVisible(sex==msg::Sex::FEMALE);
    text_diamond_->setString(tools::anything2string(diamond));
    text_stamina_->setString(tools::anything2string(hit));
    refreshPhoneNumber(phoneNumber);
    picName_=picName;
    change_picname_=picName;
    
    this->addUserPhoto();
//    bool isFileExist = GDM->is_file_exist(picName_);
//    if (!isFileExist)
//    {
//        FM.download_file(picName_);
//    }
//    else
//    {
//        this->addUserPhoto();
//    }
}

void LayerUserSetting::refreshPhoneNumber(const std::string& PhoneNumber)
{
    if (!PhoneNumber.empty()) {
        text_bind_phone_->setString(PhoneNumber);
        btn_bind_phone_->setTitleText(tools::local_string("usersetting_bind_phone_change","更换"));
    }
}

void LayerUserSetting::handle_upload_event(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (s->get_value() != change_picname_) return;
    msg::UserDetailInfo info;
    info.set_picname(change_picname_);
    auto processor = PM->changeUserSetting_up(info);
    send_data(processor, REQ_CHANGE_USER_SETTING);
}

void LayerUserSetting::handle_download_event(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (s->get_value() != picName_) return;

    addUserPhoto();
}

void LayerUserSetting::handle_change_user_nickname(cmd_data_pointer  data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()) {
        case msg::Status::SUCCESS:
        {
            user_nickname_=tf_user_name_->getText();
            GDM->get_userinfo()->set_nickname(tf_user_name_->getText());
            GDM->get_userinfo()->set_sex(Sex_);
            GDM->get_userinfo()->set_picname(change_picname_);
            if (picName_!=change_picname_)
            {
                picName_=change_picname_;
                bool isFileExist = GDM->is_file_exist(change_picname_);
                if (!isFileExist)
                {
                    FM.download_file(change_picname_);
                    
                }
                else
                {
                    this->addUserPhoto();
                }
            }
            break;
        }
        case msg::Status::FAILED:
            ShowError(processor.message());
            tf_user_name_->setText(user_nickname_.c_str());
            break;
        default:
            break;
    }
}

#pragma mark TouchEvent
bool LayerUserSetting::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void LayerUserSetting::onTouchMoved(Touch* touch, Event* event)
{
    
}



void LayerUserSetting::onTouchEnded(Touch* touch, Event* event)
{
    
    //将openGL坐标转化为node坐标
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    if (!panel_other_function_->isVisible())
    {
        if (sprite_icon_ && sprite_icon_->getBoundingBox().containsPoint(touchLocation))
        {
            is_show_other_func_part(false);
            is_choose_photo_ = true;
            LayerChoosePhoto* choose_photo = LayerChoosePhoto::create();
            choose_photo->set_choose_photo_callback(std::bind(&LayerUserSetting::after_choose_pic,this,std::placeholders::_1));
            
            this->addChild(choose_photo, 0, "LayerChoosePhoto");
        }
    }
    else
    {
        if (panel_other_function_->getBoundingBox().containsPoint(touchLocation) )
        {
            
        }
        else
        {
            is_show_other_func_part(false);
        }
    }
    
    
}

void LayerUserSetting::addUserPhoto()
{
    std::string strPicFile = "Default/image_defualt_user_icon_large.png";
    if (picName_ != "" && GDM->is_file_exist(picName_) && GameTools::isValidPicture(picName_))
    {
        strPicFile = picName_;
    }else
    {
        FM.download_file(picName_);
    }
    
    if (sprite_icon_ == nullptr)
    {
        sprite_icon_ = ShaderSprite::create(strPicFile, "Default/image_mask_circle.png");
        root_->addChild(sprite_icon_);
    }else
    {
        if (sprite_icon_->getFileName() != strPicFile)
        {
            sprite_icon_->initWithFile(strPicFile, "Default/image_mask_circle.png");
        }
    }
    sprite_icon_->setPosition(node_people_image_->getPosition());
    auto width=sprite_icon_->getContentSize().width;
    auto height=sprite_icon_->getContentSize().height;
    float scaleX=1;
    float scaleY=1;
    if (width>USER_PHOTO_IMAGE_WIDTH)
    {
        scaleX=USER_PHOTO_IMAGE_WIDTH/width;
    }
    
    if (height>USER_PHOTO_IMAGE_HEIGHT)
    {
        scaleY=USER_PHOTO_IMAGE_HEIGHT/height;
    }
    sprite_icon_->setScale(scaleX,scaleY);
}
cocos2d::Vec2 LayerUserSetting::GetErrorPoint()
{
    Size winSize = Director::getInstance()->getWinSize();
    return Vec2(winSize.width/2,winSize.height-150);
}
