//
//  ChatSettingLayer.h
//  pokerclient
//
//  Created by ll on 15/9/22.
//
//

#ifndef __pokerclient__ChatSettingLayer__
#define __pokerclient__ChatSettingLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "ShaderSprite.h"
#include "ActionSheet.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class user_base;

class ChatSettingLayer : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    CREATE_FUNC(ChatSettingLayer);
    virtual bool init();
    
    
    void set_clear_record_callback(const std::function<void()>& callback) { clear_record_callback_ = callback;}
    void set_setting_layer_status(uint8_t settingstatus);
    enum settingstatus{
        GROUP = 0x01,
        FRIEND = 0x02
    };
    
#pragma mark tableview datasource --
    virtual Size cellSizeForTable(TableView *table) {
        return Size(200, 200);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
    void set_is_ignore(const bool ignore) { is_ignore_ = ignore;}
    
private:
#pragma mark ClickEvent
    void handle_upload_succeed(cmd_data_pointer data);
    void btn_back_click(Ref* sender);
    void image_group_name_click(Ref* sender);
    void image_group_note_click(Ref* sender);
    void image_add_group_user_click(Ref* sender);
    void image_clear_chat_record_click(Ref* sender);
    void image_group_push_click(Ref* sender);
    void image_click_change_pic(Ref* sender);
    void btn_quit_click(Ref* sender);
    
    void change_name(const std::string& );
    void change_note(const std::string& );
   
    void set_ui_data();
    void register_events();
    void click_check_receive_push(Ref *r);
#pragma mark Handle
    void handle_update_ui(cmd_data_pointer data);
    void handle_receive_user_group_setting(cmd_data_pointer data);
    void after_choose_pic(const std::string& filename);
#pragma mark UI
    std::string m_StrPicName;
    Text* text_title_;
    //Text* text_back_;
    Button* btn_back_;
    
    Text* text_group_name_;
    Text* text_group_note_;
    Text* text_group_name_const_;
    Text* text_group_note_const_;
    
    Text* text_add_group_user_;
    Text* text_clear_record_;
    Text* text_group_push_;
    
    ImageView* image_group_name_;
    ImageView* image_group_note_;
    ImageView* image_add_group_user_;
    ImageView* image_clear_chat_record_;
    ImageView* image_group_push_;
    
    ImageView* image_group_name_1_;
    ImageView* image_group_note_1_;
    ImageView* image_add_user_1_;
    ImageView* image_group_push_1_;
    
    ImageView* image_head_;
    ImageView* image_user_head_;
    
    ImageView* image_groupuser_bg_;
    ImageView* image_groupinfo_bg_;
    
    CheckBox* check_receive_push_;
    
    ShaderSprite* sprite_icon_;
    
    Button* btn_quit_;
    ImageView* image_quit_;
    
    TableView* tableview_;
    
    Point point_text_add_group_user_;
    Point point_image_add_group_user_;
    
    Node* root_;
    
#pragma mark Function
    std::vector<msg::UserDetailInfo> members_;
    
    std::function<void()> clear_record_callback_;
    msg::GroupInfo group_data_;
    events::event_dispatch dispatch_;
    bool is_ignore_;
    uint8_t settinglayer_status_;
    bool m_bIsUploadSuccess;
};

#endif /* defined(__pokerclient__ChatSettingLayer__) */
