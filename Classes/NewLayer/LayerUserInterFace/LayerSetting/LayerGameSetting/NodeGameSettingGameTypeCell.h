//
//  NodeGameSettingGameTypeCell.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/19.
//
//

#ifndef NodeGameSettingGameTypeCell_hpp
#define NodeGameSettingGameTypeCell_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGui.h"
#include "cocos-ext.h"
#include "NodeCircleChoose.h"
#include "msg.pb.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeGameSettingGameTypeCell:public TableViewCell {
public:
    CREATE_FUNC(NodeGameSettingGameTypeCell);
    virtual bool init();
private:
    Node* root_;
    Node* Node_choose_btn_;
    NodeCircleChoose* btn_choose_;
    
    Text* text_game_type_;
    std::function<void(std::string)> bind_func_;
    msg::RoomType room_type_;
    bool is_selected_=false;
public:
    void set_btn_choose_status(bool visible);
    void set_text_game_type(std::string str,msg::RoomType type);
    void set_bind_func(std::function<void(std::string)> bind_func);
};

#endif /* NodeGameSettingGameTypeCell_hpp */
