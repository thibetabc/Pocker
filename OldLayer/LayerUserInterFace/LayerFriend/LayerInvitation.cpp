//
//  LayerInvitation.cpp
//  pokerclient
//
//  Created by ll on 15/8/21.
//
//

#include "LayerInvitation.h"
#include "LayerAddressBook.h"
#include "LayerFriendDetails.h"
#include "HudLayer.h"
#include "GameDataManager.h"
#include "cmd_def.h"
#include "utils.h"
#include "GameTools.h"
#include "events.h"
#include "FriendCell.h"
#include "NewFriendCell.h"
#include "InvaitationFriendCell.h"
#include "AuditFriendCell.h"
#include "msg.pb.h"
#include "InvitationManager.h"

bool LayerInvitation::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerInvitation.csb");
    this->addChild(root);
    
    
//    std::shared_ptr<friend_data> abc{};
//    friends_.push_back(std::shared_ptr<friend_data>());
//    friends_.push_back(std::shared_ptr<friend_data>());
//    friends_.push_back(std::shared_ptr<friend_data>());
//    cocos2d::log("LayerInvitation:: friends_  initsize is %zu", friends_.size());
    invitations_ = IM->get_vec_invitations_();
    IM->clear_unread_invitation_count();
    
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
    
    
//    auto text_back = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_back"));
//    text_back->setString(tools::local_string("back", "返回"));
//    text_back->addClickEventListener(std::bind(&LayerInvitation::clickback, this, std::placeholders::_1));
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->addClickEventListener(std::bind(&LayerInvitation::clickback, this, std::placeholders::_1));
    
    
    for (auto& iter : root->getChildren()) {
        log("child'name:%s", iter->getName().c_str());
    }
    
    dispatch_.register_event(EVENT_HANDLE_REQ_CLIENT_RES_FRIEND_NEW_MSG, BIND_FUNC(this, LayerInvitation::update_table_view));
    dispatch_.register_event(EVENT_HANDLE_REQ_DEAL_REQUEST_SUCCESS, BIND_FUNC(this, LayerInvitation::update_table_view));
    
    return true;
}

#pragma mark event call
void LayerInvitation::update_table_view(cmd_data_pointer data)
{
    invitations_ = IM->get_vec_invitations_();
    
    tableview_->reloadData();
}

#pragma mark tableview datasource --
TableViewCell* LayerInvitation::tableCellAtIndex(TableView *table, ssize_t idx)
{
    AuditFriendCell* cell = dynamic_cast<AuditFriendCell*>(table->dequeueCell());
    if(!cell) {
        cell = AuditFriendCell::create();
    }
    auto& data = invitations_.at(idx);
    cell->update_property(data);
    
    return cell;
}

ssize_t LayerInvitation::numberOfCellsInTableView(TableView *table)
{
    return invitations_.size();
}


#pragma mark tableview delegate --
void LayerInvitation::tableCellTouched(TableView* table, TableViewCell* cell)
{
    AuditFriendCell* message_cell = dynamic_cast<AuditFriendCell*>(cell);
    auto request_data = message_cell->get_invaitation_data();
    
    //by lvzhangle
    if (request_data.status() == msg::InvitationStatus::Accepted)
    {
        return;
    }
    
//    uint32_t userid = request_data->request().userdetailinfo().userid();
//    std::string username = request_data->request().userdetailinfo().nickname();
//    std::string head_pic_name = request_data->request().userdetailinfo().picname();
//    uint32_t user_status = 0;
//    uint32_t cell_type = friend_data::CELL_TYPE::CELL_AUDIT_FRIEND;
//    std::shared_ptr<friend_data> newfriend(new friend_data(userid, username, head_pic_name, user_status, cell_type));
//    GDM->set_select_friend(newfriend);
//    
//    LayerFriendDetails* glc = LayerFriendDetails::create();
//    glc->set_show_dir(LAYER_SHOW_DIRECTION::LEFT_TO_RIGHT);
//    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
//    parent->addChild(glc);
//    glc->update_property(newfriend);
//    glc->update_request(request_data);
//    
//    if (request_data->request().type() == msg::RequestType::ADDFRIEND){
//        if (GDM->get_my_friends().find(newfriend->get_userid()) != GDM->get_my_friends().end())
//        {
//            glc->update_type(3, false);
//        }else{
//            glc->update_type(2, false);
//        }
//    }else {
//        glc->update_type(2, false);
//    }

}

void LayerInvitation::onExit()
{
    if(back_callback_) {
        back_callback_();
    }
    LayerPop::onExit();
}

void LayerInvitation::clickback(Ref * sender)
{
   removeFromParent();
}