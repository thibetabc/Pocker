//
//  NodeChooseGameTypeCell.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/27.
//
//
#include "utils.h"
#include "NodeChooseGameTypeCell.h"

bool NodeChooseGameTypeCell::init()
{
    if (!TableViewCell::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeChooseGameTypeCell.csb");
    
        this->addChild(root_);
    
    image_gametype_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_gametype_bg"));
    image_gametype_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_gametype_icon"));
    text_gametype_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_gametype_name"));

    
    return true;
}

void NodeChooseGameTypeCell::update_data(LayerChooseGameType::CHOOSE_ROOM_TYPE game_type)
{
    cell_game_type_ = game_type;
    
    switch (cell_game_type_){
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
            text_gametype_name_->setString(tools::local_string("omaha_hl", "奥马哈 高/低"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::SNG :
        {
            text_gametype_name_->setString(tools::local_string("Tournament", "锦标赛"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::MTT_NLHOLDEM :
        {
            text_gametype_name_->setString(tools::local_string("mtt_holdempoker", "锦标赛 德州扑克"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::MTT_PLO :
        {
            text_gametype_name_->setString(tools::local_string("mtt_omaha", "锦标赛 奥马哈"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::MTT_PLOHILO :
        {
            text_gametype_name_->setString(tools::local_string("mtt_omaha_hl", "锦标赛 奥马哈 高/低"));
            image_gametype_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
            break;
        }
        case LayerChooseGameType::CHOOSE_ROOM_TYPE::SIChUANMAHJONG :
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