#pragma once



#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"
#include "user_data.h"
#include "PlayerActionNode.h"
#include "GameLayer.h"
#include "MatchScroeNode.h"
#include "RecordHudLayer.h"
#include "Room.h"
#include "msg.pb.h"
#include "net_data_recv.h"
#include "PokerCardSprite.h"
#include "LayerCompetitionInfo.h"
#include "PokerDataManager.h"
#include "Room.h"
class PlayerInfoNode;
class user_texas;
class ChipNode;
class RecordAnimationLayer;
class GameChatLayer;
class RecordLayer;


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

#define TAG_ACTION_NODE 10001
class ChipPoolNode;
class data_recv_receive_player_action;

class PokerRoomLayer : public GameLayer
{
public:
    struct POINTS
    {
        cocos2d::Point seat;
        cocos2d::Point chip;
        cocos2d::Point card;
    };

    static PokerRoomLayer* Create(uint32_t ARoomID, bool AIsRecord)
    {
        
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_res_effect.plist");
        
        PokerRoomLayer *pRet = new(std::nothrow) PokerRoomLayer();
        pRet->RoomID = ARoomID;
        pRet->isRecord = AIsRecord;
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    
    }
    
    virtual bool init();
   // ~PokerRoomLayer();//释放时关闭音效。yxd
    
    void handle_android_back()
    {
        
        PDM->send_seat_info(RoomID, msg::SittingStatus::LEAVE, 0);
    }
    void click_btn_action(Ref* sender);
    void click_btn_tourment_info(Ref* sender);
    void click_btn_match_score(Ref* sender);
    void click_btn_buy_chip(Ref* sender);
    
    void click_btn_twice(Ref *sender);
    void click_btn_twice_2(Ref *sender);
    void click_btn_giveup(Ref* sender);
    void click_btn_watch(Ref* sender);
    void click_btn_chip(Ref* sender);
    void click_btn_follow(Ref* sender);
    void click_btn_add(Ref* sender);
    void click_btn_all(Ref* sender);
    void click_btn_record(Ref* sender);
    void SetActionNodeVisible(bool b);
    void click_btn_voice(Ref* sender, Widget::TouchEventType event);
    void click_btn_pause(Ref* sender);
    void click_btn_start(Ref * sender);
    void click_btn_chat(Ref* sender);
    void click_checkbox(Ref* sender, CheckBox::EventType event);
    void click_btn_share(Ref* sender); //分享
    
    void show_chat_layer();
    void change_language(Node* node);//切换语言
    
    
    void set_btn_choice_visible(msg::ChoiceType choice_type_);
    void handle_update_property();
    void clear(float);
    
    void handle_game_hand_info(const msg::HandResultInfo& data);
    void handle_player_operation(const msg::PokerActionRequest& data);
    virtual void onEnter();
    
    const std::shared_ptr<PokerRoom> GetRoom();
    void handle_receive_poker_tournament_request_addon(uint32_t buyin);
    void UpdatePlayerAction(int userid, int countdown, bool is_using_timebank = false);
    void handle_player_standup(int userid);
    void handle_showcard(const std::vector<bool> showcard);
    void PositionUpdate();
    void update_game_status(const msg::GameStatus game_status);
    void receive_game_start();
    void receive_game_end();
    void receive_table_note(std::string note);
    void init_auto_buttons(const msg::PokerPlayerSetting& setting);
    void receive_poker_tournamentstatus(const msg::Processor_530_PokerGetTourmentStatus_DOWN& processor);
    void show_history_layer();
    void play_history_finished();
    
    void add_Shop_Layer(cmd_data_pointer data);
    void update_show_card();

private:
    MatchScroeNode* msn = NULL;
    PlayerActionNode* Actionnode = NULL;
    
    void handle_poker_receive_need_chip(cmd_data_pointer = nullptr);
    
    void handle_download_events(cmd_data_pointer);
    void handle_player_disconnect(cmd_data_pointer);
    void handle_player_gaming(cmd_data_pointer);
    
    
    void handle_receive_chat(cmd_data_pointer);
    
    void update_user_property(const std::shared_ptr<user_texas>& user);
private:
    void init_seats();
    void init_chip_pool();
    
    void register_events();
    
    void hidden_action_btns();
public:
    void set_listview_visible(const bool visible);
    void clear_view(Ref*);
    void clear_view_new();
    void update_auto_buttons();
    void set_is_playing(bool value) {bPlaying = value;}
    bool get_is_playing(){return bPlaying;};
    void add_player_info_layer(std::shared_ptr<user_texas> user);
private:
    void set_node_of_chip_visible(const bool visible);
    
    const POINTS get_position_with_seat_num(const uint32_t seat_num);
    
    PlayerInfoNode* get_player_info_node(const uint32_t user_id);
    
    bool onTouchBegan(Touch *touch, Event *event);
    
private:
    bool isExit;
    Node* root_;

    Button* btn_tourment_info_;
    Button* btn_action_;
    Button* btn_match_score_;
    Button* btn_buy_chip_;
    Button* btn_record_;
    Button* btn_voice_;
    Button* btn_pause_;
    Button* btn_start_;
    Button* btn_chat_;
    Button* btn_giveup_;
    Button* btn_twice;
    Button* btn_twice_2;
    Button* btn_share_; //分享
    ImageView* image_red_;
    Text* text_unread_message_count_;
    Text* text_btn_follow_amount_;
    
    Button* viewExitSprite;//专门用来关闭各种子界面的精灵
    Sprite* sprite_chip_icon_;//底池中的计分牌
    Sprite* sprite_pool_;//底池
    Text*   text_pool_;
    Text*   text_pool_amount_;
    Text* check_box_straddle_text_number;
    
    TableView* tableview_;
    Text* text_table_note_;
    ImageView* image_table_note_bg_;
    
    ImageView* image_sit_back_;
    
    CheckBox* check_box_straddle;
    CheckBox* check_box_sitout;
    CheckBox* check_box_autocall;
    Text* check_box_autocall_text;
    Text* check_box_autocall_amount;
    CheckBox* check_box_callorfold;
    
    msg::PokerPlayerSetting properties;
    //下注区
    ChipPoolNode* node_chip_;
    
    std::vector<Node*> nodes_of_chip_;//整个的底池
    
    std::map<uint32_t, PlayerInfoNode*> info_nodes_;
    
    //第一个id是用户id，第二个是手牌id 1~7
    std::map<uint32_t, std::map<uint8_t, Sprite*> > user_cards_;
   
    std::map<uint32_t, Sprite*> all_cards_;
    
    std::map<uint8_t, Sprite*> user_cards_gray;//
    
    std::vector<POINTS> player_seat_chip_card_positions_;
    
    std::vector<std::vector<std::shared_ptr<msg::Property> > > user_setting_properties_;
    
    std::map<uint32_t, std::vector<uint32_t> > seat_idx_with_person_amount_;
    
    std::map<std::string, Button*> player_action_buttons_;
    
    std::map<uint32_t, ChipNode*> chip_nodes_;//人物面前的下注计分牌
    
    //公牌1～5
    std::list<Sprite*> public_cards1_;
    std::list<Sprite*> has_show_cards1_;
    int show_cards_num1_ = 1;
    int show_cards_num2_ = 1;
    //公牌6～10
    std::list<Sprite*> public_cards2_;
    std::list<Sprite*> has_show_cards2_;
    msg::PokerActionRequest player_operator_action_;
    
    std::vector<ChipNode*> table_pot_chips_;
    
    std::string record_filename_;
    bool clear_room_;
    bool bCurrentUserAction = false;
    bool bPlaying = false;
    msg::ChoiceType choicetype;
    std::map<uint32_t, int32_t>  chip_nodes_flags; //判断是否要进行动画
    bool isNeedAction();
    int _last_chip_nodes_num;
    int _last_chip_num;
    int _poolNum;
};