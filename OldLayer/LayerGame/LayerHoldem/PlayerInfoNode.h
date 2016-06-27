#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"
#include "ShaderSprite.h"
#include "GameLayer.h"

class user_texas;

using namespace cocos2d;
using namespace cocos2d::ui;

class PokerRoom;

class PlayerInfoNode : public PokerChildNode {
public:
    CREATE_FUNC(PlayerInfoNode);
    virtual bool init();
    
    const uint32_t get_seat_id() { return seat_id_;}
    
    void set_seat_id(const uint32_t seat_id) { seat_id_ = seat_id;}
    
    void update_data(const std::shared_ptr<user_texas>& user);
    void update_data();
    
    void set_dealer(const bool dealer);
    void show_green_box(const bool visible);
    void hideWinAnimation();
    
    void show_progress_box(const float time,const float pastTime, bool is_using_timebank = false);
    void show_win_amount(const uint32_t amount);
    void show_card_type(const std::string& card_type);
    
    void set_description(const std::string& description);
    
    void click_image_box(Ref* sender);

    void set_giveup_callback(const std::function<void()>& callback) { giveup_callback_ = callback;}
    
    void set_dismiss();
    
    void clear();
    
    void setWholeOpacity(int opacity);
    void recreate();
    
    void lost_connect();
    void keep_ingaming();
    
    const Point get_text_point() { return text_chip_amount_->getPosition();}
    
    void show_voice_animation(const float duration);
    
    void download_head_image_finished(cmd_data_pointer);
    
    virtual void setVisible(bool vis);
    virtual void setVisible1(bool vis);
    virtual void removeFromParentAndCleanup(bool cleanup);
    virtual void removeFromParent();
    
    void click_btn_kick(Ref* sender);
private:
    ImageView* image_head_;
    Sprite* sprite_chip_;
    Sprite* sprite_box_;
    Text* text_nickname_;
    Sprite* text_status_bg_;
    Text* text_status_;
    Text* text_chip_amount_;
    Sprite* sprite_name_bg_;
    Sprite* sprite_chip_bg_;
    ImageView* image_player_bg_;
    ImageView* image_gray_;
    ImageView * image_box_bg_;
    Sprite* sprite_dealer_;
    Sprite* sprite_winner_;
    Sprite* sprite_timer_fire;
    Point sprite_timer_fire_pos;
    
    Sprite* player_info_last;
    Sprite* player_info_bet;
    Sprite* player_info_check;
    Sprite* player_info_fold;
    Sprite* player_info_raise;
    Sprite* player_info_allin;
    
    Sprite* text_win_type_bg_;
    Text* text_win_type_;
    Text* text_win_amount_;
    ParticleSystemQuad * particleSystem;
    ProgressTimer* progerss_;
    uint32_t seat_id_;
    
    std::function<void()> giveup_callback_;

    
    Node* root_;
    
    ShaderSprite* sprite_icon_;
    
    events::event_dispatch dispatch_;
    std::shared_ptr<user_texas> user_;
    
    Button* btn_kick_;
};