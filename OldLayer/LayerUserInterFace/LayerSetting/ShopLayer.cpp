//
//  ShopLayer.cpp
//  pokerclient
//
//  Created by ronaldz on 15/10/28.
//
//

#include "ShopLayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"
#include "cocos2d.h"
#include "PayManager.h"
#include "TipView.h"
#include "ShopCell.h"
using namespace cocos2d;
using namespace cocos2d::ui;
bool ShopLayer::init()
{
    if(!BaseLayer::init())
        return false;
    auto root = cocos2d::CSLoader::createNode("LayerDiamondShop.csb");
    auto back = dynamic_cast<Button *>(CSLoader::seekNodeByName(root, "btn_back"));
    back->addClickEventListener(std::bind(&ShopLayer::click_back, this,std::placeholders::_1));
    auto layerColor = LayerColor::create(Color4B(229, 227, 227, 255));
    this->addChild(layerColor,-2);
    this->addChild(root);
    listview = dynamic_cast<ListView *>(CSLoader::seekNodeByName(root, "listview"));
    return true;
}
void ShopLayer::click_back(Ref *sender)
{
    this->removeFromParentAndCleanup(true);
}
void ShopLayer::update_data(std::vector<std::shared_ptr<msg::GoodInfo>> data , bool isDiamond)
{
    for (auto iter : data)
    {
        auto cell = ShopCell::create();
        cell->update_data(iter , isDiamond);
        listview->pushBackCustomItem(cell);
    }
}