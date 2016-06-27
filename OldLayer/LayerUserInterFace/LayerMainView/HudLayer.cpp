//
//  HudLayer.cpp
//  pokerclient
//
//  Created by duanmt on 15/5/3.
//
//
//test
#include "PayManager.h"
#include "PushManager.h"

#include "HudLayer.h"
#include "CreateGroupLayer.h"
#include "SettingLayer.h"
#include "LayerGroupList.h"
#include "GameDataManager.h"
#include "GroupSearchLayer.h"
#include "SceneManager.h"
#include "cmd_def.h"
#include "utils.h"
#include "RoomLayer.h"
#include "LayerChatOld.h"
#include "LayerFriendListOld.h"
#include "PokerDataManager.h"

#include "LayerMain.h"

HudLayer::LAYERTAG HudLayer::m_nTag;

#define RECONNECTLAYERTAG 2000


HudLayer * HudLayer::create(LAYERTAG Tag)
{
    m_nTag = Tag;
    HudLayer *pRet = new HudLayer();
    pRet->setAnchorPoint(Point(0, 0));
    //pRet->setScale(640.0/750.0, 1136.0/1334.0);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }

    
}

bool HudLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    child_layer_ = nullptr;
    
    auto root = cocos2d::CSLoader::createNode("LayerHud.csb");
    this->addChild(root);
    
    btn_1_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_1"));
    btn_2_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_2"));
    btn_3_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_3"));
    btn_4_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_4"));
    btn_5_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_5"));
    
    text_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_1"));
    text_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_2"));
    text_3_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_3"));
    text_4_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_4"));
    text_5_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_5"));
    
    update_ui(nullptr);
    
    image_red_group_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_red_group"));
    image_red_friend_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_red_friend"));
    image_red_friend_->setVisible(false);
    image_red_group_->setVisible(false);
    
    auto save_btn_image = [] (std::map<Button*, ButtonImage>& btn_images, Button* btn) {
        const std::string& image_normal = btn->getNormalFileName();
        const std::string& image_select = btn->getClickedFileName();
        btn_images.insert(std::make_pair(btn, ButtonImage(image_normal, image_select)));
    };
    
    save_btn_image(btn_images_, btn_1_);
    save_btn_image(btn_images_, btn_2_);
    save_btn_image(btn_images_, btn_3_);
    save_btn_image(btn_images_, btn_4_);
    save_btn_image(btn_images_, btn_5_);
    
    btn_1_->addClickEventListener(std::bind(&HudLayer::click_btn_1, this, std::placeholders::_1));
    btn_2_->addClickEventListener(std::bind(&HudLayer::click_btn_2, this, std::placeholders::_1));
    btn_3_->addClickEventListener(std::bind(&HudLayer::click_btn_3, this, std::placeholders::_1));
    btn_4_->addClickEventListener(std::bind(&HudLayer::click_btn_4, this, std::placeholders::_1));
    btn_5_->addClickEventListener(std::bind(&HudLayer::click_btn_5, this, std::placeholders::_1));
    
    switch (this->m_nTag)
    {
        case 1:
            click_btn_1(btn_1_);
            break;
        case 2:
            click_btn_2(btn_2_);
            break;
        case 3:
            click_btn_3(btn_3_);
            break;
        case 4:
            click_btn_4(btn_4_);
            break;
        case 5:
            click_btn_5(btn_5_);
            break;
        default:
            break;
    }
    
    
    dispatch_.register_event(EVENT_HANDLE_REFRESH_GROUP_CHAT_RECORD, BIND_FUNC(this, HudLayer::receive_group_message));
    dispatch_.register_event(EVENT_HANDLE_REFRESH_FRIEND_CHAT_RECORD, BIND_FUNC(this, HudLayer::receive_friend_message));
    dispatch_.register_event(EVENT_HANDLE_RECEIVE_REQUEST_LIST, BIND_FUNC(this, HudLayer::receive_friend_message));
    dispatch_.register_event(EVENT_CHANGE_LANGUAGE, BIND_FUNC(this, HudLayer::update_ui));
    
    
    return true;
}

void HudLayer::update_ui(cmd_data_pointer)
{
    text_1_->setString(tools::local_string("my_group", "我的群"));
    text_2_->setString(tools::local_string("my_friend", "好友"));
    text_3_->setString(tools::local_string("my_game", "游戏"));
    text_4_->setString(tools::local_string("ignore", "约友圈"));
    text_5_->setString(tools::local_string("setting", "设置"));
}

void HudLayer::receive_group_message(cmd_data_pointer)
{
    if(child_layer_) {
        if(!dynamic_cast<LayerGroupList*>(child_layer_)) {
            image_red_group_->setVisible(true);
        }
    }
}

void HudLayer::receive_friend_message(cmd_data_pointer)
{
    if(child_layer_) {
        if(!dynamic_cast<LayerFriendListOld*>(child_layer_)) {
            image_red_friend_->setVisible(true);
        }
    }
}


void HudLayer::click_btn_1(Ref* sender)
{
    btn_1_->setEnabled(false);
    
    btn_2_->setEnabled(true);
    btn_3_->setEnabled(true);
    btn_4_->setEnabled(true);
    btn_5_->setEnabled(true);
    
    text_1_->setColor(Color3B(251, 114, 4));
    
    text_2_->setColor(Color3B(255, 255, 255));
    text_3_->setColor(Color3B(255, 255, 255));
    text_4_->setColor(Color3B(255, 255, 255));
    text_5_->setColor(Color3B(255, 255, 255));
    
    reset_btns();
    image_red_group_->setVisible(false);
    auto gl = LayerGroupList::create();
    gl->set_btn_add_callback(std::bind(&HudLayer::click_btn_add, this));
    gl->set_btn_search_callback(std::bind(&HudLayer::click_btn_search, this));
    
    child_layer_ = gl;
    this->addChild(child_layer_, -1);
    
    
    ShowTutorial(1);
    
}


void HudLayer::ShowTutorial(int button){
    if(!GDM->get_show_tutorial(button))
    {
        return;   // 已经显示过了， 退出。
    }
    
    std::string PicName = "weblink.png";
    switch (button)
    {
        case 1:
            PicName = "tutor_group.png";
            break;
        case 2:
            PicName = "tutor_friend.png";
            break;
        case 3:
            PicName = "tutor_gamehall.png";
            break;
        case 4:
            PicName = "tutor_moment.png";
            break;
        case 5:
            PicName = "tutor_settings.png";
            break;
        default:
            break;
    }
        
    ImageView * temp_image = ImageView::create();

    temp_image->loadTexture(PicName);
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();

    temp_image->ignoreContentAdaptWithSize(false);
    temp_image->setContentSize(Size(glview->getDesignResolutionSize().width, glview->getDesignResolutionSize().height));
    
    
    temp_image->setAnchorPoint(Point(0, 0));
    temp_image->setPosition(Vec2(0, 0));
    temp_image->setTouchEnabled(true);
    temp_image->setSwallowTouches(true);
    
    this->addChild(temp_image, 1000);
    
    auto image = ImageView::create("res/login_res/hud_res/notips.png");
    this->addChild(image, 1001);
    image->setAnchorPoint(Point(0, 0.5));
    image->setPosition(Vec2(20, glview->getDesignResolutionSize().height -86));
    image->setEnabled(true);
    image->setTouchEnabled(true);

    image->addClickEventListener([=](Ref*){
        temp_image->removeFromParent();
        image->removeFromParent();
        UserDefault::getInstance()->setStringForKey("no_guaid", "YES");
        
        GDM->set_show_tutorial(button,false);
    });
    
    temp_image->addClickEventListener([=](Ref*){
        temp_image->removeFromParent();
        image->removeFromParent();
        
    });
}

void HudLayer::click_btn_2(Ref* sender)
{
    btn_2_->setEnabled(false);
    
    btn_1_->setEnabled(true);
    btn_3_->setEnabled(true);
    btn_4_->setEnabled(true);
    btn_5_->setEnabled(true);
    
    text_2_->setColor(Color3B(251, 114, 4));
    
    text_1_->setColor(Color3B(255, 255, 255));
    text_3_->setColor(Color3B(255, 255, 255));
    text_4_->setColor(Color3B(255, 255, 255));
    text_5_->setColor(Color3B(255, 255, 255));
    
    reset_btns();
    image_red_friend_->setVisible(false);
    
    child_layer_ = LayerFriendListOld::create();
    this->addChild(child_layer_, -1);
    
    
    ShowTutorial(2);
}

void HudLayer::click_btn_3(Ref* sender)
{
    btn_3_->setEnabled(false);
    
    btn_1_->setEnabled(true);
    btn_2_->setEnabled(true);
    btn_4_->setEnabled(true);
    btn_5_->setEnabled(true);
    
    text_3_->setColor(Color3B(251, 114, 4));
    
    text_1_->setColor(Color3B(255, 255, 255));
    text_2_->setColor(Color3B(255, 255, 255));
    text_4_->setColor(Color3B(255, 255, 255));
    text_5_->setColor(Color3B(255, 255, 255));
    
    if (NULL != Director::getInstance()->getRunningScene()){
        GDM->send_get_create_room_info();
        GDM->send_get_room_info();

    }
    reset_btns();


    child_layer_ = RoomLayer::create();
    ((RoomLayer *)child_layer_)->SetTableSize();
    this->addChild(child_layer_, -1);
    
    ShowTutorial(3);
}

void HudLayer::click_btn_4(Ref* sender)//约有圈
{
    btn_4_->setEnabled(false);
    
    btn_1_->setEnabled(true);
    btn_2_->setEnabled(true);
    btn_3_->setEnabled(true);
    btn_5_->setEnabled(true);
    
    text_4_->setColor(Color3B(251, 114, 4));
    
    text_1_->setColor(Color3B(255, 255, 255));
    text_2_->setColor(Color3B(255, 255, 255));
    text_3_->setColor(Color3B(255, 255, 255));
    text_5_->setColor(Color3B(255, 255, 255));
    
    reset_btns();
    child_layer_ = GroupFriend::create();
    this->addChild(child_layer_,-1, GroupFriendLayerTag);
    
    ShowTutorial(4);
}
void HudLayer::click_btn_5(Ref* sender)
{
    btn_5_->setEnabled(false);
    
    btn_1_->setEnabled(true);
    btn_2_->setEnabled(true);
    btn_3_->setEnabled(true);
    btn_4_->setEnabled(true);
    
    text_5_->setColor(Color3B(251, 114, 4));
    
    text_1_->setColor(Color3B(255, 255, 255));
    text_2_->setColor(Color3B(255, 255, 255));
    text_3_->setColor(Color3B(255, 255, 255));
    text_4_->setColor(Color3B(255, 255, 255));
    
//    reset_btns();
//    child_layer_ = SettingLayer::create();
//    this->addChild(child_layer_, -1);
//    GDM->send_get_user_setting();
//    ShowTutorial(5);
    
    reset_btns();
    child_layer_ = LayerMain::create();
    this->addChild(child_layer_, 2);
    //GDM->send_get_user_setting();
    ShowTutorial(5);
}

void HudLayer::click_btn_add()
{
    CreateGroupLayer* cgl = CreateGroupLayer::create();
    cgl->setAnchorPoint(Point(0, 0));
    this->addChild(cgl, 1);
}
void HudLayer::click_btn_search()
{
    GroupSearchLayer* gsl = GroupSearchLayer::create();
    this->addChild(gsl, 1);
}

GroupFriend* HudLayer::get_groupfriendlayer()
{
    return dynamic_cast<GroupFriend*>(getChildByTag(GroupFriendLayerTag));
}

LayerComment* HudLayer::get_groupfriendcommentlayer()
{
    return dynamic_cast<LayerComment*>(getChildByTag(GroupFriendCommentTag));
}

void HudLayer::goto_room_list()
{
    //GDM->send_get_room_info(GDM->get_select_group_id());
    GDM->send_get_create_room_info();
    RoomLayer* rl = RoomLayer::create();
    this->addChild(rl, 1);
}

void HudLayer::goto_chat_layer()
{
    LayerChatOld* glc = LayerChatOld::create();
    this->addChild(glc);
}

void HudLayer::ShowDisconnect(bool IsDisconnect)
{
    if(IsDisconnect)
    {
        if(!getChildByTag(RECONNECTLAYERTAG))
        {
            addChild(ReconnectView::create(), INT_MAX, RECONNECTLAYERTAG);
        }
    }
    else
    {
        if(getChildByTag(RECONNECTLAYERTAG))
        {
            getChildByTag(RECONNECTLAYERTAG)->removeFromParent();
        }
    }
}

void HudLayer::reset_btns()
{
    if(child_layer_)
    {
        child_layer_->removeFromParent();
        child_layer_ = nullptr;
    }
}

void HudLayer::set_btn_image_clicked(Ref* sender)
{
    Button* btn = dynamic_cast<Button*>(sender);
    auto iter = btn_images_.find(btn);
    if(iter != btn_images_.end()) {
        btn->loadTextureNormal(iter->second.image_select);
        btn->loadTexturePressed(iter->second.image_normal);
    }
    else {
        assert(0);
    }
}

void HudLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();}