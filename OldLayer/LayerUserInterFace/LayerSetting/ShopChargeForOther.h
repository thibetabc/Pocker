//
//  ShopChargeForOther.hpp
//  pokerclient
//
//  Created by qqronaldz on 16/3/17.
//
//

#ifndef ShopChargeForOther_hpp
#define ShopChargeForOther_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
class ShopChargeForOther : public cocos2d::Layer
{
public:
    CREATE_FUNC(ShopChargeForOther);
    bool init();
    void click_charge_self();
    void click_charge_other();
    void click_cancel();
    void set_productID(std::string id){mProductID = id;}
private:
    std::string mProductID;
};
#endif /* ShopChargeForOther_hpp */
