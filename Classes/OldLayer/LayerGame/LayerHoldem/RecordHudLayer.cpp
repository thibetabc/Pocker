#include "RecordHudLayer.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "cmd_def.h"
#include "PokerRoomLayer.h"
#include <iostream>

bool RecordHudLayer::init()
{
    if (!PokerChildLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerReplayHud.csb");
    this->addChild(root);
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_play_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_play"));
    btn_pre_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_pre"));
    btn_next_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_next"));
    text_step_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_step"));
    
//    btn_pre_->setVisible(false);
//    btn_next_->setVisible(false);
    
    //btn_next_->loadTextureDisabled("replay/hui-button.png");
    
    btn_pre_->setTitleText(tools::local_string("pre_step", "上一步"));
    btn_next_->setTitleText(tools::local_string("next_step", "下一步"));
    
    btn_back_->addClickEventListener(std::bind(&RecordHudLayer::click_btn_back, this, std::placeholders::_1));
    btn_play_->addClickEventListener(std::bind(&RecordHudLayer::click_btn_play, this, std::placeholders::_1));
    btn_pre_->addClickEventListener(std::bind(&RecordHudLayer::click_btn_pre, this, std::placeholders::_1));
    btn_next_->addClickEventListener(std::bind(&RecordHudLayer::click_btn_next, this, std::placeholders::_1));
    return true;
}
void RecordHudLayer::onExit()
{
    if(dismiss_callback_) {
        dismiss_callback_();
    }
    
    GetRoom()->stop_play_record();
    BaseLayer::onExit();
}
void RecordHudLayer::click_btn_back(Ref* sender)
{
    if (GetRoom() != nullptr)
    {
        PDM->close_replay_room(GetRoom()->get_id());
    }else
    {
        cocos2d::log("Error : RecordHudLayer::click_btn_back GetRoom = nullptr!");
    }

}

void RecordHudLayer::click_btn_play(Ref* sender)
{
    if(GetRoom()->get_current_step() == GetRoom()->GetReplaySteps().replaystep_size()) {
        auto_play_ = true;
        if(clear_callback_) {
            clear_callback_();
        }
        GetRoom()->set_current_step(1);
        text_step_->setString(tools::to_string(GetRoom()->get_current_step()) + "/" + tools::to_string(GetRoom()->GetReplaySteps().replaystep_size()));
    }
    else {
        auto_play_ = !auto_play_;
        
    }
    if(auto_play_) {
        btn_next_->setVisible(false);
        btn_pre_->setVisible(false);
        
        btn_play_->loadTextureNormal("game_res/replay/button-1_0.png");
        btn_play_->loadTexturePressed("game_res/replay/button-2_0.png");
        
        this->schedule([this](float){
            click_btn_next(nullptr);
        }, 3.0f, "auto_play");
    }
    else {
        btn_next_->setVisible(true);
        btn_pre_->setVisible(true);
        btn_play_->loadTextureNormal("game_res/replay/button-2_0.png");
        btn_play_->loadTexturePressed("game_res/replay/button-1_0.png");
        this->unschedule("auto_play");
    }
}

void RecordHudLayer::click_btn_pre(Ref* sender)
{
    if (GetRoom() != nullptr)
    {
        GetRoom()->pre_step();
        text_step_->setString(tools::to_string(GetRoom()->get_current_step()) + "/" + tools::to_string(GetRoom()->GetReplaySteps().replaystep_size()));
        btn_pre_->setEnabled(GetRoom()->get_current_step() != 1);
        btn_next_->setEnabled(GetRoom()->get_current_step() != GetRoom()->GetReplaySteps().replaystep_size());
    }else
    {
        cocos2d::log("Error : RecordHudLayer::click_btn_next GetRoom = nullptr!");
    }
}

void RecordHudLayer::click_btn_next(Ref* sender)
{
    if (GetRoom() != nullptr)
    {
        GetRoom()->next_step();
        text_step_->setString(tools::to_string(GetRoom()->get_current_step()) + "/" + tools::to_string(GetRoom()->GetReplaySteps().replaystep_size()));
        btn_next_->setEnabled(GetRoom()->get_current_step() != GetRoom()->GetReplaySteps().replaystep_size());
        btn_pre_->setEnabled(GetRoom()->get_current_step() != 1);
    }else
    {
        cocos2d::log("Error : RecordHudLayer::click_btn_next GetRoom = nullptr!");
    }
}

void RecordHudLayer::Refresh()
{
    text_step_->setString("0/" + tools::to_string(GetRoom()->GetReplaySteps().replaystep_size()));
    scheduleOnce([this](float){
        this->click_btn_next(btn_next_);
    }, 0.1f, "RecordHudLayer::onEnterTransitionDidFinish");
}

void RecordHudLayer::handle_play_history_finished()
{
    btn_next_->setVisible(true);
    btn_pre_->setVisible(true);
    btn_play_->loadTextureNormal("game_res/replay/button-2_0.png");
    btn_play_->loadTexturePressed("game_res/replay/button-1_0.png");
    this->unschedule("auto_play");
}