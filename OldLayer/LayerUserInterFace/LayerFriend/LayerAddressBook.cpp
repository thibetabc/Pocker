//
//  LayerAddressBook.cpp
//  pokerclient
//  通讯录
//
//  Created by ll on 15/8/18.
//
//

#include "LayerAddressBook.h"
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

#include "BaseFriendCell.h"
#include "PayManager.h"

bool LayerAddressBook::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerAddressBook.csb");
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
    
    auto tf_keyword = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_keyword"));
    
    tf_keyword_ = GameTools::createEditBoxFromTextField(this, tf_keyword);
    tf_keyword_->setPlaceHolder(tools::local_string( "friend_local_search_word", "搜索").c_str());
    tf_keyword_->setDelegate(this);
    tf_keyword_->setInputMode(EditBox::InputMode::SINGLE_LINE);
    tf_keyword_->setReturnType(EditBox::KeyboardReturnType::SEARCH);

    image_search_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_search_icon"));
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->setTitleText("    " + tools::local_string("back", "返回"));
    btn_back->addClickEventListener(std::bind(&LayerAddressBook::clickback, this, std::placeholders::_1));
    
    for (auto& iter : root->getChildren()) {
        log("child'name:%s", iter->getName().c_str());
    }
    
    dispatch_.register_event(EVENT_HANDLE_REQ_GET_FRIENDS_LIST,
                             BIND_FUNC(this, LayerAddressBook::update_table_view));
    
        GDM->send_get_friends_list();
    
    return true;
}

void LayerAddressBook::onExit()
{
    if(back_callback_) {
        back_callback_();
    }
    LayerPop::onExit();
}

#pragma mark event call
void LayerAddressBook::update_table_view(cmd_data_pointer data)
{
    if (tableview_ != nullptr) {
        tableview_->reloadData();
    }
}

#pragma mark tableview datasource --
TableViewCell* LayerAddressBook::tableCellAtIndex(TableView *table, ssize_t idx)
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

ssize_t LayerAddressBook::numberOfCellsInTableView(TableView *table)
{
    return friends_.size();
}

#pragma mark tableview delegate --
void LayerAddressBook::tableCellTouched(TableView* table, TableViewCell* cell)
{
    GDM->m_isbyreachingfriend = true;
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell);
    auto friend_data = message_cell->get_friend_data();
    //充值，选择给好友充值
    if (mIsCharge)
    {
        TipView *tip = TipView::create();
        tip->update_note("确定要给"+friend_data.nickname()+"充值吗?");
        tip->show_cancel(true);
        tip->set_tipview_close_callback([=](){
            //PayManager::getInstance()->SendPay(mProductID,friend_data.userid());
            tip->retain();
            this->getParent()->removeFromParent();
        });
        this->addChild(tip);
        return;
    }
    
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
//        GDM->set_select_friend(friend_data);
//        LayerFriendDetails* glc = LayerFriendDetails::create();
//
//        glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
//        BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
//        parent->addChild(glc);
//        glc->update_property(friend_data);
//        
//        if (GDM->get_my_friends().find(friend_data->get_userid()) != GDM->get_my_friends().end())
//        {
//            glc->update_type(3, false);
//        }else{
//            glc->update_type(1, false);
//        }
//    }
}

void LayerAddressBook::editBoxEditingDidBegin(EditBox* editBox)
{
    image_search_icon_->setVisible(false);
}

void LayerAddressBook::editBoxEditingDidEnd(EditBox* editBox)
{
    return;
    //（＊&＃＊¥＊（）（＊（＃）＊）（¥＃&¥＊＃&&@…………@＃……&＃＊
    if (strlen(editBox->getText()) == 0) {
        image_search_icon_->setVisible(true);
    }
    
    if (tableview_ != nullptr) {
        tableview_->reloadData();
    }
}

void LayerAddressBook::editBoxTextChanged(EditBox* editBox, const std::string& text) {
    if (tableview_ != nullptr) {
        tableview_->reloadData();
    }
}

void LayerAddressBook::editBoxReturn(EditBox* editBox)
{
    
}

void LayerAddressBook::clickback(Ref * sender)
{
    removeFromParent();
}

void LayerAddressBook::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell);
    message_cell->set_is_touch();
}

void LayerAddressBook::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell);
    message_cell->set_is_touch(false);
}