//
//  NodeFriendImage.cpp
//  pokerclient
//
//  Created by zhongyu on 16/5/30.
//
//

#include "NodeFriendImage.h"

NodeFriendImage* NodeFriendImage::create()
{
    NodeFriendImage* result = new NodeFriendImage;
    if(result->init()){
        return result;
    }
    return nullptr;
   
}

bool NodeFriendImage::init()
{
    if(!Widget::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeFriendImage.csb");
    image_player=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_Friend"));

    this->addChild(root_);
    return true;
}



void NodeFriendImage::setImageShow(std::string imageroot)
{
    image_player->setAnchorPoint(Vec2(0,1));
    image_player->loadTexture(imageroot);
}
