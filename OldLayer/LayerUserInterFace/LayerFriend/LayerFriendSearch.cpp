//
//  LayerFriendSearch.cpp
//  pokerclient
//
//  Created by ll on 15/8/21.
//
//

#include "LayerFriendSearch.h"
#include "FriendCell.h"
#include "GameDataManager.h"
#include "utils.h"
#include "GameTools.h"
#include "cmd_def.h"
#include "LayerFriendDetails.h"
#include "ProcessorManager.h"
bool LayerFriendSearch::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerGroupSearch.csb");
    this->addChild(root);
    
    auto text_cancel = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    auto text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    auto btn_search = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_search"));
    
    auto tf_keyword = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_keyword"));
    tf_keyword_ = GameTools::createEditBoxFromTextField(this, tf_keyword);
    tf_keyword_->setPlaceHolder(tools::local_string("friend_search_word", "请输入玩家昵称或ID").c_str());
    tf_keyword_->setDelegate(this);
    tf_keyword_->setInputMode(EditBox::InputMode::SINGLE_LINE);
    tf_keyword_->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    
    image_search_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_search_icon"));
    
    text_title->setString(tools::local_string("search_friend", "搜索玩家"));
    
    text_cancel->addClickEventListener(std::bind(&LayerFriendSearch::text_cancel_click, this, std::placeholders::_1));
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    root->addChild(tableview_);
    
    listview->removeFromParent();
    
    dispatch_.register_event(EVENT_HANDLE_REQ_SEARCH_USER, BIND_FUNC(this, LayerFriendSearch::update_table_view));
    return true;
}



void LayerFriendSearch::update_table_view(cmd_data_pointer)
{
    tableview_->reloadData();
}

void LayerFriendSearch::text_cancel_click(Ref* sender)
{
    removeFromParent();
}

#pragma mark tableview datasource --
TableViewCell* LayerFriendSearch::tableCellAtIndex(TableView *table, ssize_t idx)
{
    FriendCell* cell = dynamic_cast<FriendCell*>(table->dequeueCell());
    if(!cell) {
        cell = FriendCell::create();
    }
    auto& data = friends_.at(idx);
    cell->update_property(data);
    
    
    return cell;
}
ssize_t LayerFriendSearch::numberOfCellsInTableView(TableView *table)
{
    return friends_.size();
}

#pragma mark tableview delegate --
void LayerFriendSearch::tableCellTouched(TableView* table, TableViewCell* cell)
{
    GDM->m_isbyreachingfriend = true;
    if(table->isTouchMoved())
    {
        return;
    }
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell);
    auto friend_data = message_cell->get_friend_data();
    
    LayerFriendDetails* glc = LayerFriendDetails::create();
    glc->set_show_dir(LAYER_SHOW_DIRECTION::LEFT_TO_RIGHT);
    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
    parent->addChild(glc);
    glc->update_property(friend_data);
    
//    if (GDM->get_my_friends().find(friend_data->get_userid()) != GDM->get_my_friends().end())
//    {
//        glc->update_type(3, false);
//    }else{
//        glc->update_type(1, false);
//    }
}


void LayerFriendSearch::editBoxEditingDidBegin(EditBox* editBox)
{
    image_search_icon_->setVisible(false);
}

void LayerFriendSearch::editBoxEditingDidEnd(EditBox* editBox)
{
    if (strlen(editBox->getText()) == 0)
    {
        image_search_icon_->setVisible(true);
    }
}

void LayerFriendSearch::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
}
void LayerFriendSearch::editBoxReturn(EditBox* editBox)
{
    std::string key_word = tf_keyword_->getText();
    if (!key_word.empty())
    {
        auto processor = PM->SearchUser_up(key_word);
        net_manager::shared_instence().send_data(processor,REQ_SEARCH_USER);
    }
    
}

void LayerFriendSearch::handle_add_user_send_succeess(cmd_data_pointer)
{
    removeFromParent();
}

void LayerFriendSearch::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell);
    message_cell->set_is_touch();
}

void LayerFriendSearch::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    BaseFriendCell* message_cell = dynamic_cast<BaseFriendCell*>(cell);
    message_cell->set_is_touch(false);
}