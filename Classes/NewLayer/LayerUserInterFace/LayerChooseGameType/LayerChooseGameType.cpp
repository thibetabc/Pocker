//
//  LayerChooseGameType.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/27.
//
//

#include "LayerChooseGameType.h"
#include "NodeChooseGameTypeCell.h"
#include "utils.h"

bool LayerChooseGameType::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerChooseGameType.csb");
    this->addChild(root_);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("select_type","选择类型"));
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerChooseGameType::click_btn_back, this, std::placeholders::_1));
    
    image_layer_choosegametype_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_layer_choosegameroom_bg"));
    
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    tableview_ = TableView::create(this, listview_->getContentSize());
    tableview_->setAnchorPoint(listview_->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
       // tableview_->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview_->getPosition());
    tableview_->setDelegate(this);

    root_->addChild(tableview_);
    listview_->removeFromParent();
    
    room_type_datas_.push_back(CHOOSE_ROOM_TYPE::NLHOLDEM);
    room_type_datas_.push_back(CHOOSE_ROOM_TYPE::OFC3);
    room_type_datas_.push_back(CHOOSE_ROOM_TYPE::OMAHA);
    room_type_datas_.push_back(CHOOSE_ROOM_TYPE::OFC4);
    room_type_datas_.push_back(CHOOSE_ROOM_TYPE::SIChUANMAHJONG);
    room_type_datas_.push_back(CHOOSE_ROOM_TYPE::SNG);

    tableview_->reloadData();
    
    return true;
}

void LayerChooseGameType::select_choose_room_type(CHOOSE_ROOM_TYPE choose_room_type)
{
    if (choose_room_type == CHOOSE_ROOM_TYPE::SNG)
    {
        LayerChooseGameType* chooseGameType = LayerChooseGameType::create();
        addChild(chooseGameType);
        chooseGameType->set_choose_call_func(choose_room_type_callfunc_);
        
        std::vector<CHOOSE_ROOM_TYPE> room_type_datas;
        room_type_datas.push_back(CHOOSE_ROOM_TYPE::MTT_NLHOLDEM);
        room_type_datas.push_back(CHOOSE_ROOM_TYPE::MTT_PLO);
        room_type_datas.push_back(CHOOSE_ROOM_TYPE::MTT_PLOHILO);
        chooseGameType->update_data(room_type_datas);
    }else if (choose_room_type == CHOOSE_ROOM_TYPE::OMAHA)
    {
        LayerChooseGameType* chooseGameType = LayerChooseGameType::create();
        addChild(chooseGameType);
        chooseGameType->set_choose_call_func(choose_room_type_callfunc_);
        
        std::vector<CHOOSE_ROOM_TYPE> room_type_datas;
        room_type_datas.push_back(CHOOSE_ROOM_TYPE::PLO);
        room_type_datas.push_back(CHOOSE_ROOM_TYPE::PLOHILO);
        
        chooseGameType->update_data(room_type_datas);
        
    }else if(choose_room_type_callfunc_)
    {
        bool istournament;
        msg::RoomType roomtype;
        switch (choose_room_type) {
            case CHOOSE_ROOM_TYPE::NLHOLDEM:
            {
                roomtype = msg::RoomType::NLHOLDEM;
                istournament = false;
                break;
            }
            case CHOOSE_ROOM_TYPE::OFC3:
            {
                roomtype = msg::RoomType::OFC3;
                istournament = false;
                break;
            }
            case CHOOSE_ROOM_TYPE::OFC4:
            {
                roomtype = msg::RoomType::OFC4;
                istournament = false;
                break;
            }
            case CHOOSE_ROOM_TYPE::PLO:
            {
                roomtype = msg::RoomType::PLO;
                istournament = false;
                break;
            }
            case CHOOSE_ROOM_TYPE::PLOHILO:
            {
                roomtype = msg::RoomType::PLOHILO;
                istournament = false;
                break;
            }
            case CHOOSE_ROOM_TYPE::SIChUANMAHJONG:
            {
                roomtype = msg::RoomType::SIChUANMahjong;
                istournament = false;
                break;
            }
            case CHOOSE_ROOM_TYPE::MTT_NLHOLDEM:
            {
                roomtype = msg::RoomType::NLHOLDEM;
                istournament = true;
                break;
            }
            case CHOOSE_ROOM_TYPE::MTT_PLO:
            {
                roomtype = msg::RoomType::PLO;
                istournament = true;
                break;
            }
            case CHOOSE_ROOM_TYPE::MTT_PLOHILO:
            {
                roomtype = msg::RoomType::PLOHILO;
                istournament = true;
                break;
            }
            default:
                break;
        }
        tableview_->touchEndedCallback_ = nullptr;
        choose_room_type_callfunc_(roomtype, istournament);
        //上面回调会被释放，后面不能有代码 by zhangch
    }
}

void LayerChooseGameType::update_data(std::vector<CHOOSE_ROOM_TYPE> room_type_data)
{
    room_type_datas_ = room_type_data;
    
    tableview_->reloadData();
}

void LayerChooseGameType::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}

#pragma mark tableview datasource --
TableViewCell* LayerChooseGameType::tableCellAtIndex(TableView *table, ssize_t idx)
{
    NodeChooseGameTypeCell * cell = dynamic_cast<NodeChooseGameTypeCell*>(table->dequeueCell());
    if (!cell)
    {
        cell = NodeChooseGameTypeCell::create();
    }
    cell->update_data(room_type_datas_[idx]);
    
    return cell;
}

Size LayerChooseGameType::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return cocos2d::Size(750, 190);
}

ssize_t LayerChooseGameType::numberOfCellsInTableView(TableView *table)
{
    return room_type_datas_.size();
}

#pragma mark tableview delegate --
void LayerChooseGameType::tableCellTouched(TableView* table, TableViewCell* cell)
{
    NodeChooseGameTypeCell * touch_cell = dynamic_cast<NodeChooseGameTypeCell*>(cell);
    tableview_->touchEndedCallback_ = ([=](Touch *pTouch, Event *pEvent){
        select_choose_room_type(touch_cell->get_data());
        }
    );
}
