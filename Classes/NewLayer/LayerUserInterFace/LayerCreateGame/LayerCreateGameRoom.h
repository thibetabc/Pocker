//
//  LayerCreateGameRoom.h
//  pokerclient
//
//  Created by lvzhangle on 16/4/5.
//
//

#ifndef __pokerclient__LayerCreateGameRoom__
#define __pokerclient__LayerCreateGameRoom__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "NodePropertyCell.h"
#include "GameDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerCreateGameRoom : public BaseLayer
{
public:
    static LayerCreateGameRoom* create(msg::RoomType room_type, bool is_tournament, const msg::GroupInfo& groupinfo, bool isCreate = true);
    static LayerCreateGameRoom* create(msg::RoomType room_type, bool is_tournament, bool isCreate = true);
    static LayerCreateGameRoom* create(msg::Processor_410_EnterRoom_DOWN enterRoomInfo, bool isCreate = false);
    void set_groupinfo(const msg::GroupInfo& groupinfo);
    void set_dismiss_callback(const std::function<void()>& callback) {
        dis_miss_callback_ = callback;
    }
    void set_is_create(bool isCreate);
    void set_enter_room_info(msg::Processor_410_EnterRoom_DOWN enterRoomInfo){enter_room_info_ = enterRoomInfo;}
    void set_room_info(msg::RoomInfo roomInfo){room_info_ = roomInfo;}
    const msg::RoomInfo get_room_info() {return room_info_;}
    void update_enter_room_info(msg::Processor_410_EnterRoom_DOWN enterRoomInfo, bool isCreate = false);
    virtual bool init(msg::RoomType room_type, bool is_tournament, bool isCreate = false);
private:
    void update_cell_int_value_min_max(NodePropertyCell* cell);
    
    void refresh_listview();
    
    void click_btn_back(Ref * sender);
    void click_btn_display_more(Ref * sender);
    void click_btn_create_game_room(Ref * sender);
    void click_btn_cancel_tournament(Ref * sender);
    void click_text_join_list(Ref * sender);
protected:
    void register_events();
    void handle_create_room(cmd_data_pointer data);
    void handle_cancel_tournament(cmd_data_pointer data);
private:
    void afterSelectGroup(const msg::GroupInfo& groupinfo);
    void afterChangeRoomName(const msg::Property& prop);
    void afterChangeProp(const msg::Property& prop);
    void AfterSelectRoomType(msg::RoomType room_type, bool is_tournament);
    
    void afterSelectTemplateSize(const msg::Property& prop);
    void afterSelectTemplateSpeed(const msg::Property& prop);
    
    void afterChangeTourBlind(msg::TournamentLevel tourLevel);
    void afterChangeTourReward(msg::TournamentPrize tourPrize);
    
    void afterSelectTime(long long int selectTime);
    
    void init_create_property();
    void set_language();
    
    void RefreshLayer();
    void LayerDisconnect();
    
    
    long long int int_time_input_;
    msg::RoomType room_type_;
    bool is_tournament_;
    msg::GroupInfo select_groupinfo_;
    msg::Property roomname;
    msg::RoomProp propertys_;
    
    msg::TournamentInfo tournament_info_;
    std::string template_size_name_;
    std::string template_speed_name_;
    
    msg::RoomInfo room_info_;
    msg::Processor_410_EnterRoom_DOWN enter_room_info_;
    
    bool is_display_all_;
    bool is_create_;
    bool has_join_;
    
    std::function<void()> dis_miss_callback_;
    //void initData();
#pragma mark UI
private:
    Button* btn_back_;
    Text* text_title_;

    Button* btn_create_game_room_;
    
    ImageView* image_layer_creategameroom_bg_;
    
    ImageView* image_listview_bg_;
    
    Text* text_join_list_;
    
    ListView* listview_;
    
    TableView * tableview_;
    
    Layout* layout_display_more_;
    
    Text* text_display_more_;
    
    Node * root_;
};

#endif /* defined(__pokerclient__LayerCreateGameRoom__) */
