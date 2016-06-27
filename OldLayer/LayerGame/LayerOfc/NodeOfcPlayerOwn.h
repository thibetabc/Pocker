//
//  NewOfcPlayerOwn.h
//  pokerclient
//
//  Created by ll on 15/10/29.
//
//

#ifndef __pokerclient__NewOfcPlayerOwn__
#define __pokerclient__NewOfcPlayerOwn__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"

#include "ShaderSprite.h"
#include "GameLayer.h"
#include "user_data.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class OFCRoom;

class NodeOfcPlayerOwn : public OFCChildNode {
public:
    CREATE_FUNC(NodeOfcPlayerOwn);
    virtual bool init();
    
public:
    void set_user(std::shared_ptr<user_ofc> user);
    std::shared_ptr<user_ofc> get_user(){if(user_ != nullptr)return user_; else return nullptr;}
    
    
    void update_record_status();
    void update_user_ui(cmd_data_pointer = nullptr);
    void update_user_score();
    void update_player_cards();
    void update_hand_cards();
    void update_dis_cards();
    void update_dealer();
    void update_poker_result();
    void update_propupdate();
    
    void update_seat_info();
    
    void set_dealer(bool isOn = false);
    void set_clear_result();
    
    void set_ProgressTimer(bool isOn = false, float operator_time = 0.0, bool is_using_timebank = false);
    void time_player_start();
    void time_player_close();
    void schedule_player_func(float dt);
    
    void schedule_dust_visible(float dt);
    void schedule_fantasy_visible(float dt);
    void schedule_close_all_result(float dt);
    
    
    void update_first_row_type();
    void update_middle_row_type();
    void update_last_row_type();
    
    void update_first_row_score();
    void update_middle_row_score();
    void update_last_row_score();
    
    void update_fantasy_result();
    
    void update_auto_buttons(msg::OFCPlayerSetting& setting);
    
private:
    
    void click_btn_confirm(Ref* sender);
    void click_btn_sit_back(Ref* sender);
    void click_btn_will_leave(Ref* sender);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void selectSpriteForTouch(cocos2d::Point touchLocation);
private:
    Node* root_;
    
    ImageView* image_player_card_bg_;
    
    ImageView* image_icon_;
    
    ShaderSprite* user_icon_;
    
    Text* text_player_nickname_;
    
    Text* text_player_score_;
    
    Text* text_player_status_;
    
    Sprite* sprite_dealer_;
    
    ImageView* image_hole_cards_bg_;
    Text* text_hole_cards_;
    
    std::vector<ImageView*> image_pokergrooves_;
    
    std::vector<int32_t> int_pokerIDs_;
    std::vector<int32_t> int_holepokerIDs_;
    
    std::vector<Sprite*> sprite_handpokers_;
    std::vector<Sprite*> sprite_holepokers_;
    std::vector<Sprite*> sprite_dispokers_;
    
    std::vector<cocos2d::Point> point_holepokers_;
    
    ImageView* image_dis_cards_bg_;
    Text* text_dis_cards_;
    
    Sprite* selSprite;
    int32_t selCard;
    
    Button* btn_confirm_;
    Button* btn_sit_back_;
    Button* btn_will_leave_;
    
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
    
    events::event_dispatch dispatch_;
private:
    bool is_record_;
    std::shared_ptr<user_ofc> user_;
    
    float operator_time_;
    float current_operator_time_;
    bool is_using_timebank_;
    
    std::vector<std::vector<std::shared_ptr<msg::Property> > > user_setting_properties_;
    
private:
    std::vector<std::string> string_image_pokergrooves_name_ = {"image_player_card_1", "image_player_card_2", "image_player_card_3", "image_player_card_4", "image_player_card_5", "image_player_card_6", "image_player_card_7", "image_player_card_8", "image_player_card_9", "image_player_card_10", "image_player_card_11", "image_player_card_12", "image_player_card_13"};

};

#endif /* defined(__pokerclient__NewOfcPlayerOwn__) */
