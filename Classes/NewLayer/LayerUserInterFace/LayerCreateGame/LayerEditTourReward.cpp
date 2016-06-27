//
//  LayerEditTourReward.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/5/09.
//
//

#include "LayerEditTourReward.h"
#include "NodeCreateTourBlindInfoCell.h"
#include "NodeCreateTourRewardInfoCell.h"

#include "GameTools.h"
#include "utils.h"

//height 810

LayerEditTourReward* LayerEditTourReward::create(uint32_t idx, msg::PrizeLevel prizeLevel)
{
    LayerEditTourReward* result = new LayerEditTourReward;
    if(result->init(idx, prizeLevel)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

bool LayerEditTourReward::init(uint32_t idx, msg::PrizeLevel prizeLevel)
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    reward_idx_  = idx;
    prize_level_ = prizeLevel;
    
    root_ = CSLoader::createNode("LayerEditTourReward.csb");
    this->addChild(root_);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("price_rewards","奖池结构"));
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerEditTourReward::click_btn_back, this, std::placeholders::_1));
    
    image_listview_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_listview_bg"));
    
    text_rank_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_rank"));
    text_rank_->setString(tools::local_string("ranking","名次"));
    text_prize_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_prize"));
    text_prize_->setString(tools::local_string("bonus","奖金"));
    text_other_prize_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_other_prize"));
    text_other_prize_->setString(tools::local_string("extra_bonus","额外奖励"));
    image_max_rank_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_max_rank_bg"));
    
    image_min_rank_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_min_rank_bg"));
    image_prize_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_prize"));
    image_other_prize_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_other_prize"));
    
    tf_prize_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_prize"));
    tf_other_prize_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_other_prize"));
    tf_min_rank_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_min_rank"));
    tf_max_rank_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_max_rank"));
    
    editbox_prize_ = GameTools::createEditBoxFromTextField(this, tf_prize_);
    editbox_prize_->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_prize_->setMaxLength(editbox_prize_->getContentSize().width);
    editbox_prize_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    editbox_prize_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    editbox_prize_->setFontColor(Color3B(255, 255, 255));
    editbox_prize_->setPlaceholderFontColor(Color3B(151, 151, 151));
    editbox_prize_->setPlaceHolder("");
    editbox_prize_->setDelegate(this);
    
    editbox_other_prize_ = GameTools::createEditBoxFromTextField(this, tf_other_prize_);
    editbox_other_prize_->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_other_prize_->setMaxLength(editbox_other_prize_->getContentSize().width);
    editbox_other_prize_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    editbox_other_prize_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    editbox_other_prize_->setFontColor(Color3B(255, 255, 255));
    editbox_other_prize_->setPlaceholderFontColor(Color3B(151, 151, 151));
    editbox_other_prize_->setPlaceHolder("");
    editbox_other_prize_->setDelegate(this);
    
    editbox_min_rank_ = GameTools::createEditBoxFromTextField(this, tf_min_rank_);
    editbox_min_rank_->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_min_rank_->setMaxLength(editbox_min_rank_->getContentSize().width);
    editbox_min_rank_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    editbox_min_rank_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    editbox_min_rank_->setFontColor(Color3B(255, 255, 255));
    editbox_min_rank_->setPlaceholderFontColor(Color3B(151, 151, 151));
    editbox_min_rank_->setPlaceHolder("");
    editbox_min_rank_->setDelegate(this);
    
    editbox_max_rank_ = GameTools::createEditBoxFromTextField(this, tf_max_rank_);
    editbox_max_rank_->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_max_rank_->setMaxLength(editbox_max_rank_->getContentSize().width);
    editbox_max_rank_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    editbox_max_rank_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    editbox_max_rank_->setFontColor(Color3B(255, 255, 255));
    editbox_max_rank_->setPlaceholderFontColor(Color3B(151, 151, 151));
    editbox_max_rank_->setPlaceHolder("");
    editbox_max_rank_->setDelegate(this);
    
    btn_save_reward_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_save_reward"));
    btn_save_reward_->setTitleText(tools::local_string("save","保存"));
    btn_save_reward_->addClickEventListener(std::bind(&LayerEditTourReward::click_btn_save_reward, this, std::placeholders::_1));
    
    btn_delete_reward_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_delete_reward"));
    btn_delete_reward_->setTitleText(tools::local_string("delete","删除"));
    btn_delete_reward_->addClickEventListener(std::bind(&LayerEditTourReward::click_btn_delete_reward, this, std::placeholders::_1));
    
    text_rank_1_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_rank_1_1"));
    text_rank_1_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_rank_1_2"));
    text_rank_1_3_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_rank_1_3"));
    text_rank_1_2_->setString(tools::local_string("to","名--第"));
    text_rank_1_1_->setString(tools::local_string("strart_rank","From"));
    text_rank_1_3_->setString(tools::local_string("rankend","名"));
    
    editbox_min_rank_->setText(tools::to_string(prize_level_.minrank()).c_str());
    editbox_max_rank_->setText(tools::to_string(prize_level_.maxrank()).c_str());
    editbox_prize_->setText(tools::to_string(prize_level_.prizepercent()).c_str());
    editbox_other_prize_->setText(prize_level_.otherprize().c_str());
    
    return true;
}

void LayerEditTourReward::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}

void LayerEditTourReward::click_btn_save_reward(Ref * sender)
{
    if (edit_reward_callback_)
    {
        edit_reward_callback_(reward_idx_, prize_level_);
    }
}

void LayerEditTourReward::click_btn_delete_reward(Ref * sender)
{
    if (reward_delete_callback_)
    {
        reward_delete_callback_(reward_idx_);
    }
}

#pragma mark EditBox delegate
void LayerEditTourReward::editBoxEditingDidBegin(EditBox* editBox)
{
    
}

void LayerEditTourReward::editBoxEditingDidEnd(EditBox* editBox)
{
    
}

void LayerEditTourReward::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    
}

void LayerEditTourReward::editBoxReturn(EditBox* editBox)
{
    std::string strMinRank = editbox_min_rank_->getText();
    std::string strMaxRank = editbox_max_rank_->getText();
    std::string strPrize = editbox_prize_->getText();
    std::string strOhterPrize = editbox_other_prize_->getText();
    int intMinRank;
    int intMaxRank;
    int intPrize;
    
    if (strMinRank.empty())
    {
        strMinRank = "1";
    }
    if (strMaxRank.empty())
    {
        strMaxRank = "1";
    }
    if (strPrize.empty())
    {
        strPrize = "0";
    }
    intMinRank = tools::stod(strMinRank);
    intMaxRank = tools::stod(strMaxRank);
    intPrize = tools::stod(strPrize);
    
    if (intMinRank < 1)
    {
        intMinRank = 1;
    }
    if (intMaxRank < 1)
    {
        intMaxRank = 1;
    }
//    if (intMinRank > intMaxRank)
//    {
//        intMinRank = intMaxRank;
//    }
    if (intPrize < 1)
    {
        intPrize = 1;
    }else if (intPrize > 100)
    {
        intPrize = 100;
    }
    
    strMinRank = tools::to_string(intMinRank);
    strMaxRank = tools::to_string(intMaxRank);
    strPrize = tools::to_string(intPrize);
    
    editbox_min_rank_->setText(strMinRank.c_str());
    editbox_max_rank_->setText(strMaxRank.c_str());
    editbox_prize_->setText(strPrize.c_str());
    
    prize_level_.set_minrank(intMinRank);
    prize_level_.set_maxrank(intMaxRank);
    prize_level_.set_prizepercent(intPrize);
    prize_level_.set_otherprize(strOhterPrize);
}