//
//  LayerSearchGroup.cpp
//  pokerclient
//  搜索俱乐部 界面
//
//  Created by duanmt on 15/5/13.
//
//

#include "LayerSearchGroup.h"
#include "GroupCell.h"
#include "GameDataManager.h"
#include "utils.h"
#include "GameTools.h"
#include "cmd_def.h"
#include "LayerChat.h"
#include "TipView.h"
#include "ProcessorManager.h"
#include "NET_CMD.h"


bool LayerSearchGroup::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerGroupSearch.csb");
    this->addChild(root);
    
    auto btn_cancel = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    auto text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    auto btn_search = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_search"));
    auto text_contacts = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_contacts"));
    auto tf_keyword = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_keyword"));
    
    tf_keyword_ = GameTools::createEditBoxFromTextField(this, tf_keyword);
    tf_keyword_->setPlaceHolder(tools::local_string("group_search_word", "请输入俱乐部名称").c_str());
    tf_keyword_->setDelegate(this);
    tf_keyword_->setInputMode(EditBox::InputMode::SINGLE_LINE);
    tf_keyword_->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    image_search_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_search_icon"));
    
    
    text_title->setString(tools::local_string("search_group", "搜索俱乐部"));
    btn_search->setVisible(false);
    text_contacts->setVisible(false);
    
    btn_cancel->addClickEventListener(std::bind(&LayerSearchGroup::text_cancel_click, this, std::placeholders::_1));
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    auto listviewSize = listview->getContentSize();
    tableview_ = TableView::create(this, Size(listviewSize.width, listviewSize.height - 50));
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    root->addChild(tableview_);
    
    listview->removeFromParent();
    
    
//    sendSearchMsg(msg::GroupSearchType::SEARCHRECOMMAND, "");
    return true;
}


void LayerSearchGroup::onEnterTransitionDidFinish()
{
}



void LayerSearchGroup::text_cancel_click(Ref* sender)
{
    removeFromParent();
}



#pragma mark tableview datasource --
TableViewCell* LayerSearchGroup::tableCellAtIndex(TableView *table, ssize_t idx)
{
    GroupCell* cell = dynamic_cast<GroupCell*>(table->dequeueCell());
    if(!cell) {
        cell = GroupCell::create();
    }
    auto data = groups_.at(idx);
    cell->update_property(data);
    return cell;
}
ssize_t LayerSearchGroup::numberOfCellsInTableView(TableView *table)
{
    return groups_.size();
}

#pragma mark tableview delegate --
void LayerSearchGroup::tableCellTouched(TableView* table, TableViewCell* cell)
{
    GroupCell* message_cell = dynamic_cast<GroupCell*>(cell);
    auto group_data = message_cell->get_group_data();
    int groupId = group_data.groupid();
    log("groupid = %d", groupId);
    
    //GDM->set_select_group_id(sp_group_data->get_id());
    //GDM->set_select_group(sp_group_data);
    //GDM->set_chat_status(1);
    if(group_data.userstatus() != msg::GroupUserStatus::NOT_GROUP_MEMBER)
    {
        log("进入俱乐部聊天");
        
        LayerChat* glc = LayerChat::createByGroup(groupId);
        glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
                AddGlobalLayer(glc);

//        BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
//        parent->addChild(glc);
//        this->removeFromParent();
    }
    else
    {
        TipView* tip = TipView::showAlertView(tools::local_string("is_request_add_group", "是否申请加入该俱乐部"));
        tip->show_cancel(true);
        tip->set_tipview_close_callback([&,groupId]()
        {
            msg::Request request;
            request.set_type(msg::RequestType::ENTERGROUP);
            int userId = GDM->get_user_id();
            log("groupid = %d, userId = %d", groupId, userId);
            request.mutable_userdetailinfo()->set_userid(userId);
            request.mutable_groupinfo()->set_groupid(groupId);
            
            auto processor = PM->SendRequest_up(request);
            send_data(processor,REQ_ADD_FRIEND);
        });
    }
}

void LayerSearchGroup::editBoxEditingDidBegin(EditBox* editBox)
{
    image_search_icon_->setVisible(false);
}

void LayerSearchGroup::editBoxEditingDidEnd(EditBox* editBox)
{
    if (strlen(editBox->getText()) == 0) {
        image_search_icon_->setVisible(true);
    } else {
        std::string key_word = tf_keyword_->getText();
        if(key_word.empty()) {
            
        }
        else {
            sendSearchMsg(msg::GroupSearchType::SEARCHBYNAME, key_word);
        }
    }
}
void LayerSearchGroup::editBoxTextChanged(EditBox* editBox, const std::string& text) {
}

void LayerSearchGroup::editBoxReturn(EditBox* editBox)
{
    
}

void LayerSearchGroup::register_events()
{
    dispatch_.register_event(REQ_SEARCH_GROUP, BIND_FUNC(this, LayerSearchGroup::handle_search_group));
    
    dispatch_.register_event(REQ_ADD_FRIEND,
                             BIND_FUNC(this, LayerSearchGroup::handle_add_group));
}

void LayerSearchGroup::handle_search_group(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    msg::Processor_142_SearchGroup_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    auto response = processor.response();
    auto status = response.status();
    
    switch (status) {
        case msg::Status::SUCCESS: {
            groups_.clear();
            for (auto iter : processor.groupinfo())
            {
                groups_.push_back(iter);
            }
            tableview_->reloadData();
            break;
        }
        case msg::Status::FAILED:
            TipView::showAlertView(response.message());
            break;
        default:
            break;
    }
    
}

void LayerSearchGroup::handle_add_group(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    msg::Response processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    TipView::showAlertView(processor.message());
    
}
void LayerSearchGroup::sendSearchMsg(msg::GroupSearchType type, const std::string key_word)
{
    msg::Processor_142_SearchGroup_UP processor;
    processor.set_type(type);
    processor.set_searchname(key_word);
    send_data(processor, REQ_SEARCH_GROUP);
}
