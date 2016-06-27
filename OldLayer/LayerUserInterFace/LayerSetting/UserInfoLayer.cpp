
#include "UserInfoLayer.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "LayerEditName.h"
#include "utils.h"
#include "cmd_def.h"
#include "ActionSheet.h"
#include "UserPropertyLayer.h"
#include "TipView.h"
#include "ShopLayer.h"
#include "LayerBindPhone.h"

bool UserInfoLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    m_bIsUploadSuccess = true;
    
    sprite_icon_ = nullptr;
    
    root_ = CSLoader::createNode("LayerUserInfo.csb");
    this->addChild(root_);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("user_setting", "用户设置"));
    //const_text_head_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_head"));
    //const_text_head_->setString(tools::local_string("user_head_image", "头像"));
    const_text_username_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "const_text_username"));
    const_text_username_->setString(tools::local_string("username", "用户名"));
    const_text_username_->setFontName("Thonburi");
    text_username_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_username"));
    text_password_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_password"));
    text_password_->setString(tools::local_string("password", "密码"));
    const_text_sex_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "const_text_sex"));
    const_text_sex_->setString(tools::local_string("sex", "性别"));
    text_sex_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_sex"));
    
    //    auto text_game_setting = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_game_setting"));
    //    text_game_setting->setString(tools::local_string("game_setting", "游戏内设置"));
    
    text_bind_phone_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_bind_phone"));;
    text_bind_phone_status_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_bind_phone_status"));;
    image_bind_phone_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_bind_phone"));
    image_bind_phone_->addClickEventListener(std::bind(&UserInfoLayer::click_image_bind_phone, this, std::placeholders::_1));
    image_bind_phone_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_bind_phone_1"));
    
    image_head_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon"));
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    image_user_head_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon_bg"));
    auto image_username = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_username"));
    auto image_password = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_password"));
    auto image_sex = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_sex"));
    auto image_game_setting = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_game_setting"));
    btn_back->addClickEventListener(std::bind(&UserInfoLayer::click_btn_back, this, std::placeholders::_1));
    image_user_head_->addClickEventListener(std::bind(&UserInfoLayer::click_image_user_head, this, std::placeholders::_1));
    image_username->addClickEventListener(std::bind(&UserInfoLayer::click_image_username, this, std::placeholders::_1));
    image_password->addClickEventListener(std::bind(&UserInfoLayer::click_image_password, this, std::placeholders::_1));
    image_sex->addClickEventListener(std::bind(&UserInfoLayer::click_image_sex, this, std::placeholders::_1));
    image_game_setting->addClickEventListener(std::bind(&UserInfoLayer::click_image_game_setting, this, std::placeholders::_1));
    //    auto imag_charge = dynamic_cast<ImageView *>(CSLoader::seekNodeByName(root, "image_game_jifen"));
    //    imag_charge->addClickEventListener(std::bind(&UserInfoLayer::click_charge, this ,std::placeholders::_1));
    //
    //    /*
    //     *屏蔽积分
    //     */
    //    imag_charge->setVisible(false);
    //    image_game_setting->setPosition(imag_charge->getPosition());
    //    auto text_game_setting = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_game_setting"));
    //    text_game_setting->setPositionY(text_game_setting->getPositionY() + image_game_setting->getContentSize().height * image_game_setting->getScale());
    //    auto arrow = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_gamesetting_1"));
    //    arrow->setVisible(false);
    //    image_game_setting->setZOrder(254);
    //    text_game_setting->setZOrder(255);
    
    
    update_data();
    
    register_events();
    
    return true;
}


void UserInfoLayer::update_data(cmd_data_pointer data)
{
    cocos2d::log("user_head_pic_name:%s", userinfo.picname().c_str());
    image_head_->loadTexture(userinfo.picname());
    image_head_->setVisible(false);
    image_head_->ignoreContentAdaptWithSize(false);
    if (sprite_icon_ && sprite_icon_->getParent()) {
        sprite_icon_->removeFromParent();
    }
    sprite_icon_ = ShaderSprite::create(userinfo.picname(), "image/head_mask.png");
    cocos2d::log("%f, %f, %f, %f", image_head_->getContentSize().width, sprite_icon_->getContentSize().width,
                 image_head_->getContentSize().height, sprite_icon_->getContentSize().height);
    sprite_icon_->setScale(image_head_->getContentSize().width*1.7 / sprite_icon_->getContentSize().width,
                           image_head_->getContentSize().height*1.7 / sprite_icon_->getContentSize().height);
    sprite_icon_->setPosition(image_head_->getPosition());
    root_->addChild(sprite_icon_);
    
    text_username_->setString(userinfo.nickname());
    std::string sex = userinfo.sex() == msg::Sex::MALE ? tools::local_string("male", "男") : tools::local_string("female", "女");
    text_sex_->setString(sex);
    
    std::string phone_number = userinfo.picname();
    if (phone_number.empty())
    {
        text_bind_phone_status_->setString("未绑定");
    }else
    {
        text_bind_phone_status_->setString(phone_number);
    }
    
    if(update_head_pic_callback_) {
        update_head_pic_callback_();
    }
}

void UserInfoLayer::click_btn_back(Ref* sender)
{
    if(m_bIsUploadSuccess)
    {
        this->removeFromParent();
    }
    else
    {
        TipView * tip = TipView::create();
        tip->update_note("头像正在上传，是否继续返回？");
        tip->set_tipview_close_callback([=]{this->removeFromParent();});
        tip->show_cancel(true);
        this->addChild(tip);
    }
}

void UserInfoLayer::click_charge(Ref *sender)
{
    TipView::showAlertView("免费期间此功能不开放");
    return;
    ShopLayer *shop = ShopLayer::create();
    this->addChild(shop);
}

void UserInfoLayer::click_image_user_head(Ref* sender)
{
    ActionSheet* as = ActionSheet::create();
    as->set_after_choose_pic_callback(std::bind(&UserInfoLayer::after_choose_pic, this, std::placeholders::_1));
    this->addChild(as);
}

void UserInfoLayer::after_choose_pic(const std::string& filename)
{
//    msg::UserDetailInfo userinfo;
//    userinfo.set_picname(filename);
//    GDM->send_change_user_setting(userinfo);
    const std::string pic_names_ = filename;
    FM.upload_file(pic_names_);
    m_StrPicName = pic_names_;
    LayerColor *black = LayerColor::create(Color4B(100, 100, 100, 100), 167, 167);
    black->setAnchorPoint(image_user_head_->getAnchorPoint());
    black->ignoreAnchorPointForPosition(false);
    //    black->setAnchorPoint(add_pic->getAnchorPoint());
    black->setPosition(image_user_head_->getPosition());
    image_user_head_->getParent()->addChild(black,300,1001);
    LabelTTF *label = LabelTTF::create("正在上传","Arial", 18);
    label->setColor(cocos2d::Color3B::WHITE);
    label->setPosition(Point(black->getContentSize()/2));
    black->addChild(label);
    m_bIsUploadSuccess = false;
}

void UserInfoLayer::click_image_username(Ref* sender)
{
    LayerEditName* len = LayerEditName::create(LayerEditName::EDIT_USER_NAME);
    this->addChild(len);
}
void UserInfoLayer::click_image_password(Ref* sender)
{
    LayerEditName* len = LayerEditName::create(LayerEditName::EDIT_USER_PASSWORD);
    this->addChild(len);
}
void UserInfoLayer::click_image_sex(Ref* sender)
{
    LayerEditName* len = LayerEditName::create(LayerEditName::EDIT_USER_SEX);
    this->addChild(len);
}

void UserInfoLayer::click_image_game_setting(Ref* sender)
{
    
    UserPropertyLayer* upl = UserPropertyLayer::create(false);
    this->addChild(upl, 10);
    
    
    //TipView::showAlertView("此功能暂未开通");
}

void UserInfoLayer::click_image_bind_phone(Ref * sender)
{
    std::string phone_number = userinfo.picname();
    LayerBindPhone* lbp;
    if (phone_number.empty())
    {
        lbp = LayerBindPhone::create(true);
    }else
    {
        lbp = LayerBindPhone::create(false);
    }
    this->addChild(lbp);
}

void UserInfoLayer::handle_upload_succeed(cmd_data_pointer data)
{
    bool bFinished = true;
    bool bNoFaild = true;
    
    auto strVec = FM.m_pUploadedFiles;
    if (std::find(strVec.begin(), strVec.end(), m_StrPicName) == strVec.end())
    {
        bFinished = false;
    }
    
    
    auto strVec1 = FM.m_pUploadFaidFiles;
    if (std::find(strVec1.begin(), strVec1.end(), m_StrPicName) != strVec1.end())
    {
        bNoFaild = false;
        
    }
    
    if (bFinished && bNoFaild)
    {
        image_user_head_->getParent()->removeChildByTag(1001);
        m_bIsUploadSuccess = true;
    }
}

void UserInfoLayer::register_events()
{
    
    dispatch_.register_event(EVENT_REFRESH_USER_INFO,
                             BIND_FUNC(this, UserInfoLayer::update_data));
    
    dispatch_.register_event(EVENT_HANDLE_EDIT_PLAYER_DATA_FINISHED,
                             BIND_FUNC(this, UserInfoLayer::update_data));
    dispatch_.register_event(EVENT_HANDLE_EDIT_PLAYER_DATA_FINISHED,
                             BIND_FUNC(this, UserInfoLayer::update_data));
    dispatch_.register_event(EVENT_UPLOAD_FILE_SUCCESS,BIND_FUNC(this, UserInfoLayer::handle_upload_succeed));
}