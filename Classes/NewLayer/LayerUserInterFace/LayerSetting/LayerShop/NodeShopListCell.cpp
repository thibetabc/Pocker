//
//  NodeShopListCell.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/24.
//
//

#include "NodeShopListCell.h"
#include "utils.h"
#include "FileManager.h"
#include "GameDataManager.h"
#include "cmd_def.h"
//#include "File"
bool NodeShopListCell::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeShopListCell.csb");
    this->addChild(root_);
    
    text_shop_desc_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_shop_desc"));
    text_shop_desc_->setString("");
    
    text_diamond_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_diamond_num"));
    text_money_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_money_num"));
    image_icon_diamond_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_icon_diamond"));
    image_shop_cell_bg_normal_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_shop_cell_bg_normal"));
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, NodeShopListCell::handle_download_event));
    return true;
}

void NodeShopListCell::update_data(msg::GoodInfo goodInfo,bool shop)
{
    good_info_.Clear();
    good_info_.CopyFrom(goodInfo);
    text_diamond_num_->setString(good_info_.name());
    if (shop)
    {
        text_money_num_->setString("￥"+tools::to_string(good_info_.price()));
        text_shop_desc_->setString("");
    }
    else
    {
        text_money_num_->setString(tools::local_string("diamond","钻石")+tools::to_string(good_info_.price()));
        text_shop_desc_->setString(good_info_.description());
    }
    
    if (shop)
    {
        image_icon_diamond_->loadTexture(good_info_.picname());
    }
    else
    {
        if (GDM->is_file_exist(good_info_.picname()))
        {
            image_icon_diamond_->loadTexture(FM.get_file_name(good_info_.picname()));
        }
        else
        {
//            ssize_t index = good_info_.picname().find("/");
//            if ( index > 0 )
//            {
//                std::string new_file = good_info_.picname().substr(0,index);
//                std::string new_path = GDM->get_user_directory() + new_file;
////                if (GDM->is_file_exist(new_path))
////                {
//                    cocos2d::FileUtils::getInstance()->createDirectory(new_path);
////                }
//            }
            FM.download_file(good_info_.picname());
        }
    }
    
}

Size NodeShopListCell::get_bg_size()
{
    return image_shop_cell_bg_normal_->getContentSize();
}

void NodeShopListCell::cell_bg_load_image(bool is_normal)
{
    if (is_normal)
    {
        image_shop_cell_bg_normal_->loadTexture("login_res/shop_res/image_shop_cell_bg_normal.png");
    }
    else
    {
        image_shop_cell_bg_normal_->loadTexture("login_res/shop_res/image_shop_cell_bg_pressed.png");
    }
}
void NodeShopListCell::handle_download_event(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (s->get_value() != FM.get_file_name(good_info_.picname())) return;
    assert (GDM->is_file_exist(good_info_.picname()));
    image_icon_diamond_->loadTexture(FM.get_file_name(good_info_.picname()));
}
