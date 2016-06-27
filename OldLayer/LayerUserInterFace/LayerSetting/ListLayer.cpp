//
//  ListLayer.cpp
//  pokerclient
//
//  Created by ronaldz on 15/10/30.
//
//

#include "ListLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "utils.h"

USING_NS_CC;
using namespace cocos2d::ui;

ListLayer* ListLayer::create(int AViewTag, int ACellIndex){
    ListLayer* result = ListLayer::create();
    result->viewTag = AViewTag;
    result->cellIndex = ACellIndex;
    return result;
}

bool ListLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto layout = Layout::create();
     layout->setContentSize(Director::getInstance()->getWinSize());
     layout->setTouchEnabled(true);
     layout->addClickEventListener([this](Ref*) {
         this->removeFromParentAndCleanup(true);
     });
     this->addChild(layout, -1);

    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setSwallowsTouches(true);
    LayerColor * layer = LayerColor::create(Color4B(200, 200, 200, 100));
    this->addChild(layer);
    return true;
}

void ListLayer::update(Point bg_base, Point base, int distance, msg::Property *property_)
{
    if (property_ == NULL)
    {
        return;
    }
    this->property_ = property_;
    switch (property_->type())
    {
        case msg::PropertyType::ENUM:
        {
            for (int i =0; i < property_->mutable_enumvalue()->items().size() ; i++)
            {
                auto item = property_->mutable_enumvalue()->items(i);
                Menu *menu = Menu::create();
                menu->setPosition(Vec2(0, 0));
    
                Sprite *sprite = Sprite::create("login_res/setting_res/1_02.png");
                MenuItemSprite *menuItemSprite = MenuItemSprite::create(sprite, sprite, this, menu_selector(ListLayer::menu_click));
                menu->addChild(menuItemSprite);
                this->addChild(menu);
                menuItemSprite->setTag(property_->mutable_enumvalue()->items(i).value());
                menuItemSprite->setPosition(Vec2(bg_base.x,bg_base.y + distance * (i)));
//                this->addChild(menuItemSprite);
                Button *cell = Button::create();
                menuItemSprite->setAnchorPoint(cell->getAnchorPoint());
                cell->setTitleText(item.text());
                cell->setTitleFontSize(32);
                cell->setTitleColor(cocos2d::Color3B::BLACK);
                cell->setPosition(Vec2(base.x,base.y + distance * (i)));
                cell->setTag(property_->mutable_enumvalue()->items(i).value());
                cell->addClickEventListener(std::bind(&ListLayer::menu_click, this,std::placeholders::_1));
                this->addChild(cell);
            }
           
        }
            break;
            
        default:
            break;
    }
    
}

void ListLayer::menu_click(Ref *sender)
{
    auto item = (MenuItemSprite*)sender;
    property_->mutable_enumvalue()->set_value(item->getTag());
    ListView* view = (ListView*) getParent()->getChildByTag(viewTag);
    RoomPropertyCell* cell = (RoomPropertyCell*) view->getItem(cellIndex);
    cell->update_data(property_);
    this->removeFromParentAndCleanup(true);
}

bool ListLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
//    this->removeFromParentAndCleanup(true);
    return true;
}
void ListLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    //this->removeFromParentAndCleanup(true);
}