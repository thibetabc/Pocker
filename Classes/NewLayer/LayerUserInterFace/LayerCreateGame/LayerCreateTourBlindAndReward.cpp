//
//  LayerCreateTourBlindAndReward.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/5/09.
//
//

#include "LayerCreateTourBlindAndReward.h"
#include "NodeCreateTourBlindInfoCell.h"
#include "NodeCreateTourRewardInfoCell.h"
#include "LayerEditTourReward.h"

//height 810

LayerCreateTourBlindAndReward* LayerCreateTourBlindAndReward::create(msg::TournamentInfo & tourInfo, bool isCreate, SELECT_TYPE selectType)
{
    LayerCreateTourBlindAndReward* result = new LayerCreateTourBlindAndReward;
    if(result->init(tourInfo, isCreate, selectType)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

bool LayerCreateTourBlindAndReward::init(msg::TournamentInfo & tourInfo, bool isCreate, SELECT_TYPE selectType)
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerCreateTourBlindAndReward.csb");
    this->addChild(root_);
    
    layout_blind_info_ = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "layout_blind_info"));
    layout_reward_info_ = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "layout_reward_info"));
    layout_blind_info_title_ = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "layout_blind_info_title"));
    layout_reward_info_title_ = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "layout_reward_info_title"));
    layout_add_reward_cell_ = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "layout_add_reward_cell"));
    
    text_blind_info_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(layout_blind_info_, "text_blind_info"));
    text_reward_info_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(layout_reward_info_, "text_reward_info"));
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerCreateTourBlindAndReward::click_btn_back, this, std::placeholders::_1));
    
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    
    image_listview_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_listview_bg"));
    
    layout_blind_info_->addClickEventListener(std::bind(&LayerCreateTourBlindAndReward::click_blind_info, this, std::placeholders::_1));
    
    layout_reward_info_->addClickEventListener(std::bind(&LayerCreateTourBlindAndReward::click_reward_info, this, std::placeholders::_1));
    
    layout_add_reward_cell_->addClickEventListener(std::bind(&LayerCreateTourBlindAndReward::click_add_reward_cell, this, std::placeholders::_1));
    
    select_type_ = selectType;
    update_data(tourInfo, isCreate);
    
    return true;
}

void LayerCreateTourBlindAndReward::update_data(msg::TournamentInfo &tourInfo, bool isCreate)
{
    is_create_ = isCreate;
    tournament_info_ = tourInfo;
    refresh_listview();
}

void LayerCreateTourBlindAndReward::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}

void LayerCreateTourBlindAndReward::click_blind_info(Ref * sender)
{
    select_type_ = SELECT_TYPE::BLIND_INFO;
    refresh_listview();
}

void LayerCreateTourBlindAndReward::click_reward_info(Ref * sender)
{
    select_type_ = SELECT_TYPE::REWARD_INFO;
    refresh_listview();
}

void LayerCreateTourBlindAndReward::click_add_reward_cell(Ref * sender)
{
    cocos2d::log("click_add_reward_cell");
    if (tournament_info_.tournamentprize().level().size() > 0)
    {
        int intCopyCount = tournament_info_.tournamentprize().level().size() - 1;
        tournament_info_.mutable_tournamentprize()->mutable_level()->Add()->CopyFrom(tournament_info_.tournamentprize().level(intCopyCount));
    }else
    {
        msg::PrizeLevel* tempReward = tournament_info_.mutable_tournamentprize()->add_level();
        tempReward->set_minrank(1);
        tempReward->set_maxrank(1);
        tempReward->set_prizepercent(10);
        tempReward->set_otherprize("");
    }
    after_prize_change();
    refresh_listview();
}

void LayerCreateTourBlindAndReward::refresh_listview()
{
    float listviewHeight = 0.0;
    
    float listviewHeightMin = 150.0;
    float listviewHeightMax = 810.0;
    float listviewBgHeight = 0.0;
    
    listview_->removeAllItems();
    
    if (select_type_ == SELECT_TYPE::BLIND_INFO)
    {
        text_blind_info_->setColor(cocos2d::Color3B(255, 255, 255));
        text_reward_info_->setColor(cocos2d::Color3B(151, 151, 151));
        
        layout_blind_info_title_->setVisible(true);
        layout_reward_info_title_->setVisible(false);
        layout_add_reward_cell_->setVisible(false);
        
        listviewBgHeight += layout_blind_info_title_->getContentSize().height;
        listviewHeightMin += layout_blind_info_title_->getContentSize().height;
        listviewHeightMax -= layout_blind_info_title_->getContentSize().height;
        
        for (int intBlindInfoCount = 0; intBlindInfoCount < tournament_info_.tournamentlevel().level().size(); intBlindInfoCount++)
        {
            NodeCreateTourBlindInfoCell * cellBlindInfo = NodeCreateTourBlindInfoCell::create(intBlindInfoCount, tournament_info_.tournamentlevel().level(intBlindInfoCount), is_create_);
            cellBlindInfo->set_cell_callback(std::bind(&LayerCreateTourBlindAndReward::click_blind_info_cell, this, std::placeholders::_1));
            cellBlindInfo->set_delete_callback(std::bind(&LayerCreateTourBlindAndReward::delete_blind_info, this, std::placeholders::_1));
            cellBlindInfo->set_add_callback(std::bind(&LayerCreateTourBlindAndReward::add_blind_info, this, std::placeholders::_1));
            cellBlindInfo->set_change_callback(std::bind(&LayerCreateTourBlindAndReward::change_blind_info, this, std::placeholders::_1, std::placeholders::_2));
            listview_->pushBackCustomItem(cellBlindInfo);
            
            listviewHeight += cellBlindInfo->getContentSize().height;
        }
        
        
    }else if (select_type_ == SELECT_TYPE::REWARD_INFO)
    {
        text_reward_info_->setColor(cocos2d::Color3B(255, 255, 255));
        text_blind_info_->setColor(cocos2d::Color3B(151, 151, 151));
        
        layout_blind_info_title_->setVisible(false);
        layout_reward_info_title_->setVisible(true);

        
        listviewBgHeight += layout_reward_info_title_->getContentSize().height;
        listviewHeightMin += layout_reward_info_title_->getContentSize().height;
        listviewHeightMax -= layout_reward_info_title_->getContentSize().height;
        
        if (is_create_)
        {
            layout_add_reward_cell_->setVisible(true);
            listviewBgHeight += layout_add_reward_cell_->getContentSize().height;
            listviewHeightMin += layout_add_reward_cell_->getContentSize().height;
            listviewHeightMax -= layout_add_reward_cell_->getContentSize().height;
        }else
        {
            layout_add_reward_cell_->setVisible(false);
        }

        
        for (int intBrwardInfoCount = 0; intBrwardInfoCount < tournament_info_.tournamentprize().level().size(); intBrwardInfoCount++)
        {
            NodeCreateTourRewardInfoCell * cellRewardInfo = NodeCreateTourRewardInfoCell::create(intBrwardInfoCount, tournament_info_.tournamentprize().level(intBrwardInfoCount), is_create_);
            cellRewardInfo->set_cell_callback(std::bind(&LayerCreateTourBlindAndReward::click_reward_info_cell, this, std::placeholders::_1));
            listview_->pushBackCustomItem(cellRewardInfo);
            
            listviewHeight += cellRewardInfo->getContentSize().height;
        }
    }
    
    listviewHeight = std::max(listviewHeight, listviewHeightMin);
    listviewHeight = std::min(listviewHeight, listviewHeightMax);
    listview_->setContentSize(cocos2d::Size(listview_->getContentSize().width, listviewHeight));
    
    listviewBgHeight += listviewHeight;
    image_listview_bg_->setContentSize(cocos2d::Size(image_listview_bg_->getContentSize().width, listviewBgHeight + 80));
    
    
    if (select_type_ == SELECT_TYPE::BLIND_INFO)
    {
        cocos2d::Point posLayoutBlindInfoTitle = layout_blind_info_title_->getPosition();
        cocos2d::Size sizeLayoutBlindInfoTitle = layout_blind_info_title_->getContentSize();
        posLayoutBlindInfoTitle.y = image_listview_bg_->getPosition().y - 10;
        
        layout_blind_info_title_->setPosition(posLayoutBlindInfoTitle);
        
        listview_->setPositionY(posLayoutBlindInfoTitle.y - sizeLayoutBlindInfoTitle.height);
        
    }else if (select_type_ == SELECT_TYPE::REWARD_INFO)
    {
        cocos2d::Point posLayoutRewardInfoTitle = layout_reward_info_title_->getPosition();
        cocos2d::Size sizeLayoutRewardInfoTitle = layout_reward_info_title_->getContentSize();
        posLayoutRewardInfoTitle.y = image_listview_bg_->getPosition().y - 10;
        
        layout_reward_info_title_->setPosition(posLayoutRewardInfoTitle);
        
        listview_->setPositionY(posLayoutRewardInfoTitle.y - sizeLayoutRewardInfoTitle.height);
        layout_add_reward_cell_->setPositionY(listview_->getPositionY() - listview_->getContentSize().height - 30);
    }
    
}

void LayerCreateTourBlindAndReward::click_blind_info_cell(uint32_t idx)
{
    auto Items = listview_->getItems();
    
    for (int iterCount = 0; iterCount < Items.size(); iterCount++)
    {
        NodeCreateTourBlindInfoCell * select_cell = dynamic_cast<NodeCreateTourBlindInfoCell*>(Items.at(iterCount));
        if (select_cell)
        {
            if (iterCount == idx)
            {
                if (!select_cell->get_editting() && is_create_)
                {
                    select_cell->set_editting(true);
                }else if (select_cell->get_editting())
                {
                    select_cell->set_editting(false);
                    tournament_info_.mutable_tournamentlevel()->mutable_level(idx)->CopyFrom(select_cell->get_blind_info());
                }
            }else
            {
                select_cell->set_editting(false);
                tournament_info_.mutable_tournamentlevel()->mutable_level(idx)->CopyFrom(select_cell->get_blind_info());
            }

        }
    }


}

void LayerCreateTourBlindAndReward::click_reward_info_cell(uint32_t idx)
{
    NodeCreateTourRewardInfoCell * select_cell = dynamic_cast<NodeCreateTourRewardInfoCell*>(listview_->getItem(idx));
    if (select_cell && is_create_)
    {
        cocos2d::log("edit reward cell");
        LayerEditTourReward * layerEditReward = LayerEditTourReward::create(idx, tournament_info_.tournamentprize().level(idx));
        layerEditReward->set_change_callback(std::bind(&LayerCreateTourBlindAndReward::change_reward_info, this, std::placeholders::_1, std::placeholders::_2));
        layerEditReward->set_delete_callback(std::bind(&LayerCreateTourBlindAndReward::delete_reward_info, this, std::placeholders::_1));
        this->addChild(layerEditReward, 0, layerEditReward->GetName());
    }
}

void LayerCreateTourBlindAndReward::change_reward_info(uint32_t idx, msg::PrizeLevel prizeLevel)
{
    tournament_info_.mutable_tournamentprize()->mutable_level(idx)->CopyFrom(prizeLevel);
    auto layer = getChildByName(typeid(LayerEditTourReward).name());
    if (layer)
        layer->removeFromParent();
    after_prize_change();
    refresh_listview();
}

void LayerCreateTourBlindAndReward::delete_reward_info(uint32_t idx)
{
    if (idx < tournament_info_.tournamentprize().level().size())
    {
        tournament_info_.mutable_tournamentprize()->mutable_level()->DeleteSubrange(idx, 1);
        after_prize_change();
        auto layer = getChildByName(typeid(LayerEditTourReward).name());
        if (layer)
            layer->removeFromParent();
        refresh_listview();
    }
}

void LayerCreateTourBlindAndReward::change_blind_info(uint32_t idx, msg::BlindLevel blindLevel)
{
    tournament_info_.mutable_tournamentlevel()->mutable_level(idx)->CopyFrom(blindLevel);
    after_level_change();
    //refresh_listview();
}

void LayerCreateTourBlindAndReward::add_blind_info(uint32_t idx)
{
    if (idx < tournament_info_.tournamentlevel().level().size())
    {
        tournament_info_.mutable_tournamentlevel()->mutable_level()->Add()->CopyFrom(tournament_info_.tournamentlevel().level(idx));

    }else
    {
        msg::BlindLevel* tempblind = tournament_info_.mutable_tournamentlevel()->add_level();
        tempblind->set_ante(0);
        tempblind->set_smallblind(50);
        tempblind->set_bigblind(100);
        tempblind->set_lasttime(0);
        tempblind->set_resttime(0);
    }
    for (int i = tournament_info_.tournamentlevel().level().size() - 1; i > idx; i--)
    {
        tournament_info_.mutable_tournamentlevel()->mutable_level()->SwapElements(i, i-1);
    }
    after_level_change();
    refresh_listview();
    click_blind_info_cell(idx);
}

void LayerCreateTourBlindAndReward::delete_blind_info(uint32_t idx)
{
    if (idx < tournament_info_.tournamentlevel().level().size())
    {
        tournament_info_.mutable_tournamentlevel()->mutable_level()->DeleteSubrange(idx, 1);
        after_level_change();
        refresh_listview();
    }
}

void LayerCreateTourBlindAndReward::after_level_change()
{
    if (level_change_callback_)
    {
        level_change_callback_(tournament_info_.tournamentlevel());
    }
}

void LayerCreateTourBlindAndReward::after_prize_change()
{
    if (prize_change_callback_)
    {
        prize_change_callback_(tournament_info_.tournamentprize());
    }
}