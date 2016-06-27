//
//  LayerMahjongSetting.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#include "LayerMahjongSetting.h"
bool LayerMahjongSetting::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    root_ = CSLoader::createNode("LayerMahjongSetting.csb");
    this->addChild(root_);
    
    node_first_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_first"));
    node_second_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_second"));
    node_third_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_third"));
    node_fourth_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_fourth"));
    node_fifth_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_fifth"));
    node_sixth_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_sixth"));
    
//    cell_first_ = NodeMahjongSettingCell::create();
//    node_first_->addChild(cell_first_);
//    cell_first_->refresh_node_data(<#std::string title#>, <#bool is_on#>)
    
    return true;
}