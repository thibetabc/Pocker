//
//  LayerChooseGameRoom.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/28.
//
//

#include "LayerChooseGameRoom.h"
#include "NET_CMD.h"
#include "NodeChooseGameRoomCell.h"
#include "PokerDataManager.h"
#include "utils.h"
#include "ProcessorManager.h"

#include "LayerCreateGameRoom.h"

LayerChooseGameRoom* LayerChooseGameRoom::create(msg::RoomType room_type, bool is_tournament, const msg::GroupInfo& groupinfo)
{
    LayerChooseGameRoom* result = new LayerChooseGameRoom;
    result->set_groupinfo(groupinfo);
    if(result->init(room_type, is_tournament)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

LayerChooseGameRoom* LayerChooseGameRoom::create(msg::RoomType room_type, bool is_tournament)
{
    LayerChooseGameRoom* result = new LayerChooseGameRoom;
    msg::GroupInfo nullgroup;
    nullgroup.set_groupid(0);
    result->set_groupinfo(nullgroup);
    if(result->init(room_type, is_tournament)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

bool LayerChooseGameRoom::init(msg::RoomType room_type, bool is_tournament)
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    room_type_ = room_type;
    is_tournament_ = is_tournament;
    
    root_ = CSLoader::createNode("LayerChooseGameRoom.csb");
    this->addChild(root_);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("choose_room","选择房间"));
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerChooseGameRoom::click_btn_back, this, std::placeholders::_1));
    
    btn_create_room_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_create_room"));
    btn_create_room_->addClickEventListener(std::bind(&LayerChooseGameRoom::click_btn_create_room, this, std::placeholders::_1));
    
    image_layer_choosegameroom_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_layer_choosegameroom_bg"));
    
    image_choosegameroom_search_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_choosegameroom_search_bg"));
    
    image_choosegameroom_search_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_choosegameroom_search_icon"));
    
    tf_choosegameroom_search_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_choosegameroom_search"));
    
    tf_choosegameroom_search_->removeFromParent();
    image_choosegameroom_search_bg_->removeFromParent();
    image_choosegameroom_search_icon_->removeFromParent();
    
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    tableview_ = TableView::create(this, listview_->getContentSize());
    tableview_->setAnchorPoint(listview_->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview_->getPosition());
    tableview_->setDelegate(this);
    
    tableview_->setPosition(cocos2d::Point(tableview_->getPositionX(), tableview_->getPositionY() - 180));
    tableview_->setContentSize(cocos2d::Size(tableview_->getContentSize().width, tableview_->getContentSize().height + 180));
    
    root_->addChild(tableview_);
    listview_->removeFromParent();
    
    if (select_groupinfo_.groupid() == 0)
    {
        btn_create_room_->setVisible(true);
    }else
    {
        btn_create_room_->setVisible(select_groupinfo_.allowcreateroom());
    }
    
    send_get_room_info();
    
    return true;
}

void LayerChooseGameRoom::set_groupinfo(const msg::GroupInfo& groupinfo)
{
    select_groupinfo_.CopyFrom(groupinfo);
}

void LayerChooseGameRoom::send_get_room_info()
{
    auto processor = PM->GetRoomInfo_UP(select_groupinfo_.groupid());
    send_data(processor, REQ_GET_ROOM_INFO);
}

void LayerChooseGameRoom::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}

void LayerChooseGameRoom::click_btn_create_room(Ref * sender)
{
    msg::RoomType select_type;
    if (room_type_ == msg::RoomType::ALL)
    {
        select_type = GDM->get_last_game_room_type();
    }else
    {
        select_type = room_type_;
    }
    LayerCreateGameRoom* lcgr = LayerCreateGameRoom::create(select_type, is_tournament_, select_groupinfo_);
    lcgr->set_dismiss_callback([=](){lcgr->removeFromParent();});
    this->addChild(lcgr);
}

#pragma mark tableview datasource --
TableViewCell* LayerChooseGameRoom::tableCellAtIndex(TableView *table, ssize_t idx)
{
    NodeChooseGameRoomCell * cell = dynamic_cast<NodeChooseGameRoomCell*>(table->dequeueCell());
    if (!cell)
    {
        cell = NodeChooseGameRoomCell::create();
    }
    cell->update_data(room_info_datas_[idx]);
    
    return cell;
}

void LayerChooseGameRoom::register_events()
{
    dispatch_.register_event(REQ_GET_ROOM_INFO,
                             BIND_FUNC(this, LayerChooseGameRoom::handle_get_room_info));
}

void LayerChooseGameRoom::RefreshLayer()
{
    send_get_room_info();
}

void LayerChooseGameRoom::handle_get_room_info(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    msg::Processor_150_GetRoomInfo_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS: {
            room_info_datas_.clear();
            for (auto iter: processor.roominfo()){
                if ((iter.groupinfo().groupid() == select_groupinfo_.groupid() || select_groupinfo_.groupid() == 0) &&
                    
                    (room_type_ == msg::RoomType::ALL || (iter.roomtype().roomtype() == room_type_ &&
                                                          
                      iter.istournament() == is_tournament_)))
                {
                    room_info_datas_.push_back(iter);
                }
            }
            
            tableview_->reloadData();
            if (room_info_datas_.size()==0)
            {
                ShowTipMessage(true);
            }
            else
            {
                ShowTipMessage(false);
            }
            break;
        }
        case msg::Status::FAILED: {
            ShowError(tools::local_string("get_room_list_failed", "获取房间列表失败"));
            break;
        }
        default:
            break;
    }
}

Size LayerChooseGameRoom::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    
    return cocos2d::Size(750, 232);
}

ssize_t LayerChooseGameRoom::numberOfCellsInTableView(TableView *table)
{
    return room_info_datas_.size();
}

#pragma mark tableview delegate --
void LayerChooseGameRoom::tableCellTouched(TableView* table, TableViewCell* cell)
{
    NodeChooseGameRoomCell* gameRoomCell = dynamic_cast<NodeChooseGameRoomCell*>(cell);
    msg::RoomInfo room_data = gameRoomCell->get_data();
    if (room_data.istournament())
    {
        if (room_data.roomstatus() == msg::RoomStatus::WAITINGTOSTART)
        {
            PDM->send_enter_room(room_data.roomid());
        }else
        {
            
        }
    }else
    {
        
        PDM->send_enter_room(room_data.roomid());
        
    }
}

