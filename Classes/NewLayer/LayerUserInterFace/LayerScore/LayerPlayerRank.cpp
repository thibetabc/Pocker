//
//  LayerPlayerRank.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/21.
//
//

#include "LayerPlayerRank.h"
#include "NodeScoreListCell.h"
#include "ProcessorManager.h"
#include "net_data.h"
#include "NET_CMD.h"
#include "utils.h"

LayerPlayerRank* LayerPlayerRank::create(std::string groupName, msg::RoomType roomType, int groupId,bool istournament)
{
    LayerPlayerRank* rank=new LayerPlayerRank();
    if(rank->init(groupName, roomType, groupId,istournament))
    {
        rank->autorelease();
        return rank;
    }
    return nullptr;
}
bool LayerPlayerRank::init(std::string groupName, msg::RoomType roomType, int groupId,bool istournament)
{
    if(!BaseLayer::init())
    {
        return false;
    }
    root_=CSLoader::createNode("LayerPlayerRank.csb");
    this->addChild(root_);
    
    group_id_=groupId;
    group_name_=groupName;
    room_type_=roomType;
    istournament_=istournament;
    
    list_rank_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "list_rank"));
    tableview_ = TableView::create(this, list_rank_->getContentSize());
    tableview_->setAnchorPoint(list_rank_->getAnchorPoint());
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(list_rank_->getPosition());
    tableview_->setDelegate(this);
    root_->addChild(tableview_);
    list_rank_->removeFromParent();
    
    node_select_btn_=dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_select_btn"));
    node_select_game_=NodeSelectGameType::create();
    node_select_game_->setTouchEnabled(true);
    node_select_btn_->addChild(node_select_game_);
    node_select_game_->setEnabled(true);
    node_select_game_->set_click_callback(std::bind(&LayerPlayerRank::click_node_select_game,this));
    
    node_select_game_->refresh_data(room_type_);
    
    text_title_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(group_name_);
    
    btn_back_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerPlayerRank::click_btn_back,this,std::placeholders::_1));
    
    auto processor=PM->GetGroupUserRank_Up(room_type_, istournament_, group_id_);
    send_data(processor, REQ_GET_GROUP_USER_RANK);
    
    return true;
}
void LayerPlayerRank::register_events()
{
    dispatch_.register_event(REQ_GET_GROUP_USER_RANK, BIND_FUNC(this,LayerPlayerRank::handle_get_rank));
}
void LayerPlayerRank::click_node_select_game()
{
    LayerChooseGameType* choose= LayerChooseGameType::create();
    choose->set_choose_call_func(std::bind(&LayerPlayerRank::update_tableview_data, this, std::placeholders::_1, std::placeholders::_2));
    addChild(choose,0,"LayerChooseGameType");
}
void LayerPlayerRank::click_btn_back(cocos2d::Ref *sender)
{
    this->removeFromParent();
}
void LayerPlayerRank::update_tableview_data(msg::RoomType type, bool istournament)
{
    removeChildByName("LayerChooseGameType");
    istournament_=istournament;
    room_type_=type;
    node_select_game_->refresh_data(room_type_);
    auto processor=PM->GetGroupUserRank_Up(room_type_, istournament_, group_id_);
    send_data(processor, REQ_GET_GROUP_USER_RANK);
}

void LayerPlayerRank::handle_get_rank(cmd_data_pointer data)
{
    ShowTipMessage(false);
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor=PM->GetGroupUserRank_Down(rec);
    if ( processor.groupinfo().rankvisible() )
    {
        data_source_.clear();
        if (processor.playerscores().score_size() == 0)
        {
            ShowTipMessage(true,tools::local_string("player_rank_is_null","亲，您还没有战绩排行，赶快拉小伙伴组局玩游戏吧"));
        }
        else
        {
            for(int i=0;i<processor.playerscores().score_size();i++)
            {
                msg::PlayerScore score;
                score.CopyFrom(processor.playerscores().score(i));
                data_source_.push_back(score);
            }
            auto sortfuncitonWin = [](msg::PlayerScore a,msg::PlayerScore b){return a.totalwin() > b.totalwin();};
            std::sort(data_source_.begin(), data_source_.end(), sortfuncitonWin);
            tableview_->reloadData();
        }
    }
    else
    {
        ShowTipMessage(true,tools::local_string("not_open_player_rank","群主暂时没有公开用户排行哦"));
    }
}

TableViewCell* LayerPlayerRank::tableCellAtIndex(TableView *table, ssize_t idx)
{
    NodeScoreListCell* cell = dynamic_cast<NodeScoreListCell*>(table->dequeueCell());
    if(!cell) {
        cell = NodeScoreListCell::create(true);
    }
    cell->update_data(data_source_.at(idx));
    if (idx==1)
    {
        cell->set_silver_visible();
    }
    if (idx==0)
    {
        cell->set_gold_visible();
    }
    return cell;
}
ssize_t LayerPlayerRank::numberOfCellsInTableView(TableView *table)
{
    return data_source_.size();
}