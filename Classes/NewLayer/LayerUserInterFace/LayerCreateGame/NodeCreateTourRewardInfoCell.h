//
//  NodeCreateTourRewardInfoCell.h
//  pokerclient
//
//  Created by lvzhangle on 16/5/26.
//
//
#ifndef __pokerclient__NodeCreateTourRewardInfoCell__
#define __pokerclient__NodeCreateTourRewardInfoCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class NodeCreateTourRewardInfoCell : public Widget{
public:
    
    static NodeCreateTourRewardInfoCell* create(uint32_t idx, msg::PrizeLevel prizeLevel, bool isCreate);
    
    virtual bool init(uint32_t idx, msg::PrizeLevel prizeLevel, bool isCreate);
    
    void set_cell_callback(std::function<void(const int &)> reward_cell_callback) {reward_cell_callback_ = reward_cell_callback;}
    
    uint32_t get_idx() {return cell_idx_;}
    
private:
    uint32_t cell_idx_;
    msg::PrizeLevel prize_level_;
    bool is_create_;
    int reward_info_cell_idx_;
    
    std::function<void(const int &)> reward_cell_callback_;
    
    void click_cell(Ref * sender);
#pragma mark UI
    
    Text* text_reward_info_rank_;
    Text* text_reward_info_reward_;
    Text* text_reward_info_other_reward_;
    
    Node* root_;
};

#endif /* defined(__pokerclient__NodeCreateTourRewardInfoCell__) */