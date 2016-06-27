//
//  LayerMenu.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/4/19.
//
//

#include "LayerMenu.h"

LayerMenu* LayerMenu::create(Point posWorld, cocos2d::Size sizeElement, const msg::Property& property)
{
    LayerMenu* result = new LayerMenu;
    if(result->init(posWorld, sizeElement, property)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

bool LayerMenu::init(Point posWorld, cocos2d::Size sizeElement, const msg::Property& property)
{
    if(!Layer::init()) {
        return false;
    }
    
    posWorld_ = posWorld;
    sizeElement_ = sizeElement;
    property_ = property;
    
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

    switch (property_.type())
    {
        case msg::PropertyType::ENUM:
        {
            for (int i =0; i < property_.enumvalue().items().size() ; i++)
            {
                auto item = property_.enumvalue().items(i);
//                Menu *menu = Menu::create();
//                menu->setPosition(Vec2(0, 0));
//                
//                Sprite *sprite = Sprite::create("login_res/creategameroom_res/btn_enum_normal.png");
//                MenuItemSprite *menuItemSprite = MenuItemSprite::create(sprite, sprite, this, menu_selector(LayerMenu::click_menu));
//                //menu->addChild(menuItemSprite);
//                //this->addChild(menu);
//                menuItemSprite->setTag(property_.enumvalue().items(i).value());
//                menuItemSprite->setPosition(Vec2(posWorld.x, posWorld.y - sizeElement.height * (i)));
                //                this->addChild(menuItemSprite);
                Button *cell = Button::create();

                cell->loadTextures("login_res/creategameroom_res/btn_enum_normal.png", "login_res/creategameroom_res/btn_enum_normal.png");
                                cell->setTitleFontSize(28);
                                cell->setContentSize(sizeElement);
//                menuItemSprite->setAnchorPoint(cell->getAnchorPoint());
                cell->setTitleText(item.text());

                cell->setTitleColor(cocos2d::Color3B(151, 151, 151));
                cell->setPosition(Vec2(posWorld.x,posWorld.y - sizeElement.height * (i) -1 ));
                cell->setTag(property_.enumvalue().items(i).value());
                cell->addClickEventListener(std::bind(&LayerMenu::click_menu, this,std::placeholders::_1));
                this->addChild(cell);
            }
            
        }
            break;
            
        default:
            break;
    }
    
    return true;
}

void LayerMenu::click_menu(Ref * sender)
{
    auto item = (Button*)sender;
    property_.mutable_enumvalue()->set_value(item->getTag());
    
    if (property_callback_)
    {
        property_callback_(property_);
    }
    
    this->removeFromParentAndCleanup(true);
}
