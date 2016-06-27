//
//  LayerTourmentScore.cpp
//  pokerclient
//
//  Created by ll on 15/12/4.
//
//

#include "LayerTourmentScore.h"
#include "GameDataManager.h"
#include "cmd_def.h"
#include "utils.h"

#include "NodeScoreCellOld.h"

bool LayerTourmentScore::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerTourmentScore.csb");
    this->addChild(root_);
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    btn_share_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_share"));
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    text_room_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_time_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_time"));
    text_time_->setString("");
    
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    root_->addChild(tableview_);
    
    listview->removeFromParent();
    
    btn_back_->addClickEventListener(std::bind(&LayerTourmentScore::click_btn_back, this, std::placeholders::_1));
    btn_share_->setVisible(false);
    return true;
}

void LayerTourmentScore::handle_get_score(cmd_data_pointer data)
{
//    match_objects_.clear();
    
//    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
//    if (rec){
//        
//        msg::Processor_420_PokerGetScore_DOWN processor;
//        processor.ParseFromArray(rec->body, rec->bodysize);
//        
//        if (processor.playerscores().score().size() == 0)
//        {
//            text_time_->setString("房间未曾有人进入 无战绩");
//        }else
//        {
//            std::string submit_time = processor.time();
//            if (submit_time == "")
//            {
//                text_time_->setString("游戏时长：无");
//            }else
//            {
//                text_time_->setString(tools::local_string("game_time", "游戏时长：") + submit_time);
//            }
//        }
//        text_room_name_->setString(processor.roomname());
//        
//        for (auto& it: processor.playerscores().score()) {
//            ::std::string nickname = it.userdetailinfo().nickname();
//            ::std::string head_pic_name = it.userdetailinfo().picname();
//            int32_t buy_amount = it.totalbuyin();
//            int32_t win_amount = it.totalwin();
//            uint32_t user_id=it.userdetailinfo().userid();
//            msg::RoomType room_type = processor.roomtype();
//            bool is_tournament = processor.istournament();
//            std::string otherprize = it.otherprize();
//            
//            int32_t rank = it.rank();
//            std::shared_ptr<MATCH_OBJECT> obj(new MATCH_OBJECT());
//            obj->is_group_user_rank = false;
//            obj->nickname = nickname;
//            obj->head_pic_name = head_pic_name;
//            obj->buy_amount = buy_amount;
//            obj->win_amount = win_amount;
//            obj->user_id = user_id;
//            obj->room_type = room_type;
//            obj->is_tournament = is_tournament;
//            obj->otherprize = otherprize;
//            obj->rank = rank;
//            obj->hp_lost = it.hplost();
//            match_objects_.push_back(obj);
//        }
    
//        tableview_->reloadData();
//    }
}

#pragma mark tableview datasource --
TableViewCell* LayerTourmentScore::tableCellAtIndex(TableView *table, ssize_t idx)
{
    NodeScoreCellOld* cell = NodeScoreCellOld::create();
//    cell->update_data(match_objects_[idx]);
    
    return cell;
}
ssize_t LayerTourmentScore::numberOfCellsInTableView(TableView *table)
{
//    return match_objects_.size();
    return 2;
}

void LayerTourmentScore::click_btn_back(Ref *sender)
{
    this->removeFromParent();
}