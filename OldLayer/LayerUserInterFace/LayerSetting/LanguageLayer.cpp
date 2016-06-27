#include "LanguageLayer.h"
#include "LanguageCell.h"
#include "GameTools.h"
#include "utils.h"
#include "events.h"
#include "cmd_def.h"


bool LanguageLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerLanguage.csb");
    this->addChild(root);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    text_title_->setString(tools::local_string("language_setting", "多语言设置"));
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back_->setTitleText("    " + tools::local_string("back", "返回"));
    btn_back_->addClickEventListener(std::bind(&LanguageLayer::click_btn_back, this, std::placeholders::_1));
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    tableview_->setDataSource(this);
    root->addChild(tableview_);
    
    listview->removeFromParent();
    
    languages_ = GameTools::get_languages();
    
    tableview_->reloadData();
    return true;
}


TableViewCell* LanguageLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    LanguageCell* cell = dynamic_cast<LanguageCell*>(table->dequeueCell());
    if(!cell) {
        cell = LanguageCell::create();
    }
    auto& data = languages_.at(idx);
    cell->update_ui(data);
    
    return cell;
}
ssize_t LanguageLayer::numberOfCellsInTableView(TableView *table)
{
    return languages_.size();
}

void LanguageLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    auto c = dynamic_cast<LanguageCell*>(cell);
    auto& data = c->get_data();
    tools::set_current_language(data.second.second);
    
    events::global_dispatch.raise_event(EVENT_CHANGE_LANGUAGE);
    
    this->scheduleOnce([this](float){
        this->removeFromParent();
    }, 0.01f, "reomve");
}

void LanguageLayer::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}