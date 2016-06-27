//
//  LayerCompetitionInfo.cpp
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#include "LayerCompetitionInfo.h"

bool LayerCompetitionInfo::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerCompetitionInfoPortrait.csb");
    this->addChild(root_);
    
    image_competition_info_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_competition_info_bg"));
    
    btn_overview_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_title_1"));
    btn_overview_->addClickEventListener(std::bind(&LayerCompetitionInfo::click_btn_overview, this, std::placeholders::_1));
    btn_blind_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_title_3"));
    btn_blind_->addClickEventListener(std::bind(&LayerCompetitionInfo::click_btn_blind, this, std::placeholders::_1));
    btn_reward_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_title_4"));
    btn_reward_->addClickEventListener(std::bind(&LayerCompetitionInfo::click_btn_reward, this, std::placeholders::_1));
    btn_player_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_title_2"));
    btn_player_->addClickEventListener(std::bind(&LayerCompetitionInfo::click_btn_player, this, std::placeholders::_1));
    btn_quit_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_quit"));
    btn_quit_->addClickEventListener(std::bind(&LayerCompetitionInfo::click_btn_quit, this, std::placeholders::_1));
    
    layer_over_view_ = LayerComInfoOverView::create();
    root_->addChild(layer_over_view_);
    layer_over_view_->setVisible(false);
    
    layer_blind_ = LayerComInfoBlind::create();
    root_->addChild(layer_blind_);
    layer_blind_->setVisible(false);
    
    layer_reward_ = LayerComInfoReward::create();
    root_->addChild(layer_reward_);
    layer_reward_->setVisible(false);
    
    layer_player_ = LayerComInfoPlayer::create();
    root_->addChild(layer_player_);
    layer_player_->setVisible(false);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerCompetitionInfo::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerCompetitionInfo::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerCompetitionInfo::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}
#pragma mark Data
void LayerCompetitionInfo::update_data(const msg::Processor_530_PokerGetTourmentStatus_DOWN& tourment_status)
{
    tourment_status_.CopyFrom(tourment_status);
    layer_over_view_->update_data(tourment_status_);
    layer_blind_->update_data(tourment_status_);
    layer_reward_->update_data(tourment_status_);
    layer_player_->update_data(tourment_status_);
    
    click_btn_overview(btn_overview_);
}

#pragma mark ClickEvent
void LayerCompetitionInfo::click_btn_quit(Ref * sender)
{
    this->removeFromParent();
}

void LayerCompetitionInfo::click_btn_overview(Ref* sender)
{
    btn_overview_->setEnabled(false);
    btn_blind_->setEnabled(true);
    btn_reward_->setEnabled(true);
    btn_player_->setEnabled(true);
    
    btn_overview_->setTitleColor(Color3B(255, 255, 255));
    btn_blind_->setTitleColor(Color3B(255, 186, 0));
    btn_reward_->setTitleColor(Color3B(255, 186, 0));
    btn_player_->setTitleColor(Color3B(255, 186, 0));
    
    layer_over_view_->setVisible(true);
    layer_blind_->setVisible(false);
    layer_reward_->setVisible(false);
    layer_player_->setVisible(false);
}

void LayerCompetitionInfo::click_btn_blind(Ref* sender)
{
    btn_overview_->setEnabled(true);
    btn_blind_->setEnabled(false);
    btn_reward_->setEnabled(true);
    btn_player_->setEnabled(true);
    
    btn_overview_->setTitleColor(Color3B(255, 186, 0));
    btn_blind_->setTitleColor(Color3B(255, 255, 255));
    btn_reward_->setTitleColor(Color3B(255, 186, 0));
    btn_player_->setTitleColor(Color3B(255, 186, 0));
    
    layer_over_view_->setVisible(false);
    layer_blind_->setVisible(true);
    layer_reward_->setVisible(false);
    layer_player_->setVisible(false);
}

void LayerCompetitionInfo::click_btn_reward(Ref* sender)
{
    btn_overview_->setEnabled(true);
    btn_blind_->setEnabled(true);
    btn_reward_->setEnabled(false);
    btn_player_->setEnabled(true);
    
    btn_overview_->setTitleColor(Color3B(255, 186, 0));
    btn_blind_->setTitleColor(Color3B(255, 186, 0));
    btn_reward_->setTitleColor(Color3B(255, 255, 255));
    btn_player_->setTitleColor(Color3B(255, 186, 0));
    
    layer_over_view_->setVisible(false);
    layer_blind_->setVisible(false);
    layer_reward_->setVisible(true);
    layer_player_->setVisible(false);
}

void LayerCompetitionInfo::click_btn_player(Ref* sender)
{
    btn_overview_->setEnabled(true);
    btn_blind_->setEnabled(true);
    btn_reward_->setEnabled(true);
    btn_player_->setEnabled(false);
    
    btn_overview_->setTitleColor(Color3B(255, 186, 0));
    btn_blind_->setTitleColor(Color3B(255, 186, 0));
    btn_reward_->setTitleColor(Color3B(255, 186, 0));
    btn_player_->setTitleColor(Color3B(255, 255, 255));
    
    layer_over_view_->setVisible(false);
    layer_blind_->setVisible(false);
    layer_reward_->setVisible(false);
    layer_player_->setVisible(true);
}

#pragma mark TouchEvent
bool LayerCompetitionInfo::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void LayerCompetitionInfo::onTouchMoved(Touch* touch, Event* event)
{

}

void LayerCompetitionInfo::onTouchEnded(Touch* touch, Event* event)
{
    //将openGL坐标转化为node坐标
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    if (image_competition_info_bg_->getBoundingBox().containsPoint(touchLocation))
    {
        
    }else
    {
        this->removeFromParent();
    }

}
