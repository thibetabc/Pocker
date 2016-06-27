//
//  LayerChatOld.h
//  pokerclient
//
//  Created by ll on 15/9/8.
//
//

#ifndef __pokerclient__LayerChatOld__
#define __pokerclient__LayerChatOld__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;
class chat_data;

class RecordAnimationLayer;

class LayerChatOld : public BaseLayer, public TableViewDataSource, public TableViewDelegate, public EditBoxDelegate
{
public:
    CREATE_FUNC(LayerChatOld);
    
    virtual bool init();
    
    enum REFRESH_MODE{
        REFRESH_OLD_MESSAGE,
        RECEIVE_NEW_MESSAGE
    };
    
    void click_btn_back(Ref* sender);
    void click_btn_member(Ref* sender);
    void click_btn_room(Ref* sender);
    void click_btn_group_match(Ref* sender);
    void click_btn_emotion(Ref* sender);
    void click_btn_voice(Ref* sender);
    void click_btn_keyboard(Ref* sender);
    void click_btn_add(Ref* sender);
    void click_btn_send(Ref* sneder);
    void touch_btn_record(Ref* sender, Touch* touch, Widget::TouchEventType event);
    void touch_listview(Ref* sender, Touch* touch, Widget::TouchEventType event);
    
    void update_data(cmd_data_pointer data = nullptr);
    void set_not_touch(bool is_not_touch = true){is_not_touch_ = is_not_touch;};
    
    void show_keyboard();
    void remove_emotion_layer();
    
    void handle_send_msg_clear_editBox(cmd_data_pointer);
    
    void refresh_chat_message(REFRESH_MODE set_refresh_mode);
    
#pragma mark tableview datasource --
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
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
    
    void keyBoardWillShow(const float duration, const float distance);
    void keyBoardWillHide(const float duration, const float distance);
    void onExit();
private:
    void register_events();
    
    void register_events_group();
    void register_events_friend();
    void init_group();
    void init_friend();
    void after_choose_pic(const std::string& filename);
    void reload_data(cmd_data_pointer);
    void remove_from_parent(cmd_data_pointer);
private:
    uint32_t chat_status_;
    uint32_t group_id_;
    uint32_t friend_id_;
    bool is_not_touch_;
    bool isSize = false;
    Button* btn_back_;
    Text* text_title_;
    Button* btn_member_;
    EditBox* tf_chat_;
    TableView* tableview_;
    ListView* listview_;
    Text* text_room_;
    cocos2d::Node* root_;
    Button* btn_voice_;
    Button* btn_keyboard_;
    Button* btn_record_;
    Button* btn_send_;
    Button* btn_room_;
    Button* btn_group_match_;
    Sprite* room_number_;
    Button* btn_emotion_;
    Button* btn_add_;
    Point m_pEditBoxPosition;
    
    std::list<std::shared_ptr<msg::ChatMessage> > chats_;
    
    std::map<uint32_t, Size> cell_sizes_;
    
    events::event_dispatch dispatch_;
    
    std::string record_filename_;
    
    std::map<Node*, Point> node_about_textfileds_;
    msg::GroupInfo* group_data_;
    Point touch_record_begin_position_;
public:
    bool m_bIsTouchedLayout;
    
    bool can_refresh_list_;
};


#endif /* defined(__pokerclient__LayerChatOld__) */

