//
//  LayerCompetitionInfo.h
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#ifndef __pokerclient__LayerCompetitionInfo__
#define __pokerclient__LayerCompetitionInfo__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"
#include "SceneManager.h"
#include "msg.pb.h"

#include "LayerComInfoOverView.h"
#include "LayerComInfoBlind.h"
#include "LayerComInfoReward.h"
#include "LayerComInfoPlayer.h"

class LayerComInfoPlayer;

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerCompetitionInfo : public BaseLayer {
public:
    
    CREATE_FUNC(LayerCompetitionInfo);
    virtual bool init();
public:
    
    void update_data(const msg::Processor_530_PokerGetTourmentStatus_DOWN& tourment_status);
    
    
private:
    void click_btn_overview(Ref* sender);
    void click_btn_blind(Ref* sender);
    void click_btn_reward(Ref* sender);
    void click_btn_player(Ref* sender);
    void click_btn_quit(Ref * sender);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
private:
    Node* root_;
    
    Button* btn_overview_;
    Button* btn_blind_;
    Button* btn_reward_;
    Button* btn_player_;
    Button* btn_quit_;
    
    ImageView* image_competition_info_bg_;
    
    LayerComInfoOverView* layer_over_view_;
    LayerComInfoBlind* layer_blind_;
    LayerComInfoReward* layer_reward_;
    LayerComInfoPlayer* layer_player_;
    
    msg::Processor_530_PokerGetTourmentStatus_DOWN tourment_status_;
};

#endif /* defined(__pokerclient__LayerCompetitionInfo__) */
