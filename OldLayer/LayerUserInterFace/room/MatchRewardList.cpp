//
//  MatchRewardList.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/30.
//
//

#include "MatchRewardList.h"
#include "MatchRewardNode.h"
#include "MatchRewardAdd.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "msg.pb.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "cmd_def.h"

#define TAG_TABLEVIEW 10001
bool MatchRewardList::m_bIsCreate;

bool MatchRewardList::init()
{
    if (!Layer::init())
    {
        return false;
    }
   
    root = CSLoader::createNode("LayerMatchRewardShow.csb");
    
    this->addChild(root);
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
    this->addChild(tableview);
    listview->removeFromParentAndCleanup(true);
    auto btn_add = dynamic_cast<Button *>(CSLoader::seekNodeByName(root, "btn_add"));
    if (btn_add)
    {
        btn_add->addClickEventListener(std::bind(&MatchRewardList::click_btn_add, this,std::placeholders::_1));
    }
    events::event_dispatch dispatch_;
    dispatch_.register_event(EVENT_REFRESH_PRIZE_LIST,BIND_FUNC(this, MatchRewardList::refresh_list));
//    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MatchRewardList::refresh_list), "refresh_match_reward_list", NULL);
    return true;
}
void MatchRewardList::refresh_list(cmd_data_pointer data)
{
    auto table = (TableView *)this->getChildByTag(TAG_TABLEVIEW);
    table->reloadData();
}

void MatchRewardList::click_btn_add(Ref *sender)
{
    
    
}
#pragma mark tableview datasource --
TableViewCell* MatchRewardList::tableCellAtIndex(TableView *table, ssize_t idx)
{
    MatchRewardNode* cell = dynamic_cast<MatchRewardNode*>(table->dequeueCell());
    if(!cell) {
        cell = MatchRewardNode::create();
    }
    
    if(!m_bIsCreate)
    {
        m_pTournamentPrize = GDM->get_410_data()->mutable_tournamentinfo()->mutable_tournamentprize();
    }
    else
    {
//        m_pTournamentPrize = GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentprize();
    }
    auto data = m_pTournamentPrize->mutable_level((int)idx) ;
    cell->update_data(data,(int)idx,m_bIsCreate);
    
    return cell;
}

ssize_t MatchRewardList::numberOfCellsInTableView(TableView *table)
{
    ssize_t size;
    if (m_bIsCreate)
    {
//        size = GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentprize()->mutable_level()->size();
    }
    else
    {
        size = GDM->get_410_data()->mutable_tournamentinfo()->mutable_tournamentprize()->mutable_level()->size();
    }
    return size;
}
#pragma mark tableview delegate --
void MatchRewardList::tableCellTouched(TableView* table, TableViewCell* cell)
{
    MatchRewardNode *node = (MatchRewardNode *)cell;
    node->click_edit();
}

void MatchRewardList::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    MatchRewardNode* message_cell = dynamic_cast<MatchRewardNode*>(cell);
    message_cell->set_is_touch(true);
}

void MatchRewardList::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    MatchRewardNode* message_cell = dynamic_cast<MatchRewardNode*>(cell);
    message_cell->set_is_touch(false);
}
