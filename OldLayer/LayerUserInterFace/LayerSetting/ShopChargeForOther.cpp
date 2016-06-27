//
//  ShopChargeForOther.cpp
//  pokerclient
//
//  Created by qqronaldz on 16/3/17.
//
//

#include "ShopChargeForOther.h"
#include "PayManager.h"
#include "GameDataManager.h"
#include "LayerAddressBook.h"

bool ShopChargeForOther::init()
{
    if (!Layer::init()) {
        return false;
    }
    auto root = CSLoader::createNode("LayerChargeForOther.csb");
    this->addChild(root);
    auto btn_charge_self = dynamic_cast<cocos2d::ui::Button *>(CSLoader::seekNodeByName(root, "btn_charge_self"));
    btn_charge_self->addClickEventListener(std::bind(&ShopChargeForOther::click_charge_self,this));
    auto btn_charge_other = dynamic_cast<cocos2d::ui::Button *>(CSLoader::seekNodeByName(root, "btn_charge_other"));
    btn_charge_other->addClickEventListener(std::bind(&ShopChargeForOther::click_charge_other,this));
    auto btn_cancel = dynamic_cast<cocos2d::ui::Button *>(CSLoader::seekNodeByName(root, "btn_cancel"));
    btn_cancel->addClickEventListener(std::bind(&ShopChargeForOther::click_cancel,this));
    return true;
}
void ShopChargeForOther:: click_charge_self()
{
    //PayManager::getInstance()->SendPay(mProductID,GDM->get_user_id());
    
    this->removeFromParent();
}
void ShopChargeForOther:: click_charge_other()
{
    LayerAddressBook *friendlist = LayerAddressBook::create();
    friendlist->set_is_charge(true, mProductID);
    this->addChild(friendlist);
}
void ShopChargeForOther::click_cancel()
{
    this->removeFromParent();
}