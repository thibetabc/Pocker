//
//  NodeScoreCell.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/11.
//
//

#ifndef NodeScoreCell_hpp
#define NodeScoreCell_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeScoreCell : public TableViewCell {
public:
    static NodeScoreCell* create(bool is_group_match=false);
    virtual bool init(bool is_group_match);
    
    void update_data(const msg::HistoryScore& reward_cell_data);
    void set_is_select(bool is_touch);
private:
    msg::HistoryScore reward_cell_data_;
private:
    Node* root_;
    Text* text_room_time_;
    Text* text_come_from_;
    Text* text_room_name_;
    Text* text_group_name_;
    Text* text_chip_num_;
    Text* text_time_;
    Text* text_earn_chip_;
    
    ImageView* image_chip_;
    ImageView* image_time_;
    bool is_group_match_;
    
    int room_id_;
    
    
    std::function<void(std::string,std::string)> func_click_groupname_;
    std::function<void(std::string)> func_click_chip_;
public:
    void set_click_group_name_callback(const std::function<void(std::string,std::string)> func_click_groupname) {func_click_groupname_ = func_click_groupname;};
    void set_click_chip_name_callback(const std::function<void(std::string)> func_click_chip) {func_click_chip_ = func_click_chip;};
    void response_for_chip_callback(Ref* sender);
    void response_for_group_callback(Ref* sender);
    int getRoomId();
};

#endif /* NodeScoreCell_hpp */
