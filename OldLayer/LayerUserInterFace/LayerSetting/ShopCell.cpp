//
//  ShopCell.cpp
//  pokerclient
//
//  Created by qqronaldz on 16/1/4.
//
//

#include "ShopCell.h"
#include "utils.h"
#include "PayManager.h"
#include "TipView.h"
#include "GameDataManager.h"
#include "ShopChargeForOther.h"
#include "ProcessorManager.h"

bool ShopCell::init()
{
    if (!Widget::init())
    {
        return false;
    }
    auto root = CSLoader::createNode("NodeShopCell.csb");
    this->addChild(root);
    bg = dynamic_cast<ImageView *>(CSLoader::seekNodeByName(root, "Image_1"));
    bg->addClickEventListener(std::bind(&ShopCell::click_image_bg,this));
    name = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_name"));
    descrip = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_2"));
    money = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_3"));
    this->setContentSize(cocos2d::Size(750,138));
    bg->loadTexture("res/login_res/setting_res/setting_cell_bg_1.png");
    return true;
    
}
void ShopCell::update_data(const std::shared_ptr<msg::GoodInfo> &data , bool isDiamond)
{
    name->setString(data->name());
    descrip->setString(data->description());
    std::string sign = "￥";
    if (isDiamond)
    {
        sign = "钻石";
    }
    money->setString(sign + tools::anything2string(data->price()));
    ID = data->id();
    this->m_bIsdiamond = isDiamond;
}
void ShopCell::set_clicked(bool IsClicked)
{
    if (IsClicked)
    {
        bg->loadTexture("res/login_res/setting_res/shop_cell_click.png");
    }
    else
    {
        bg->loadTexture("res/login_res/setting_res/setting_cell_bg_1.png");
    }
}
void ShopCell::click_image_bg()
{

    auto listview = (ListView *)this->getParent()->getParent();
    Vector<Widget*> items= listview->getItems();
    for (auto item : items)
    {
        ((ShopCell *)item)->set_clicked(false);
    }
    this->set_clicked(true);
    if (m_bIsdiamond)
    {
        //auto processor = PM->buygood_up();
    }
    else
    {
        ShopChargeForOther *tipChargeFor = ShopChargeForOther::create();
        tipChargeFor->set_productID(ID);
        this->getParent()->getParent()->getParent()->addChild(tipChargeFor);
    }

}
