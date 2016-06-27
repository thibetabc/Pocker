#include "RecordLayer.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "utils.h"
#include "RecordCell.h"
#include "cmd_def.h"
#define ROOT_TAG 10001
bool RecordLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    LayerColor *layer = LayerColor::create(Color4B(100,100,100,100));
    this->addChild(layer);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
    this->setSwallowsTouches(true);
    
    auto root = CSLoader::createNode("LayerRecord.csb");
//    root->setPosition(root->getPosition() + Point(456, 0));
    this->addChild(root);
    root->setTag(ROOT_TAG);
    root_ = root;
//    auto text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    auto text_hand = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_hold_cards"));
    auto text_winner = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_winer"));
    auto text_pool = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_pot"));
    
    auto btn_close = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_1"));
    btn_close->setZOrder(1);
    btn_close->addClickEventListener([=](Ref*){
        this->click_btn_close(nullptr);
    });
    
    text_hand->setString(tools::local_string("hand_poker", "手牌"));
    text_winner->setString(tools::local_string("winner", "赢家"));
    text_pool->setString(tools::local_string("pool", "底池"));
  

    
    auto listview_record = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "ListView_1"));
    tableview_record_ = TableView::create(this, listview_record->getContentSize());
    tableview_record_->setAnchorPoint(listview_record->getAnchorPoint());
    tableview_record_->setContentOffset(Vec2(0, 0));
    tableview_record_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_record_->setPosition(listview_record->getPosition());
    tableview_record_->setDelegate(this);
    root->addChild(tableview_record_);
    
    listview_record->removeFromParent();
    
    isShow = false;
    is_select_ = false;
    
    return true;
}

void RecordLayer::set_select_handcard(bool isSelect)
{
    is_select_ = true;
}

bool RecordLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    removeFromParent();
    return true;
}

void RecordLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

void RecordLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (is_select_)
        this->removeFromParent();
}

void RecordLayer::update_data()
{
    records_ = PDM->get_records();
    tableview_record_->reloadData();
}

void RecordLayer::update_step()
{
    
    tableview_note_->reloadData();
}

Size RecordLayer::cellSizeForTable(TableView *table) {
    if(table == tableview_record_) {
        return RecordCell::size();
    }
    return Size::ZERO;
};

TableViewCell* RecordLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if(table == tableview_record_) {
        RecordCell* record_cell = dynamic_cast<RecordCell*>(cell);
        if(!record_cell) {
            record_cell = RecordCell::create();
        }
        auto& data = records_.at(idx);
        record_cell->update_data(data);
        cell = record_cell;
    }
    else if(table == tableview_note_) {

    }
    return cell;
}
ssize_t RecordLayer::numberOfCellsInTableView(TableView *table)
{
    if(table == tableview_record_) {
        return records_.size();
    }
    else if(table == tableview_note_) {
        return GetRoom()->GetReplaySteps().replaystep().size();
    }
    
    return 0;
}

void RecordLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    if(table == tableview_record_)
    {
        auto cells = table->get_cellsUsed();
        for (auto&c : cells) {
            RecordCell* rc = dynamic_cast<RecordCell*>(c);
            rc->select(false);
        }
        
        RecordCell* record_cell = dynamic_cast<RecordCell*>(cell);
        record_cell->select(true);
        if (isFriHandCard_)
        {
            gr_selected_ = record_cell->get_gameRecord();
            events::global_dispatch.raise_event(EVENT_SELECT_HANDCARD);
        }
    }
    
}


void RecordLayer::click_btn_close(Ref* sender)
{
   
    removeFromParent();

}
void RecordLayer::handle_get_history()
{
    update_step();
}

