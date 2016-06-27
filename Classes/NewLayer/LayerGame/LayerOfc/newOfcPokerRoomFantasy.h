//
//  newOfcPokerRoomFantasy.h
//  pokerclient
//
//  Created by ll on 15/11/5.
//
//

#ifndef __pokerclient__newOfcPokerRoomFantasy__
#define __pokerclient__newOfcPokerRoomFantasy__

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cmd_def.h"
#include "shaderSprite.h"
#include "user_data.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"
#include "GameLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class OFCRoom;

class newOfcPokerRoomFantasy : public OFCChildLayer
{
public:
    
    CREATE_FUNC(newOfcPokerRoomFantasy);
    virtual bool init();
    
    enum SUITPOKERTYPE{
        TYPE_NORMAL = 0,
        TYPE_ROYAL_STRAIGHT_FLUSH = 1, //皇家同花顺
        TYPE_STRAIGHT_FLUSH = 2, //同花顺
        TYPE_FOUR_OF_A_KIND = 3, //四条
        TYPE_FULL_HOUSE = 4, //葫芦
        TYPE_FLUSH = 5, //同花
        TYPE_STRAIGHT = 6, //顺子
        TYPE_THREE_OF_A_KIND = 7, //三条
        TYPE_TWO_PAIRS = 8, //两对
        TYPE_ONE_PAIR = 9, //一对
        TYPE_HIGH_CARDS = 10 //高牌
    };
    
    void set_user(std::shared_ptr<user_ofc> user);
    
    void click_btn_hand_sort_1(Ref* sender);
    void click_btn_hand_sort_2(Ref* sender);
    void click_btn_hand_sort_3(Ref* sender);
    
    void click_btn_confirm(Ref* sender);
    
    void click_btn_first_row_refresh(Ref* sender);
    void click_btn_middle_row_refresh(Ref* sender);
    void click_btn_last_row_refresh(Ref* sender);
    
    void click_btn_all_row_refresh(Ref* sender);
    
    void click_btn_put_first_row(Ref* sender);
    void click_btn_put_middle_row(Ref* sender);
    void click_btn_put_last_row(Ref* sender);
    
    void show_chat_layer();
    
    void update_user_ui(cmd_data_pointer = nullptr);
    void update_player_cards();
    void update_hand_cards();
    void update_propupdate();
    void update_ofcfantasy_player_choose_card_success();
    void update_put_row_btn();
    
    void set_ProgressTimer(bool isOn = false, float operator_time = 0.0, bool is_using_timebank = false);
    void time_player_start();
    void time_player_close();
    void schedule_player_func(float dt);
    
    void hand_card_suit_sort();
    void click_btn_suit_poker_sort(SUITPOKERTYPE  click_suit_sort_type);
    
    void update_bg_status();
    void update_bcakground_record_status();
    void update_bcakground_game_status(msg::GameStatus game_status);
private:
    Node* root_;
    
    ImageView* image_player_card_bg_;
    
    ImageView* image_icon_;
    
    ShaderSprite* user_icon_;
    
    Text* text_player_nickname_;
    
    Text* text_player_score_;
    
    Text* text_player_status_;
    
    Sprite* sprite_dealer_;
    
    std::vector<ImageView*> image_pokergrooves_;
    std::vector<ImageView*> image_holegrooves_;
    
    std::vector<int32_t> int_pokerIDs_;
    std::vector<int32_t> int_holepokerIDs_;
    
    std::vector<Sprite*> sprite_handpokers_;
    std::vector<Sprite*> sprite_holepokers_;
    
    ProgressTimer* progress_player_;
    Sprite* sprite_time_bar_;
    Sprite* sprite_time_bar_normal_;
    Text* text_time_;
    
    Button* btn_will_leave_;
    
    Button* btn_hand_sort_1_;
    Button* btn_hand_sort_2_;
    Button* btn_hand_sort_3_;
    
    Button* btn_confirm_;
    Button* btn_first_row_refresh_;
    Button* btn_middle_row_refresh_;
    Button* btn_last_row_refresh_;
    Button* btn_all_row_refresh_;
    
    Button* btn_put_first_row_;
    Button* btn_put_middle_row_;
    Button* btn_put_last_row_;
    
    Button* btn_suit_pattern_sort_1_;
    Button* btn_suit_pattern_sort_2_;
    Button* btn_suit_pattern_sort_3_;
    Button* btn_suit_pattern_sort_4_;
    Button* btn_suit_pattern_sort_5_;
    Button* btn_suit_pattern_sort_6_;
    Button* btn_suit_pattern_sort_7_;
    Button* btn_suit_pattern_sort_8_;
    Button* btn_suit_pattern_sort_9_;
    Button* btn_suit_pattern_sort_10_;
    
    ImageView* image_loading_bg_;
    
private:
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void selectSpriteForTouch(cocos2d::Point touchLocation);
    
    void register_events();
    
private:
    uint32_t selcard;
    cocos2d::Point selStartPoint;
    Sprite* selSprite;
    
    std::vector<Sprite*> sprite_sel_mark_;
    std::vector<uint32_t> int_sel_mark_;
    
    SUITPOKERTYPE current_suit_poker_type_ = SUITPOKERTYPE::TYPE_NORMAL;
    int32_t current_suit_poker_rank_ = -1;
    std::map<SUITPOKERTYPE, std::pair<Button*, std::vector<std::vector<uint32_t>>> > suit_poker_result_;
    
    float operator_time_;
    float current_operator_time_;
    bool is_using_timebank_;
    
    uint8_t handcard_sort_type_;
    
    std::shared_ptr<user_ofc> user_;
    
    
private:
    std::vector<std::string> string_image_pokergrooves_name_ = {"image_player_card_1", "image_player_card_2", "image_player_card_3", "image_player_card_4", "image_player_card_5", "image_player_card_6", "image_player_card_7", "image_player_card_8", "image_player_card_9", "image_player_card_10", "image_player_card_11", "image_player_card_12", "image_player_card_13"};
    
    std::vector<std::string> string_image_holegrooves_name_ = {
        "image_hand_card_1", "image_hand_card_2", "image_hand_card_3", "image_hand_card_4", "image_hand_card_5","image_hand_card_6", "image_hand_card_7", "image_hand_card_8", "image_hand_card_9", "image_hand_card_10", "image_hand_card_11", "image_hand_card_12", "image_hand_card_13", "image_hand_card_14", "image_hand_card_15", "image_hand_card_16", "image_hand_card_17"
    };
};


#endif /* defined(__pokerclient__newOfcPokerRoomFantasy__) */
