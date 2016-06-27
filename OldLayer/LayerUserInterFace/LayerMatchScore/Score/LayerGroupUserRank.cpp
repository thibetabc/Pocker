//
//  LayerGroupUserRank.cpp
//  pokerclient
//
//  Created by ll on 16/2/17.
//
//

#include "LayerGroupUserRank.h"
#include "GameDataManager.h"
#include "PokerDataManager.h"
#include "RoomTitle.h"
#include "GameNameCell.h"

//#include "NodeScoreCellOld.h"

bool LayerGroupUserRank::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    
    auto size = Director::getInstance()->getWinSize();
    
    size = Size(std::min(size.width, size.height), std::max(size.width, size.height));
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    //layout->addTouchCallback(std::bind(&RoomLayer::touch_image, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    //    this->addChild(layout, -9);
    
    root_ = CSLoader::createNode("LayerGroupUserRank.csb");
    //    this->addChild(root_);
    
    
    checkbox_game_status_ = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root_, "checkbox_game_status"));
    checkbox_game_status_->addEventListener(std::bind(&LayerGroupUserRank::click_checkbox_game_status, this, std::placeholders::_1));
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back->addClickEventListener(std::bind(&LayerGroupUserRank::click_btn_back, this, std::placeholders::_1));
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString("");
    
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    cocos2d::Size pSize = listview->getContentSize();
    
    tableview_ = TableView::create(this, pSize);
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    
    tableview_->touchBeganCallback_ = std::bind(&LayerGroupUserRank::touchBegan, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchMovedCallback_ = std::bind(&LayerGroupUserRank::touchMoved, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchEndedCallback_ = std::bind(&LayerGroupUserRank::touchEnded, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchCancelledCallback_ = std::bind(&LayerGroupUserRank::touchCancel, this, std::placeholders::_1, std::placeholders::_2);
    
    
    root_->addChild(tableview_);
    listview->removeFromParent();
    
    auto listview_type_name = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview_type_name"));
    room_title_ = RoomTitle::create();
    room_title_->setPosition(listview_type_name->getPosition());
    room_title_->set_click_item(std::bind(&LayerGroupUserRank::click_room_item, this, std::placeholders::_1));
    room_title_->setContentSize(listview_type_name->getContentSize());
    
    this->addChild(room_title_);
    listview_type_name->removeFromParent();
    
    register_events();
    //GDM->send_get_create_room_info();
    
    return true;
}

void LayerGroupUserRank::set_group_user_rank_status(uint32_t group_id, msg::RoomType current_room_type, bool is_tournament)
{
    group_id_ = group_id;
    checkbox_game_status_->setSelected(is_tournament);
    select_game_idx_ = current_room_type;
    
    update_room_type_list(nullptr);
}

void LayerGroupUserRank::click_room_item(msg::RoomType game_idx)
{
    if (game_idx == 0)
        return;
    select_game_idx_ = game_idx;
    match_objects_.clear();
    tableview_->reloadData();
    bool is_tournament = checkbox_game_status_->isSelected();
    
    GDM->send_get_group_user_rank(group_id_, select_game_idx_, is_tournament);
}

void LayerGroupUserRank::click_checkbox_game_status(Ref* sender)
{
    match_objects_.clear();
    tableview_->reloadData();
    
    
    update_room_type_list(nullptr);
}

void LayerGroupUserRank::click_btn_back(Ref* sender)
{
    this->removeFromParent();
}

TableViewCell* LayerGroupUserRank::tableCellAtIndex(TableView *table, ssize_t idx)
{
    NodeScoreCellOld* cell = NodeScoreCellOld::create();
    //    match_objects_[idx]->rank = idx+1;
    //    cell->update_data(match_objects_[idx]);
    
    return cell;
}

ssize_t LayerGroupUserRank::numberOfCellsInTableView(TableView *table)
{
    return match_objects_.size();
}
#pragma mark tableview delegate --
void LayerGroupUserRank::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

void LayerGroupUserRank::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    
}

void LayerGroupUserRank::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
}

#pragma mark touch
bool LayerGroupUserRank::touchBegan(Touch* touch, Event* event)
{
    return true;
}
void LayerGroupUserRank::touchMoved(Touch* touch, Event* event)
{
    
}
void LayerGroupUserRank::touchEnded(Touch* touch, Event* event)
{
    
}
void LayerGroupUserRank::touchCancel(Touch* touch, Event* event)
{
    
}

void LayerGroupUserRank::update_room_type_list(cmd_data_pointer)
{
    auto& room_info = GDM->get_room_infomations();
    room_title_->clear_all_items();
    
    GameNameCell* cell_front = GameNameCell::create();
    cell_front->update_data();
    room_title_->push_back_item(cell_front);
    bool select = false;
    uint32_t idx = 0;
    msg::RoomType new_room_type_idx;
    bool is_find_old_type = false;
    for (auto& iter : room_info.createroominfo()) {
        if (checkbox_game_status_->isSelected() && !iter.supporttournament())
        {
            continue;
        }
        GameNameCell* cell = GameNameCell::create();
        
        cell->update_data(iter.roomtype().roomtype(),iter.roomtype().roomtypename());
        if(idx == 0 || idx==1)
        {
            cell->set_selected(true);
            select = true;
            new_room_type_idx = iter.roomtype().roomtype();
        }
        if (select_game_idx_ == iter.roomtype().roomtype())
            is_find_old_type = true;
        
        room_title_->push_back_item(cell);
        idx++;
    }
    GameNameCell* cell_back = GameNameCell::create();
    cell_back->update_data();
    room_title_->push_back_item(cell_back);
    if (!is_find_old_type)
        select_game_idx_ = new_room_type_idx;
    
    room_title_->click_item(select_game_idx_);
}


void LayerGroupUserRank::handle_get_group_user_rank(cmd_data_pointer)
{
    match_objects_.clear();
    
    group_id_ = group_user_rank_->groupid();
    std::string group_name = group_user_rank_->groupname();
    text_title_->setString(group_name);
    
    for (auto& it: group_user_rank_->playerscores().score()) {
        std::string nickname = it.userdetailinfo().nickname();
        std::string head_pic_name = it.userdetailinfo().picname();
        uint32_t user_id=it.userdetailinfo().userid();
        int32_t win_amount = it.totalwin();
        std::shared_ptr<MATCH_OBJECT> obj(new MATCH_OBJECT());
        obj->is_group_user_rank = true;
        obj->nickname = nickname;
        obj->head_pic_name = head_pic_name;
        obj->user_id = user_id;
        obj->room_type = group_user_rank_->roomtype();
        obj->win_amount = win_amount;
        
        match_objects_.push_back(obj);
    }
    
    auto sortfuncitonWin = [](std::shared_ptr<MATCH_OBJECT> a, std::shared_ptr<MATCH_OBJECT> b){return a->win_amount > b->win_amount;};
    std::sort(match_objects_.begin(), match_objects_.end(), sortfuncitonWin);// 按照赢筹码排序
    
    tableview_->reloadData();
}

void LayerGroupUserRank::refresh_data()
{
    
}

void LayerGroupUserRank::register_events()
{
    //    dispatch_.register_event(EVENT_HANDLE_ROOM_INFO_FINISHED,
    //                             BIND_FUNC(this, LayerUserMatchInfo::update_room_type_list));
    dispatch_.register_event(EVENT_HANDLE_GROUP_USER_RANK,
                             BIND_FUNC(this, LayerGroupUserRank::handle_get_group_user_rank));
}