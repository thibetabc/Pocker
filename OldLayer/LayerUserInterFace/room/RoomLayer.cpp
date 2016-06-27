#include "RoomLayer.h"
#include "LayerGroupList.h"
#include "utils.h"
#include "GameDataManager.h"
#include "RoomInfoCell.h"
#include "cmd_def.h"
#include "RoomCreateLayer.h"
#include "GameNameCell.h"
#include "game_net_data.h"
#include "PokerDataManager.h"
#include "RoomTitle.h"
#include "MatchScroeNode.h"
#include "LayerScoreOld.h"
#include "RoomCreateMatch.h"
#include "MatchCreateType.h"
#include "TipView.h"

#include <time.h>
bool RoomLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    m_bCanTouch =false;
    top_cell_idx_ = 0;
    auto size = Director::getInstance()->getWinSize();
    
    size = Size(std::min(size.width, size.height), std::max(size.width, size.height));
    
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->addTouchCallback(std::bind(&RoomLayer::touch_image, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    this->addChild(layout, -9);
    
    select_game_idx_ = msg::RoomType::OFC3;
    auto root = CSLoader::createNode("LayerRoomList.csb");
    this->addChild(root);
    checkbox_game_status_ = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root, "checkbox_game_status"));
    checkbox_game_status_->addEventListener(std::bind(&RoomLayer::click_checkbox_game_status, this, std::placeholders::_1));
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->addClickEventListener(std::bind(&RoomLayer::click_btn_back, this, std::placeholders::_1));
    auto btn_create = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_create"));
    btn_create->addClickEventListener(std::bind(&RoomLayer::click_btn_create, this, std::placeholders::_1));
    
    image_display_offical_room_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_display_offical_room"));
    text_display_offical_room_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_display_offical_room"));
    check_display_offical_room_ = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root, "check_display_offical_room"));
    check_display_offical_room_->addEventListener(std::bind(&RoomLayer::click_check_display_offical_room, this, std::placeholders::_1));
    //check_display_offical_room_->setSelected(GDM->get_is_display_offical_groups());
    
    text_show_all_record_ = Text::create(tools::local_string("game_hint", "1选比赛或普通，2选德州奥马哈大菠萝，3点➕号选群开桌！"), "Thonburi", 22);
    text_show_all_record_->setTextColor(Color4B(100, 51, 51, 255));
    text_show_all_record_->setPosition(Point(375, text_show_all_record_->getContentSize().height));
    
    //if (GDM->get_room_layer_status() == GameDataManager::ROOM_LIST_TYPE::ALLROOM)
    //{
    //    btn_back->setVisible(false);
    //    //btn_create->setVisible(false);
    //     text_show_all_record_->setPosition(Point(375, 134+text_show_all_record_->getContentSize().height));
    // }else
    {
        image_display_offical_room_->setVisible(false);
        text_display_offical_room_->setVisible(false);
        check_display_offical_room_->setVisible(false);
        
        // if (GDM->get_select_group()->get_group_type() == msg::GroupType::OFFICALGROUP)
        //  {
        //      if (GDM->get_select_group()->get_owner_id() != GDM->get_user_id())
        //      {
        //          btn_create->setVisible(false);
        //      }
        //  }
    }
    root->addChild(text_show_all_record_, 2);
    
    //ImageView* image = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_1"));
    //image->setVisible(false);
    auto listview_name = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview_name"));
    
    //    auto image_left = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_left"));
    //    image_left->addClickEventListener(std::bind(&RoomLayer::click_image_left, this, std::placeholders::_1));
    //    image_left->setSwallowTouches(true);
    //
    //    auto image_right = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_right"));
    //    image_right->addClickEventListener(std::bind(&RoomLayer::click_image_right, this, std::placeholders::_1));
    //    image_right->setSwallowTouches(true);
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    cocos2d::Size pSize = listview->getContentSize();
    
    tableview_ = TableView::create(this, pSize);
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    tableview_->touchBeganCallback_ = std::bind(&RoomLayer::touchBegan, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchMovedCallback_ = std::bind(&RoomLayer::touchMoved, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchEndedCallback_ = std::bind(&RoomLayer::touchEnded, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchCancelledCallback_ = std::bind(&RoomLayer::touchCancel, this, std::placeholders::_1, std::placeholders::_2);
    root->addChild(tableview_);
    listview->removeFromParent();
    
    register_events();
    
    
    text_show_all_room_ = Text::create(tools::to_string(""), "Thonburi", 20);
    text_show_all_room_->setTextColor(Color4B::BLACK);
    text_show_all_room_->setPosition(Point(375, tableview_->getPosition().y + tableview_->getViewSize().height - text_show_all_room_->getContentSize().height));
    root->addChild(text_show_all_room_, -1);
    text_show_all_room_->setVisible(false);
    
    room_title_ = RoomTitle::create();
    room_title_->setPosition(listview_name->getPosition());
    room_title_->set_click_item(std::bind(&RoomLayer::click_room_item, this, std::placeholders::_1));
    this->addChild(room_title_);
    room_title_->setContentSize(listview_name->getContentSize());
    listview_name->removeFromParent();
    
    move_ = false;
    
    select_game_idx_ = msg::RoomType::NLHOLDEM;
    //send_update_data();
    return true;
}

void RoomLayer::SetTableSize()
{
    cocos2d::Size pSize = tableview_->getViewSize();
    pSize = pSize + Size(0, (-134 - image_display_offical_room_->getContentSize().height - 4));
    tableview_->setViewSize(pSize);
    tableview_->setPositionY(tableview_->getPositionY() + 134);
}
void RoomLayer::click_room_item(msg::RoomType game_idx)
{
    select_game_idx_ = game_idx;
    tableview_->reloadData();
    send_update_data();
}

void RoomLayer::click_btn_back(Ref* sender)
{
    this->removeFromParent();
}
void RoomLayer::create_sit_and_go()
{
    RoomCreateMatch *create_match = RoomCreateMatch::create(true,true);
    Director::getInstance()->getRunningScene()->addChild(create_match,2);
    create_match->set_room_type(select_game_idx_);
    create_match->set_room_id(room_id_);
}
void RoomLayer::create_time_match()
{
    
    RoomCreateMatch *create_match = RoomCreateMatch::create(true,false);
    Director::getInstance()->getRunningScene()->addChild(create_match,2);
    create_match->set_room_type(select_game_idx_);
    create_match->set_room_id(room_id_);
}

void RoomLayer::click_btn_create(Ref* sender)
{
    if (true)
    {
        auto gl = LayerGroupList::create();
        gl->set_group_layer_type(LayerGroupList::GROUPLAYERTYPE::GAMESELECTGROUP);
        gl->set_select_group_callback(std::function<void()>([=](){
            if(!checkbox_game_status_->isSelected())
            {
                RoomCreateLayer* rcl = RoomCreateLayer::create();
                rcl->click_item(select_game_idx_);
                this->getParent()->addChild(rcl);
            }
            else
            {
                MatchCreateType *typeLayer = MatchCreateType::create();
                typeLayer->roomLayer_ = this;
                this->getParent()->addChild(typeLayer);
            }
            //gl->removeFromParent();
        }));
        this->addChild(gl);
        
    }else
    {
        if(!checkbox_game_status_->isSelected())
        {
            RoomCreateLayer* rcl = RoomCreateLayer::create();
            rcl->click_item(select_game_idx_);
            this->addChild(rcl);
        }
        else
        {
            MatchCreateType *typeLayer = MatchCreateType::create();
            typeLayer->roomLayer_ = this;
            this->addChild(typeLayer);
        }
    }
}

void RoomLayer::join_list(cmd_data_pointer)
{
    bool isSitAndGo = GDM->get_410_data()->tournamentinfo().issitandgo();
    RoomCreateMatch *create_match = RoomCreateMatch::create(false,isSitAndGo);
    Director::getInstance()->getRunningScene()->addChild(create_match,2);
    create_match->set_room_type(select_game_idx_);
    create_match->set_room_id(room_id_);
    
}

void RoomLayer::click_checkbox_game_status(Ref* sender)
{
    update_room_type_list(nullptr);
}

void RoomLayer::click_check_display_offical_room(Ref * sender)
{
    GDM->set_is_display_offical_groups(check_display_offical_room_->isSelected());
    update_data_all_room();
}

void RoomLayer::scroll_ended()
{
    
}

void RoomLayer::click_image_left(Ref* sender)
{
    
    room_title_->move_to_left();
}
void RoomLayer::click_image_right(Ref* sender)
{
    
    room_title_->move_to_right();
    
}

void RoomLayer::update_data(cmd_data_pointer)
{
    update_data_all_room();
}

void RoomLayer::send_update_data()
{
    GDM->send_get_room_info();
}

void RoomLayer::update_data_all_room()
{
    tableview_->reloadData();
}

void RoomLayer::update_room_type_list(cmd_data_pointer)
{
    auto& room_info = GDM->get_room_infomations();
    room_title_->clear_all_items();
    
    GameNameCell* cell_front = GameNameCell::create();
    cell_front->update_data();
    room_title_->push_back_item(cell_front);
    bool select = false;
    uint32_t idx = 0;
    msg::RoomType new_room_type_idx;
    bool is_find_old_type = false;
    for (auto& iter : room_info.createroominfo()) {
        if (checkbox_game_status_->isSelected() && !iter.supporttournament())
        {
            continue;
        }
        GameNameCell* cell = GameNameCell::create();
        
        // by ronaldz, we get the size of the room
        //auto datas_ = GDM->get_select_group_rooms(iter.roomtype().roomtype(), iter.supporttournament());
        
        //cell->update_data(iter.roomtype().roomtype(),iter.roomtype().roomtypename(), datas_.size());
        //if(!select) { //by ronaldz. 我们想显示第二个。把这个地方改了。 这样至少显示三个。
        
        if(idx == 0 || idx == 1)
        {
            cell->set_selected(true);
            select = true;
            new_room_type_idx = iter.roomtype().roomtype();
        }
        if (select_game_idx_ == iter.roomtype().roomtype())
            is_find_old_type = true;
        
        room_title_->push_back_item(cell);
        idx++;
    }
    GameNameCell* cell_back = GameNameCell::create();
    cell_back->update_data();
    room_title_->push_back_item(cell_back);
    if (!is_find_old_type)
        select_game_idx_ = new_room_type_idx;
    
    room_title_->click_item(select_game_idx_);
    //send_update_data();
    m_bCanTouch = true;
}

void RoomLayer::handle_get_score(cmd_data_pointer data)
{
    //    auto& size = Director::getInstance()->getWinSize();
    //    auto layout = Layout::create();
    //    layout->setContentSize(size);
    //    layout->setTouchEnabled(true);
    //    layout->addClickEventListener([=](Ref*){
    //        layout->removeFromParent();
    //    });
    //    this->addChild(layout, 10);
    //
    //    MatchScroeNode* msn = MatchScroeNode::create();
    //    msn->btn_close->setVisible(false);
    ////    msn->setPosition(1136 + msn->getContentSize().width/2, 320);
    //    msn->setPosition(Point(size) * 0.5);
    //    msn->set_dismiss_callback([=](){
    ////        btn_match_score_->setTouchEnabled(true);
    //    });
    //    layout->addChild(msn, 10);
    ////    msn->runAction(MoveBy::create(0.5, Point(-msn->getContentSize().width, 0)));
    //
    //    msn->handle_get_score(data);
    LayerScoreOld* ls = LayerScoreOld::create();
    this->addChild(ls);
    ls->handle_get_score(data);
}

void RoomLayer::handle_create_room_fail(cmd_data_pointer)//
{
    click_btn_create(nullptr);
}

void RoomLayer::touch_listview_name(Ref*, ListView::EventType event)
{
    
}

#pragma mark touch
bool RoomLayer::touchBegan(Touch* touch, Event* event)
{
    
    text_show_all_record_->setVisible(false);
    tableview_touch_location_ = touch->getLocation();
    time_ = clock();
    return m_bCanTouch;
}
void RoomLayer::touchMoved(Touch* touch, Event* event)
{
    Point p = touch->getLocation();
    if(fabsf(p.y - tableview_touch_location_.y) > 200 && top_cell_idx_ < 6) {
        text_show_all_room_->setVisible(true);
    }
    else {
        text_show_all_room_->setVisible(false);
    }
}
void RoomLayer::touchEnded(Touch* touch, Event* event)
{
    
    Point p = touch->getLocation();
    
    /*if(fabsf(p.y - tableview_touch_location_.y) > 200) {
     update_data_all_room();
     }*/
    if((tableview_touch_location_.y - p.y) > 200 && top_cell_idx_ < 6)
    {
        update_data_all_room();
    }
    auto now_time = clock();
    float speed = (p.x - touch->getStartLocation().x)/(now_time - time_) * 1000;
    bool b = abs((int)(p.x - touch->getStartLocation().x)) > abs((int)(p.y - touch->getStartLocation().y));
    if (speed > 5 && b && m_bCanTouch)
    {
        room_title_->move_to_left();
    }
    if (speed < -5 && b && m_bCanTouch)
    {
        room_title_->move_to_right();
    }
    text_show_all_record_->setVisible(true);
}
void RoomLayer::touchCancel(Touch* touch, Event* event)
{
    text_show_all_record_->setVisible(true);
}

void RoomLayer::touch_image(Ref* sender, Touch* touch, ui::Widget::TouchEventType event)
{
    
    switch (event) {
        case Widget::TouchEventType::BEGAN:
            
            move_ = false;
            began_location_ = touch->getLocation();
            break;
        case Widget::TouchEventType::MOVED:{
            
            Point p = touch->getLocation();
            float move = p.x - began_location_.x;
            if(move > 3) {
                move_ = true;
                
            }
            
            break;
        }
        case Widget::TouchEventType::ENDED:{
            
            Point p = touch->getLocation();
            //            float move = p.x - began_location_.x;
            //            if(move > 220) {
            //                room_title_->move_to_left();
            //            }
            //            else if(move < -220) {
            //                room_title_->move_to_right();
            //            }
            
            move_ = false;
            break;
        }
        case Widget::TouchEventType::CANCELED:
            
            break;
            
        default:
            break;
    }
}

#pragma mark tableview datasource --
TableViewCell* RoomLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    //top_cell_idx_ = std::max(idx, top_cell_idx_);
    top_cell_idx_ = idx;
    RoomInfoCell* cell = dynamic_cast<RoomInfoCell*>(table->dequeueCell());
    if(!cell) {
        cell = RoomInfoCell::create();
    }
    auto& data = room_datas_.at(idx);
    cell->update_data(data);
    
    return cell;
}

ssize_t RoomLayer::numberOfCellsInTableView(TableView *table)
{
    return room_datas_.size();
}
#pragma mark tableview delegate --
void RoomLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    if(move_) {
        return;
    }
    RoomInfoCell* info_cell = dynamic_cast<RoomInfoCell*>(cell);
    if(info_cell)
    {
        auto& data = info_cell->get_room_data();
        //auto it = GDM->get_groups().find(data.groupid());
        //if (it != GDM->get_groups().end())
        //    GDM->set_select_group_id(it->second->get_id());
        //    GDM->set_select_group(it->second);
        //GDM->set_code(2);
        if(checkbox_game_status_->isSelected())
        {
            if(data.roomstatus() == msg::RoomStatus::WAITINGTOSTART)
            {
                dispatch_.register_event(EVENT_GET_TOURNAMENTIFO, BIND_FUNC(this, RoomLayer::join_list));
                room_id_ = data.roomid();
            }
        }
        PDM->send_enter_room(data.roomid());
    }
}

void RoomLayer::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    RoomInfoCell* message_cell = dynamic_cast<RoomInfoCell*>(cell);
    message_cell->set_is_touch();
}

void RoomLayer::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    RoomInfoCell* message_cell = dynamic_cast<RoomInfoCell*>(cell);
    message_cell->set_is_touch(false);
}

#pragma mark private
void RoomLayer::register_events()
{
    dispatch_.register_event(EVENT_HANDLE_ROOM_INFO_FINISHED,
                             BIND_FUNC(this, RoomLayer::update_room_type_list));
    dispatch_.register_event(EVENT_CANCEL_MATCH,
                             BIND_FUNC(this, RoomLayer::update_room_type_list));
    dispatch_.register_event(EVENT_CREATE_ROOM_FAIL, BIND_FUNC(this, RoomLayer::handle_create_room_fail));
}
