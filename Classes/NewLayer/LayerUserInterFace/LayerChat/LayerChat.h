//
//  LayerChat.h
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef __pokerclient__LayerChat__
#define __pokerclient__LayerChat__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerChat : public BaseLayer, public TableViewDataSource, public TableViewDelegate, public EditBoxDelegate
{
public:
    enum CHAT_STATUS{
        NONE,
        GROUP_CHAT,
        FRIEND_CHAT
    };
    
    static LayerChat* createByGroup(const msg::GroupInfo & group_info);
    static LayerChat* createByFriend(const msg::UserDetailInfo & user_info);
    static LayerChat* createByGroup(const uint32_t group_id);
    static LayerChat* createByFriend(const uint32_t friend_id);
    
    static LayerChat* createByGroup();
    static LayerChat* createByFriend();

    virtual bool init();
    
    void update_chat_status_by_group(const msg::GroupInfo & group_info);
    void update_chat_status_by_friend(const msg::UserDetailInfo & user_info);
    void update_chat_status_by_group(const uint32_t group_id);
    void update_chat_status_by_friend(const uint32_t friend_id);
    
    void update_chat_status_by_group();
    void update_chat_status_by_friend();
    
    CHAT_STATUS get_chat_status(){return chat_status_;}
    const msg::GroupInfo get_chat_group_info() {return group_info_;}
    const msg::UserDetailInfo get_chat_user_info() {return user_info_;}
    
    uint32_t get_chat_group_id() {return group_id_;}
    uint32_t get_chat_friend_id() {return friend_id_;}
    
    void update_chat_message_datas();
    void update_chat_message_datas(const std::vector<msg::ChatMessage> & chat_message_datas);
    
    void send_get_room_info();
    void send_get_group_info();
protected:
    void RefreshLayer();
    void onExit();
private:
    void send_chatmessage_text(std::string chat_text);
    void send_chatmessage_picture(std::string pic_file_name);
    void send_chatmessage_voice(std::string voice_file_name, int voice_time);
    
    bool is_display_time(uint32_t idx);
    
    void init_test();
    
#pragma mark click_event
private:
    void click_btn_keyboard(Ref * sender);
    void touch_btn_record(Ref* sender, Touch* touch, Widget::TouchEventType event);
    void click_btn_voice(Ref * sender);
    void click_btn_back(Ref * sender);
    void click_btn_chat_emotion(Ref * sender);
    void click_btn_add(Ref * sender);
    void click_btn_chat_setting(Ref* sender);
    void click_btn_group_match_info(Ref* sender);
    void click_btn_play(Ref* sender);

    void refresh_user_info_view();
    void refresh_group_info_view();
    
    void keyBoardWillShow(const float duration, const float distance);
    void keyBoardWillHide(const float duration, const float distance);
    void show_keyboard();
    void remove_emotion_layer();
    void refresh_tableview();
    
    void set_is_allow_chat(bool isAllowChat);

    void register_events();
    void handle_get_room_info(cmd_data_pointer data);
    void handle_get_user_info(cmd_data_pointer data);
    void handle_get_group_info(cmd_data_pointer data);
private:
#pragma mark tableview datasource --
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
    bool ChatViewOnTouchBegan(Touch * touch, Event* event);
    void ChatViewOnTouchMoved(Touch * touch, Event* event);
    void ChatViewOnTouchEnded(Touch * touch, Event* event);
    void ChatViewOnTouchCancelled(Touch * touch, Event* event);
    
#pragma mark value
    std::string record_filename_;
    
    Point touch_record_begin_position_;
    
    msg::GroupInfo group_info_;
    msg::UserDetailInfo user_info_;
    
    CHAT_STATUS chat_status_;
    
    uint32_t group_id_;
    uint32_t friend_id_;
    
    std::vector<msg::ChatMessage> chat_message_datas_;
    
    bool has_info_;
    
    bool isTouchLayerout_ = true;
    
#pragma mark UI
private:
    std::map<Node*, Point> node_about_textfileds_;
    
    
    Button* btn_back_;
    Text* text_title_;
    
    Text* text_chat_status_;
    
    ImageView* image_layer_chat_bg_;
    ImageView* image_chat_bar_bg_;
    Button* btn_voice_;
    Button* btn_keyboard_;
    Button* btn_add_;
    Button* btn_chat_setting_;
    Button* btn_chat_emotion_;
    Button* btn_group_match_info_;
    Button* btn_record_;
    ImageView* image_input_bg_;
    TextField* tf_input_;
    EditBox* editbox_input_;

    Button* btn_play_;
    ImageView* image_play_count_bg_;
    Text* text_play_count_;
    ListView* listview_;
    TableView* tableview_;
    
    Node* root_;
};

#endif /* defined(__pokerclient__LayerChat__) */
