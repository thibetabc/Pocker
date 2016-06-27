//
//  NodeScoreCell.cpp
//  pokerclient
//
//  Created by ll on 15/10/27.
//
//

#include "NodeScoreCell.h"

#include "GameDataManager.h"
#include "utils.h"
#include "ShaderSprite.h"
#include "FileManager.h"
#include "cmd_def.h"

bool NodeScoreCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeScoreCell.csb");
    this->addChild(root_);
    
    text_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_user_name"));
    text_buy_chip_amount_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_user_buy_info"));
    text_win_chip_amount_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_income"));
    image_avator_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_user_avatar"));
    image_rank_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_rank"));
    
    image_avator_frame_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_user_avatar_frame"));
    score_cell_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "score_cell_bg"));
    text_rank_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_rank_num"));
    income_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_income_line"));
    
    text_user_other_prize_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_user_other_prize"));
    text_user_other_prize_->setVisible(false);
    
    text_user_hp_lost_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_user_hp_lost"));
    
    auto image_king_icon = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_king"));
    image_king_icon->setVisible(false);
    
    return true;
}

void NodeScoreCell::update_data(const std::shared_ptr<MATCH_OBJECT>& obj)
{
    obj_ = obj;
    
    auto data = obj_.lock();
    std::string imageNumName="login_res/score_res/image_num_forth.png";
    
   
    if(data->rank == 1){
        imageNumName="login_res/score_res/image_num_first.png";
    }else if(data->rank == 2){
        imageNumName="login_res/score_res/image_num_second.png";
    }else if(data->rank == 3){
        imageNumName="login_res/score_res/image_num_third.png";
    }
    
    image_rank_->loadTexture(imageNumName);
    text_nickname_->setString(data->nickname);
    if(data->win_amount<0){
        text_win_chip_amount_->setColor(Color3B(153, 153, 153));
        income_line_->loadTexture("login_res/score_res/image_lose_normal_line.png");
    }
    text_win_chip_amount_->setString(tools::to_string(data->win_amount));
    text_rank_->setString(tools::to_string(data->rank));
    
    if (data->is_group_user_rank)
    {
        text_user_hp_lost_->setVisible(false);
        text_user_other_prize_->setVisible(false);
        text_buy_chip_amount_->setVisible(false);
        
        text_nickname_->setPositionY(57.0);
    }else
    {
        text_nickname_->setPositionY(90.0);
        text_user_hp_lost_->setVisible(true);
        if(data->hp_lost > 0)
        {
            text_user_hp_lost_->setString(std::string("体力：") + std::string("+") + tools::to_string(data->hp_lost));
        }else
        {
            text_user_hp_lost_->setString(std::string("体力：") + tools::to_string(data->hp_lost));
        }
        if (data->is_tournament)
        {
            if (data->otherprize != "")
            {
                text_user_other_prize_->setVisible(true);
                text_user_other_prize_->setString(tools::local_string("extra_award", "额外奖励：") + data->otherprize);
            }else
            {
                text_user_other_prize_->setVisible(false);
            }
        }else
        {
            text_user_other_prize_->setVisible(false);
        }
        if (data->room_type == msg::RoomType::OFC3 ||
            data->room_type == msg::RoomType::OFC4)
        {
            text_buy_chip_amount_->setVisible(false);
        }
        else
        {
            text_buy_chip_amount_->setVisible(true);
            text_buy_chip_amount_->setString(tools::local_string("buy_chip", "买入")+std::string("：") + tools::to_string(data->buy_amount));
        }
    }


    //image_avator_->loadTexture(match_objects_[idx]->head_pic_name);
    if(GDM->get_user_id()==data->user_id){
        text_win_chip_amount_->setFontSize(48);
        text_rank_->setFontSize(28);
        //text_win_chip_amount_->setColor(Color3B(255,165,0));
        income_line_->loadTexture("login_res/score_res/image_win_select_line.png");
        if(data->win_amount<0){
            income_line_->loadTexture("login_res/score_res/image_lose_select_line.png");
        }
       
        image_avator_->setPositionX(image_avator_->getPosition().x+20);
        image_avator_->setScale(0.9);
        image_avator_frame_->setPositionX(image_avator_frame_->getPosition().x+20);
        image_avator_frame_->setScale(0.9);
        
        text_buy_chip_amount_->setPositionX(text_buy_chip_amount_->getPosition().x+30);
        text_buy_chip_amount_->setFontSize(24);
        text_buy_chip_amount_->setColor(Color3B(255,165,0));
        
        text_nickname_->setPositionX(text_nickname_->getPosition().x+30);
        text_nickname_->setFontSize(24);
        text_nickname_->setColor(Color3B(255,165,0));
        
        text_user_hp_lost_->setPositionX(text_user_hp_lost_->getPosition().x+30);
        text_user_hp_lost_->setFontSize(24);
        text_user_hp_lost_->setColor(Color3B(255, 165, 0));
        
        text_user_other_prize_->setPositionX(text_user_other_prize_->getPosition().x+30);
        text_user_other_prize_->setFontSize(24);
        text_user_other_prize_->setColor(Color3B(255,165,0));
        
        image_rank_->setScale(1.2);
        score_cell_bg_->loadTexture("login_res/score_res/image_record_select_bg.png");
        score_cell_bg_->setOpacity(255);
    }
    update_ui();
}

void NodeScoreCell::update_ui()
{
    auto data = obj_.lock();
    
    if(data != nullptr && !data->head_pic_name.empty())
    {
        if(GDM->is_file_exist(data->head_pic_name)) {
            image_avator_->setOpacity(0);
            //image_avator_->loadTexture("login_res/score_res/image_avator.png");
            
            if (sprite_icon_ && sprite_icon_->getParent()) {
                sprite_icon_->removeFromParent();
            }
            sprite_icon_ = ShaderSprite::create(data->head_pic_name, "login_res/friend_res/node_friend_icon_mask.png");
            sprite_icon_->setScale(image_avator_->getContentSize().width * image_avator_->getScaleX() / sprite_icon_->getContentSize().width,
                                   image_avator_->getContentSize().height * image_avator_->getScaleY() / sprite_icon_->getContentSize().height);
            sprite_icon_->setPosition(image_avator_->getPosition());
            root_->addChild(sprite_icon_, image_avator_->getZOrder());
            
        }
        else {
            register_download_events();
            
            FM.download_file(data->head_pic_name);
        }
    }
}

void NodeScoreCell::register_download_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, NodeScoreCell::handle_download_events));
}
void NodeScoreCell::handle_download_events(cmd_data_pointer)
{
    update_ui();
}
