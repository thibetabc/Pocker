//
//  LayerAddGroupUser.cpp
//  pokerclient
//
//  Created by ll on 15/9/9.
//
//

#include "LayerAddGroupUser.h"
#include "HudLayer.h"
#include "GameDataManager.h"
#include "cmd_def.h"
#include "utils.h"
#include "GameTools.h"
#include "events.h"
#include "ProcessorManager.h"
#include "GroupUserSelectCell.h"

#include "msg.pb.h"

bool LayerAddGroupUser::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerAddGroupUser.csb");
    this->addChild(root);
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    
    //this->setSwallowsTouches(false);
    
    root->addChild(tableview_);
    
    listview->removeFromParent();
    
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->setTitleText("    " + tools::local_string("back", "返回"));
    btn_back->addClickEventListener(std::bind(&LayerAddGroupUser::clickback, this, std::placeholders::_1));
    
    auto text_confirm = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_confirm"));
    text_confirm->addClickEventListener(std::bind(&LayerAddGroupUser::clickconfirm, this, std::placeholders::_1));
    
    for (auto& iter : root->getChildren()) {
        log("child'name:%s", iter->getName().c_str());
    }
    
    return true;
}

#pragma mark event call
void LayerAddGroupUser::update_table_view(cmd_data_pointer data)
{
    tableview_->reloadData();
}

#pragma mark tableview datasource --
TableViewCell* LayerAddGroupUser::tableCellAtIndex(TableView *table, ssize_t idx)
{
    GroupUserSelectCell* cell = dynamic_cast<GroupUserSelectCell*>(table->dequeueCell());
    if(!cell) {
        cell = GroupUserSelectCell::create();
    }
    auto& data = friends_.at(idx);
    cell->update_property(data);
    
    cell->set_select(GroupUserSelectCell::select_status::ISFALSE);
    for (int i = 0; i < members_.size(); i++)
    {
        if (members_[i].userid() == data.userid())
        {
            cell->set_select(GroupUserSelectCell::select_status::ISFORBID);
            break;
        }
    }

    return cell;
}

ssize_t LayerAddGroupUser::numberOfCellsInTableView(TableView *table)
{
    return friends_.size();
}

#pragma mark tableview delegate --
void LayerAddGroupUser::tableCellTouched(TableView* table, TableViewCell* cell)
{
    GroupUserSelectCell* message_cell = dynamic_cast<GroupUserSelectCell*>(cell);
    auto friend_data = message_cell->get_friend_data();
        
    if (message_cell->get_is_select() == GroupUserSelectCell::select_status::ISTRUE){
        for (int i = 0; i < select_list.size(); i++)
        {
            if (select_list[i] == friend_data.userid())
            {
                select_list.erase(select_list.begin()+i);
                break;
            }
        }
        message_cell->set_select(GroupUserSelectCell::select_status::ISFALSE);
    }else if (message_cell->get_is_select() == GroupUserSelectCell::select_status::ISFALSE){
        select_list.push_back(friend_data.userid());
        message_cell->set_select(GroupUserSelectCell::select_status::ISTRUE);
    }else if (message_cell->get_is_select() == GroupUserSelectCell::select_status::ISFORBID){
        
    }
}

void LayerAddGroupUser::clickconfirm(Ref * sender)
{
    for (int i = 0; i < select_list.size(); i++)
    {
        msg::Request request;
        request.set_receiverid(select_list[i]);
        request.set_type(msg::RequestType::INVITETOGROUP);
        request.mutable_groupinfo()->set_groupid(group_id_);
        request.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
        auto processor = PM->SendRequest_up(request);
        net_manager::shared_instence().send_data(processor,REQ_ADD_FRIEND);
    }
    removeFromParent();
}

void LayerAddGroupUser::clickback(Ref * sender)
{
    if(back_callback_) {
        back_callback_();
    }
    removeFromParent();
}

void LayerAddGroupUser::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell);
    message_cell->set_is_touch();
}

void LayerAddGroupUser::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell);
    message_cell->set_is_touch(false);
}