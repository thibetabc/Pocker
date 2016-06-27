//
//  LayerGroupMenu.cpp
//  pokerclient
//
//  Created by ll on 15/10/13.
//
//

#include "LayerGroupMenu.h"
#include "PokerDataManager.h"
#include "game_net_data.h"
#include "utils.h"

bool LayerGroupMenu::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    auto layout = Layout::create();
    layout->setContentSize(Director::getInstance()->getWinSize());
    layout->setTouchEnabled(true);
    layout->addClickEventListener([this](Ref*) {
        this->setVisible(false);
    });
    this->addChild(layout, -1);
    
    auto root = CSLoader::createNode("LayerGroupMenu.csb");
    this->addChild(root);
    
    void click_group_create(Ref* sender);
    void click_group_search(Ref* sender);
    
    auto btn_group_create = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_group_create"));
    btn_group_create->setTitleText("          " + tools::local_string("create_group", "创建群"));
    btn_group_create->addClickEventListener(std::bind(&LayerGroupMenu::click_group_create, this, std::placeholders::_1));

    auto btn_group_search = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_group_search"));
    btn_group_search->setTitleText("          " + tools::local_string("search_group", "搜索群"));
    btn_group_search->addClickEventListener(std::bind(&LayerGroupMenu::click_group_search, this, std::placeholders::_1));
    

    return true;
}

void LayerGroupMenu::click_group_create(Ref* sender)
{
    if(click_group_create_) {
        click_group_create_();
    }
    removeFromParent();
}

void LayerGroupMenu::click_group_search(Ref* sender)
{
    if(click_group_search_) {
        click_group_search_();
    }
    removeFromParent();
}

void LayerGroupMenu::onExit()
{
    if(dismiss_callback_) {
        dismiss_callback_();
    }
    LayerPop::onExit();
}
