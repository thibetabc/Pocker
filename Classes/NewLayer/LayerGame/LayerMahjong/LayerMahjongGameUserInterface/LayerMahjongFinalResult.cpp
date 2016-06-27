//
//  LayerMahjongFinalResult.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#include "LayerMahjongFinalResult.h"
#include "NodeMahjongFinalResultCell.h"
#include "Room.h"
bool LayerMahjongFinalResult::init()
{
    if (!MahjongChildLayer::init())
    {
        return false;
    }
    auto layer = LayerColor::create(Color4B(143, 143, 143, 100));
    this->addChild(layer);
    
    root_ = CSLoader::createNode("LayerMahjongFinalResult.csb");
    this->addChild(root_);
    
    btn_close_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_close"));
    btn_close_->addClickEventListener([&](Ref* sender){
        this->removeFromParent();
    });
    
    list_view_result_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "list_view_result"));
//    tableview_ = TableView::create(this, list_view_result_->getContentSize());
//    tableview_->setAnchorPoint(list_view_result_->getAnchorPoint());
//    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
//    tableview_->setPosition(list_view_result_->getPosition());
//    tableview_->setDelegate(this);
//    root_->addChild(tableview_);
    list_view_result_->removeFromParent();
    
    
    return true;
}
void LayerMahjongFinalResult::refresh_data()
{
    auto &room = this->GetRoom();
    auto users = room->get_users();
    user_ids_.clear();
    int i = 0;
    for ( auto iter : users )
    {
        user_ids_.insert(std::make_pair(i, iter.first));
        i = i + 1;
    }
    
    for ( int i = 0; i < user_ids_.size() ; i++ )
    {
        NodeMahjongFinalResultCell* cell = NodeMahjongFinalResultCell::create(user_ids_[i] == GDM->get_user_id() ? true : false);
        auto &room = this->GetRoom();
        cell->set_data(*room->get_user(user_ids_[i])->get_properties());
        this->addChild(cell);
        cell->setPosition(cell_pos_[i].x, cell_pos_[i].y);
    }
}
