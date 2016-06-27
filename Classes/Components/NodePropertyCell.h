//
//  NodePropertyCell.h
//  pokerclient
//
//  Created by lvzhangle on 16/3/27.
//
//
#ifndef __pokerclient__NodePropertyCell__
#define __pokerclient__NodePropertyCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class NodePropertyCell : public Widget, public EditBoxDelegate {
public:
    enum CELL_TYPE{
        NONE,
        MORE_INFO,
        GAME_TYPE,
        SELECT_GROUP,
        VALUE_STRING,
        VALUE_INT_INPUT,
        VALUE_INT_SLIDER,
        VALUE_ENUM,
        VALUE_BOOL,
        VALUE_INT_SLIDER_SECOND,
        VALUE_TIME_INPUT,
        CLICK_CELL
    };
    static NodePropertyCell* create(Node* ParentLayer, bool is_edit = true);
    
    virtual bool init(bool is_edit = true);
    
    CELL_TYPE get_cell_type(){return cell_type_;}
    const msg::Property* get_cell_property();
    
    void update_roomtype_data(msg::RoomType room_type, bool is_tournament);
    void update_group_data(const msg::GroupInfo& groupinfo);
    void update_data(const msg::Property& property);
    void update_value_min_max(const int value_min,const int value_max);
    void update_select_enum(const msg::Property& property);
    void update_slider_data(const msg::Property& property);
    void update_time_input(long long int timeSec);
    void update_click_cell(const std::string displayName);
    
    void refresh_data();
    void set_ParentLayer(Node* ParentLayer);
    
    void set_property_callback(std::function<void(const msg::Property&)> property_callback) {property_callback_ = property_callback;}
    
    void set_select_group_callback(std::function<void(const msg::GroupInfo&)> select_group_callback) {select_group_callback_ = select_group_callback;}
    
    void set_select_roomtype_callback(std::function<void((msg::RoomType room_type, bool is_tournament))> select_roomtype_callback) {select_roomtype_callback_ = select_roomtype_callback;}
    
    void set_click_cell_callback(std::function<void()> click_cell_callback){ click_cell_callback_ = click_cell_callback;}
    
    void set_select_time_callback(std::function<void(long long int)> select_time_callback){ select_time_callback_ = select_time_callback;}
    
    Slider* get_Slider(){return slider_;};
    
    void click_btn_enum(Ref * sender);
private:
    void property_int_slider(Ref * sender);
    void property_bool_checkbox(Ref * sender);
    void AfterSelectGroup(const msg::GroupInfo& groupinfo);
    void AfterSelectRoomType(msg::RoomType room_type, bool is_tournament);
    void TouchGroup(Ref *sender, Widget::TouchEventType touchtype);
    void TouchRoomType(Ref *sender, Widget::TouchEventType touchtype);
    void click_click_cell(Ref * sender);
    msg::Property property_;
    
    msg::GroupInfo group_info_;
    std::function<void(const msg::GroupInfo&)> select_group_callback_;
    std::function<void(msg::RoomType room_type, bool is_tournament)> select_roomtype_callback_;
    std::function<void(const msg::Property&)> property_callback_;
    std::function<void()> click_cell_callback_;
    std::function<void(long long int)> select_time_callback_;
    Node* ParentLayer_;
    
    long long int int_time_input_;
    bool is_edit_;
    CELL_TYPE cell_type_;
   
    int int_value_max_;
    int int_value_min_;
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
#pragma mark UI_TIME_INPUT
    TextField* tf_property_year_;
    EditBox* ed_property_year_;
    TextField* tf_property_month_;
    EditBox* ed_property_month_;
    TextField* tf_property_day_;
    EditBox* ed_property_day_;
    TextField* tf_property_hour_;
    EditBox* ed_property_hour_;
    TextField* tf_property_min_;
    EditBox* ed_property_min_;
    
//height 170
#pragma mark UI_INT_SLIDER
    Text* text_slider_cur_min_;
    Text* text_slider_cur_max_;
    Text* text_slider_cur_;
    Slider* slider_;
    
//height 110
#pragma mark UI_INT_INPUT
    ImageView* image_property_input_bg_;
    TextField* tf_property_value_;
    EditBox* ed_property_value_;
    
#pragma mark UI_BOOL
    CheckBox* checkbox_;
    
#pragma mark UI_ENUM
    Button* btn_enum_;
    
#pragma mark UI_SELECT_GROUP
    
#pragma mark UI_ROOM_TYPE
    ImageView* image_property_icon_;
    
#pragma mark UI
    ImageView* image_cell_line_;
    Text* text_property_name_;
    
    ImageView* image_select_icon_1_;
    
    
    Node* root_;
};

#endif /* defined(__pokerclient__NodePropertyCell__) */