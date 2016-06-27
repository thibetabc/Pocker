//
//  LayerFriendListOld.cpp
//  pokerclient
//
//  Created by ll on 15/8/27.
//
//

#include "LayerFriendListOld.h"
#include "HudLayer.h"
#include "GameDataManager.h"
#include "cmd_def.h"
#include "utils.h"
#include "GameTools.h"

#include "FriendCell.h"
#include "NewFriendCell.h"
#include "InvaitationFriendCell.h"
#include "AuditFriendCell.h"
#include "FriendAddressCell.h"
#include "CreateGroupLayer.h"
#include "LayerInvitation.h"
#include "LayerFriendDetails.h"
#include "LayerFriendSearch.h"
#include "LayerChatOld.h"
#include "LayerAddressBook.h"


#include "BaseFriendCell.h"

bool LayerFriendListOld::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    m_bCanSearch = false;
    auto root = CSLoader::createNode("LayerFriendListOld.csb");
    this->addChild(root);
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    root->addChild(tableview_);
    
    listview->removeFromParent();
    
    for (auto& iter : root->getChildren()) {
        log("child'name:%s", iter->getName().c_str());
    }
    
    dispatch_.register_event(EVENT_HANDLE_REQ_GET_FRIENDS_LIST,
                             BIND_FUNC(this, LayerFriendListOld::update_table_view));
    dispatch_.register_event(EVENT_HANDLE_REFRESH_CHAT_RECORD,
                             BIND_FUNC(this, LayerFriendListOld::update_table_view));
    
   //dispatch_.register_event(EVENT_HANDLE_REQ_DELETE_FRIEND_SUCCESS, BIND_FUNC(this, LayerFriendListOld::update_table_view));
    
    GDM->send_get_friends_list();
    
    return true;
}

#pragma mark event call
void LayerFriendListOld::update_table_view(cmd_data_pointer data)
{
    friends_.clear();
    tableview_->reloadData();
    m_bCanSearch = true;
}

#pragma mark tableview datasource --
TableViewCell* LayerFriendListOld::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto& data = friends_.at(idx);
    
//    if (data->get_cell_type() == friend_data::CELL_TYPE::CELL_NEW_FRIEND)
//    {
//        NewFriendCell* cell = dynamic_cast<NewFriendCell*>(table->dequeueCell());
//        if(!cell) {
//            cell = NewFriendCell::create();
//        }
//        cell->update_property(data);
//        return cell;
//        
//    }else if (data->get_cell_type() == friend_data::CELL_TYPE::CELL_INVAITATION_FRIEND){
//        InvaitationFriendCell* cell = dynamic_cast<InvaitationFriendCell*>(table->dequeueCell());
//        if(!cell) {
//            cell = InvaitationFriendCell::create();
//        }
//        cell->update_property(data);
//        return cell;
//    }else if (data->get_cell_type() == friend_data::CELL_TYPE::CELL_FRIEND){
//        FriendCell* cell = dynamic_cast<FriendCell*>(table->dequeueCell());
//        if(!cell) {
//            cell = FriendCell::create();
//        }
//        cell->update_property(data);
//        return cell;
//    }else if (data->get_cell_type() == friend_data::CELL_TYPE::CELL_FRIEND_ADDRESS){
//        FriendAddressCell* cell = dynamic_cast<FriendAddressCell*>(table->dequeueCell());
//        if (!cell) {
//            cell = FriendAddressCell::create();
//        }
//        cell->update_property(data);
//        return cell;
//    }
    
    FriendCell* cell = dynamic_cast<FriendCell*>(table->dequeueCell());
    if(!cell) {
        cell = FriendCell::create();
        
    }
    
    return cell;
}

ssize_t LayerFriendListOld::numberOfCellsInTableView(TableView *table)
{
    return friends_.size();
}

#pragma mark tableview delegate --
void LayerFriendListOld::tableCellTouched(TableView* table, TableViewCell* cell)
{
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell);
    auto friend_data = message_cell->get_friend_data();
    
    
//    if (friend_data->get_cell_type() == friend_data::CELL_TYPE::CELL_NEW_FRIEND)
//    {
//        LayerFriendSearch* glc = LayerFriendSearch::create();
//        glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
//        BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
//        parent->addChild(glc);
//        
//    }else if (friend_data->get_cell_type() == friend_data::CELL_TYPE::CELL_INVAITATION_FRIEND){
//        LayerInvitation* glc = LayerInvitation::create();
//        glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
//        BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
//        parent->addChild(glc);
//    }else if (friend_data->get_cell_type() == friend_data::CELL_TYPE::CELL_FRIEND){
//        
//        GDM->set_select_friend(friend_data);
//        GDM->set_chat_status(2);
//
//        LayerChatOld* glc = LayerChatOld::create();
//        glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
//        BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
//        parent->addChild(glc);
//    }else if (friend_data->get_cell_type() == friend_data::CELL_TYPE::CELL_FRIEND_ADDRESS){
//        LayerAddressBook* glc = LayerAddressBook::create();
//        glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
//        BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
//        parent->addChild(glc);
//    }
}

void LayerFriendListOld::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell);
    message_cell->set_is_touch();
}

void LayerFriendListOld::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell); 
    message_cell->set_is_touch(false);
}