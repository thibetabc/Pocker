//
//  RoomCreateMatch.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/26.
//
//

#include "RoomCreateMatch.h"
#include "cocostudio/CocoStudio.h"
#include "MatchBaseInfo.h"
#include "MatchChipLevel.h"
#include "MatchRewardList.h"
#include "GameDataManager.h"
#include "MatchJoinList.h"
#include "PokerDataManager.h"

#define LAYERTAG1   10001
#define LAYERTAG2   10002
#define LAYERTAG3   10003
#define LAYERTAG4   10004
#define TAG_TITLE   10005
using namespace cocos2d;
using namespace cocos2d::ui;

bool RoomCreateMatch::m_bIsFirstOpen  = true;
int  RoomCreateMatch::m_nChoosedSizeModel = 1;
int RoomCreateMatch::m_nChoosedSpeedModel  = 1;

bool RoomCreateMatch::m_bIsCreate;
bool RoomCreateMatch::isSitAndGO;

bool RoomCreateMatch::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    root = CSLoader::createNode("LayerCreateMatch.csb");
    this->addChild(root);
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->addClickEventListener(std::bind(&RoomCreateMatch::click_btn_back, this, std::placeholders::_1));
    auto text_room_name = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "text_title"));
    text_room_name->setFontName("");
    text_room_name->setTag(TAG_TITLE);
    btn_create = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_save"));
    btn_create->addClickEventListener(std::bind(&RoomCreateMatch::click_btn_create, this, std::placeholders::_1));
    auto btn_baseinfo = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_base_info"));
    btn_baseinfo->addClickEventListener(std::bind(&RoomCreateMatch::click_btn_base_info, this, std::placeholders::_1));
    auto btn_chiptype = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_chip_type"));
    btn_chiptype->addClickEventListener(std::bind(&RoomCreateMatch::click_btn_chip_type, this, std::placeholders::_1));
    auto btn_rewardtype = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_reward_type"));
    btn_rewardtype->addClickEventListener(std::bind(&RoomCreateMatch::click_btn_reward_type, this, std::placeholders::_1));
    auto btn_joinlist = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_join_list"));
    btn_joinlist->addClickEventListener(std::bind(&RoomCreateMatch::click_btn_join_list, this, std::placeholders::_1));
    if(!m_bIsCreate)
    {
        btn_create->setVisible(false);
        image_view = Button::create("b-j_05.png","b-j-pre_05.png","b-j-pre_05.png");
        image_view->addClickEventListener(std::bind(&RoomCreateMatch::click_join, this));
        image_view->setPosition(btn_create->getPosition()+Point(-15,5));
        root->addChild(image_view);
        image_view->setTitleColor(Color3B::YELLOW);
        image_view->setTitleFontSize(30);
        refresh(nullptr);
    }
    dispatch_.register_event(EVENT_GET_TOURNAMENTIFO_LIST,BIND_FUNC(this, RoomCreateMatch::refresh));
    dispatch_.register_event(EVENT_TO_GET_TOURNAMENTIFO,BIND_FUNC(this, RoomCreateMatch::send_get_jion_list));
    click_btn_base_info(nullptr);
    dispatch_.register_event(EVENT_CANCEL_MATCH,BIND_FUNC(this,RoomCreateMatch::remove_self));
    return true;
}
void RoomCreateMatch::send_get_jion_list(cmd_data_pointer)
{
    //GDM->set_code(1);
    PDM->send_enter_room(room_id);
}
void RoomCreateMatch::refresh(cmd_data_pointer)
{
    if(m_bIsCreate)
        return;
    auto data = GDM->get_410_data()->userinfo();
    hasJoin = false;
    
    for (auto i :data)
    {
        if(i.userid() == GDM->get_user_id())
        {
            hasJoin = true;
            break;
        }
    }
    if (!hasJoin)
    {
        image_view->setTitleText("报名");
    }
    else
    {
        image_view->setTitleText("取消报名");
    }
    
}
void RoomCreateMatch::remove_self(cmd_data_pointer data)
{
    this->removeFromParent();
}
void RoomCreateMatch::click_join()
{
    GDM->send_join_match(room_id, !hasJoin);
}
void RoomCreateMatch::click_btn_back(Ref *sender)
{
    this->removeFromParent();
}

void RoomCreateMatch::click_btn_base_info(Ref *sender)
{
    for (int i = LAYERTAG1; i <= LAYERTAG4; i++)
    {
        auto LayerTag = this->getChildByTag(i);
        if (LayerTag != NULL)
        {
            LayerTag->removeFromParent();
        }
    }
    MatchBaseInfo *layer = (MatchBaseInfo *)this->getChildByTag(LAYERTAG1);
    if(layer)
    {
        layer->setVisible(true);
    }
    else
    {
        auto layer = MatchBaseInfo::create(this->m_bIsCreate,isSitAndGO);
        layer->set_isCreate(this->m_bIsCreate);
        this->addChild(layer);
        layer->setTag(LAYERTAG1);
    }
    
    
    
}
void RoomCreateMatch::click_btn_chip_type(Ref *sender)
{
    for (int i = LAYERTAG1; i <= LAYERTAG4; i++)
    {
        auto LayerTag = this->getChildByTag(i);
        if (LayerTag != NULL)
        {
            LayerTag->removeFromParent();
        }
    }
    MatchChipLevel *layer = (MatchChipLevel *)this->getChildByTag(LAYERTAG2);
    if(layer)
    {
        layer->setVisible(true);
    }
    else
    {
        auto layer = MatchChipLevel::create(this->m_bIsCreate);
        layer->set_isCreate(this->m_bIsCreate);
        this->addChild(layer);
        layer->setTag(LAYERTAG2);
    }
    
}
void RoomCreateMatch::click_btn_reward_type(Ref *sender)
{
    for (int i = LAYERTAG1; i <= LAYERTAG4; i++)
    {
        auto LayerTag = this->getChildByTag(i);
        if (LayerTag != NULL)
        {
            LayerTag->removeFromParent();
        }
    }

    MatchRewardList *layer = (MatchRewardList *)this->getChildByTag(LAYERTAG3);
    if(layer)
    {
        layer->setVisible(true);
    }
    else
    {
        auto layer = MatchRewardList::create(this->m_bIsCreate);
        this->addChild(layer);
        layer->setTag(LAYERTAG3);
        layer->set_isCreate(this->m_bIsCreate);
    }
    
}
void RoomCreateMatch::click_btn_join_list(Ref *sender)
{
    if(m_bIsCreate)
        return;
    for (int i = LAYERTAG1; i <= LAYERTAG4; i++)
    {
        auto LayerTag = this->getChildByTag(i);
        if (LayerTag != NULL)
        {
            LayerTag->removeFromParent();
        }
    }
    MatchJoinList *layer = (MatchJoinList *)this->getChildByTag(LAYERTAG4);
    if(layer)
    {
        layer->setVisible(true);
    }
    else
    {
        auto layer = MatchJoinList::create();
        this->addChild(layer);
        layer->setTag(LAYERTAG4);
        layer->set_room_id(room_id);
    }
    
}

void RoomCreateMatch::set_room_type(msg::RoomType roomtype)
{
    msg::RoomType type = (msg::RoomType)roomtype;
    //GDM->get_create_room_info()->mutable_roominfo()->mutable_roomtype()->set_roomtype(type);
    auto room_info =  GDM->get_room_infomations();
    auto text_title = (Text *)root->getChildByTag(TAG_TITLE);
    if (!text_title)
    {
        return;
    }
    for (auto& iter : room_info.createroominfo())
    {
        if (iter.roomtype().roomtype() == roomtype)
        {
            text_title->setString(iter.roomtype().roomtypename());
            break;
        }
    }
}


void RoomCreateMatch::click_btn_create(Ref *sender)
{
    //GDM->send_create_match(group_id_);
    this->removeFromParent();
}