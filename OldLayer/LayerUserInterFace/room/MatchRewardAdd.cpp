//
//  MatchReward.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/30.
//
//

#include "MatchRewardAdd.h"
#include "RoomCreateMatch.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "msg.pb.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "utils.h"
#include "cmd_def.h"
#include "MatchRewardList.h"
#define TAG_RANKMIN       10001
#define TAG_RANKMAX       10002
#define TAG_REWARDRATE    10003
#define TAG_EXTRAREWARD   10004
//bool RoomCreateMatch::m_bIsFirstOpen ;
//int  RoomCreateMatch::m_nChoosedSizeModel ;
//int  RoomCreateMatch::m_nChoosedSpeedModel ;

bool MatchRewardAdd::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    root = CSLoader::createNode("LayerMatchRewardAdd.csb");
    this->addChild(root);
    auto rank1 = dynamic_cast<TextField *>(CSLoader::seekNodeByName(root, "TextField_1"));
    rank1->setTag(TAG_RANKMIN);
    auto rank2 = dynamic_cast<TextField *>(CSLoader::seekNodeByName(root, "TextField_2"));
    rank2->setTag(TAG_RANKMAX);
    auto reward_rate =dynamic_cast<TextField *>(CSLoader::seekNodeByName(root, "TextField_3"));
    reward_rate->setTag(TAG_REWARDRATE);
    auto extra_reward =dynamic_cast<TextField *>(CSLoader::seekNodeByName(root, "TextField_4"));
    extra_reward->setTag(TAG_EXTRAREWARD);
    auto save = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "text_save"));
    save->addClickEventListener(std::bind(&MatchRewardAdd::click_btn_save, this));
    auto rank_min = GameTools::createEditBoxFromTextField(root, rank1);
    rank_min->setDelegate(this);
    rank_min->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    auto rank_max = GameTools::createEditBoxFromTextField(root, rank2);
    rank_max->setDelegate(this);
    rank_max->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    auto rate = GameTools::createEditBoxFromTextField(root, reward_rate);
    rate->setDelegate(this);
    rate->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    auto extra = GameTools::createEditBoxFromTextField(root, extra_reward);
    extra->setDelegate(this);
    extra->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    auto btn_back = dynamic_cast<Button *>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->addClickEventListener(std::bind(&MatchRewardAdd::click_btn_back, this));
    
    auto btn_delete = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "text_delete"));
    btn_delete->addClickEventListener(std::bind(&MatchRewardAdd::click_btn_delete, this));

    return true;
}
void MatchRewardAdd::remove_this()
{
    auto parent = (MatchRewardList *)this->getParent();
    
    this->removeFromParent();
    parent->refresh_list(nullptr);
    
}
void MatchRewardAdd::click_btn_delete()
{
//    if(GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentprize()->mutable_level()->size() == 1)
//        return;
//    GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentprize()->mutable_level()->DeleteSubrange(idx, 1);
    remove_this();
//    NotificationCenter::getInstance()->postNotification("refresh_match_reward_list");
    
}

void MatchRewardAdd::click_btn_back()
{
    remove_this();
}

void MatchRewardAdd::update_data(msg::PrizeLevel *data,int idx)
{
    this->idx = idx;
    m_pData = data;
    auto minRank = (EditBox *)root->getChildByTag(TAG_RANKMIN);
    if (minRank)
    {
        minRank->setText(tools::anything2string(data->minrank()).c_str());
    }
    auto maxRank = (EditBox *)root->getChildByTag(TAG_RANKMAX);
    if (maxRank)
    {
        maxRank->setText(tools::anything2string(data->maxrank()).c_str());
    }
    auto rate = (EditBox *)root->getChildByTag(TAG_REWARDRATE);
    if (rate)
    {
        rate->setText(tools::anything2string(data->prizepercent()).c_str());
    }
    auto extra = (EditBox *)root->getChildByTag(TAG_EXTRAREWARD);
    if (extra)
    {
        if(data->has_otherprize())
            
            extra->setText(tools::anything2string(data->otherprize()).c_str());
    }
}

void MatchRewardAdd::click_btn_save()
{

    remove_this();
}

void MatchRewardAdd::editBoxEditingDidBegin(EditBox* editBox)
{
    
}
void MatchRewardAdd::editBoxEditingDidEnd(EditBox* editBox)
{
}
void MatchRewardAdd::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    auto rank1 = (EditBox *)root->getChildByTag(TAG_RANKMIN);
    if (rank1)
    {
        std::string s = rank1->getText();
        m_pData->set_minrank(atoi(s.c_str()));
    }
    auto rank2 = (EditBox *)root->getChildByTag(TAG_RANKMAX);
    if (rank2)
    {
        std::string s = rank2->getText();
        m_pData->set_maxrank(atoi(s.c_str()));
    }
    auto rate = (EditBox *)root->getChildByTag(TAG_REWARDRATE);
    if (rate)
    {
        std::string s = rate->getText();
        m_pData->set_prizepercent(atoi(s.c_str()));
    }
    auto extra = (EditBox *)root->getChildByTag(TAG_EXTRAREWARD);
    if (extra)
    {
        std::string s = extra->getText();
        m_pData->set_otherprize(s.c_str());
    }
    RoomCreateMatch::m_nChoosedSizeModel = 0;
}
void MatchRewardAdd::editBoxReturn(EditBox* editBox)
{
}
