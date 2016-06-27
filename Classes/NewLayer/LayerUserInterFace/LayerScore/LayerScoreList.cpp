//
//  LayerSelfTrackRecord.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/11.
//
//

#include "LayerScoreList.h"
#include "NodeScoreCell.h"
#include "ProcessorManager.h"
#include "utils.h"
#include "NodeCircleChoose.h"
#include "time_utils.h"
#include "LayerScore.h"
#include "NET_CMD.h"
#include "LayerPlayerRank.h"
#define NODE_SCORE_CELL_HIGHT 208
#define NODE_SCORE_CELL_WIDTH 564

LayerScoreList* LayerScoreList::create(bool isGroupMatch,int groupId,std::string groupName)
{
    LayerScoreList* score=new LayerScoreList;
    if(score->init(isGroupMatch,groupId,groupName))
    {
        score->autorelease();
        return score;
    }
    return nullptr;
}


bool LayerScoreList::init(bool isGroupMatch,int groupId,std::string groupName)
{
    if (!BaseLayer::init())
    {
        return false;
    }
    root_=CSLoader::createNode("LayerScore.csb");
    this->addChild(root_);
    is_group_match_=isGroupMatch;
    group_name_=groupName;
    group_id_=groupId;
    isChipChoose_=false;
    isGroupChoose_=false;
    isWeekChoose_=false;
    isMonthChoose_=false;
    isAllChoose_=false;
    text_title_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    if (!isGroupMatch)
    {
        text_title_->setString(tools::local_string("my_result", "个人战绩"));
    }
    btn_back_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerScoreList::click_btn_back,this,std::placeholders::_1));
    
    
    node_select_type_=dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_select_type"));
    select_game_=NodeSelectGameType::create();
    node_select_type_->addChild(select_game_);
    select_game_->refresh_data(msg::RoomType::NLHOLDEM);
    select_game_->set_click_callback(std::bind(&LayerScoreList::click_btn_choose_gametype,this));
    
    text_current_month_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_current_month"));
    text_current_month_->setString(tools::local_string("Current_month_scores", "30天之内"));
    
    text_current_week_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_current_week"));
    text_current_week_->setString(tools::local_string("Current_week_scores", "7天之内"));
    
    text_all_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_all"));
    text_all_->setString(tools::local_string("all_scores","所有"));
    
    node_month_btn_=dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_month_btn"));
    choose_month_=NodeCircleChoose::create();
    node_month_btn_->addChild(choose_month_);
    choose_month_->set_click_callback(std::bind(&LayerScoreList::click_select_month, this,std::placeholders::_1));
    
    node_week_btn_=dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_week_btn"));
    choose_week_=NodeCircleChoose::create();
    node_week_btn_->addChild(choose_week_);
    choose_week_->set_click_callback(std::bind(&LayerScoreList::click_select_week, this,std::placeholders::_1));
    
    node_all_=dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_all"));
    choose_all_=NodeCircleChoose::create();
    node_all_->addChild(choose_all_);
    choose_all_->set_click_callback(std::bind(&LayerScoreList::click_select_all, this,std::placeholders::_1));
    
    
    text_choose_type_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_choose_type"));
    text_choose_type_->setString("");
    text_choose_type_->addClickEventListener(std::bind(&LayerScoreList::click_text_select_type, this,std::placeholders::_1));
    text_choose_type_->setTouchEnabled(true);
    
    text_choose_type_second_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_choose_type_second"));
    text_choose_type_second_->setString("");
    text_choose_type_second_->addClickEventListener(std::bind(&LayerScoreList::click_text_select_type_second, this,std::placeholders::_1));
    text_choose_type_second_->setTouchEnabled(true);
    
    
    text_hour_score_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_hour_score"));
    text_hour_score_->setString(tools::local_string("per_hour_profit","每小时盈利"));
    
    text_whole_score_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_whole_score"));
    text_whole_score_->setString(tools::local_string("total_profit","总盈利"));
    
    text_times_score_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_times_score"));
    text_times_score_->setString(tools::local_string("per_100_profit","每100手盈利"));
    
    text_times_score_num_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_times_score_num"));
    text_times_score_num_->setString(tools::local_string("NA","暂无"));
    
    text_whole_score_num_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_whole_score_num"));
    text_whole_score_num_->setString(tools::local_string("NA","暂无"));
    
    text_hour_score_num_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_hour_score_num"));
    text_hour_score_num_->setString(tools::local_string("NA","暂无"));
    
    
    
    panel_score_group_=dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "panle_score_group"));
    btn_player_rank_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_player_rank"));
    btn_player_rank_->setVisible(false);
    btn_player_rank_->setTouchEnabled(false);
    btn_player_rank_->addClickEventListener(std::bind(&LayerScoreList::click_btn_player_rank,this,std::placeholders::_1));
    if(isGroupMatch)
    {
        panel_score_group_->setVisible(false);
        btn_player_rank_->setVisible(true);
        btn_player_rank_->setTouchEnabled(true);
        text_title_->setString(group_name_);
    }
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview_show_self_track"));
    tableview_ = TableView::create(this, listview_->getContentSize());
    tableview_->setAnchorPoint(listview_->getAnchorPoint());
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview_->getPosition());
    tableview_->setDelegate(this);
    
    root_->addChild(tableview_);
    listview_->removeFromParent();

    current_room_type_=msg::RoomType::NLHOLDEM;
    is_current_romm_tournament_=false;
    endtime_ = tools::date();
    long time=7*24*60*60;
    startTime_=endtime_-time;
    isWeekChoose_=true;
    choose_week_->set_node_unchoose_status(true);
    groupid_ = groupId;
    send_get_history_score();
    return true;
}

void LayerScoreList::send_get_history_score()
{
    auto processor= PM->GetHistoryScore_Up(current_room_type_,is_current_romm_tournament_, startTime_ == 0 ? "" : tools::to_string(startTime_), endtime_ == 0 ? "" : tools::to_string(endtime_), groupid_);
    send_data(processor, REQ_PLYAER_HISTORY_SOCRE);
}

void LayerScoreList::register_events()
{
    dispatch_.register_event(REQ_PLYAER_HISTORY_SOCRE,
                             BIND_FUNC(this, LayerScoreList::handle_get_user_match));
}

void LayerScoreList::click_btn_back(cocos2d::Ref *sender)
{
    this->removeFromParent();
}

void LayerScoreList::click_btn_choose_gametype()
{
    LayerChooseGameType* choose=LayerChooseGameType::create();
    choose->set_choose_call_func(std::bind(&LayerScoreList::update_tableview_data, this, std::placeholders::_1, std::placeholders::_2));
    addChild(choose,0,"LayerChooseGameType");
}
void LayerScoreList::click_btn_player_rank(cocos2d::Ref *sender)
{
    LayerPlayerRank* rank=LayerPlayerRank::create(group_name_, current_room_type_, group_id_,is_current_romm_tournament_);
    this->addChild(rank, 0, "LayerPlayerRank");
}
void LayerScoreList::update_tableview_data(msg::RoomType room_type,bool istournament)
{
    removeChildByName("LayerChooseGameType");
    current_room_type_=room_type;
    is_current_romm_tournament_=istournament;
    select_game_->refresh_data(current_room_type_);
    endtime_ = tools::date();
    long time=7*24*60*60;
    startTime_=endtime_-time;
    isWeekChoose_=true;
    isMonthChoose_=false;
    isAllChoose_=false;
    choose_chip_num="";
    choose_group_name_="";
    isGroupChoose_=false;
    isChipChoose_=false;
    text_choose_type_->setString("");
    text_choose_type_second_->setString("");
    choose_all_->set_node_unchoose_status();
    choose_month_->set_node_unchoose_status();
    choose_week_->set_node_unchoose_status(true);
    send_get_history_score();
}


void LayerScoreList::handle_get_user_match(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec))
    {
        return;
    }
    auto processor=PM->GetHistoryScore_Down(rec);
    if (is_current_romm_tournament_)
    {
        text_hour_score_->setString("");
        text_times_score_->setString(tools::local_string("average_game_profit","平均场次盈利"));
    }
    blind_cell_datas_.clear();
    blind_cell_datas_copy_.clear();
    this->reset_score();
    for(int i=processor.score().size()-1;i>=0;i--)
    {
        if (processor.score(i).playtime()!= 0)
        {
            allscore_time += processor.score(i).score();
            alltime += (1.0 * processor.score(i).playtime() / (1000 * 60 * 60));
        }
        if (processor.score(i).playhands() != 0)
        {
            allscore_hands += processor.score(i).score();
            allhands += processor.score(i).playhands();
        }
        allscore += processor.score(i).score();
        msg::HistoryScore cellData;
        cellData.CopyFrom(processor.score(i));
        blind_cell_datas_.push_back(cellData);
        blind_cell_datas_copy_.push_back(cellData);
    }
    if(isChipChoose_)
    {
        this->getChipChoosedData(choose_chip_num);
    }
    if (isGroupChoose_)
    {
        this->getGroupChoosedData(choose_group_name_);
    }
    
    this->refresh_score_data();
    
    tableview_->reloadData();
}

void LayerScoreList::refresh_score_data()
{
    if (blind_cell_datas_.size()>0)
    {
        if (is_group_match_)
        {
            panel_score_group_->setVisible(false);
        }
        else
        {
            panel_score_group_->setVisible(true);
            if(is_current_romm_tournament_)
            {
                text_whole_score_num_->setString(tools::to_string(allscore));
                long game_size = blind_cell_datas_.size();
                text_times_score_num_->setString(tools::to_string(allscore / game_size));
                text_hour_score_num_->setString("");
            }
            else
            {
                text_whole_score_num_->setString(tools::to_string(allscore));
                if (alltime != 0)
                {
                    char str_per_hour_profit[64] = {0};
                    sprintf(str_per_hour_profit, "%.2f", allscore_time/ alltime);
                    
                    text_hour_score_num_->setString(str_per_hour_profit);
                }else
                {
                    text_hour_score_num_->setString(tools::local_string("NA", "暂无"));
                }
                
                if (allhands == 0)
                {
                    text_times_score_num_->setString(tools::local_string("NA", "暂无"));
                }else
                {
                    char str_per_100_profit[64];
                    sprintf(str_per_100_profit, "%.2f", allscore_hands / allhands * 100);
                    text_times_score_num_->setString(str_per_100_profit);
                }
            }
            
        }
    }
    else
    {
        text_whole_score_num_->setString(tools::local_string("NA","暂无"));
        text_times_score_num_->setString(tools::local_string("NA","暂无"));
        if(!is_current_romm_tournament_)
        {
            text_hour_score_num_->setString(tools::local_string("NA","暂无"));
        }
        else
        {
            text_hour_score_num_->setString("");
        }
        
    }
}


void LayerScoreList::select_chip_num(std::string chipNum)
{
    if(isChipChoose_)
    {
        isChipChoose_=false;
        choose_chip_num="";
        if (text_choose_type_->getString()==chipNum)
        {
            text_choose_type_->setString(text_choose_type_second_->getString());
            text_choose_type_second_->setString("");
        }
        else if (text_choose_type_second_->getString()==chipNum)
        {
            text_choose_type_second_->setString("");
        }
        
        blind_cell_datas_.clear();
        this->reset_score();
        for (int i=0; i<blind_cell_datas_copy_.size(); i++)
        {
            if (blind_cell_datas_copy_[i].playtime()!= 0)
            {
                allscore_time += blind_cell_datas_copy_[i].score();
                alltime += (1.0 * blind_cell_datas_copy_[i].playtime() / (1000 * 60 * 60));
            }
            if (blind_cell_datas_copy_[i].playhands() != 0)
            {
                allscore_hands += blind_cell_datas_copy_[i].score();
                allhands += blind_cell_datas_copy_[i].playhands();
            }
            
            allscore += blind_cell_datas_copy_[i].score();
            msg::HistoryScore score;
            score.CopyFrom(blind_cell_datas_copy_[i]);
            blind_cell_datas_.push_back(score);
        }
        if (isGroupChoose_)
        {
            this->getGroupChoosedData(choose_group_name_);
            
        }
        this->refresh_score_data();
        tableview_->reloadData();
    }
    else
    {
        this->getChipChoosedData(chipNum);
        this->refresh_score_data();
        tableview_->reloadData();
    }
}
void LayerScoreList::reset_score()
{
    allscore = 0;
    allscore_time = 0;
    allscore_hands = 0;
    alltime = 0;
    allhands = 0;
}
void LayerScoreList::getChipChoosedData(std::string chipNum)
{
    this->reset_score();
    std::vector<msg::HistoryScore> newDataSource;
    for (int i=0; i<blind_cell_datas_.size(); i++)
    {
        if (blind_cell_datas_[i].label_size()>=1 && blind_cell_datas_[i].label(0)==chipNum)
        {
            msg::HistoryScore score;
            score.CopyFrom(blind_cell_datas_[i]);
            newDataSource.push_back(score);
        }
    }
    blind_cell_datas_.clear();
    for(int j=0;j<newDataSource.size();j++)
    {
        if (newDataSource[j].playtime()!= 0)
        {
            allscore_time += newDataSource[j].score();
            alltime += (1.0 * newDataSource[j].playtime() / (1000 * 60 * 60));
        }
        if (newDataSource[j].playhands() != 0)
        {
            allscore_hands += newDataSource[j].score();
            allhands += newDataSource[j].playhands();
        }
        
        allscore += newDataSource[j].score();
        msg::HistoryScore score;
        score.CopyFrom(newDataSource[j]);
        blind_cell_datas_.push_back(score);
    }
    
    choose_chip_num=chipNum;
    if (!isChipChoose_)
    {
        isChipChoose_=true;
        if (isGroupChoose_)
        {
            text_choose_type_second_->setString(chipNum);
        }
        else
        {
            text_choose_type_->setString(chipNum);
        }
    }
}
void LayerScoreList::select_group_name(std::string groupId,std::string groupName)
{
    if(isGroupChoose_)
    {
        isGroupChoose_=false;
        choose_group_name_="";
        if (text_choose_type_->getString()==groupName)
        {
            text_choose_type_->setString(text_choose_type_second_->getString());
            text_choose_type_second_->setString("");
        }
        else if (text_choose_type_second_->getString()==groupName)
        {
            text_choose_type_second_->setString("");
        }
        
        blind_cell_datas_.clear();
        this->reset_score();
        for (int i=0; i<blind_cell_datas_copy_.size(); i++)
        {
            if (blind_cell_datas_copy_[i].playtime()!= 0)
            {
                allscore_time += blind_cell_datas_copy_[i].score();
                alltime += (1.0 * blind_cell_datas_copy_[i].playtime() / (1000 * 60 * 60));
            }
            if (blind_cell_datas_copy_[i].playhands() != 0)
            {
                allscore_hands += blind_cell_datas_copy_[i].score();
                allhands += blind_cell_datas_copy_[i].playhands();
            }
            
            allscore += blind_cell_datas_copy_[i].score();
            msg::HistoryScore score;
            score.CopyFrom(blind_cell_datas_copy_[i]);
            blind_cell_datas_.push_back(score);
        }
        if (isChipChoose_)
        {
            this->getChipChoosedData(choose_chip_num);
            
            
        }
        this->refresh_score_data();
        tableview_->reloadData();
    }
    else
    {
        this->getGroupChoosedData(groupName);
        this->refresh_score_data();
        tableview_->reloadData();
    }
}

void LayerScoreList::getGroupChoosedData(std::string groupName)
{
    this->reset_score();
    std::vector<msg::HistoryScore> newDataSource;
    for (int i=0; i<blind_cell_datas_.size(); i++)
    {
        if (blind_cell_datas_[i].groupname() ==groupName)
        {
            msg::HistoryScore score;
            score.CopyFrom(blind_cell_datas_[i]);
            newDataSource.push_back(score);
        }
    }
    blind_cell_datas_.clear();
    for(int j=0;j<newDataSource.size();j++)
    {
        if (newDataSource[j].playtime()!= 0)
        {
            allscore_time += newDataSource[j].score();
            alltime += (1.0 * newDataSource[j].playtime() / (1000 * 60 * 60));
        }
        if (newDataSource[j].playhands() != 0)
        {
            allscore_hands += newDataSource[j].score();
            allhands += newDataSource[j].playhands();
        }
        
        allscore += newDataSource[j].score();
        msg::HistoryScore score;
        score.CopyFrom(newDataSource[j]);
        blind_cell_datas_.push_back(score);
    }
    
    choose_group_name_=groupName;
    if(!isGroupChoose_)
    {
        isGroupChoose_=true;
        if (isChipChoose_)
        {
            text_choose_type_second_->setString(groupName);
        }
        else
        {
            text_choose_type_->setString(groupName);
        }
    }
}
void LayerScoreList::click_select_month(std::string choose)
{
    
    if(!isMonthChoose_)
    {
        choose_week_->set_node_unchoose_status();
        choose_all_->set_node_unchoose_status();
        endtime_ = tools::date();
        long time=30*24*60*60;
        startTime_=endtime_-time;
        send_get_history_score();
        isMonthChoose_=true;
        isWeekChoose_=false;
        isAllChoose_=false;
    }
}

void LayerScoreList::click_select_week(std::string choose)
{
    
    if(!isWeekChoose_)
    {
        choose_month_->set_node_unchoose_status();
        choose_all_->set_node_unchoose_status();
        endtime_ = tools::date();
        long time=7*24*60*60;
        startTime_=endtime_-time;
        send_get_history_score();
        isWeekChoose_=true;
        isMonthChoose_=false;
        isAllChoose_=false;
    }
}
void LayerScoreList::click_select_all(std::string choose)
{
    if (!isAllChoose_)
    {
        choose_month_->set_node_unchoose_status();
        choose_week_->set_node_unchoose_status();
        isWeekChoose_=false;
        isMonthChoose_=false;
        isAllChoose_=true;
        endtime_ = 0;
        startTime_= 0;
        send_get_history_score();
    }
}
void LayerScoreList::click_text_select_type(cocos2d::Ref *sender)
{
    if (text_choose_type_->getString()!="")
    {
        blind_cell_datas_.clear();
        this->reset_score();
        for (int i=0; i<blind_cell_datas_copy_.size(); i++)
        {
            if (blind_cell_datas_copy_[i].playtime()!= 0)
            {
                allscore_time += blind_cell_datas_copy_[i].score();
                alltime += (1.0 * blind_cell_datas_copy_[i].playtime() / (1000 * 60 * 60));
            }
            if (blind_cell_datas_copy_[i].playhands() != 0)
            {
                allscore_hands += blind_cell_datas_copy_[i].score();
                allhands += blind_cell_datas_copy_[i].playhands();
            }
            
            allscore += blind_cell_datas_copy_[i].score();
            msg::HistoryScore score;
            score.CopyFrom(blind_cell_datas_copy_[i]);
            blind_cell_datas_.push_back(score);
        }
        if (text_choose_type_->getString()==choose_chip_num)
        {
            
            if (isGroupChoose_)
            {
                this->getGroupChoosedData(choose_group_name_);
            }
            choose_chip_num="";
            isChipChoose_=false;
        }
        else if (text_choose_type_->getString()==choose_group_name_)
        {
            if (isChipChoose_)
            {
                this->getChipChoosedData(choose_chip_num);
               
            }
            choose_group_name_="";
            isGroupChoose_=false;
        }
        this->refresh_score_data();
        tableview_->reloadData();
        text_choose_type_->setString(text_choose_type_second_->getString());
        text_choose_type_second_->setString("");
    }
}

void LayerScoreList::click_text_select_type_second(cocos2d::Ref *sender)
{
    if (text_choose_type_second_->getString()!="")
    {
        blind_cell_datas_.clear();
        this->reset_score();
        for (int i=0; i<blind_cell_datas_copy_.size(); i++)
        {
            if (blind_cell_datas_copy_[i].playtime()!= 0)
            {
                allscore_time += blind_cell_datas_copy_[i].score();
                alltime += (1.0 * blind_cell_datas_copy_[i].playtime() / (1000 * 60 * 60));
            }
            if (blind_cell_datas_copy_[i].playhands() != 0)
            {
                allscore_hands += blind_cell_datas_copy_[i].score();
                allhands += blind_cell_datas_copy_[i].playhands();
            }
            
            allscore += blind_cell_datas_copy_[i].score();
            msg::HistoryScore score;
            score.CopyFrom(blind_cell_datas_copy_[i]);
            blind_cell_datas_.push_back(score);
        }
        if (text_choose_type_second_->getString()==choose_chip_num)
        {
            if (isGroupChoose_)
            {
                this->getGroupChoosedData(choose_group_name_);
                
            }
            choose_chip_num="";
            isChipChoose_=false;
        }
        else if (text_choose_type_second_->getString()==choose_group_name_)
        {
            if (isChipChoose_)
            {
                this->getChipChoosedData(choose_chip_num);
            }
            choose_group_name_="";
            isGroupChoose_=false;
        }
        this->refresh_score_data();
        tableview_->reloadData();
        text_choose_type_second_->setString("");
    }
}


TableViewCell* LayerScoreList::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    
    NodeScoreCell* cell = dynamic_cast<NodeScoreCell*>(table->dequeueCell());
    if(!cell) {
        cell = NodeScoreCell::create(is_group_match_);
    }
    cell->update_data(blind_cell_datas_.at(idx));
    cell->set_click_chip_name_callback(std::bind(&LayerScoreList::select_chip_num,this,std::placeholders::_1));
    cell->set_click_group_name_callback(std::bind(&LayerScoreList::select_group_name,this,std::placeholders::_1, std::placeholders::_2));
    return cell;
}


cocos2d::Size LayerScoreList::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(NODE_SCORE_CELL_WIDTH,NODE_SCORE_CELL_HIGHT);
}

ssize_t LayerScoreList::numberOfCellsInTableView(TableView *table)
{
    return blind_cell_datas_.size();
}

void LayerScoreList::tableCellTouched(cocos2d::extension::TableView *table, cocos2d::extension::TableViewCell *cell)
{
    NodeScoreCell* scoreCell = dynamic_cast<NodeScoreCell*>(cell);
    int roomid = scoreCell->getRoomId();
    LayerScore* scoreList=LayerScore::create(roomid);
    this->addChild(scoreList,0,"LayerScore");
    
}


