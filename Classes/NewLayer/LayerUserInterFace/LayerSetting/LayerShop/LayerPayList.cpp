//
//  LayerPayList.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/26.
//
//

#include "LayerPayList.h"
#include "PayManager.h"
#include "NodePayListCell.h"
#include <utils.h>

bool LayerPayList::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    auto layer = LayerColor::create(Color4B(143, 143, 143, 100));
    this->addChild(layer);
    
    root_ = CSLoader::createNode("LayerPayList.csb");
    this->addChild(root_);
    
    listview_pay_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview_pay"));
    listview_pay_->setAnchorPoint(Vec2(0, 1));
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("order_list","订单列表"));
    
    image_pay_list_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_pay_list_bg"));
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPayList::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerPayList::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerPayList::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->update_data();
    
    return true;
}
bool LayerPayList::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}
void LayerPayList::onTouchMoved(Touch* touch, Event* event)
{
    
}
void LayerPayList::onTouchEnded(Touch* touch, Event* event)
{
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    if (image_pay_list_bg_->getBoundingBox().containsPoint(touchLocation))
    {
        
    }
    else
    {
        this->removeFromParent();
    }
}
void LayerPayList::update_data()
{
    std::vector<std::pair<std::string,std::string>> data_source = PayManager::getInstance()->getiOSPayList();
    float height = 0;
    float width = 0;
    for (int i = 0; i<data_source.size(); i++)
    {
        NodePayListCell* cell = NodePayListCell::create();
        cell->update_data(data_source.at(i));
        width = cell->getContentSize().width;
        height = height + cell->getContentSize().height;
        listview_pay_->pushBackCustomItem(cell);
        
        if (height>800)
        {
            listview_pay_->setContentSize(cocos2d::Size(width, 800));
        }
        else
        {
            listview_pay_->setContentSize(cocos2d::Size(width, height));
        }
        
        listview_pay_->setInnerContainerSize(cocos2d::Size(width, height));
        listview_pay_->refreshView();
    }
    
}