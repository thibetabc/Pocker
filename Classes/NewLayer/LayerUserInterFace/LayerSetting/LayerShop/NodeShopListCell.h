//
//  NodeShopListCell.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/24.
//
//

#ifndef NodeShopListCell_hpp
#define NodeShopListCell_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeShopListCell : public Node
{
public:
    CREATE_FUNC(NodeShopListCell);
    virtual bool init();
private:
#pragma mark ui
    Node* root_;
    ImageView* image_shop_cell_bg_normal_;
    ImageView* image_icon_diamond_;
    Text* text_diamond_num_;
    Text* text_money_num_;
    Text* text_shop_desc_;
public:
#pragma mark func
    void update_data( msg::GoodInfo goodInfo , bool shop);
    void set_callback_func( std::function<void()> callback ){ callback_ = callback; };
    Size get_bg_size();
    void cell_bg_load_image(bool is_normal);
    void handle_download_event(cmd_data_pointer data);
private:
#pragma mark value
    msg::GoodInfo good_info_;
    std::function<void()> callback_;
    events::event_dispatch dispatch_;
};

#endif /* NodeShopListCell_hpp */
