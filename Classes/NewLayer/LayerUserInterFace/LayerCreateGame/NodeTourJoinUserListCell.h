//
//  NodeTourJoinUserListCell.h
//  pokerclient
//
//  Created by lvzhangle on 16/5/26.
//
//
#ifndef __pokerclient__NodeTourJoinUserListCell__
#define __pokerclient__NodeTourJoinUserListCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class NodeTourJoinUserListCell : public Widget{
public:
    
    static NodeTourJoinUserListCell* create(msg::UserDetailInfo userInfo);
    
    virtual bool init();
    
    void update_data(msg::UserDetailInfo userInfo);
    
private:
    msg::UserDetailInfo user_info_;
#pragma mark UI
    
    Text* text_join_user_nickname_;
    
    Node* root_;
};

#endif /* defined(__pokerclient__NodeTourJoinUserListCell__) */