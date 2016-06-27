//
//  NodeGameInPause.cpp
//  pokerclient
//
//  Created by zhongyu on 16/3/7.
//
//

#include "NodeGameInPause.h"
#include "utils.h"

bool NodeGameInPause::init()
{
    if (!Node::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeGameInPause.csb");
    this->addChild(root);
    
    text_in_pause_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_game_in_pause_1"));
    
    text_in_pause_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_game_in_pause_2"));
    
    text_in_pause_1_->setString(tools::local_string("game_pause","牌局暂停中"));
    text_in_pause_2_->setString(tools::local_string("please_wait","请等待..."));
    
    auto image_game_in_pause_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_game_in_pause_bg"));
    
    this->setContentSize(image_game_in_pause_bg->getContentSize());

    return true;
}