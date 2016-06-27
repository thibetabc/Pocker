//
//  GroupPushSetting.cpp
//  pokerclient
//
//  Created by ronaldz on 15/10/29.
//
//

#include "GroupPushSetting.h"
#include "cocos2d.h"
#include "utils.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameDataManager.h"
#include "GroupPushSettingCell.h"
USING_NS_CC;
bool GroupPushSetting::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    auto root = CSLoader::createNode("LayerGroupPushSetting.csb");
    this->addChild(root);
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->addClickEventListener(std::bind(&GroupPushSetting::click_btn_back, this,std::placeholders::_1));
    auto text_save = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "text_save"));
    text_save->addClickEventListener(std::bind(&GroupPushSetting::click_btn_save_, this,std::placeholders::_1));
    this->listview_property_ = dynamic_cast<cocos2d::ui::ListView *>(CSLoader::seekNodeByName(root, "ListView_1"));
    
    
    return true;
}

void GroupPushSetting::click_btn_back(cocos2d::Ref *sender)
{
    this->removeFromParentAndCleanup(true);
}

void GroupPushSetting::click_btn_save_(cocos2d::Ref *sender)
{
    
}

void GroupPushSetting::UpdateCellData()
{
    listview_property_->removeAllItems();
    msg::UserGameSetting * setting ;
    for (int i=0; i<setting->property_size(); i++) {
        if(setting->mutable_property(i)->type() != msg::PropertyType::BOOL)
            continue;
        GroupPushSettingCell* cell = GroupPushSettingCell::create();
        cell->update(setting->mutable_property(i));
        listview_property_->pushBackCustomItem(cell);
        cell->m_pListView = listview_property_;
    }

}