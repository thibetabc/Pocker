//
//  NodeUserMatchInfoCell.cpp
//  pokerclient
//
//  Created by ll on 15/11/27.
//
//

#include "NodeUserMatchInfoCell.h"
#include "Room.h"
#include "GameDataManager.h"
#include "PokerDataManager.h"
#include "FileManager.h"
#include "cmd_def.h"
#include "user_data.h"

bool NodeUserMatchInfoCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeUserMatchInfoCell.csb");
    this->addChild(root_);
    
    image_cell_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_cell_bg"));
    
    image_score_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_score_bg"));
    
    image_player_icon_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon_bg"));
    image_player_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon"));
    
    image_date_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_date_icon"));
    
    text_game_date_day_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_game_date"));
    text_game_date_time_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_game_date_time"));
    
    text_room_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_name"));
    text_score_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score"));
    
    sprite_icon_ = nullptr;
    
    image_player_icon_bg_->setVisible(false);
    image_player_icon_->setVisible(false);
    
    return true;
}

void NodeUserMatchInfoCell::update_data(const std::shared_ptr<msg::HistoryScore> & match_cell_data)
{
    match_cell_data_ = match_cell_data;
    auto match_data = match_cell_data_.lock();
    if (match_data == nullptr)
    {
        cocos2d::log("Error : NodeUserMatchInfoCell match_cell_data == nullptr!");
        return;
    }
    
    std::string room_name = match_data->roomname();
    int32_t match_score = match_data->score();
    
    text_room_name_->setString(room_name);
    if (match_score > 0)
    {
        image_score_bg_->loadTexture("login_res/matchinfo_res/image_playerchips_win_icon.png");
        text_score_->setString("+" + tools::to_string(match_score));
    }else
    {
        image_score_bg_->loadTexture("login_res/matchinfo_res/image_playerchips_lose_icon.png");
        text_score_->setString(tools::to_string(match_score));
    }
    std::string submit_time = match_data->endtime();
    if(!submit_time.empty()) {
        double time = tools::stod(submit_time);
        time /= 1000;
        std::string text_date = tools::time_display(time, tools::TIMEDISPLAYTYPE::ONLYDAY);
        std::string text_time = tools::time_display(time, tools::TIMEDISPLAYTYPE::ONLYTIME);
        
        text_game_date_day_->setString(text_date);
        text_game_date_time_->setString(text_time);
    }
    
    for (auto btn_iter : btn_labels_)
    {
        btn_iter->removeFromParent();
    }
    btn_labels_.clear();
    
    //290 90  290 30 400 90
    for (int i = 0; i < match_data->label().size(); i++)
    {
        if (i > 5)
            break;
        
        std::string text_label = match_data->label(i);
        Button* btn_label = Button::create();
        
        root_->addChild(btn_label);
        btn_label->setContentSize(cocos2d::Size(90, 45));
        btn_label->setAnchorPoint(Vec2(0, 0.5));
        
        btn_label->loadTextureNormal("login_res/matchinfo_res/btn_label_normal.png");
        btn_label->loadTexturePressed("login_res/matchinfo_res/btn_label_pressed.png");
        btn_label->loadTextureDisabled("login_res/matchinfo_res/btn_label_pressed.png");
        
        if (i < 2)
            btn_label->setPosition(Point(290 + (i % 3)*110, 90));
        else
            btn_label->setPosition(Point(290 + (i % 3)*110, 30));
        
        std::string s = text_label;
        std::string t;
        uint32_t str_length = 0;
        for (int k = 0; k < s.length(); k++)
        {
            
            if (s[k] < 255 && s[k] > 0)
            {
                str_length += 1;
                if (str_length > 7)
                {
                    t.append("...");
                    break;
                }else
                    t.append(s.substr(k, 1));
            }else
            {
                str_length += 2;
                if (str_length > 7)
                {
                    t.append("...");
                    break;
                }else
                    t.append(s.substr(k, 3));
                ++k;
                ++k;
                
            }
        }
        //t += '\0';
        
        btn_label->setTitleText(t);
        btn_label->setTitleFontSize(22);
        btn_label->setTitleColor(Color3B(255, 255, 255));
        btn_label->setLocalZOrder(100);
        btn_label->addClickEventListener(std::function<void(Ref*)>([=](Ref* sender){
            if (click_label_)
                click_label_(text_label);
        }));
        btn_labels_.push_back(btn_label);
    }
    //update_ui();
}

void NodeUserMatchInfoCell::set_date_visiable(bool isVisiable)
{
    text_game_date_day_->setVisible(isVisiable);
    image_date_icon_->setVisible(isVisiable);
}

void NodeUserMatchInfoCell::update_ui()
{
    if(GDM->is_file_exist(userinfo.picname())) {
        image_player_icon_->loadTexture(userinfo.picname());
        image_player_icon_->setVisible(false);
        if (sprite_icon_ && sprite_icon_->getParent()) {
            sprite_icon_->removeFromParent();
        }
        sprite_icon_ = ShaderSprite::create(userinfo.picname(), "login_res/friend_res/node_friend_icon_mask.png");
        sprite_icon_->setPosition(image_player_icon_->getPosition());
        sprite_icon_->setScale(image_player_icon_->getContentSize().width / sprite_icon_->getContentSize().width,
                               image_player_icon_->getContentSize().height / sprite_icon_->getContentSize().height);
        root_->addChild(sprite_icon_, image_player_icon_->getZOrder());
    }
    else {
        FM.download_file(userinfo.picname());
        register_download_events();
    }
    
}

void NodeUserMatchInfoCell::register_download_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, NodeUserMatchInfoCell::handle_download_events));
}
void NodeUserMatchInfoCell::handle_download_events(cmd_data_pointer)
{
    update_ui();
}

void NodeUserMatchInfoCell::set_is_touch(bool is_touch)
{
    std::string image_bg_name;
    if (is_touch)
    {
        image_bg_name = "login_res/matchinfo_res/image_matchinfo_cell_bg_select.png";
        
    }else {
        image_bg_name = "login_res/matchinfo_res/image_matchinfo_cell_bg.png";
    }
    image_cell_bg_->loadTexture(image_bg_name);
}

void NodeUserMatchInfoCell::set_is_group_match(bool is_group_match)
{
    image_score_bg_->setVisible(!is_group_match);
    text_score_->setVisible(!is_group_match);
}