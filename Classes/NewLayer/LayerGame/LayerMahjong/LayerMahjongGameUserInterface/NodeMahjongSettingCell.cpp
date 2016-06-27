//
//  NodeMahjongSettingCell.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#include "NodeMahjongSettingCell.h"

bool NodeMahjongSettingCell::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeMahjongSetting.csb");
    this->addChild(root_);
    
    text_setting_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_setting_name"));
    text_setting_name_->setString("");
    
    btn_control_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_control"));
    btn_control_->addClickEventListener(std::bind(&NodeMahjongSettingCell::click_btn_control, this , std::placeholders::_1));
    
    return true;
}

void NodeMahjongSettingCell::refresh_node_data(std::string title, bool is_on)
{
    title_ = title;
    text_setting_name_->setString(title);
    is_on_ = is_on;
    if (is_on_)
    {
        btn_control_->loadTextures("image/Mahjong_game_res/mahjong_setting_res/btn_mahjong_on.png", "image/Mahjong_game_res/mahjong_setting_res/btn_mahjong_off.png");
    }
    else
    {
        btn_control_->loadTextures("image/Mahjong_game_res/mahjong_setting_res/btn_mahjong_off.png", "image/Mahjong_game_res/mahjong_setting_res/btn_mahjong_on.png");
    }
}