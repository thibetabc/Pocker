//
//  NodePayListCell.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/26.
//
//

#ifndef NodePayListCell_hpp
#define NodePayListCell_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodePayListCell : public Widget {
public:
    CREATE_FUNC(NodePayListCell);
    virtual bool init();
private:
#pragma mark ui
    Node* root_;
    Text* text_title_first_;
    Text* text_title_second_;
    Text* text_diamond_num_;
    Text* text_should_pay_;
    Layout* panel_pay_cell_;
public:
    void update_data(std::pair<std::string,std::string> &data);
};

#endif /* NodePayListCell_hpp */
