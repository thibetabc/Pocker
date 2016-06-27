//
//  NodeScoreListCell.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/11.
//
//

#include "NodeScoreListCell.h"
#include "utils.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "cmd_def.h"
#include "GameTools.h"

#define PLAYER_SCORE_PHOTO_WIDTH 100
#define PLAYER_SCORE_PHOTO_HEIGHT 100

NodeScoreListCell* NodeScoreListCell::create(bool is_player_rank , bool is_show_buy_num)
{
    NodeScoreListCell* cell = new NodeScoreListCell;
    if (cell->init(is_player_rank , is_show_buy_num))
    {
        cell->autorelease();
        return cell;
    }
    return nullptr;
}


bool NodeScoreListCell::init(bool is_player_rank , bool is_show_buy_num)
{
    if (!TableViewCell::init())
    {
        return false;
    }
    
    is_player_rank_ = is_player_rank;
    is_show_buy_num_ = is_show_buy_num;
    
    root_=CSLoader::createNode("NodeScoreListCell.csb");
    this->addChild(root_);
    sprite_icon_ = nullptr;
    
    text_earn_diamond_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_earn_diamond"));
    text_hit_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_hit_num"));
    text_room_member_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_member_name"));
    text_buy_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_buy_num"));
    text_gift_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_gift"));
    
    node_room_member_photo_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_room_member_photo"));
    
    image_gold_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_gold"));
    image_silver_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_silver"));
    
    text_gift_->setString("");
    text_buy_num_->setString("");
    
    text_room_member_name_->setString("");
    text_hit_num_->setString("");
    text_room_member_name_->setString("");
    
    if (!is_show_buy_num_)
    {
        text_buy_num_->setVisible(false);
    }
    
    if (is_player_rank_)
    {
        text_hit_num_->setVisible(false);
        text_buy_num_->setVisible(false);
    }
    
    
    
    image_gold_->setVisible(false);
    image_silver_->setVisible(false);
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, NodeScoreListCell::handle_download_event));

    return true;
}

void NodeScoreListCell::update_data(const msg::PlayerScore &list_cell_data)
{
    list_cell_data_=list_cell_data;
    text_earn_diamond_->setString(tools::to_string(list_cell_data_.totalwin()));
    text_hit_num_->setString(tools::local_string("Health_point", "体力：")+tools::to_string(list_cell_data_.hplost()));
    text_room_member_name_->setString(list_cell_data_.userdetailinfo().nickname());
    text_gift_->setString(list_cell_data_.otherprize());
    text_buy_num_->setString(tools::local_string("Buy", "买入：")+tools::to_string(list_cell_data_.totalbuyin()));
    image_silver_->setVisible(false);
    image_gold_->setVisible(false);
    std::string picname=list_cell_data_.userdetailinfo().picname();
    picname_=picname;
    this->addPlayPhoto(picname);
}
void NodeScoreListCell::handle_download_event(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (s->get_value() != picname_) return;
    assert (GDM->is_file_exist(picname_));
    this->addPlayPhoto(picname_);
    
}
void NodeScoreListCell::addPlayPhoto(std::string picname)
{
    
    if (!GDM->is_file_exist(picname))
    {
        FM.download_file(picname);
    }
    else
    {
        if(picname == "" || !GameTools::isValidPicture(picname))
        {
            picname = "Default/image_defualt_user_icon_large.png";
        }
        if (sprite_icon_ && sprite_icon_->getParent()) {
            sprite_icon_->removeFromParent();
            sprite_icon_ = nullptr;
        }
        sprite_icon_ = ShaderSprite::create(picname, "Default/head_mask_circle_second.png");
        node_room_member_photo_->addChild(sprite_icon_);
        auto width=sprite_icon_->getContentSize().width;
        auto height=sprite_icon_->getContentSize().height;
        float scaleX=1;
        float scaleY=1;
        if (width>PLAYER_SCORE_PHOTO_WIDTH)
        {
            scaleX=PLAYER_SCORE_PHOTO_WIDTH/width;
        }
        
        if (height>PLAYER_SCORE_PHOTO_HEIGHT)
        {
            scaleY=PLAYER_SCORE_PHOTO_HEIGHT/height;
        }
        sprite_icon_->setScale(scaleX,scaleY);
    }
}
void NodeScoreListCell::set_gold_visible()
{
    image_gold_->setVisible(true);
}
void NodeScoreListCell::set_silver_visible()
{
    image_silver_->setVisible(true);
}