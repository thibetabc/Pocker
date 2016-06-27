//
//  MatchJoinList.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/12/2.
//
//

#include "MatchJoinList.h"
#include "MatchJoinListNode.h"
#include "GameDataManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "msg.pb.h"
#include "GameTools.h"
#include "cmd_def.h"
#include "PokerDataManager.h"

bool MatchJoinList::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto root = CSLoader::createNode("LayerJoinList.csb");
    this->addChild(root);
    btn_join = dynamic_cast<Button *>(CSLoader::seekNodeByName(root, "image_join"));
    btn_join->addClickEventListener(std::bind(&MatchJoinList::click_join, this));
    btn_cancel_match = dynamic_cast<Button *>(CSLoader::seekNodeByName(root, "btn_cancel_match"));
    btn_cancel_match->setVisible(false);
    btn_cancel_match->addClickEventListener(std::bind(&MatchJoinList::click_cancel_match,this));
    auto listview = dynamic_cast<ListView *>(CSLoader::seekNodeByName(root, "ListView_1"));
    
    auto pSize = listview->getContentSize();
    tableview = TableView::create(this, pSize);
    tableview->setAnchorPoint(listview->getAnchorPoint());
    tableview->setContentOffset(Vec2(0, 0));
    tableview->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview->setPosition(listview->getPosition());
    tableview->setDelegate(this);
//    tableview->setTag(TAG_TABLEVIEW);
    tableview->setColor(Color3B::RED);
    this->addChild(tableview,255);
    listview->removeFromParentAndCleanup(true);
//    text1 = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_1")) ;
    
    refresh(nullptr);
    
    dispatch_.register_event(EVENT_GET_TOURNAMENTIFO_LIST,BIND_FUNC(this, MatchJoinList::refresh));
    dispatch_.register_event(EVENT_TO_GET_TOURNAMENTIFO,BIND_FUNC(this, MatchJoinList::send_get_jion_list));
    return true;
}
void MatchJoinList::set_room_id(uint32_t room_id)
{
    this->room_id = room_id;
    bool isMaster = false;
    //auto room = GDM->get_room_datas();
    
//    if (room[room_id].createrinfo().userid() == GDM->get_user_id())
//    {
//        isMaster = true;
//    }
    if (isMaster)
    {
        btn_cancel_match->setVisible(true);
//        text1->setPositionX(text1->getPositionX()-150);
        btn_join->setPositionX(btn_join->getPositionX()-150);
        btn_cancel_match->setPositionX(btn_cancel_match->getPositionX()-150);
    }
}

void MatchJoinList::send_get_jion_list(cmd_data_pointer)
{
    PDM->send_enter_room(room_id);
}
void MatchJoinList::click_cancel_match()
{
    GDM->send_cancel_match(room_id);
}
void MatchJoinList::click_join()
{
    GDM->send_join_match(room_id, !hasJoin);
}
void MatchJoinList::refresh(cmd_data_pointer)
{
    auto data = GDM->get_410_data()->userinfo();
    hasJoin = false;

    for (auto i :data)
    {
        if(i.userid() == GDM->get_user_id())
        {
            hasJoin = true;
        }
    }
    if (!hasJoin)
    {
        btn_join->setTitleText("报名");
    }
    else
    {
        btn_join->setTitleText("取消报名");
    }
    
    tableview->reloadData();
}
#pragma mark tableview datasource --
TableViewCell* MatchJoinList::tableCellAtIndex(TableView *table, ssize_t idx)
{
    MatchJoinListNode* cell = dynamic_cast<MatchJoinListNode*>(table->dequeueCell());
    if(!cell) {
        cell = MatchJoinListNode::create();
    }
    auto data = GDM->get_410_data()->userinfo((int)idx);
    cell->update_data(data,(int)idx);
    
    return cell;
}

ssize_t MatchJoinList::numberOfCellsInTableView(TableView *table)
{
    ssize_t size = GDM->get_410_data()->userinfo().size();
    return size;
}
#pragma mark tableview delegate --
void MatchJoinList::tableCellTouched(TableView* table, TableViewCell* cell)
{
   
}

void MatchJoinList::tableCellHighlight(TableView* table, TableViewCell* cell)
{

}

void MatchJoinList::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

}