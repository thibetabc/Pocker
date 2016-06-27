//
//  LayerCreateTourBlindAndReward.hpp
//  pokerclient
//
//  Created by lvzhangle on 16/5/09.
//
//

#ifndef __pokerclient__LayerCreateTourBlindAndReward__
#define __pokerclient__LayerCreateTourBlindAndReward__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerCreateTourBlindAndReward : public BaseLayer
{
public:
    enum SELECT_TYPE{
        NONE,
        BLIND_INFO,
        REWARD_INFO
    };
    static LayerCreateTourBlindAndReward* create(msg::TournamentInfo & tourInfo, bool isCreate, SELECT_TYPE selectType);
    virtual bool init(msg::TournamentInfo & tourInfo, bool isCreate, SELECT_TYPE selectType);
    
    void set_level_change_callback(std::function<void(msg::TournamentLevel)> level_change_callback) {level_change_callback_ = level_change_callback;}
    void set_prize_change_callback(std::function<void(msg::TournamentPrize)> prize_change_callback) {prize_change_callback_ = prize_change_callback;}
    
    void update_data(msg::TournamentInfo & tourInfo, bool isCreate);
    
private:
    void click_blind_info_cell(uint32_t idx);
    void click_reward_info_cell(uint32_t idx);
    void change_reward_info(uint32_t idx, msg::PrizeLevel prizeLevel);
    void delete_reward_info(uint32_t idx);
    void after_prize_change();
    
    void change_blind_info(uint32_t idx, msg::BlindLevel blindLevel);
    void add_blind_info(uint32_t idx);
    void delete_blind_info(uint32_t idx);
    void after_level_change();
    
    void click_btn_back(Ref * sender);
    void refresh_listview();
    void click_blind_info(Ref * sender);
    void click_reward_info(Ref * sender);
    void click_add_reward_cell(Ref * sender);
    
    std::function<void(msg::TournamentLevel)> level_change_callback_;
    std::function<void(msg::TournamentPrize)> prize_change_callback_;
    
    bool is_editing_ = false;
    bool is_create_;
    SELECT_TYPE select_type_;
    msg::TournamentInfo tournament_info_;
#pragma mark UI
    Layout* layout_blind_info_;
    Layout* layout_reward_info_;
    Layout* layout_blind_info_title_;
    Layout* layout_reward_info_title_;
    Layout* layout_add_reward_cell_;
    
    Text* text_blind_info_;
    Text* text_reward_info_;
    
    Text* text_title_;
    
    Button* btn_back_;
    
    ListView* listview_;
    
    ImageView * image_listview_bg_;
    
    Node* root_;
};

#endif /* defined(__pokerclient__LayerCreateTourBlindAndReward__) */
