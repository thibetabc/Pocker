//
//  NodeShopCell.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/24.
//
//

#ifndef NodeShopCell_hpp
#define NodeShopCell_hpp

#include "NodeShopListCell.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeShopCell : public TableViewCell
{
public:
    CREATE_FUNC(NodeShopCell);
    virtual bool init();
    
private:
    NodeShopListCell* cell_left_;
    NodeShopListCell* cell_right_;
    bool shop_;
    
    std::vector<msg::GoodInfo> goods_info_;
    std::function<void()> callback_;
public:
    void update_cell_data(std::vector<msg::GoodInfo> goods_info , bool shop);
    void click_cell();
    bool is_touch_in_left(Point pos);
    bool is_touch_in_right(Point pos);
    std::vector<msg::GoodInfo> get_cell_data();
    NodeShopListCell* get_left_cell();
    NodeShopListCell* get_right_cell();
};


#endif /* NodeShopCell_hpp */
