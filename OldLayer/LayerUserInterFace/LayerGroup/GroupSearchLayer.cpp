//
//  GroupSearchLayer.cpp
//  pokerclient
//  搜索群 界面
//
//  Created by duanmt on 15/5/13.
//
//

#include "GroupSearchLayer.h"
#include "GroupCell.h"
#include "GameDataManager.h"
#include "utils.h"
#include "GameTools.h"
#include "cmd_def.h"
#include "LayerChatOld.h"
#include "EnterGroupLayer.h"
#include "TipView.h"
#include "ProcessorManager.h"

bool GroupSearchLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerGroupSearch.csb");
    this->addChild(root);
    
    auto btn_cancel = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    auto text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    //auto btn_search = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_search"));
    auto tf_keyword = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_keyword"));

    tf_keyword_ = GameTools::createEditBoxFromTextField(this, tf_keyword);
    tf_keyword_->setPlaceHolder(tools::local_string("group_search_word", "请输入群名称").c_str());
    tf_keyword_->setDelegate(this);
    tf_keyword_->setInputMode(EditBox::InputMode::SINGLE_LINE);
    tf_keyword_->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    image_search_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_search_icon"));
    
    btn_cancel->setTitleText("    " + tools::local_string("cancel", "取消"));
    text_title->setString(tools::local_string("search_group", "搜索群"));
    //btn_search->setTitleText(tools::local_string("search", "搜索"));
    
    btn_cancel->addClickEventListener(std::bind(&GroupSearchLayer::text_cancel_click, this, std::placeholders::_1));
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    root->addChild(tableview_);
    
    listview->removeFromParent();
    
    dispatch_.register_event(EVENT_HANDLE_GROUP_LIST_FINISHED, BIND_FUNC(this, GroupSearchLayer::update_table_view));
    dispatch_.register_event(EVENT_HANDLE_ADD_GROUP_FINISHED, BIND_FUNC(this, GroupSearchLayer::handle_create_group_finished));
    GDM->send_search_group(msg::GroupSearchType::SEARCHRECOMMAND, "");
    
    return true;
}

void GroupSearchLayer::onEnterTransitionDidFinish()
{
}

void GroupSearchLayer::update_table_view(cmd_data_pointer data)
{
    //groups_ = GDM->get_vec_groups(1);
    tableview_->reloadData();
}

void GroupSearchLayer::text_cancel_click(Ref* sender)
{
    removeFromParent();
}

void GroupSearchLayer::handle_create_group_finished(cmd_data_pointer)
{
    LayerChatOld* glc = LayerChatOld::create();
    glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
    this->removeFromParent();
    parent->addChild(glc);
}

#pragma mark tableview datasource --
TableViewCell* GroupSearchLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    GroupCell* cell = dynamic_cast<GroupCell*>(table->dequeueCell());
    if(!cell) {
        cell = GroupCell::create();
    }
    auto data = groups_.at(idx);
    cell->update_property(data);
    return cell;
}
ssize_t GroupSearchLayer::numberOfCellsInTableView(TableView *table)
{
    return groups_.size();
}

#pragma mark tableview delegate --
void GroupSearchLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    GroupCell* message_cell = dynamic_cast<GroupCell*>(cell);
    auto group_data = message_cell->get_group_data();
    if(group_data) {
        //GDM->set_select_group_id(sp_group_data->get_id());
        //GDM->set_select_group(sp_group_data);
        //GDM->set_chat_status(1);
        if(group_data->userstatus() != msg::GroupUserStatus::NOT_GROUP_MEMBER) {
            
            LayerChatOld* glc = LayerChatOld::create();
            glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
            BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
            parent->set_dismiss_callback([=](){
            });
            parent->addChild(glc, 2);
        }
        else {
            selectgroupid = group_data->groupid();
            TipView* tip = TipView::showAlertView(tools::local_string("is_request_add_group", "是否申请加入该群"));
            tip->show_cancel(true);
            tip->set_tipview_close_callback([this](){
                msg::Request request;
                request.set_type(msg::RequestType::ENTERGROUP);
                request.mutable_groupinfo()->set_groupid(selectgroupid);
                request.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
                auto processor = PM->SendRequest_up(request);
                net_manager::shared_instence().send_data(processor,REQ_ADD_FRIEND);
            });
        }
    }
}

void GroupSearchLayer::editBoxEditingDidBegin(EditBox* editBox)
{
    image_search_icon_->setVisible(false);
}

void GroupSearchLayer::editBoxEditingDidEnd(EditBox* editBox)
{
    if (strlen(editBox->getText()) == 0) {
        image_search_icon_->setVisible(true);
    } else {
        std::string key_word = tf_keyword_->getText();
        if(key_word.empty()) {
            GDM->send_search_group(msg::GroupSearchType::SEARCHALL, "");
        }
        else {
            GDM->send_search_group(msg::GroupSearchType::SEARCHBYNAME, key_word);
        }
    }
}
void GroupSearchLayer::editBoxTextChanged(EditBox* editBox, const std::string& text) {
}

void GroupSearchLayer::editBoxReturn(EditBox* editBox)
{
    
}
