//
//  MatchReward.hpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/30.
//
//

#ifndef MatchReward_h
#define MatchReward_h

#include <stdio.h>
#include "BaseLayer.h"
#include "cocos2d.h"
#include "msg.pb.h"
using namespace cocos2d;
using namespace cocos2d::ui;
class MatchRewardAdd : public BaseLayer,public  cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(MatchRewardAdd);
    virtual bool init();
    void click_btn_back();
    void click_btn_save();
    void update_data(msg::PrizeLevel *data,int idx);
    void click_btn_delete();
    void remove_this();
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
private:
    Node *root;
    msg::PrizeLevel *m_pData;
    int idx;
};

#endif /* MatchReward_hpp */
