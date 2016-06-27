//
//  GameDataManager.h
//  pokerclient
//
//  Created by duanmt on 15/5/11.
//
//

#ifndef __pokerclient__GameDataManager__
#define __pokerclient__GameDataManager__

#include <string>
#include "events.h"
#include "net_data_recv.h"
#include "msg.pb.h"

#define GDM GameDataManager::getInstance()

class data_recv_login_resources;
class user_data;
class Property;
class user_base;
class GameDataManager {
public:
    static GameDataManager* getInstance() {
        if(!__game_data_manager__) {
            __game_data_manager__ = new GameDataManager();
        }
        return __game_data_manager__;
    }
    
    static void destroy() {
        delete __game_data_manager__;
        __game_data_manager__ = nullptr;
    }
    

#pragma mark get
    //获取is_newcard_
    const bool get_is_newcard() { return is_newcard_; }
    const uint32_t get_user_id() {return userinfo_.userid();}
    msg::UserDetailInfo* get_userinfo() {return &userinfo_;}
    const msg::RoomInfomation& get_room_infomations() { return room_infomations_;}
    const msg::RoomProp& get_Property_by_game_id(const msg::RoomType game_id);
    const std::string get_user_directory();
    
    void clear_session_key() {session_key_ = "";}
    
    bool is_file_exist(const std::string& filename);
    bool get_is_check_version(){return is_check_version_;};
    
    const std::string get_session_key();
    msg::RoomType get_last_game_room_type(){return client_user_setting_data.lastroomtype();}
    bool get_last_game_is_tournament(){return client_user_setting_data.lastistournament();}
    int get_last_game_group_id(){return client_user_setting_data.lastgroupid();}
    int get_last_game_room_id(){return client_user_setting_data.lastroomid();}
    const msg::CreateRoomInfoForSave* get_create_room_info(msg::RoomType type, bool is_tournament);
    const msg::RoomProp* get_create_room_prop(msg::RoomType type, bool is_tournament);
    const bool get_show_tutorial(int index);
    std::vector<float> get_self_score(msg::Processor_170_GetHistoryScore_Down processor);
#pragma mark set
    void set_createroom_info(msg::RoomType type, bool is_tournament, const msg::RoomInfo& roominfo, const msg::RoomProp& roomprop);
    void set_createroom_info(msg::RoomType type, bool is_tournament, const msg::RoomInfo& roominfo, const std::string & template_size_name,  const std::string & template_speed_name,const msg::TournamentInfo& tournament_info);
    void set_last_game_info(msg::RoomType type, bool is_tournament, int groupid, int roomid);
    void set_IsInBackGround(bool value) {IsInBackGround_ = value;}
    const bool get_IsInBackGround() { return IsInBackGround_; }
    void add_room_server(std::string serverIPv4, std::string serverIPv6, uint32_t port);
    void set_is_newcard(bool isnewcard);
    void set_show_tutorial(int index, bool show);
    void InitUserData(const std::string& username, const uint32_t user_id);
    void set_is_check_version(bool is_check_version){ is_check_version_ = is_check_version;};
#pragma mark handle receive data
    void handle_login_resource_server(cmd_data_pointer data);
    void handle_offline(cmd_data_pointer data);
    void handle_login(cmd_data_pointer data);
    void handle_get_roommessage(cmd_data_pointer data);
    void handle_notifychange(cmd_data_pointer data);
    void request_change_language_room();
    void handle_change_language_room(cmd_data_pointer data);
    void login_resource_server();
    bool has_session_key_();
private:
    void register_events();
    
    bool IsInBackGround_;
    msg::UserDetailInfo userinfo_;
    uint32_t userid;
    events::event_dispatch dispatch_;
    msg::RoomInfomation room_infomations_;
    std::string session_key_ = "";
    bool is_newcard_;    //是否使用四色牌

    msg::ClientUserSetting client_user_setting_data;
    bool show_group_tutor_ =true;  //是否显示向导
    bool show_friend_tutor_ = true;
    bool show_game_tutor_ = true;
    bool show_moment_tutor_ = true;
    bool show_setting_tutor_ = true;
    bool is_check_version_ = false;//是否是审核版本

    static GameDataManager* __game_data_manager__;    GameDataManager();
    GameDataManager(const GameDataManager&);
    GameDataManager operator=(const GameDataManager&);
};

#endif /* defined(__pokerclient__GameDataManager__) */
