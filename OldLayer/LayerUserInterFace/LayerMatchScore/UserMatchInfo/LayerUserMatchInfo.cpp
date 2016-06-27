//
//  LayerUserMatchInfo.cpp
//  pokerclient
//
//  Created by ll on 15/11/27.
//
//

#include "LayerUserMatchInfo.h"
#include "GameDataManager.h"
#include "PokerDataManager.h"
#include "RoomTitle.h"
#include "GameNameCell.h"

#include "NodeUserMatchInfoCell.h"
#include "NodeMatchInfoLabelCell.h"

#include "LayerScoreOld.h"
#include "LayerTourmentScore.h"
#include "LayerGroupUserRank.h"

bool LayerUserMatchInfo::init()
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
    this->addChild(layout, -9);
    
    root_ = CSLoader::createNode("LayerUserMatchInfo.csb");
    this->addChild(root_);
    
    
    checkbox_game_status_ = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root_, "checkbox_game_status"));
    checkbox_game_status_->addEventListener(std::bind(&LayerUserMatchInfo::click_checkbox_game_status, this, std::placeholders::_1));
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back->addClickEventListener(std::bind(&LayerUserMatchInfo::click_btn_back, this, std::placeholders::_1));
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString("");
    
    text_user_rank_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_user_rank"));
    text_user_rank_->setVisible(false);
    
    text_mark_name_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_name_1"));
    text_mark_name_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_name_2"));
    text_mark_name_3_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_name_3"));
    
    text_mark_num_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_num_1"));
    text_mark_num_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_num_2"));
    text_mark_num_3_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_num_3"));
    
    text_mark_name_1_->setString("");
    text_mark_name_2_->setString("");
    text_mark_name_3_->setString("");
    text_mark_num_1_->setString("");
    text_mark_num_2_->setString("");
    text_mark_num_3_->setString("");
    
    text_mark_name_1_->setVisible(false);
    text_mark_name_2_->setVisible(false);
    text_mark_name_3_->setVisible(false);
    text_mark_num_1_->setVisible(false);
    text_mark_num_2_->setVisible(false);
    text_mark_num_3_->setVisible(false);
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    cocos2d::Size pSize = listview->getContentSize();
    
    tableview_ = TableView::create(this, pSize);
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::BOTTOM_UP);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    tableview_->touchBeganCallback_ = std::bind(&LayerUserMatchInfo::touchBegan, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchMovedCallback_ = std::bind(&LayerUserMatchInfo::touchMoved, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchEndedCallback_ = std::bind(&LayerUserMatchInfo::touchEnded, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchCancelledCallback_ = std::bind(&LayerUserMatchInfo::touchCancel, this, std::placeholders::_1, std::placeholders::_2);
    root_->addChild(tableview_);
    listview->removeFromParent();
    
    auto listview_type_name = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview_type_name"));
    room_title_ = RoomTitle::create();
    room_title_->setPosition(listview_type_name->getPosition());
    room_title_->set_click_item(std::bind(&LayerUserMatchInfo::click_room_item, this, std::placeholders::_1));
    room_title_->setContentSize(listview_type_name->getContentSize());
    
    this->addChild(room_title_);
    listview_type_name->removeFromParent();
    
    listview_label_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview_label"));
    
    text_user_rank_->addClickEventListener(std::bind(&LayerUserMatchInfo::click_text_user_rank, this, std::placeholders::_1));
    
    is_group_match_ = false;
    
    select_game_idx_ = msg::RoomType::NLHOLDEM;
    
    register_events();
    GDM->send_get_create_room_info();
    
    refresh_times_ = 0;
    
    return true;
}

bool LayerUserMatchInfo::set_is_group_match(bool is_group_match, uint32_t group_id)
{
    is_group_match_ = is_group_match;
    if (is_group_match_)
    {
        text_title_->setString(tools::local_string("group_result", "群战绩"));
        group_id_ = group_id;
        
        text_mark_name_1_->setVisible(false);
        text_mark_name_2_->setVisible(false);
        text_mark_name_3_->setVisible(false);
        
        text_mark_num_1_->setVisible(false);
        text_mark_num_2_->setVisible(false);
        text_mark_num_3_->setVisible(false);
        
        text_user_rank_->setVisible(true);
    }else
    {
        text_title_->setString(tools::local_string("my_result","个人战绩"));
        text_user_rank_->setVisible(false);
        
        group_id = 0;
        
        if (checkbox_game_status_->isSelected())
        {
            text_mark_name_1_->setVisible(true);
            text_mark_name_2_->setVisible(false);
            text_mark_name_3_->setVisible(true);
            
            text_mark_num_1_->setVisible(true);
            text_mark_num_2_->setVisible(false);
            text_mark_num_3_->setVisible(true);
            
            text_mark_name_1_->setString(tools::local_string("total_profit", "总成绩"));
            text_mark_name_3_->setString(tools::local_string("average_profit", "平均场次成绩"));
            
            text_mark_num_1_->setString(tools::local_string("NA", "暂无"));
            text_mark_num_3_->setString(tools::local_string("NA", "暂无"));
        }else
        {
            text_mark_name_1_->setVisible(true);
            text_mark_name_2_->setVisible(true);
            text_mark_name_3_->setVisible(true);
            
            text_mark_num_1_->setVisible(true);
            text_mark_num_2_->setVisible(true);
            text_mark_num_3_->setVisible(true);
            
            text_mark_name_1_->setString(tools::local_string("total_profit", "总成绩"));
            text_mark_name_2_->setString(tools::local_string("per_hour_profit", "每小时成绩"));
            text_mark_name_3_->setString(tools::local_string("per_100_profit", "每100手成绩"));
            
            text_mark_num_1_->setString(tools::local_string("NA", "暂无"));
            text_mark_num_2_->setString(tools::local_string("NA", "暂无"));
            text_mark_num_3_->setString(tools::local_string("NA", "暂无"));
        }
    }
}

void LayerUserMatchInfo::click_room_item(msg::RoomType game_idx)
{
    if (game_idx == 0)
        return;
    select_game_idx_ = game_idx;
    user_score_datas_.clear();
    tableview_->reloadData();
    bool is_tournament = checkbox_game_status_->isSelected();
    if (is_group_match_)
        GDM->send_get_history_score(select_game_idx_, is_tournament, group_id_);
    else
        GDM->send_get_history_score(select_game_idx_, is_tournament);
}

void LayerUserMatchInfo::click_checkbox_game_status(Ref* sender)
{
    user_score_datas_.clear();
    check_labels_.clear();
    tableview_->reloadData();
    if (is_group_match_)
    {
        
        text_mark_name_1_->setVisible(false);
        text_mark_name_2_->setVisible(false);
        text_mark_name_3_->setVisible(false);
        
        text_mark_num_1_->setVisible(false);
        text_mark_num_2_->setVisible(false);
        text_mark_num_3_->setVisible(false);
        
    }else
    {
        if (checkbox_game_status_->isSelected())
        {
            text_mark_name_1_->setVisible(true);
            text_mark_name_2_->setVisible(false);
            text_mark_name_3_->setVisible(true);
            
            text_mark_num_1_->setVisible(true);
            text_mark_num_2_->setVisible(false);
            text_mark_num_3_->setVisible(true);
            
            text_mark_name_1_->setString(tools::local_string("total_profit", "总成绩"));
            text_mark_name_3_->setString(tools::local_string("average_game_profit", "平均场次盈利"));
            
            text_mark_num_1_->setString(tools::local_string("NA", "暂无"));
            text_mark_num_3_->setString(tools::local_string("NA", "暂无"));
        }else
        {
            text_mark_name_1_->setVisible(true);
            text_mark_name_2_->setVisible(true);
            text_mark_name_3_->setVisible(true);
            
            text_mark_num_1_->setVisible(true);
            text_mark_num_2_->setVisible(true);
            text_mark_num_3_->setVisible(true);
            
            text_mark_name_1_->setString(tools::local_string("total_profit", "总盈利"));
            text_mark_name_2_->setString(tools::local_string("per_hour_profit", "每小时盈利"));
            text_mark_name_3_->setString(tools::local_string("per_100_profit", "每100手盈利"));
            
            text_mark_num_1_->setString(tools::local_string("NA", "暂无"));
            text_mark_num_2_->setString(tools::local_string("NA", "暂无"));
            text_mark_num_3_->setString(tools::local_string("NA", "暂无"));
        }
    }
    
    update_room_type_list(nullptr);
    //click_room_item(select_game_idx_);
    //
    //    bool is_tournament = checkbox_game_status_->getSelectedState();
    //    GDM->send_get_history_score(select_game_idx_, is_tournament);
}

void LayerUserMatchInfo::click_label(std::string label_text)
{
    bool is_find_label = false;
    for (int i = 0; i < check_labels_.size(); i++)
    {
        if (check_labels_[i] == label_text)
        {
            is_find_label = true;
            check_labels_.erase(check_labels_.begin()+i);
            break;
        }
    }
    if (!is_find_label)
        check_labels_.push_back(label_text);
    
    refresh_data();
    tableview_->reloadData();
    
}

void LayerUserMatchInfo::click_btn_back(Ref* sender)
{
    this->removeFromParent();
}

void LayerUserMatchInfo::click_text_user_rank(Ref * sender)
{
    LayerGroupUserRank* lgur = LayerGroupUserRank::create();
    bool is_tournament = checkbox_game_status_->isSelected();
    lgur->set_group_user_rank_status(group_id_, select_game_idx_, is_tournament);
    this->getParent()->addChild(lgur, 2);
}

TableViewCell* LayerUserMatchInfo::tableCellAtIndex(TableView *table, ssize_t idx)
{
    NodeUserMatchInfoCell* cell = dynamic_cast<NodeUserMatchInfoCell*>(table->dequeueCell());
    if(!cell) {
        cell = NodeUserMatchInfoCell::create();
    }
    
    auto& data = user_score_datas_.at(idx);
    cell->update_data(data);
    
    if (idx == 0)
        cell->set_date_visiable(true);
    else
    {
        auto & data2 = user_score_datas_.at(idx-1);
        
        std::string submit_time1 = data->endtime();
        std::string submit_time2 = data2->endtime();
        if(!submit_time1.empty()) {
            double time1 = tools::stod(submit_time1);
            double time2 = tools::stod(submit_time2);
            time1 /= 1000;
            time2 /= 1000;
            bool date_is_visiable = tools::time_is_same_day(time1, time2);
            cell->set_date_visiable(!date_is_visiable);
        }else
        {
            cell->set_date_visiable(false);
        }
    }
    
    cell->set_click_label(std::function<void(std::string)>([=](std::string label_word){click_label(label_word);}));
    
    cell->set_is_group_match(is_group_match_);
    
    last_cell_idx_ = idx;
    
    return cell;
}

ssize_t LayerUserMatchInfo::numberOfCellsInTableView(TableView *table)
{
    return user_score_datas_.size();
}
#pragma mark tableview delegate --
void LayerUserMatchInfo::tableCellTouched(TableView* table, TableViewCell* cell)
{
    NodeUserMatchInfoCell* table_cell = dynamic_cast<NodeUserMatchInfoCell*>(cell);
    int32_t roomID = table_cell->get_match_cell_data().lock()->roomid();
    PDM->send_get_score(roomID);
}

void LayerUserMatchInfo::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    NodeUserMatchInfoCell* message_cell = dynamic_cast<NodeUserMatchInfoCell*>(cell);
    message_cell->set_is_touch(true);
}

void LayerUserMatchInfo::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    NodeUserMatchInfoCell* message_cell = dynamic_cast<NodeUserMatchInfoCell*>(cell);
    message_cell->set_is_touch(false);
}

#pragma mark touch
bool LayerUserMatchInfo::touchBegan(Touch* touch, Event* event)
{
    //cocos2d::log("LayerUserMatchInfo::touchBegan");
    if (last_cell_idx_ != 0)
        return false;
    return true;
}
void LayerUserMatchInfo::touchMoved(Touch* touch, Event* event)
{
    //cocos2d::log("LayerUserMatchInfo::touchMoved");
    
}
void LayerUserMatchInfo::touchEnded(Touch* touch, Event* event)
{
    //cocos2d::log("LayerUserMatchInfo::touchEnded");
    //by lvzhangle -20160217
    //一次加载所有数据，不再下拉分段请求
    //    Point newtouchLocation = touch->getStartLocation();
    //    Point oldtouchLocation = touch->getLocation();
    //
    //    cocos2d::log("newtouchLocation %f, %f", newtouchLocation.x, newtouchLocation.y);
    //    cocos2d::log("oldtouchLocation %f, %f", oldtouchLocation.x, oldtouchLocation.y);
    //    cocos2d::log("last_cell_idx_ : %u", last_cell_idx_);
    //    if ((oldtouchLocation.y - newtouchLocation.y > 200) &&
    //        (last_cell_idx_ == 0))
    //    {
    //       // cocos2d::log("触发");
    //        refresh_times_ += 1;
    //
    //        long endtime = tools::date();
    //        long starttime = endtime - 30*24*60*60 - (refresh_times_ * 7 * 24 * 60 * 60);
    //
    //        bool is_tournament = checkbox_game_status_->isSelected();
    //        if (is_group_match_)
    //            GDM->send_get_history_score(select_game_idx_, is_tournament, group_id_, tools::to_string(starttime), tools::to_string(endtime));
    //        else
    //            GDM->send_get_history_score(select_game_idx_, is_tournament, 0, tools::to_string(starttime), tools::to_string(endtime));
    //    }
}
void LayerUserMatchInfo::touchCancel(Touch* touch, Event* event)
{
    // cocos2d::log("LayerUserMatchInfo::touchCancel");
}

void LayerUserMatchInfo::update_room_type_list(cmd_data_pointer)
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
        
        if(idx == 0 || idx == 1)
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


void LayerUserMatchInfo::handle_receive_get_history_score(cmd_data_pointer)
{
    //historyscore_ = GDM->get_player_history_score();
    check_labels_.clear();
    refresh_data();
    tableview_->reloadData();
}

void LayerUserMatchInfo::refresh_data()
{
    if (historyscore_ == nullptr)
    {
        cocos2d::log("Error : LayerUserMatchInfo refresh_data historyscore_ = nullptr");
        return;
    }
    user_score_datas_.clear();
    
    float allscore = 0;
    float allscore_time = 0;
    float allscore_hands = 0;
    float alltime = 0;
    float allhands = 0;
    for (int i = 0; i < historyscore_->score().size(); i++)
    {
        bool is_label_find = true;
        
        for (int j = 0; j < check_labels_.size(); j++)
        {
            bool is_check_label_find = false;
            std::string label1 = check_labels_[j];
            for (int k = 0; k < historyscore_->score(i).label().size(); k++)
            {
                std::string label2 = historyscore_->score(i).label(k);
                if (label1 == label2)
                {
                    is_check_label_find = true;
                    break;
                }
            }
            if (!is_check_label_find)
            {
                is_label_find = false;
                break;
            }
        }
        if (is_label_find)
        {
            std::shared_ptr<msg::HistoryScore> score_data(new msg::HistoryScore);
            *score_data = historyscore_->score(i);
            user_score_datas_.push_back(score_data);
            
            if (score_data->playtime() != 0)
            {
                allscore_time += score_data->score();
                alltime += (1.0 * score_data->playtime() / (1000 * 60 * 60));
            }
            if (score_data->playhands() != 0)
            {
                allscore_hands += score_data->score();
                allhands += score_data->playhands();
            }
            
            allscore += score_data->score();
            //            alltime += (1.0 * score_data->playtime() / (1000 * 60 * 60));
            //            allhands += score_data->playhands();
        }
    }
    
    
    listview_label_->removeAllItems();
    for (auto iter : check_labels_) {
        NodeMatchInfoLabelCell* cell = NodeMatchInfoLabelCell::create();
        cell->setAnchorPoint(Vec2(0, 0.5));
        cell->update_data(iter);
        cell->set_click_label(std::function<void(std::string)>([=](std::string label_word){click_label(label_word);}));
        listview_label_->pushBackCustomItem(cell);
    }
    
    if (!is_group_match_)
    {
        if (checkbox_game_status_->isSelected())
        {
            text_mark_name_1_->setVisible(true);
            text_mark_name_2_->setVisible(false);
            text_mark_name_3_->setVisible(true);
            
            text_mark_num_1_->setVisible(true);
            text_mark_num_2_->setVisible(false);
            text_mark_num_3_->setVisible(true);
            
            text_mark_name_1_->setString(tools::local_string("total_profit", "总成绩"));
            text_mark_name_3_->setString(tools::local_string("average_profit", "平均场次成绩"));
            
            if (user_score_datas_.size() != 0)
            {
                text_mark_num_1_->setString(tools::to_string(allscore));
                int32_t game_size = user_score_datas_.size();
                text_mark_num_3_->setString(tools::to_string(allscore / game_size));
            }else
            {
                text_mark_num_1_->setString(tools::local_string("NA", "暂无"));
                text_mark_num_3_->setString(tools::local_string("NA", "暂无"));
            }
        }else
        {
            text_mark_name_1_->setVisible(true);
            text_mark_name_2_->setVisible(true);
            text_mark_name_3_->setVisible(true);
            
            text_mark_num_1_->setVisible(true);
            text_mark_num_2_->setVisible(true);
            text_mark_num_3_->setVisible(true);
            
            text_mark_name_1_->setString(tools::local_string("total_profit", "总盈利"));
            text_mark_name_2_->setString(tools::local_string("per_hour_profit", "每小时盈利"));
            text_mark_name_3_->setString(tools::local_string("per_100_profit", "每100手盈利"));
            
            if (user_score_datas_.size() != 0)
            {
                text_mark_num_1_->setString(tools::to_string(allscore));
                
                if (alltime != 0)
                {
                    char str_per_hour_profit[64] = {0};
                    sprintf(str_per_hour_profit, "%.2f", allscore_time/ alltime);
                    
                    text_mark_num_2_->setString(str_per_hour_profit);
                }else
                {
                    text_mark_num_2_->setString(tools::local_string("NA", "暂无"));
                }
                
                if (allhands == 0)
                {
                    text_mark_num_3_->setString(tools::local_string("NA", "暂无"));
                }else
                {
                    char str_per_100_profit[64];
                    sprintf(str_per_100_profit, "%.2f", allscore_hands / allhands * 100);
                    
                    text_mark_num_3_->setString(str_per_100_profit);
                }
            }else
            {
                text_mark_num_1_->setString(tools::local_string("NA", "暂无"));
                text_mark_num_2_->setString(tools::local_string("NA", "暂无"));
                text_mark_num_3_->setString(tools::local_string("NA", "暂无"));
            }
        }
    }else
    {
        text_mark_name_1_->setVisible(false);
        text_mark_name_2_->setVisible(false);
        text_mark_name_3_->setVisible(false);
        
        text_mark_num_1_->setVisible(false);
        text_mark_num_2_->setVisible(false);
        text_mark_num_3_->setVisible(false);
    }
}

void LayerUserMatchInfo::register_events()
{
    dispatch_.register_event(EVENT_HANDLE_ROOM_INFO_FINISHED,
                             BIND_FUNC(this, LayerUserMatchInfo::update_room_type_list));
    dispatch_.register_event(EVENT_HANDLE_PLAYER_HISTORY_SCORE,
                             BIND_FUNC(this, LayerUserMatchInfo::handle_receive_get_history_score));
}

