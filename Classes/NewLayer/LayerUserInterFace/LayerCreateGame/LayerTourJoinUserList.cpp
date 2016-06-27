//
//  LayerTourJoinUserList.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/5/09.
//
//

#include "LayerTourJoinUserList.h"

#include "GameDataManager.h"
#include "ProcessorManager.h"
#include "PokerDataManager.h"

#include "NET_CMD.h"

#include "NodeTourJoinUserListCell.h"
#include "GameTools.h"
#include "utils.h"

#include "TipView.h"

LayerTourJoinUserList* LayerTourJoinUserList::create(uint32_t roomID, std::vector<msg::UserDetailInfo> vecUserInfos)
{
    LayerTourJoinUserList* result = new LayerTourJoinUserList;
    if(result->init()) {
        result->autorelease();
        result->update_data(roomID, vecUserInfos);
        return result;
    }
    return nullptr;
}

bool LayerTourJoinUserList::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerTourJoinUserList.csb");
    this->addChild(root_);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerTourJoinUserList::click_btn_back, this, std::placeholders::_1));
    
    image_listview_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_listview_bg"));

    image_join_user_count_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_join_user_count_bg"));
    
    text_join_user_count_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_join_user_count"));
    
    btn_join_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_join"));
    btn_join_->addClickEventListener(std::bind(&LayerTourJoinUserList::click_btn_join, this, std::placeholders::_1));
    
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    
    register_events();
    
    return true;
}

void LayerTourJoinUserList::update_data(uint32_t roomID, std::vector<msg::UserDetailInfo> vecUserInfos)
{
    room_id_ = roomID;
    vec_user_infos_ = vecUserInfos;
    has_join_ = false;
    listview_->removeAllItems();
    
    float listviewHeightMin = 150.0;
    float listviewHeightMax = 810.0;
    float listviewHeight = 0.0;
    float listviewBgHeight = 0.0;
    
    for (auto iter : vec_user_infos_)
    {
        if (iter.userid() == GDM->get_user_id())
        {
            has_join_ = true;
        }
        NodeTourJoinUserListCell * joinUserCell = NodeTourJoinUserListCell::create(iter);
        listview_->pushBackCustomItem(joinUserCell);
        listviewHeight += joinUserCell->getContentSize().height;
    }
    
    listviewHeight = std::max(listviewHeight, listviewHeightMin);
    listviewHeight = std::min(listviewHeight, listviewHeightMax);
    listview_->setContentSize(cocos2d::Size(listview_->getContentSize().width, listviewHeight));
    
    listviewBgHeight += listviewHeight;
    image_listview_bg_->setContentSize(cocos2d::Size(image_listview_bg_->getContentSize().width, listviewBgHeight + 80));
    
    btn_join_->setPositionY(image_listview_bg_->getPositionY() - image_listview_bg_->getContentSize().height - 30);
    
    text_join_user_count_->setString("报名人数：" + tools::to_string(vecUserInfos.size()));
    btn_join_->setTitleText(has_join_ ? "取消报名" : "参加报名");
}

void LayerTourJoinUserList::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}

void LayerTourJoinUserList::click_btn_join(Ref * sender)
{
    auto processor = PM->JoinTournament_UP(room_id_, !has_join_);
    send_data(processor, REQ_SEND_JOIN);
}

void LayerTourJoinUserList::handle_join(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    TipView::showAlertView(processor.message());
    PDM->send_enter_room(room_id_);
}

void LayerTourJoinUserList::register_events()
{
    dispatch_.register_event(REQ_SEND_JOIN,
                             BIND_FUNC(this, LayerTourJoinUserList::handle_join));
}
