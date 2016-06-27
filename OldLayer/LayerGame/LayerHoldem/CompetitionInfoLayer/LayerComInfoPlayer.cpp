//
//  LayerComInfoPlayer.cpp
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#include "LayerComInfoPlayer.h"
#include "GameTools.h"

bool LayerComInfoPlayer::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerComInfoPlayerPortrait.csb");
    this->addChild(root_);
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    root_->addChild(tableview_);
    
    listview->removeFromParent();
    
    LayerComInfoTable* layer_table = LayerComInfoTable::create();
    this->addChild(layer_table, 10, "LayerComInfoTable");
    
    auto tf_keyword = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_player_search"));
    tf_keyword_ = GameTools::createEditBoxFromTextField(this, tf_keyword);
    tf_keyword_->setDelegate(this);
    tf_keyword_->setInputMode(EditBox::InputMode::SINGLE_LINE);
    tf_keyword_->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    
    return true;
}

void LayerComInfoPlayer::update_data(const msg::Processor_530_PokerGetTourmentStatus_DOWN&tourment_status)
{
    player_cell_datas_.clear();
    for (int i = 0; i < tourment_status.tableinfo().size(); i++)
    {
        std::shared_ptr<msg::TourmentTableInfo> player_data(new msg::TourmentTableInfo);
        *player_data = tourment_status.tableinfo(i);
        
        player_cell_datas_.push_back(player_data);
    }
    
    tableview_->reloadData();
    
    if (player_cell_datas_.size() != 0)
    {
        roomid = tourment_status.roomid();
        
        LayerComInfoTable* layer_table = dynamic_cast<LayerComInfoTable*>(this->getChildByName("LayerComInfoTable"));
        if (layer_table == nullptr)
        {
            layer_table = LayerComInfoTable::create();
            this->addChild(layer_table, 10, "LayerComInfoTable");
        }
        layer_table->set_roomid(roomid);
        layer_table->update_data(player_cell_datas_[0]);
        select_table_id_ = player_cell_datas_[0]->tableid();
    }
}

#pragma mark tableview datasource --
TableViewCell* LayerComInfoPlayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto& data = player_cell_datas_.at(idx);
    
    NodeComInfoPlayerCell* cell = dynamic_cast<NodeComInfoPlayerCell*>(table->dequeueCell());
    if (!cell){
        cell = NodeComInfoPlayerCell::create();
    }
    cell->update_data(data);
    
    if (select_table_id_ == data->tableid())
    {
        cell->set_is_select(true);
    }else
    {
        cell->set_is_select(false);
    }
    
    return cell;
}

ssize_t LayerComInfoPlayer::numberOfCellsInTableView(TableView *table)
{
    return player_cell_datas_.size();
}

#pragma mark tableview delegate --
void LayerComInfoPlayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    NodeComInfoPlayerCell* table_cell = dynamic_cast<NodeComInfoPlayerCell*>(cell);
    LayerComInfoTable* layer_table = dynamic_cast<LayerComInfoTable*>(this->getChildByName("LayerComInfoTable"));
    if (layer_table == nullptr)
    {
        layer_table = LayerComInfoTable::create();
        this->addChild(layer_table, 10, "LayerComInfoTable");
    }
    layer_table->set_roomid(roomid);
    layer_table->update_data(table_cell->get_player_cell_data().lock());
    
    for (auto & cell_iter : tableview_->get_cellsUsed())
    {
        NodeComInfoPlayerCell* used_cell = dynamic_cast<NodeComInfoPlayerCell*>(cell_iter);
        used_cell->set_is_select(false);
    }
    
    select_table_id_ = table_cell->get_player_cell_data().lock()->tableid();
    table_cell->set_is_select(true);
}

void LayerComInfoPlayer::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    
}

void LayerComInfoPlayer::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
}
void LayerComInfoPlayer::editBoxEditingDidBegin(EditBox* editBox)
{
    
}

void LayerComInfoPlayer::editBoxEditingDidEnd(EditBox* editBox)
{
    if (strlen(editBox->getText()) == 0)
    {
        
    }
}

void LayerComInfoPlayer::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    LayerComInfoTable* layer_table = dynamic_cast<LayerComInfoTable*>(this->getChildByName("LayerComInfoTable"));
    if (layer_table != nullptr)
    {
        layer_table = LayerComInfoTable::create();
        this->addChild(layer_table, 10, "LayerComInfoTable");
    }
    for (auto & data : player_cell_datas_)
    {
        int32_t tableid = data->tableid();
        if (tools::stoi(text) == tableid)
        {
            layer_table->set_roomid(roomid);
            layer_table->update_data(data);
            break;
        }
        bool isfind = false;
        for (int i = 0; i < data->player().size(); i++)
        {
            if (data->player(i).userinfo().nickname().find(text) != std::string::npos)
            {
                layer_table->set_roomid(roomid);
                layer_table->update_data(data);
                isfind = true;
                break;
            }
        }
        if (isfind)
            break;
    }
}

void LayerComInfoPlayer::editBoxReturn(EditBox* editBox)
{

    
}

