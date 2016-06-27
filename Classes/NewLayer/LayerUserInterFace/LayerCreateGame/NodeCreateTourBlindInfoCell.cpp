//
//  NodeCreateTourBlindInfoCell.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/5/26.
//
//

#include "NodeCreateTourBlindInfoCell.h"
#include "GameTools.h"
#include "utils.h"

NodeCreateTourBlindInfoCell* NodeCreateTourBlindInfoCell::create(uint32_t idx, msg::BlindLevel blindLevel, bool isCreate)
{
    NodeCreateTourBlindInfoCell* result = new NodeCreateTourBlindInfoCell;
    if(result->init(idx, blindLevel, isCreate)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

bool NodeCreateTourBlindInfoCell::init(uint32_t idx, msg::BlindLevel blindLevel, bool isCreate)
{
    if(!Widget::init()) {
        return false;
    }
    
    cell_idx_ = idx;
    blind_level_ = blindLevel;
    is_create_ = isCreate;
    
    root_ = CSLoader::createNode("NodeTournamentBlindInfoCell.csb");
    this->addChild(root_);
    
    btn_delete_blind_cell_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_delete_blind_cell"));
    btn_delete_blind_cell_->addClickEventListener(std::bind(&NodeCreateTourBlindInfoCell::click_delete_blind_cell, this, std::placeholders::_1));
    btn_add_blind_cell_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_add_blind_cell"));
    btn_add_blind_cell_->addClickEventListener(std::bind(&NodeCreateTourBlindInfoCell::click_add_blind_cell, this, std::placeholders::_1));
    
    image_property_input_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_property_input_bg"));
    
    text_blind_info_rank_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_blind_info_rank"));
    text_blind_info_blind_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_blind_info_blind"));
    text_blind_info_ante_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_blind_info_ante"));
    text_blind_info_lasttime_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_blind_info_lasttime"));
    text_blind_info_resttime_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_blind_info_resttime"));
    
    tf_blind_info_blind_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_blind_info_blind"));
    tf_blind_info_ante_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_blind_info_ante"));
    tf_blind_info_lasttime_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_blind_info_lasttime"));
    tf_blind_info_resttime_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_blind_info_resttime"));
    
    editbox_blind_info_blind_ = GameTools::createEditBoxFromTextField(this, tf_blind_info_blind_);
    editbox_blind_info_blind_->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_blind_info_blind_->setMaxLength(editbox_blind_info_blind_->getContentSize().width);
    editbox_blind_info_blind_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    editbox_blind_info_blind_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    editbox_blind_info_blind_->setFontColor(Color3B(255, 255, 255));
    editbox_blind_info_blind_->setPlaceholderFontColor(Color3B(151, 151, 151));
    editbox_blind_info_blind_->setPlaceHolder("");
    editbox_blind_info_blind_->setDelegate(this);
    
    editbox_blind_info_ante_ = GameTools::createEditBoxFromTextField(this, tf_blind_info_ante_);
    editbox_blind_info_ante_->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_blind_info_ante_->setMaxLength(editbox_blind_info_ante_->getContentSize().width);
    editbox_blind_info_ante_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    editbox_blind_info_ante_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    editbox_blind_info_ante_->setFontColor(Color3B(255, 255, 255));
    editbox_blind_info_ante_->setPlaceholderFontColor(Color3B(151, 151, 151));
    editbox_blind_info_ante_->setPlaceHolder("");
    editbox_blind_info_ante_->setDelegate(this);
    
    editbox_blind_info_lasttime_ = GameTools::createEditBoxFromTextField(this, tf_blind_info_lasttime_);
    editbox_blind_info_lasttime_->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_blind_info_lasttime_->setMaxLength(editbox_blind_info_lasttime_->getContentSize().width);
    editbox_blind_info_lasttime_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    editbox_blind_info_lasttime_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    editbox_blind_info_lasttime_->setFontColor(Color3B(255, 255, 255));
    editbox_blind_info_lasttime_->setPlaceholderFontColor(Color3B(151, 151, 151));
    editbox_blind_info_lasttime_->setPlaceHolder("");
    editbox_blind_info_lasttime_->setDelegate(this);

    editbox_blind_info_resttime_ = GameTools::createEditBoxFromTextField(this, tf_blind_info_resttime_);
    editbox_blind_info_resttime_->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_blind_info_resttime_->setMaxLength(editbox_blind_info_ante_->getContentSize().width);
    editbox_blind_info_resttime_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    editbox_blind_info_resttime_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    editbox_blind_info_resttime_->setFontColor(Color3B(255, 255, 255));
    editbox_blind_info_resttime_->setPlaceholderFontColor(Color3B(151, 151, 151));
    editbox_blind_info_resttime_->setPlaceHolder("");
    editbox_blind_info_resttime_->setDelegate(this);
    
    text_blind_info_rank_->setString(tools::to_string(cell_idx_ + 1));

    this->setContentSize(cocos2d::Size(630, 80));
    
    set_editting(false);
    
    return true;
}

void NodeCreateTourBlindInfoCell::set_editting(bool isEditting)
{
    is_editting_ = isEditting;
    
    if (is_editting_)
    {
        image_property_input_bg_->setVisible(true);
        
        btn_add_blind_cell_->setVisible(false);
        
        text_blind_info_ante_->setVisible(false);
        text_blind_info_blind_->setVisible(false);
        text_blind_info_lasttime_->setVisible(false);
        text_blind_info_resttime_->setVisible(false);
        
        btn_delete_blind_cell_->setVisible(true);
        
        editbox_blind_info_ante_->setVisible(true);
        editbox_blind_info_blind_->setVisible(true);
        editbox_blind_info_lasttime_->setVisible(true);
        editbox_blind_info_resttime_->setVisible(true);
        
        editbox_blind_info_blind_->setText(tools::to_string(blind_level_.bigblind()).c_str());
        editbox_blind_info_ante_->setText(tools::to_string(blind_level_.ante()).c_str());
        editbox_blind_info_lasttime_->setText(tools::to_string(blind_level_.lasttime()).c_str());
        editbox_blind_info_resttime_->setText(tools::to_string(blind_level_.resttime()).c_str());
        
    }else
    {
        image_property_input_bg_->setVisible(false);

        btn_delete_blind_cell_->setVisible(false);
            
        btn_add_blind_cell_->setVisible(is_create_);
        
        text_blind_info_rank_->setVisible(true);
        text_blind_info_ante_->setVisible(true);
        text_blind_info_blind_->setVisible(true);
        text_blind_info_lasttime_->setVisible(true);
        text_blind_info_resttime_->setVisible(true);
        
        editbox_blind_info_ante_->setVisible(false);
        editbox_blind_info_blind_->setVisible(false);
        editbox_blind_info_lasttime_->setVisible(false);
        editbox_blind_info_resttime_->setVisible(false);
        
        text_blind_info_blind_->setString(tools::to_string(blind_level_.smallblind()) + "/" + tools::to_string(blind_level_.bigblind()));
        text_blind_info_ante_->setString(tools::to_string(blind_level_.ante()));
        text_blind_info_lasttime_->setString(tools::to_string(blind_level_.lasttime()));
        text_blind_info_resttime_->setString(tools::to_string(blind_level_.resttime()));
    }
    
    this->setTouchEnabled(true);
    
    this->addClickEventListener(std::bind(&NodeCreateTourBlindInfoCell::click_cell, this, std::placeholders::_1));
}

void NodeCreateTourBlindInfoCell::click_cell(Ref * sender)
{
    if (is_create_ && blind_cell_callback_)
    {
        blind_cell_callback_(cell_idx_);
    }
}

void NodeCreateTourBlindInfoCell::click_add_blind_cell(Ref * sender)
{
    if (is_create_ && blind_add_callback_)
    {
        blind_add_callback_(cell_idx_);
    }
}

void NodeCreateTourBlindInfoCell::click_delete_blind_cell(Ref * sender)
{
    if (is_create_ && blind_delete_callback_)
    {
        blind_delete_callback_(cell_idx_);
    }
}

#pragma mark EditBox delegate
void NodeCreateTourBlindInfoCell::editBoxEditingDidBegin(EditBox* editBox)
{
    
}

void NodeCreateTourBlindInfoCell::editBoxEditingDidEnd(EditBox* editBox)
{
    
}

void NodeCreateTourBlindInfoCell::editBoxTextChanged(EditBox* editBox, const std::string& text)
{

}

void NodeCreateTourBlindInfoCell::editBoxReturn(EditBox* editBox)
{
    editbox_blind_info_ante_->setVisible(true);
    editbox_blind_info_blind_->setVisible(true);
    editbox_blind_info_lasttime_->setVisible(true);
    editbox_blind_info_resttime_->setVisible(true);
    
    std::string strAnte = editbox_blind_info_ante_->getText();
    std::string strBlind = editbox_blind_info_blind_->getText();
    std::string strLasttime = editbox_blind_info_lasttime_->getText();
    std::string strResttime = editbox_blind_info_resttime_->getText();
    
    int intAnte;
    int intSmallBlind;
    int intBigBlind;
    int intLasttime;
    int intResttime;
    
    if (strAnte.empty())
    {
        intAnte = 0;
    }else
    {
        intAnte = tools::stod(strAnte);
    }
    if (intAnte < 0)
    {
        intAnte = 0;
    }
    if (strBlind.empty())
    {
        strBlind = "2";
    }else
    {
        intBigBlind = tools::stod(strBlind);
    }
    if (intBigBlind < 2)
    {
        intBigBlind = 2;
    }
    if (strLasttime.empty())
    {
        strLasttime = "0";
    }else
    {
        intLasttime = tools::stod(strLasttime);
    }
    if (intLasttime < 0)
    {
        intLasttime = 0;
    }
    if (strResttime.empty())
    {
        strResttime = "0";
    }else
    {
        intResttime = tools::stod(strResttime);
    }
    if (intResttime < 0)
    {
        intResttime = 0;
    }
    
    if (intBigBlind % 2 != 0)
    {
        intSmallBlind = intBigBlind / 2 + 1;
    }else
    {
        intSmallBlind = intBigBlind / 2;
    }
    
    editbox_blind_info_blind_->setText(tools::to_string(intBigBlind).c_str());
    editbox_blind_info_ante_->setText(tools::to_string(intAnte).c_str());
    editbox_blind_info_lasttime_->setText(tools::to_string(intLasttime).c_str());
    editbox_blind_info_resttime_->setText(tools::to_string(intResttime).c_str());
    
    blind_level_.set_ante(intAnte);
    blind_level_.set_smallblind(intSmallBlind);
    blind_level_.set_bigblind(intBigBlind);
    blind_level_.set_lasttime(intLasttime);
    blind_level_.set_resttime(intResttime);
    
    if (change_blind_callback_)
    {
        change_blind_callback_(cell_idx_, blind_level_);
    }
}