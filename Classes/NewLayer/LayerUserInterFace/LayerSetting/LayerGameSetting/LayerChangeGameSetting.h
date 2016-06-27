//
//  LayerChangeGameSetting.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/19.
//
//

#ifndef LayerChangeGameSetting_hpp
#define LayerChangeGameSetting_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "SceneManager.h"
#include "NodePropertyCell.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerChangeGameSetting:public BaseLayer {
public:
    static LayerChangeGameSetting* create(msg::RoomType type,std::string name);
    virtual bool init(msg::RoomType type,std::string name);
private:
    Node* root_;
    ListView* list_;
    Button* btn_back_;
    Text* text_title_;
    Button* btn_submit_setting_;
    std::vector<msg::UserGameSetting> game_setting_;
    msg::UserGameSetting game_data_;
    msg::RoomType type_;
    std::string name_;
    
    NodePropertyCell* cell_1_;
    NodePropertyCell* cell_2_;
    NodePropertyCell* cell_3_;
    
public:
    void click_btn_back(Ref* sender);
    void click_btn_submit_setting(Ref* sender);
    void update_data(cmd_data_pointer data);
    void register_events();
    void update_lisview_data();
    void handle_change_down(cmd_data_pointer data);
};

#endif /* LayerChangeGameSetting_hpp */
