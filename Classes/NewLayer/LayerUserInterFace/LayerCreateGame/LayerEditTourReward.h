//
//  LayerEditTourReward.h
//  pokerclient
//
//  Created by lvzhangle on 16/5/09.
//
//

#ifndef __pokerclient__LayerEditTourReward__
#define __pokerclient__LayerEditTourReward__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerEditTourReward : public BaseLayer, public EditBoxDelegate
{
public:

    static LayerEditTourReward* create(uint32_t idx, msg::PrizeLevel prizeLevel);
    virtual bool init(uint32_t idx, msg::PrizeLevel prizeLevel);
    
    void set_change_callback(std::function<void(const int &, msg::PrizeLevel prizeLevel)> edit_reward_callback) {edit_reward_callback_ = edit_reward_callback;}
    void set_delete_callback(std::function<void(const int &)> reward_delete_callback) {reward_delete_callback_ = reward_delete_callback;}
private:
    void click_btn_back(Ref * sender);
    void click_btn_save_reward(Ref * sender);
    void click_btn_delete_reward(Ref * sender);
    
    uint32_t reward_idx_;
    msg::PrizeLevel prize_level_;
    std::function<void(const int &, msg::PrizeLevel prizeLevel)> edit_reward_callback_;
    std::function<void(const int &)> reward_delete_callback_;
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
#pragma mark UI
    Text* text_title_;
    
    Button* btn_back_;
    
    ImageView * image_listview_bg_;
    
    Text* text_rank_;
    Text* text_prize_;
    Text* text_other_prize_;
    
    ImageView* image_max_rank_bg_;
    
    ImageView* image_min_rank_bg_;
    
    ImageView* image_prize_;
    
    ImageView* image_other_prize_;
    
    TextField * tf_prize_;
    TextField * tf_other_prize_;
    TextField * tf_min_rank_;
    TextField * tf_max_rank_;
    
    EditBox * editbox_prize_;
    EditBox * editbox_other_prize_;
    EditBox * editbox_min_rank_;
    EditBox * editbox_max_rank_;
    
    Button * btn_save_reward_;
    Button * btn_delete_reward_;
    
    Text* text_rank_1_1_;
    Text* text_rank_1_2_;
    Text* text_rank_1_3_;
    
    Node* root_;
};

#endif /* defined(__pokerclient__LayerEditTourReward__) */