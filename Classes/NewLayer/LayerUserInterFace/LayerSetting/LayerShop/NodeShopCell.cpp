//
//  NodeShopCell.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/24.
//
//

#include "NodeShopCell.h"
bool NodeShopCell::init()
{
    if ( !TableViewCell::init() )
    {
        return false;
    }

    Size win_size = Director::getInstance()->getWinSize();
    
    cell_left_ = NodeShopListCell::create();
    this->addChild(cell_left_);
    cell_left_->setPosition(win_size.width/2-358,0);
    
    cell_right_ = NodeShopListCell::create();
    this->addChild(cell_right_);
    cell_right_->setPosition(win_size.width/2,0);

    return true;
}
void NodeShopCell::update_cell_data(std::vector<msg::GoodInfo> goods_info, bool shop)
{
    goods_info_.clear();
    for (int i = 0 ; i < goods_info.size() ; i++ )
    {
        msg::GoodInfo good;
        good.CopyFrom( goods_info[i] );
        goods_info_.push_back( good );
    }
    cell_left_->update_data( goods_info_[0] ,shop);
    if ( goods_info_.size() > 1 ) {
        cell_right_->setVisible(true);
        cell_right_->update_data(goods_info_[1],shop);
    }
    else
    {
        cell_right_->setVisible(false);
    }
}
void NodeShopCell::click_cell()
{
    
}

bool NodeShopCell::is_touch_in_left(Point pos)
{
    if ( pos.x>=cell_left_->getPositionX()+25 && pos.x<=cell_left_->getPositionX()+cell_left_->get_bg_size().width-25)
    {
        return true;
    }
    return false;
}

bool NodeShopCell::is_touch_in_right(Point pos)
{
    if (goods_info_.size()==1)
    {
        return false;
    }
    if ( pos.x>=cell_right_->getPositionX()+25 && pos.x<=cell_right_->getPositionX()+cell_right_->get_bg_size().width-25)
    {
        return true;
    }
    return false;
}
std::vector<msg::GoodInfo> NodeShopCell::get_cell_data()
{
    return goods_info_;
}
NodeShopListCell* NodeShopCell::get_left_cell()
{
    return cell_left_;
}
NodeShopListCell* NodeShopCell::get_right_cell()
{
    return cell_right_;
}
