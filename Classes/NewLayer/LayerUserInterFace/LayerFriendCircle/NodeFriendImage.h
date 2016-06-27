//
//  NodeFriendImage.hpp
//  pokerclient
//
//  Created by zhongyu on 16/5/30.
//
//

#ifndef NodeFriendImage_h
#define NodeFriendImage_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
using namespace cocos2d;
using namespace cocos2d::ui;
class NodeFriendImage:public Widget
{
public:
    
    virtual bool init();
    void setImageShow(std::string imageroot);
    static NodeFriendImage* create();
    Node* root_;
    ImageView* image_player;
    
};

#endif /* NodeFriendImage_hpp */
