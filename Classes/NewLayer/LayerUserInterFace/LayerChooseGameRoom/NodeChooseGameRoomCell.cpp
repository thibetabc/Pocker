//
//  NodeChooseGameRoomCell.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/28.
//
//

#include "NodeChooseGameRoomCell.h"
#include "utils.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "cmd_def.h"

bool NodeChooseGameRoomCell::init()
{
    if (!TableViewCell::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeChooseGameRoomCell.csb");
    
    this->addChild(root_);
    
    image_choosegameroom_cell_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_choosegameroom_cell_bg"));
    image_group_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_group_icon"));
    image_choosegameroom_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_choosegameroom_icon"));
    
    text_room_status_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_status_1"));
    text_room_status_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_status_2"));
    text_room_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_name"));
    text_room_note_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_note_1"));
    text_room_note_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_note_2"));
    text_room_note_3_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_room_note_3"));
    
    sprite_icon_ = ShaderSprite::create("Default/image_defualt_group_icon_large.png", "Default/image_mask_circle.png");
    root_->addChild(sprite_icon_);
    sprite_icon_->setScale(image_group_icon_->getContentSize().width / sprite_icon_->getContentSize().width, image_group_icon_->getContentSize().height / sprite_icon_->getContentSize().height);
    sprite_icon_->setPosition(image_group_icon_->getPosition());
    image_group_icon_->setVisible(false);
    
    return true;
}

void NodeChooseGameRoomCell::update_data(msg::RoomInfo room_data)
{
    room_data_ = room_data;
    
    std::string playerNum = tools::to_string(room_data_.playernum()) + std::string("/") + tools::to_string(room_data_.maxplayernum());
    
    text_room_name_->setString(room_data_.roomname());
    
    if (room_data_.has_groupinfo())
    {
        text_room_note_1_->setString(room_data_.groupinfo().groupname());
    }
    else
    {
        text_room_note_1_->setString(tools::to_string(room_data_.roomid()));
    }
    text_room_note_2_->setString(room_data_.roomnote());
    
    bool is_tournament = room_data_.istournament();
    if (is_tournament)
    {
        double db_time = room_data_.starttime();
        db_time /= 1000;
        std::string text_start_time = tools::time_display(db_time, tools::TIMEDISPLAYTYPE::ALLTIME);
        text_room_note_3_->setString(tools::local_string("starting_at", "开始时间：") + text_start_time);
        text_room_note_3_->setVisible(true);
    }else
    {
        if (text_room_note_3_->isVisible())
        {
            text_room_note_3_->setVisible(false);
        }
    }
    
    std::string key;
    switch (room_data_.roomstatus()) {
        case 0:
            key = "open";
            break;
        case 1:
            key = "close";
            break;
        case 2:
            key = "waiting";
            break;
            
        default:
            break;
    }
    std::string status = tools::local_string(key, "关闭还是开启");
    text_room_status_1_->setString(status);
    
    text_room_status_2_->setString(playerNum);
    
    if (room_data_.roomtype().roomtype() == msg::RoomType::OFC3 ||
        room_data_.roomtype().roomtype() == msg::RoomType::OFC4)
    {
        image_choosegameroom_icon_->loadTexture("login_res/choosegameroom_res/image_choosegameroom_icon_ofc3.png");
    }else if (room_data_.roomtype().roomtype() == msg::RoomType::NLHOLDEM ||
              room_data_.roomtype().roomtype() == msg::RoomType::PLO ||
              room_data_.roomtype().roomtype() == msg::RoomType::PLOHILO)
    {
        image_choosegameroom_icon_->loadTexture("login_res/choosegameroom_res/image_choosegameroom_icon_holdem.png");
    }
    
    update_ui();
}

void NodeChooseGameRoomCell::handle_download_events(cmd_data_pointer)
{
    update_ui();
}

void NodeChooseGameRoomCell::update_ui()
{
    if (room_data_.has_groupinfo())
    {
            if(GDM->is_file_exist(room_data_.groupinfo().grouppic()))
            {
                sprite_icon_->initWithFile(room_data_.groupinfo().grouppic(), "Default/image_mask_circle.png");
                sprite_icon_->setScale(image_group_icon_->getContentSize().width / sprite_icon_->getContentSize().width, image_group_icon_->getContentSize().height / sprite_icon_->getContentSize().height);
                //sprite_icon_->setZOrder(ZORDER_PLAYER_UI+1);
                sprite_icon_->setPosition(image_group_icon_->getPosition());
//                sprite_icon_->setVisible(true);
//                image_group_icon_->setVisible(false);
            }else{
//                sprite_icon_->setVisible(false);
//                image_group_icon_->setVisible(true);
                FM.download_file(room_data_.groupinfo().grouppic());
                dispatch_.register_event(EVENT_DOWLOAD_FILE, BIND_FUNC(this, NodeChooseGameRoomCell::handle_download_events));
            }
        
    }else
    {
//        sprite_icon_->setVisible(false);
//        image_group_icon_->setVisible(true);
    }
    
    
}