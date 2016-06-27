//
//  MatchRewardNode.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/30.
//
//

#include "MatchRewardNode.h"
#include "GameDataManager.h"
#include "MatchRewardAdd.h"

#define TAG_RANK         10001
#define TAG_REWARD_RATE  10002
#define TAG_EXTRA_REWARD 10003
#define TAG_BG           10004
#define TAG_ADD          10005

bool MatchRewardNode::init()
{
    if (!TableViewCell::init())
    {
        return false;
    }
    root = CSLoader::createNode("NodeMatchReward.csb");
    this->addChild(root);
    auto rank = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_8"));
    rank->setTag(TAG_RANK);
    auto reward_rate =dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_9"));
    reward_rate->setTag(TAG_REWARD_RATE);
    auto extra_reward =dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_10"));
    extra_reward->setTag(TAG_EXTRA_REWARD);
    auto bg = dynamic_cast<ImageView *>(CSLoader::seekNodeByName(root, "image_shared"));
    bg->setTag(TAG_BG);
    bg->addClickEventListener(std::bind(&MatchRewardNode::click_edit, this));
    auto btn_add = dynamic_cast<ImageView *>(CSLoader::seekNodeByName(root, "Image_2"));
    btn_add->addClickEventListener(std::bind(&MatchRewardNode::click_add, this));
    btn_add->setTag(TAG_ADD);
    return true;
}
void MatchRewardNode::click_edit()
{
    if (m_bIsCreate)
    {
        MatchRewardAdd *layer = MatchRewardAdd::create();
        this->getParent()->getParent()->getParent()->addChild(layer);
        layer->update_data(m_pData, idx);
    }
    
}
void MatchRewardNode::click_add()
{
    msg::PrizeLevel *newLevel = new msg::PrizeLevel(*m_pData);
    newLevel->set_minrank(newLevel->maxrank() +1);
    newLevel->set_maxrank(newLevel->maxrank() +1);
//    GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentprize()->mutable_level()->AddAllocated(newLevel);
//    for(int i = GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentprize()->mutable_level()->size() - 1; i >idx + 1;i--)
//    {
//        GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentprize()->mutable_level()->SwapElements(i, i-1);
//    }
    TableView *table = (TableView *)this->getParent()->getParent();
    if(table)
    {
        table->reloadData();
    }
}

void MatchRewardNode::update_data(msg::PrizeLevel *data,int idx,bool isCreate)
{
    this->idx = idx;
    m_pData = data;
    m_bIsCreate = isCreate;
    if (!isCreate)
    {
        root->getChildByTag(TAG_ADD)->setVisible(false);
    }
    auto rank = (Text *)root->getChildByTag(TAG_RANK);
    if (rank)
    {
        std::stringstream s;
        s<<"第";
        int minRank = data->minrank();
        int maxRank = data->maxrank();
        if (minRank == maxRank)
        {
            s<<minRank;
        }
        else
        {
            s<<minRank;
            s<<"-";
            s<<maxRank;
        }
        s<<"名";
        rank->setString(s.str());
    }
    auto rate = (Text *)root->getChildByTag(TAG_REWARD_RATE);
    if (rate)
    {
        std::stringstream s;
        s<<data->prizepercent();
        rate->setString(s.str());
        rate->setColor(Color3B::BLACK);
    }
    auto extra_reward = (Text *)root->getChildByTag(TAG_EXTRA_REWARD);
    if (extra_reward)
    {
        std::stringstream s;
        s<<data->otherprize();
        std::string extra = s.str();
        if (extra.empty())
        {
            extra = "-";
        }
        extra_reward->setString(extra);
    }
}
void MatchRewardNode::set_is_touch(bool is_touch)
{
    std::string image_bg_name;
    if (is_touch)
    {
        image_bg_name = "login_res/group_res/group_cell_bg_select.png";
        
    }else {
        image_bg_name = "login_res/group_res/group_cell_bg.png";
    }
    auto bg = (ImageView *)root->getChildByTag(TAG_BG);
    if (bg)
    {
        bg->loadTexture(image_bg_name);

    }
}
