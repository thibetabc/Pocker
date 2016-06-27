//
//  LayerMahjongOnTheWater.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#include "LayerMahjongOnTheWater.h"
#include "utils.h"
#include "NodeMahjongOnTheWaterCell.h"
bool LayerMahjongOnTheWater::init()
{
    if ( !BaseLayer::init() )
    {
        return false;
    }
    auto layer = LayerColor::create(Color4B(143, 143, 143, 100));
    this->addChild(layer);
    
    root_ = CSLoader::createNode("LayerMahjongOnTheWater.csb");
    this->addChild(root_);
    
    btn_close_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_close"));
    btn_close_->addClickEventListener(std::bind(&LayerMahjongOnTheWater::click_btn_close, this,std::placeholders::_1));
    
    image_title_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_title"));
    
    text_des_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_des"));
    text_des_->setString(tools::local_string("mahjong_water_desc","本局当前输赢："));
    
    text_score_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score"));
    text_score_->setString("0");
    
    list_mahjong_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "list_mahjong"));
    tableview_ = TableView::create(this, list_mahjong_->getContentSize());
    tableview_->setAnchorPoint(list_mahjong_->getAnchorPoint());
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(list_mahjong_->getPosition());
    tableview_->setDelegate(this);
    root_->addChild(tableview_);
    list_mahjong_->removeFromParent();
    return true;
}

TableViewCell* LayerMahjongOnTheWater::tableCellAtIndex(TableView *table, ssize_t idx)
{
    NodeMahjongOnTheWaterCell* cell = dynamic_cast<NodeMahjongOnTheWaterCell*>(table->dequeueCell());
    if(!cell) {
        cell = NodeMahjongOnTheWaterCell::create();
    }
    cell->set_data(player_info_.scorechange(idx));
    return cell;
}
ssize_t LayerMahjongOnTheWater::numberOfCellsInTableView(TableView *table)
{
    if ( player_info_.has_playerinfo() )
    {
        return player_info_.scorechange_size();
    }
    return 0;
}

void LayerMahjongOnTheWater::set_player_info(msg::MahjongPlayerInfo &player_info)
{
    if ( player_info.has_playerinfo() )
    {
        player_info_.CopyFrom(player_info);
        
        int score = 0;
        for ( int i = 0;i< player_info_.scorechange_size();i++ )
        {
            score = score + player_info_.scorechange(i).score();
        }
        
        text_score_->setString(tools::to_string(score));
        tableview_->reloadData();
    }
}



