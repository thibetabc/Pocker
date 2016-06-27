#pragma once

#include <string>
#include "events.h"
#include "msg.pb.h"
#include "net_data_recv.h"
#include "cocos2d.h"

#define PDM PokerDataManager::getInstance()

class Room;
class PokerRoom;
class user_base;
class user_texas;
class RecordObject;
class StepObject;
class OFCRoom;
class user_ofc;
class room_data;

class PokerDataManager {
public:
    static PokerDataManager* getInstance() {
        if(!__poker_data_manager__) {
            __poker_data_manager__ = new PokerDataManager();
        }
        return __poker_data_manager__;
    }
    
    static void destroy() {
        delete __poker_data_manager__;
        __poker_data_manager__ = nullptr;
    }
    
    const std::shared_ptr<Room> get_room(uint32_t roomid);
    const std::shared_ptr<Room> get_replay_room(uint32_t handid);
    const std::string get_card_image(const uint32_t card_value);
    
    
    void leave_all_room();
public:
#pragma mark request
    /**
     *  请求补充计分牌
     *
     *  @param room_id 房间id
     *  @param amount  补充量
     */
    void send_poker_add_chips(const uint32_t amount,uint32_t room_id);
    void send_poker_choice(uint32_t roomid, msg::ChoiceType type, bool result);
    /**
     *  请求进入房间
     *
     *  @param room_id 房间id
     */
    void send_enter_room(uint32_t room_id);
    /**
     *  发送玩家座位属性
     */
    void send_seat_info(const uint32_t room_id, const msg::SittingStatus status, const uint32_t seat_id);
    /**
     *  发送玩家游戏动作
     *
     *  @param room_id   房间id
     *  @param action_id     enum {
     //        0正常 1弃牌 2让牌 3下注 4跟注 5加注 6全下
     NORMAL = 0,
     GIVEUP,
     GIVEAWAY,
     PUT_CHIP,
     FOLLOW_CHIP,
     ADD_CHIP,
     SHOW_HAND,
     };
     *  @param amount    数量
     */
    void send_poker_player_game_action(const uint32_t room_id , const msg::ActionStatus action_id, const uint32_t amount);
    
    /**
     *  发送玩家设置属性
     *
     *  @param room_id         房间id
     *  @param property_ids    属性id
     *  @param property_values 属性值
     */
    void send_send_set_change(const msg::PokerPlayerSetting& properties,uint32_t room_id);
    void send_ofc_set_change(const msg::OFCPlayerSetting& properties, uint32_t room_id);
    
    void send_show_card(const std::vector<bool>& ShowCard,uint32_t room_id);
    
    
    /**
     *  接收到游戏内属性更新后发给服务器的反馈
     */
    void send_poker_receive_prop_update();
    
    /**
     *  接收到玩家作为属性后发给服务器的反馈
     */
    void send_receive_seat_info();
    
    /**
     *  接收到桌子文字属性后发给服务器的反馈
     */
    void send_receive_table_note();
    
    /**
     *  接收到玩家动作请求后发给服务器的反馈
     */
    void send_receive_player_action();
    /**
     *  请求战绩
     *
     *  @param room_id 房间id
     */
    void send_get_score(const uint32_t room_id);
    /**
     *  接收到手牌结果后发给服务器的反馈
     */
    void send_poker_receive_hand_result();
    /**
     *  请求刷新所有数据
     *
     *  @param room_id 房间id
     */
    void send_poker_refresh_all();
    /**
     *  请求牌谱
     *
     *  @param hand_id 牌局id
     */
    void send_poker_get_hand_history(const uint32_t hand_id, bool isPoker);
    /**
     *  接收到游戏开始后给服务器的反馈
     */
    void send_receive_game_start();
    /**
     *  接收到其他玩家动作后给fwuuqi的反馈
     */
    void send_receive_other_player_action();
    /**
     *  反馈：接收到需要补充计分牌
     */
    void send_poker_receive_need_chip();
    //    /**
    //     *  反馈:接受到玩家设置信息
    //     */
    //    void send_receive_game_set_info();
    
    /**
     *  断线重新登录房间服务器
     */
    void send_login_roomserver();
    
    /**
     *  发送语音名字
     *
     *  @param message 语音名字
     */
    void send_game_voice(const std::string& message, const uint32_t time, const uint32_t room_id);
    
    /**
     *  收到别人发的消息后给服务器的回应
     */
    void send_receive_game_voice();
    
    void send_ofc_receive_holecards();
    void send_receive_send_pleyer_time();
    void send_ofc_receive_result();
    void send_receive_send_ofcgame_info();
    void send_receive_send_pleyer_score();
    void send_player_choose_cards(const std::vector<msg::OFCCardLocation> card_location_send,const uint32_t room_id);
    void send_poker_get_tourmentstatus(const uint32_t roomID);
    void send_poker_tournament_addon(const uint32_t roomID, bool is_addon);
    void send_poker_tourment_change_table(const uint32_t roomID, const uint32_t tableID);
    
    /**
     *  调整游戏暂停/开始
     *
     *  @param room_id 房间ID
     *  @param game_status 游戏状态
     */
    void send_set_game_status(uint32_t room_id, msg::GameStatus game_status);
    
    /**
     *  踢人
     *
     *  @param room_id 房间ID
     *  @param kick_user_id 被踢玩家ID
     */
    void send_kick_player(uint32_t room_id, uint32_t kick_user_id);
    
#pragma mark handle
    void handle_room_close(cmd_data_pointer data);
    void handle_receive_enter_match(cmd_data_pointer data);
    void handle_room_will_close(cmd_data_pointer data);
    void handle_poker_add_chips(cmd_data_pointer);
    void handle_enter_room(cmd_data_pointer);
    void handle_send_seat_info(cmd_data_pointer);
    void handle_send_player_game_action(cmd_data_pointer);
    void handle_poker_send_set_change(cmd_data_pointer);
    void handle_poker_receive_prop_update(cmd_data_pointer);
    void handle_receive_seat_info(cmd_data_pointer);
    void handle_receive_table_note(cmd_data_pointer);
    void handle_receive_player_action(cmd_data_pointer);
    void handle_get_score(cmd_data_pointer);
    void handle_poker_receive_hand_result(cmd_data_pointer);
    void handle_poker_refresh_all(cmd_data_pointer);
    void handle_poker_get_hand_history(cmd_data_pointer);
    void handle_receive_game_start(cmd_data_pointer);
    void handle_receive_other_player_action(cmd_data_pointer);
    void handle_upload_file_success(cmd_data_pointer data);
    void handle_login_room(cmd_data_pointer);
    void handle_receive_game_voice(cmd_data_pointer);
    void handle_poker_receive_set_change(cmd_data_pointer);
    void handle_ofc_poker_receive_set_change(cmd_data_pointer);
    void handle_receive_game_end(cmd_data_pointer);
    void handle_ofc_receive_result(cmd_data_pointer);
    void handle_ofc_send_player_cards(cmd_data_pointer);
    void handle_ofc_refresh_all(cmd_data_pointer);
    void handle_receive_ofc_prop_update(cmd_data_pointer);
    void handle_receive_choice(cmd_data_pointer);
    void handle_receive_set_game_status(cmd_data_pointer);
    void handle_receive_kick_player(cmd_data_pointer);
    void handle_receive_game_status(cmd_data_pointer);
    
    //CompetitionInfo by lvzhangle
    void handle_receive_poker_get_tourmentstatus(cmd_data_pointer data);
    void handle_poker_tournament_request_addon(cmd_data_pointer data);
    void handle_poker_receive_table_note(cmd_data_pointer data);
    void send_disable_auto_action(const uint32_t &room_id);
    
    
#pragma mark mahjong
    void handle_mahjong_receive_set_change(cmd_data_pointer);
    void handle_mahjong_receive_action(cmd_data_pointer);
    void handle_mahjong_refresh_all(cmd_data_pointer);
    void handle_mahjong_receive_prop_update(cmd_data_pointer);
    void send_mahjong_playersetting(const msg::MahjongPlayerSetting& properties, uint32_t room_id);
    void send_mahjong_choice(const uint32_t& room_id, const msg::MahjongActionChoice& choice, const msg::MahjongCard &operationcard = msg::MahjongCard::INVISIBLE);
    void send_mahjong_discard(const uint32_t& room_id, const msg::MahjongCard& card);
    void send_mahjong_blanksuit(const uint32_t& room_id, const msg::MahjongSuit& suit);
    void send_mahjong_switchcard(const uint32_t& room_id, const std::vector<msg::MahjongCard>& cards);
    
    void read_records_fri();
    void AddRecord(const std::shared_ptr<msg::GameRecord> record);
    
    enum {
        AFTER_RECEIVE_ALL_INFO,
        AFTER_RECEIVE_SEAT_INFO,
        AFTER_RECEIVE_STEP_HISTORY,
    };
    const std::vector<std::shared_ptr<msg::GameRecord>>& get_records() {return records_;} //牌局手牌
    void read_records(int roomid);
    void save_records();
public:
    void close_replay_room(int handid); //change move to public --by lvzhangle 20151229
    void close_room(int roomid);  //change move to public --by zhaoxuewei 20150105
    
private:
    void register_events();
    void register_history_events();
    void init_card_images();
    
    void add_room(std::shared_ptr<Room> Room);
    void add_replay_room(std::shared_ptr<Room> Room);
    
private:
    std::shared_ptr<events::event_dispatch> dispatch_;
    std::map<int, std::string> card_id_images_;
    std::map<int, std::shared_ptr<Room>> room_map;
    std::map<int, std::shared_ptr<Room>> replay_room_map;
    
    
    uint32_t player_action_;
    
    uint32_t buy_chip_amount_;  //购买的计分牌数量
    
    static std::string room_ip_;
    static uint32_t room_port_;
    uint32_t room_id_;
    std::string upload_filename;
    uint32_t time_;
    
    std::vector<std::shared_ptr<msg::GameRecord>> records_;//牌局手牌记录
    
    msg::RoomInfo select_tour_room_;
private:
    uint32_t enter_room_id = 0;
    static PokerDataManager* __poker_data_manager__;
    PokerDataManager();
    PokerDataManager(const PokerDataManager&);
    PokerDataManager operator=(const PokerDataManager&);
    
    CC_SYNTHESIZE(bool, isDetail, IsDetail);
    
};