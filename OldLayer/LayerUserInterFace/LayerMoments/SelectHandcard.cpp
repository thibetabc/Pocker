//
//  SelectHandcard.cpp
//  pokerclient
//
//  Created by admin on 15-9-7.
//
//

#include "SelectHandcard.h"
#include "utils.h"

bool SelectHandcard::init()
{
    auto root = CSLoader::createNode("LayerSelectHandCard.csb");
    this->addChild(root);
    
    auto btn_publish = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_publish"));
    auto btn_cancel = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_cancel"));
    
    btn_publish->setTitleText(tools::local_string("publish", "发布"));
    btn_cancel->setTitleText(tools::local_string("cancel", "取消"));
    
    btn_publish->addClickEventListener(std::bind(&SelectHandcard::click_btn_back, this, std::placeholders::_1));
    btn_cancel->addClickEventListener(std::bind(&SelectHandcard::click_btn_create, this, std::placeholders::_1));
    
    auto text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_content"));
    text_title->setString(tools::local_string("select_HC", "选择手牌"));
    
    auto text_1 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_2"));
    text_1->setString(tools::local_string("handcard", "手牌"));
    auto text_2 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_2_0"));
    text_2->setString(tools::local_string("winer", "赢家"));
    auto text_3 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_2_1"));
    text_3->setString(tools::local_string("dichi", "底池"));
    auto text_4 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_2_2"));
    text_4->setString(tools::local_string("lookat", "查看"));
    
    auto listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "ListView_"));
    
    tableview_ = TableView::create(this, listview_->getContentSize());
    tableview_->setAnchorPoint(listview_->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 100));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview_->getPosition());
    tableview_->setDelegate(this);
    
    listview_->removeFromParent();

    
    
    return true;
}

void SelectHandcard::click_btn_back(Ref* sender)
{}
void SelectHandcard::click_btn_create(Ref* sender)
{}


#pragma mark tableview datasource --
Size SelectHandcard::cellSizeForTable(TableView *table) {
    return Size(DESIGN_WIDTH, 142);
}
TableViewCell* SelectHandcard::tableCellAtIndex(TableView *table, ssize_t idx)
{

}
ssize_t SelectHandcard::numberOfCellsInTableView(TableView *table)
{

}
#pragma mark tableview delegate --
void SelectHandcard::tableCellTouched(TableView* table, TableViewCell* cell)
{

}
