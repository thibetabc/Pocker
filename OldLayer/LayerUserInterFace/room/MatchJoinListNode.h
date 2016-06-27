//
//  MatchJoinListNode.hpp
//  pokerclient
//
//  Created by qqronaldz on 15/12/2.
//
//

#ifndef MatchJoinListNode_hpp
#define MatchJoinListNode_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "msg.pb.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
using namespace cocos2d;
using namespace extension;
using namespace cocos2d::ui;

class MatchJoinListNode : public TableViewCell
{
public:
    CREATE_FUNC(MatchJoinListNode);
    virtual bool init();
    void update_data(msg::UserDetailInfo data,int idx);
private:
    Node *root;
    Text *num;
    Text *name;
};

#endif /* MatchJoinListNode_hpp */
