//
//  NodeComInfoPlayerCell.cpp
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#include "NodeComInfoPlayerCell.h"

bool NodeComInfoPlayerCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeComInfoPlayerCellPortrait.csb");
    this->addChild(root_);
    
    text_table_id_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_table_id"));
        image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_bg"));
    
    text_table_id_->setString("");
    
    return true;
}

void NodeComInfoPlayerCell::update_data(const std::shared_ptr<msg::TourmentTableInfo> & player_cell_data)
{
    player_cell_data_ = player_cell_data;
    auto data = player_cell_data_.lock();
    
    if (data == nullptr)
    {
        cocos2d::log("Error : NodeComInfoPlayerCell data == nullptr");
        return;
    }
    
    int32_t table_id = data->tableid();
    
    text_table_id_->setString(tools::to_string(table_id));
}

void NodeComInfoPlayerCell::set_is_select(bool is_touch)
{
    std::string image_bg_name;
    if (is_touch)
    {
        image_bg_name = "image/tournament_info_portrait_res/image_cominfo_cell_bg_select.png";
        
    }else {
        image_bg_name = "image/tournament_info_portrait_res/image_cominfo_cell_bg.png";
    }
    image_bg_->loadTexture(image_bg_name);
}