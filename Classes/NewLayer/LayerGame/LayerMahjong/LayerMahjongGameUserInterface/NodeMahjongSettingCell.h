//
//  NodeMahjongSettingCell.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#ifndef NodeMahjongSettingCell_hpp
#define NodeMahjongSettingCell_hpp

#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class NodeMahjongSettingCell : public BaseLayer
{
public:
    CREATE_FUNC(NodeMahjongSettingCell);
    virtual bool init();
private:
    
#pragma mark ---ui
    Node* root_;
    
    Text* text_setting_name_;
    Button* btn_control_;
    
#pragma mark ---value
    std::function<void(bool)> call_back_;
    bool is_on_;
    std::string title_;
public:
#pragma mark --click_events
    void click_btn_control(Ref* sender){
        is_on_ = is_on_ == true ? false : true ;
        refresh_node_data(title_, is_on_);
        if (call_back_)
        {
            call_back_(is_on_);
        }
        
        
    }
#pragma mark --func
    void refresh_node_data(std::string title , bool is_on);
    void set_click_btn_callback(std::function<void(bool)> call_back){ call_back_ = call_back; };
};

#endif /* NodeMahjongSettingCell_hpp */
