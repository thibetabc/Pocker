#include "RoomTitle.h"
#include "GameNameCell.h"
#include "global_header.h"
#include "msg.pb.h"
bool RoomTitle::init()
{
    if(!Layout::init()) {
        return false;
    }
    
    in_game_ = false;
    current_select_idx_  = msg::RoomType::NLHOLDEM;
    inner_box_ = Node::create();
    inner_box_->setPosition(0, 0);
    clipper_ = ClippingRectangleNode::create();
    clipper_->setAnchorPoint(Point::ZERO);
    
    clipper_->setClippingRegion(Rect(0, 0, DESIGN_WIDTH, 360));
    this->addChild(clipper_);
    
    clipper_->addChild(inner_box_);
    
    this->setContentSize(Size(DESIGN_WIDTH, 360));
    this->setTouchEnabled(true);
    this->addTouchCallback(std::bind(&RoomTitle::touch_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    return true;
    
}

void RoomTitle::set_view_size(const Size& size)
{
    in_game_ = true;
    clipper_->setClippingRegion(Rect(0, 0, size.width, size.height));
    this->setContentSize(size);
}

void RoomTitle::push_back_item(GameNameCell* item)
{
    if(in_game_) {
        item->setScale(0.5f);
        item->setContentSize(item->getContentSize() / 2);
    }
    item->set_callback(std::bind(&RoomTitle::click_item, this, item->get_room_type()));
    items_.push_back(item);
    item->setAnchorPoint(Vec2(0, 0));
    if (items_.size() == 1)
    {
        item->setPosition(Point(0, 10));
    }else{
        item->setPosition(Point(inner_box_->getContentSize().width, 10));
    }
    inner_box_->addChild(item);
    inner_box_->setContentSize(cocos2d::Size(inner_box_->getContentSize().width + get_item_width(), 360));
}

void RoomTitle::clear_all_items()
{
    inner_box_->setContentSize(Size(0, 360));
    for (auto& iter : items_) {
        iter->removeFromParent();
    }
    items_.clear();
}

void RoomTitle::touch_event(Ref* sender, Touch* touch, Widget::TouchEventType event)
{
    if(items_.empty()) return;
    switch (event) {
        case Widget::TouchEventType::BEGAN:
            move_ = false;
            counter_ = 0;
            break;
        case Widget::TouchEventType::MOVED: {
            counter_++;
//            log("Widget::TouchEventType::MOVED");
            auto start1 = touch->getStartLocation();
            auto location1 = touch->getLocation();
            int speed = std::abs(start1.x - location1.x)/counter_;
            if(speed > 100)
            {
                speed = 2;
            }
            else if(speed > 70)
            {
                speed = 1.5;
            }
            else if (speed > 35) {
                speed = 1.2;
            }
            else
            {
                speed = 1;
            }

            auto pre = touch->getPreviousLocation();
            auto location = touch->getLocation();
            auto sub = (location - pre) * speed;
            inner_box_->setPosition(inner_box_->getPosition().x + sub.x, inner_box_->getPosition().y);

            move_ = true;
            break;
        }
        case Widget::TouchEventType::ENDED:
        case Widget::TouchEventType::CANCELED:{
            if(std::abs(this->getTouchBeganPosition().x - this->getTouchEndPosition().x) < 5)
            {
                move_ = false;
            }
            if(move_) {
            
                auto item = get_middle_item();
                click_item(item);
                
            }
            move_ = false;
            break;
        }
        default:
            break;
    }
}

msg::RoomType RoomTitle::get_middle_item()
{
    Point p = inner_box_->getPosition();
    float width = get_item_width();
    int idx = (DESIGN_WIDTH/2 - p.x)/width;
    if(idx < 1)
        idx = 1;
    else if(idx > items_.size() - 2)
        idx = items_.size() - 2;
    current_select_idx_ = items_.at(idx)->get_room_type();
    return current_select_idx_;
}

void RoomTitle::click_item(msg::RoomType room_type)
{
    GameNameCell *item = NULL;
    for (auto i : items_)
    {
        if (i->get_room_type() == room_type)
        {
            item = i;
        }
        
    }
    if (item == NULL)
    {
        return;
    }
    int32_t idx = -1;
    for (size_t i = 0; i < items_.size(); i++) {
        auto& it = items_.at(i);
        if(it->get_room_type() == item->get_room_type())
        {
            idx = (int)i;
            break;
        }
    }
    
    if(idx <= 0 )
        idx = 1;
    if(idx >= items_.size() - 1)
    {
        idx = (int)items_.size() - 1;
    }
    
    for (auto& iter : items_) {
        iter->set_selected(false);
        iter->setScale(0.9);
    }

    item->set_selected(true);
    
    current_select_idx_ = item->get_room_type();
    item->setScale(1.1);
    
    Point p = inner_box_->getPosition();
    if(idx >= 0 && idx <= items_.size() - 1 )
    {
        Point aim = Point((1-idx) * get_item_width(), 0);
        inner_box_->stopAllActions();
        inner_box_->runAction(MoveTo::create(0.5f, aim));
    }
    
    if(click_item_) {
        click_item_(room_type);
    }
}

float RoomTitle::get_item_width()
{
    return DESIGN_WIDTH/3;
    if(in_game_)
        return DESIGN_WIDTH/3;
}

void RoomTitle::move_to_left()
{
    for (int i=1; i<items_.size()-1; i++)
        if (items_.at(i)->get_room_type() == current_select_idx_)
        {
            if (i != 1)
                current_select_idx_ = items_.at(i-1)->get_room_type();
            break;
        }
    click_item(current_select_idx_);
}

void RoomTitle::move_to_right()
{
    for (int i=1; i<items_.size()-1; i++)
        if (items_.at(i)->get_room_type() == current_select_idx_)
        {
            if (i != items_.size()-2)
            current_select_idx_ = items_.at(i+1)->get_room_type();
            break;
        }
    click_item(current_select_idx_);
}