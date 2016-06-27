//
//  LayerMahjongReminder.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/12.
//
//

#include "LayerMahjongReminder.h"
#include "Room.h"
#include "NodeMahjongReminder.h"
bool LayerMahjongReminder::init()
{
    if (!Node::init())
    {
        return false;
    }
    image_bg_ = ImageView::create("image/Mahjong_game_res/mahjong_reminder/image_reminder_bg.png");
    this->addChild(image_bg_);
    image_bg_->setAnchorPoint(Vec2(1, 0));
    image_bg_->setTouchEnabled(true);
    
    image_hu_ = ImageView::create("image/Mahjong_game_res/mahjong_reminder/image_hu.png");
    this->addChild(image_hu_);
    image_hu_->setAnchorPoint(Vec2(0, 0.5));
    
    return true;
}
void LayerMahjongReminder::set_data(std::vector<msg::MahjongTing> &ting)
{
    int num = ting.size();
    image_bg_->setScale(scale_X_[num-1],scale_Y_[num-1]);
    for (int i = 0; i < ting.size(); i++)
    {
        NodeMahjongReminder* node = NodeMahjongReminder::create();
        this->addChild(node);
        node->set_data(ting.at(i));
        
        if ( i > 4 )
        {
            node->setPosition(pos_[4][i%5].x,10);
        }
        else
        {
            if ( num > 5 )
            {
                node->setPosition(pos_[4][i].x,image_bg_->getContentSize().height/2 + 10);
            }
            else
            {
                node->setPosition(pos_[num-1][i].x,10);
            }
            
        }

    }
//
//    for (int i = 0 ; i < num ; i++)
//    {
//        NodeMahjongReminder* node = NodeMahjongReminder::create();
//        this->addChild(node);
//        if ( i > 4 )
//        {
//            node->setPosition(pos_[4][i%5].x,10);
//        }
//        else
//        {
//            if ( num > 5 )
//            {
//                node->setPosition(pos_[4][i].x,image_bg_->getContentSize().height/2 + 10);
//            }
//            else
//            {
//                node->setPosition(pos_[num-1][i].x,10);
//            }
//            
//        }
//        
//    }
    this->setContentSize(Size(image_bg_->getContentSize().width*scale_X_[num-1],image_bg_->getContentSize().height*scale_Y_[num-1]));
    image_bg_->setPosition(Vec2(this->getContentSize().width, 0));
    if ( num > 5 )
    {
        image_hu_->setPosition(Vec2(5, image_bg_->getContentSize().height*scale_Y_[num-1]*3/4));
    }
    else
    {
        image_hu_->setPosition(Vec2(5, image_bg_->getContentSize().height*scale_Y_[num-1]/2));
    }
    
    reminder_num_ = num;
}








