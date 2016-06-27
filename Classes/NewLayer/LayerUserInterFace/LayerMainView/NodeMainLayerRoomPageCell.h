//
//  NodeMainLayerRoomPageCell.h
//  pokerclient
//
//  Created by lvzhangle on 16/3/27.
//
//
#ifndef __pokerclient__NodeMainLayerRoomPageCell__
#define __pokerclient__NodeMainLayerRoomPageCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class NodeMainLayerRoomPageCell : public Widget {
public:
    CREATE_FUNC(NodeMainLayerRoomPageCell);
    
    virtual bool init();
    
    void update_data(std::pair<std::string , std::string> room_page_cell_data);
    
    std::pair<std::string , std::string> room_page_cell_data_;
    
private:
    Text* text_1_;
    Text* text_2_;
    Node* root_;
};

#endif /* defined(__pokerclient__NodeMainLayerRoomPageCell__) */
