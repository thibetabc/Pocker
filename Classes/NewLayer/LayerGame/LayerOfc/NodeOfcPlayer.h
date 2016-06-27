//
//  NodeOfcPlayer.h
//  pokerclient
//
//  Created by ll on 15/10/29.
//
//

#ifndef __pokerclient__NodeOfcPlayer__
#define __pokerclient__NodeOfcPlayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"

#include "ShaderSprite.h"

#include "user_data.h"
#include "msg.pb.h"

#include "GameLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class OFCRoom;

class NodeOfcPlayer : public OFCChildNode {
public:
    static NodeOfcPlayer* create(bool is_large_cell = false);
//    CREATE_FUNC(NodeOfcPlayer);
    virtual bool init(bool is_large_cell);
    
    void set_sitdown_callback(const std::function<void()>& callback) { sitdown_callback_ = callback;}
    
    void set_nodeNum(const int32_t nodeNum) {nodeNum_ = nodeNum;}
    const int32_t get_nodeNum() {return nodeNum_;}
    void set_displayPositionIdx(const int32_t displayPositionIdx) {displayPositionIdx_ = displayPositionIdx;}
    const int32_t get_displayPositionIdx() {return displayPositionIdx_;}
    
    void set_user(std::shared_ptr<user_ofc> user);
    std::shared_ptr<user_ofc> get_user(){if(user_ != nullptr)return user_; else return nullptr;}
    
    void update_seat_info();
    
    void update_user_score();
    
    void update_player_cards();
    
    void refresh_poker_card();
    
    void update_dealer();
    
    void update_poker_result();
    
    void update_propupdate(bool isRefreshAll = false);
    
    void set_dealer(bool isOn = false);
    
    void set_ProgressTimer(bool isOn = false, float operator_time = 0.0, bool is_using_timebank = false);
    void time_player_start();
    void time_player_close();
    void schedule_player_func(float dt);
    
    void schedule_dust_visible(float dt);
    void schedule_fantasy_visible(float dt);
    void schedule_close_all_result(float dt);
    
    void set_clear_result();
    
    void update_first_row_type(bool isRefreshAll = false);
    void update_middle_row_type(bool isRefreshAll = false);
    void update_last_row_type(bool isRefreshAll = false);
    
    void update_first_row_score();
    void update_middle_row_score();
    void update_last_row_score();
    
    void update_fantasy_result();
    
    void update_data(bool isRefreshAll = false);
    
    void set_dismiss();
    void update_record_status();
    
    void update_is_large_cell(bool is_large_cell);
    bool get_is_large_cell(){ return is_large_cell_; };
    void call_sit_down_callback(){if (sitdown_callback_) { sitdown_callback_(); } };
    void click_user_icon(Ref* sender);
    void set_click_photo_callback(const std::function<void(std::shared_ptr<user_ofc>)>& callback) { click_photo_callback_ = callback; };
    ImageView* getImageView(){return image_icon_;};
private:
    void click_image_player_card_bg(Ref* sender);
    void click_image_icon(Ref * sender);
    void click_btn_kick_player(Ref * sender);

    void update_user_ui(cmd_data_pointer = nullptr);
    void hide_ui();
private:
    
    std::function<void()> sitdown_callback_;
    std::function<void(std::shared_ptr<user_ofc>)> click_photo_callback_;
    
    int32_t nodeNum_;
    int32_t displayPositionIdx_;
    
    std::shared_ptr<user_ofc> user_;
    
    float operator_time_;
    float current_operator_time_;
    bool is_using_timebank_;
    
    msg::OFCCardType first_row_type_;
    msg::OFCCardType middle_row_type_;
    msg::OFCCardType last_row_type_;
    

    
private:
    Node* root_;
    
    Button* btn_sitdown_;
    
    ImageView* image_player_card_bg_;
    
    ImageView* image_icon_;
    
    ShaderSprite* user_icon_;
    
    Text* text_player_nickname_;
    
    Text* text_player_score_;
    
    ImageView* sprite_dealer_;
    
    std::vector<ImageView*> image_pokergrooves_;
    
    std::vector<Sprite*> sprite_handpokers_;
    
    std::vector<int32_t> int_pokerIDs_;
    
    ProgressTimer* progress_player_;
    Sprite* sprite_time_bar_;
    Sprite* sprite_time_bar_normal_;
    Text* text_time_;
    
    Text* text_player_score_first_;
    Text* text_player_score_middle_;
    Text* text_player_score_last_;
    
    Text* text_player_patterns_first_;
    Text* text_player_patterns_middle_;
    Text* text_player_patterns_last_;
    
    //Text* text_player_result_;
    Text* text_player_result_score_;
    
    Sprite* sprite_player_dust_;
    Sprite* sprite_player_fantasy_;
    
    ImageView* image_fantasy_mark_;
    ImageView* image_dust_bg_;
    
    Text* text_player_status_;
    
    Button* btn_kick_player_;
    
    ImageView* image_player_score_last_0_;
    ImageView* image_player_score_last_1_;
    ImageView* image_player_score_last_2_;
    ImageView* image_player_score_last_3_;
    
    Sprite* image_last_row_patterns_;
    
    ImageView* image_player_score_middle_0_;
    ImageView* image_player_score_middle_1_;
    ImageView* image_player_score_middle_2_;
    ImageView* image_player_score_middle_3_;
    
    Sprite* image_middle_row_patterns_;
    
    ImageView* image_player_score_first_0_;
    ImageView* image_player_score_first_1_;
    ImageView* image_player_score_first_2_;
    ImageView* image_player_score_first_3_;
    
    Sprite* image_first_row_patterns_;
    
    ImageView* image_time_;
    ImageView* image_result_bg_;
    
    events::event_dispatch dispatch_;
    bool is_large_cell_;
    
    Sprite* sprite_box_;
private:
    std::vector<std::string> string_image_pokergrooves_name_ = {"image_player_card_1", "image_player_card_2", "image_player_card_3", "image_player_card_4", "image_player_card_5", "image_player_card_6", "image_player_card_7", "image_player_card_8", "image_player_card_9", "image_player_card_10", "image_player_card_11", "image_player_card_12", "image_player_card_13"};
};

#endif /* defined(__pokerclient__NodeOfcPlayer__) */
