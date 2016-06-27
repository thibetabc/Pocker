//
//  NodeSelectGameType.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/21.
//
//
#include "utils.h"
#include "NodeSelectGameType.h"
#include "LayerChooseGameType.h"
bool NodeSelectGameType::init()
{
    if (!ImageView::init())
    {
        return false;
    }
    root_=CSLoader::createNode("NodeSelectGameType.csb");
    this->addChild(root_);
    text_gametype_name_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_gametype_name"));
    btn_choose_gametype_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_choose_gametype"));
    image_gametype_icon_=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_gametype_icon"));
    btn_choose_gametype_->setTouchEnabled(false);
    image_select_type_game_bg_=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_select_type_game_bg"));
    image_select_type_game_bg_->setTouchEnabled(true);
    image_select_type_game_bg_->setEnabled(true);
    image_select_type_game_bg_->addClickEventListener(std::bind(&NodeSelectGameType::click_image_bg, this,std::placeholders::_1));
    return true;
}
void NodeSelectGameType::click_image_bg(cocos2d::Ref *sender)
{
    if(callback_)
    {
        callback_();
    }
}

void NodeSelectGameType::refresh_data(msg::RoomType type)
{
    switch (type) {
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::NONE :
        {
            
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::NLHOLDEM :
        {
            text_gametype_name_->setString(tools::local_string("holdempoker", "德州扑克"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::OFC3 :
        {
            text_gametype_name_->setString(tools::local_string("pineapplepoker", "大菠萝"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_ofc3.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::PLO :
        {
            text_gametype_name_->setString(tools::local_string("omaha", "底池限注奥马哈"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::OFC4 :
        {
            text_gametype_name_->setString(tools::local_string("openfacechinesepoker", "明牌十三张"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_ofc3.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::OMAHA :
        {
            text_gametype_name_->setString(tools::local_string("omaha", "底池限注奥马哈"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::PLOHILO :
        {
            text_gametype_name_->setString(tools::local_string("omaha_hl", "底池限注奥马哈 高/低"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::SNG :
        {
            text_gametype_name_->setString(tools::local_string("Tournament","锦标赛"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::SNG_NLHOLDEM :
        {
            text_gametype_name_->setString(tools::local_string("holdempoker", "德州扑克"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::SNG_PLO :
        {
            text_gametype_name_->setString(tools::local_string("omaha", "底池限注奥马哈"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::SNG_PLOHILO :
        {
            text_gametype_name_->setString(tools::local_string("omaha_hl", "底池限注奥马哈 高/低"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case msg::RoomType::SIChUANMahjong :
        {
            text_gametype_name_->setString(tools::local_string("sichuan_mahjong", "四川麻将"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        default :
        {
            break;
        }
    }
}