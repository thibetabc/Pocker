//
//  GroupLayer.cpp
//  pokerclient
//
//  Created by duanmt on 15/5/3.
//
//
#include "PushManager.h"
#include "LayerGroupList.h"
#include "GroupCell.h"
#include "NET_CMD.h"
#include "GameDataManager.h"
#include "cmd_def.h"
#include "LayerChat.h"
#include "LayerCreateGroup.h"
#include "utils.h"
#include "LayerSearchGroup.h"
#include "ProcessorManager.h"
#include "LayerGroupSetting.h"
#include "ChatMessageManager.h"


#define ZORDER_UI  1000
LayerGroupList::LayerGroupList() : sizeOffset(Size::ZERO)
{
    
}
bool LayerGroupList::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerGroup_0.csb");
    this->addChild(root_);
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerGroupList::click_btn_back, this, std::placeholders::_1));
    //    btn_back_->setVisible(false);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("title_club", "俱乐部"));
    
    btn_group_search_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_group_search"));
    btn_group_search_->addClickEventListener(std::bind(&LayerGroupList::click_btn_search, this, std::placeholders::_1));
    
    btn_group_create_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_group_create"));
    btn_group_create_->addClickEventListener(std::bind(&LayerGroupList::click_btn_add, this, std::placeholders::_1));
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    tableview_->setClippingToBounds(true);
//    tableview_->setScrollEndedCallback([](){});
//    tableview_->scrollViewDidScroll(<#cocos2d::extension::ScrollView *view#>)
//    tableview_->scrollViewDidScroll(ScrollView* view){};
    
    root_->addChild(tableview_);
    
    listview->removeFromParent();
    requestGroupList();
    group_layer_type_ = LayerGroupList::GROUPLAYERTYPE::GROUPLAYER;
    
    is_select_group_ = false;
    
    return true;
}


void LayerGroupList::register_events()
{
    dispatch_.register_event(REQ_SEARCH_GROUP, BIND_FUNC(this, LayerGroupList::handle_search_group));
}


void LayerGroupList::touchend(Touch*, Event*)
{
    if (is_select_group_)
        this->removeFromParent();
}

void LayerGroupList::set_group_layer_type(enum LayerGroupList::GROUPLAYERTYPE group_layer_type)
{
    group_layer_type_ = group_layer_type;
    if (group_layer_type_ == LayerGroupList::GROUPLAYERTYPE::GAMESELECTGROUP)
    {
        btn_group_search_->setVisible(false);
        btn_group_create_->setVisible(false);
        text_title_->setString(tools::local_string("choose_club","选择俱乐部"));
        btn_back_->setVisible(true);
        tableview_->touchEndedCallback_ = std::bind(&LayerGroupList::touchend, this, std::placeholders::_1, std::placeholders::_2);
    }else
    {
        btn_group_search_->setVisible(true);
        btn_group_create_->setVisible(true);
        text_title_->setString(tools::local_string("GoodHand", "好牌"));
        btn_back_->setVisible(false);
        tableview_->touchEndedCallback_ = nullptr;
    }
}

void LayerGroupList::update_unread_message_count()
{
    Vector<TableViewCell*> cellsUsed = tableview_->get_cellsUsed();
    if (cellsUsed.size() > 0)
    {
        for (auto cell_iter : cellsUsed)
        {
            GroupCell * groupCellIter = dynamic_cast<GroupCell*>(cell_iter);
            if (groupCellIter)
            {
                groupCellIter->update_unread_message_count();
            }
        }
    }
}

void LayerGroupList::click_btn_back(Ref* sender)
{
    this->removeFromParent();
}

void LayerGroupList::click_btn_add(Ref* sender)
{
    auto layer = LayerCreateGroup::create();
    this->addChild(layer);
}

void LayerGroupList::click_btn_search(Ref* sender)
{
    auto layer = LayerSearchGroup::create();
    AddGlobalLayer(layer);
}

void LayerGroupList::click_btn_menu(Ref* sender)
{

}

#pragma mark event call
void LayerGroupList::handle_search_group(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    
    auto processor = PM->SearchGroup_Down(rec);
    auto response = processor.response();
    auto status = response.status();
    switch (status) {
        case msg::Status::SUCCESS: {
            countGroupList = groups_.size();
            groups_.clear();
            for (auto iterGroup : processor.groupinfo())
            {
                if (!(group_layer_type_ == GROUPLAYERTYPE::GAMESELECTGROUP && !iterGroup.allowcreateroom()))
                {
                    groups_.push_back(iterGroup);
                }
            }
            
            //聊天时间排序
            auto sortFunc = [&](const msg::GroupInfo &data1, const msg::GroupInfo data2)
            {
                auto lastmessage1 = CM->get_last_message(data1.groupid(), ChatMessageManager::MESSAGETYPE::GROUPCHAT);
                if(!lastmessage1)
                {
                    return false;
                }
                auto lastmessage2 = CM->get_last_message(data2.groupid(), ChatMessageManager::MESSAGETYPE::GROUPCHAT);
                if(!lastmessage2)
                {
                    return true;
                }
                
                const std::string submit_time1 = lastmessage1->subtime();
                if(submit_time1.empty())
                {
                    return false;
                }
                const std::string submit_time2 = lastmessage2->subtime();
                if(submit_time2.empty())
                {
                    return true;
                }
                
                return tools::stod(submit_time1) >  tools::stod(submit_time2);
            };
            
            std::sort(groups_.begin(), groups_.end(), sortFunc);
            tableview_->reloadData();
            if(countGroupList == groups_.size() && !sizeOffset.equals(Size::ZERO))
            {
                tableview_->setContentOffset(sizeOffset);
            }
            break;
        }
        case msg::Status::FAILED:
            ShowError(response.message());
            break;
        default:
            break;
    }
   
    
}

#pragma mark tableview datasource --
TableViewCell* LayerGroupList::tableCellAtIndex(TableView *table, ssize_t idx)
{
    GroupCell* cell = dynamic_cast<GroupCell*>(table->dequeueCell());
    if(!cell)
    {
        cell = GroupCell::create();
    }
    auto data = groups_.at(idx);
    cell->update_property(data);
    
    return cell;
}

ssize_t LayerGroupList::numberOfCellsInTableView(TableView *table)
{
    return groups_.size();
}

#pragma mark tableview delegate --
void LayerGroupList::tableCellTouched(TableView* table, TableViewCell* cell)
{
    ClearError();
    GroupCell* message_cell = dynamic_cast<GroupCell*>(cell);
    auto group_data = groups_.at(cell->getIdx());
    sizeOffset = table->getContentOffset();
    
    //GDM->set_select_group_id(sp_group_data->get_id());
    //GDM->set_select_group(sp_group_data);
    //GDM->set_chat_status(1);
    //        if(sp_group_data->get_is_member())
    //        {
    //            LayerChat* glc = LayerChat::create();
    //            glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
    //            BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
    //            parent->addChild(glc);
    //
    //            GDM->clear_unreal_message_by_group_id(GDM->get_select_group()->get_id());
    //
    //            message_cell->update_property(sp_group_data);
    //        }
    //        else
    //        {
    ////            BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
    ////            EnterGroupLayer* egl = EnterLayerGroupList::create();
    ////            parent->addChild(egl);
    //        }
    if (group_layer_type_ == LayerGroupList::GROUPLAYERTYPE::GROUPLAYER)
    {
        
        //        GDM->clear_unreal_message_by_group_id(group_data->groupid());
        //
        //        message_cell->update_property(group_data);
        
        log("进入俱乐部聊天");
        is_select_group_ = false;
        LayerChat* glc = LayerChat::createByGroup(group_data.groupid());
        glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
        AddGlobalLayer(glc);
        
        message_cell->update_ui();
    }
    else if (group_layer_type_ == LayerGroupList::GROUPLAYERTYPE::GAMESELECTGROUP)
    {
        if (group_data.type() == msg::GroupType::OFFICALGROUP)
        {
            ShowError(tools::local_string("create_no_club","无法在官方俱乐部创建房间"));
            is_select_group_ = false;
        }
        else if (select_group_callback_)
        {
            select_group_callback_(group_data);
            is_select_group_ = true;
        }
        
    }
}

void LayerGroupList::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    GroupCell* message_cell = dynamic_cast<GroupCell*>(cell);
    message_cell->set_is_touch();
}

void LayerGroupList::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    GroupCell* message_cell = dynamic_cast<GroupCell*>(cell);
    message_cell->set_is_touch(false);
}

//请求列表
void LayerGroupList::requestGroupList()
{
    auto processor = PM->SearchGroup_UP(msg::GroupSearchType::SEARCHMINE, "");
    send_data(processor, REQ_SEARCH_GROUP);
}

void LayerGroupList::RefreshMessage()
{
    auto map = CM->get_messages_by_type(ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    bool bNeedUpdate = false;
    for (auto iter = map->begin(); iter != map->end(); iter++){
        bool bFind = false;
        for (auto group: groups_)
            if (group.groupid() == iter->first){
                bFind = true;
                break;
            }
        if (!bFind){
            bNeedUpdate = true;
            break;
        }
    }
    if (bNeedUpdate)
    {
        requestGroupList();
    }
    else
    {
        tableview_->reloadData();
    }
}

void LayerGroupList::RefreshLayer()
{
    requestGroupList();
}
