//
//  MatchChipLevel.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/26.
//
//

#include "MatchChipLevel.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "GameTools.h"
#include "TipView.h"
#include "GameDataManager.h"
#include "MatchChipLevelNode.h"

#define TAG_BLIND_CHIP     10001
#define TAG_PRE_CHIP       10002
#define TAG_RAISE_TIME     10003
#define TAG_REST_TIME      10004
#define TAG_TABLEVIEW      10005

bool MatchChipLevel::m_bIsCreate;

bool MatchChipLevel::init()
{
    if(!Layer::init())
    {
        return false;
    }
    this->setSwallowsTouches(true);
    Node * root;
    if (m_bIsCreate)
    {
        root = CSLoader::createNode("LayerChipLevel.csb");
    }
    else
    {
        root = CSLoader::createNode("LayerChipLevelShow.csb");
    }
    this->addChild(root);
    
    auto btn_add = dynamic_cast<Button *>(CSLoader::seekNodeByName(root, "btn_add"));
    if (btn_add)
    {
        btn_add->addClickEventListener(std::bind(&MatchChipLevel::click_btn_add, this,std::placeholders::_1));
    }
    
    auto listview = dynamic_cast<ListView *>(CSLoader::seekNodeByName(root, "ListView_1"));
    TableView *tableview;
    auto pSize = listview->getContentSize();
    tableview = TableView::create(this, pSize);
    tableview->setAnchorPoint(listview->getAnchorPoint());
    tableview->setContentOffset(Vec2(0, 0));
    tableview->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview->setPosition(listview->getPosition());
    tableview->setDelegate(this);
    tableview->setTag(TAG_TABLEVIEW);
    tableview->touchBeganCallback_ = std::bind(&MatchChipLevel::touchBegan, this, std::placeholders::_1, std::placeholders::_2);
    tableview->touchMovedCallback_ = std::bind(&MatchChipLevel::touchMoved, this, std::placeholders::_1, std::placeholders::_2);
    tableview->touchEndedCallback_ = std::bind(&MatchChipLevel::touchEnded, this, std::placeholders::_1, std::placeholders::_2);
    tableview->touchCancelledCallback_ = std::bind(&MatchChipLevel::touchCancel, this, std::placeholders::_1, std::placeholders::_2);
    this->addChild(tableview);
    listview->removeFromParentAndCleanup(true);
    //m_pTournamentInfo = GDM->get_create_room_info()->mutable_tournamentinfo();
    return true;
}

void MatchChipLevel::click_btn_add(Ref *sender)
{
    m_pTournamentInfo->mutable_tournamentlevel()->mutable_level()->Add();
    TableView *table = (TableView *)this->getChildByTag(TAG_TABLEVIEW);
    table->reloadData();
}
#pragma mark touch
bool MatchChipLevel::touchBegan(Touch* touch, Event* event)
{
    
    return true;
}
void MatchChipLevel::touchMoved(Touch* touch, Event* event)
{
    
}
void MatchChipLevel::touchEnded(Touch* touch, Event* event)
{
    
}
void MatchChipLevel::touchCancel(Touch* touch, Event* event)
{
    
}
#pragma mark tableview datasource --
TableViewCell* MatchChipLevel::tableCellAtIndex(TableView *table, ssize_t idx)
{
    MatchChipLevelNode* cell = dynamic_cast<MatchChipLevelNode*>(table->dequeueCell());
    if(!cell) {
        cell = MatchChipLevelNode::create();
    }
    msg::BlindLevel *data = nullptr;
    if (m_bIsCreate)
    {
//        data = GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentlevel()->mutable_level((int)idx);
    }
    else
    {
        data = GDM->get_410_data()->mutable_tournamentinfo()->mutable_tournamentlevel()->mutable_level((int)idx) ;
    }
    cell->update_data(data,(int)idx+1,m_bIsCreate);
    
    return cell;
}

ssize_t MatchChipLevel::numberOfCellsInTableView(TableView *table)
{
    ssize_t size ;
    
    if (!m_bIsCreate)
    {
        size = GDM->get_410_data()->mutable_tournamentinfo()->mutable_tournamentlevel()->mutable_level()->size();
    }
    else{
//        size = GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentlevel()->mutable_level()->size();
    }
    return size;
}
#pragma mark tableview delegate --
void MatchChipLevel::tableCellTouched(TableView* table, TableViewCell* cell)
{
    if(!m_bIsCreate)
    {
        return;
    }
    MatchChipLevelNode* message_cell = dynamic_cast<MatchChipLevelNode*>(cell);
    message_cell->set_edit(!message_cell->get_editing());

}

void MatchChipLevel::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    MatchChipLevelNode* message_cell = dynamic_cast<MatchChipLevelNode*>(cell);
    message_cell->set_is_touch(true);
}

void MatchChipLevel::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    MatchChipLevelNode* message_cell = dynamic_cast<MatchChipLevelNode*>(cell);
    message_cell->set_is_touch(false);
}
