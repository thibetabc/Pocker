//
//  MatchRewardNode.hpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/30.
//
//

#ifndef MatchRewardNode_h
#define MatchRewardNode_h

#include <stdio.h>
#include "cocos2d.h"
#include "msg.pb.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
using namespace cocos2d;
using namespace extension;
using namespace cocos2d::ui;
class MatchRewardNode :public TableViewCell
{
public:
    CREATE_FUNC(MatchRewardNode);
    virtual  bool init();
    void update_data(msg::PrizeLevel *data,int idx,bool isCreate);
    void set_is_touch(bool is_touch);
    void click_add();
    void click_edit();
    msg::PrizeLevel* get_data(){return m_pData;}
private:
    Node *root;
    int idx;
    msg::PrizeLevel *m_pData;
    bool m_bIsCreate;
    
};

#endif /* MatchRewardNode_hpp */
