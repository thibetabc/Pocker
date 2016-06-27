//
//  NodeMahjongFinalResultCell.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#include "NodeMahjongFinalResultCell.h"
#include "NodeMahjongResultScoreCell.h"
#include "utils.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "cmd_def.h"

NodeMahjongFinalResultCell* NodeMahjongFinalResultCell::create(bool is_self)
{
    NodeMahjongFinalResultCell* result_cell = new NodeMahjongFinalResultCell;
    if ( result_cell->init(is_self) )
    {
        result_cell->autorelease();
        return result_cell;
    }
    return nullptr;
}

bool NodeMahjongFinalResultCell::init(bool is_self)
{
    if ( !TableViewCell::init() )
    {
        return false;
    }
    is_self_ = is_self;
    
    root_ = CSLoader::createNode("NodeMahjongFinalResultCell.csb");
    this->addChild(root_);
    
    image_gold_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_gold"));
    image_cell_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_cell_bg"));
    image_score_title_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_score_title"));
    
    text_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_name"));
    
    node_player_photo_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_player_photo"));
    
    atlas_label_score_ = dynamic_cast<TextAtlas*>(CSLoader::seekNodeByName(root_, "atlas_label_score"));
    
    if ( is_self_ )
    {
        image_cell_bg_->loadTexture("image/Mahjong_game_res/mahjong_on_the_water_res/image_mahjong_result_cell_bg_own.png");
    }
    
    sprite_icon_ = nullptr;
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, NodeMahjongFinalResultCell::handle_download_event));
    
    list_veiw_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "list_veiw"));
    tableview_ = TableView::create(this, list_veiw_->getContentSize());
    tableview_->setAnchorPoint(list_veiw_->getAnchorPoint());
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(list_veiw_->getPosition());
    tableview_->setDelegate(this);
    root_->addChild(tableview_);
    list_veiw_->removeFromParent();
    
    
    return true;
}

void NodeMahjongFinalResultCell::set_data(msg::MahjongPlayerInfo &player_info)
{
    if ( player_info.has_playerinfo() )
    {
        player_info_.CopyFrom(player_info);
        int score = 0;
        for ( int i = 0;i< player_info_.scorechange_size();i++ )
        {
            score = score + player_info_.scorechange(i).score();
        }
        if ( score > 0 )
        {
            atlas_label_score_->setProperty(tools::to_string(score), "image/Mahjong_game_res/mahjong_on_the_water_res/awarded_digit.png", 29, 39, ".");
            image_score_title_->loadTexture("image/Mahjong_game_res/mahjong_on_the_water_res/image_mahjong_jia_other.png");
            
        }
        else
        {
            atlas_label_score_->setProperty(tools::to_string(score), "image/Mahjong_game_res/mahjong_on_the_water_res/deduction_digit.png",  29, 39, ".");
            image_score_title_->loadTexture("image/Mahjong_game_res/mahjong_on_the_water_res/image_reduce.png");
        }
        text_name_->setString(player_info_.playerinfo().userinfo().nickname());
        addPlayPhoto(player_info_.playerinfo().userinfo().picname());
        tableview_->reloadData();
    }
    
}

void NodeMahjongFinalResultCell::handle_download_event(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if ( player_info_.has_playerinfo() )
    {
        if (s->get_value() != player_info_.playerinfo().userinfo().picname()) return;
        assert (GDM->is_file_exist(player_info_.playerinfo().userinfo().picname()));
        this->addPlayPhoto(player_info_.playerinfo().userinfo().picname());
    }
    
}
void NodeMahjongFinalResultCell::addPlayPhoto(std::string picname)
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
        node_player_photo_->addChild(sprite_icon_);
        node_player_photo_->setScale(100/sprite_icon_->getContentSize().width, 100/sprite_icon_->getContentSize().height);
    }
}

TableViewCell* NodeMahjongFinalResultCell::tableCellAtIndex(TableView *table, ssize_t idx)
{
    NodeMahjongResultScoreCell* cell = dynamic_cast<NodeMahjongResultScoreCell*>(table->dequeueCell());
    if(!cell) {
        cell = NodeMahjongResultScoreCell::create();
    }
    cell->set_data(player_info_.scorechange(idx));
    
    return cell;
}
ssize_t NodeMahjongFinalResultCell::numberOfCellsInTableView(TableView *table)
{
    if ( player_info_.has_playerinfo() )
    {
        return player_info_.scorechange_size();
    }
    return 0;
}