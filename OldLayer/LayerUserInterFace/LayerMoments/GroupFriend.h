//
//  GroupFriend.h
//  pokerclient
//
//  Created by admin on 15-8-17.
//
//

#ifndef __pokerclient__GroupFriend__
#define __pokerclient__GroupFriend__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class GroupFriend : public BaseLayer, public TableViewDataSource, public TableViewDelegate, public EditBoxDelegate
{
public:
    
    CREATE_FUNC(GroupFriend);
    virtual bool init();
    
    void set_btn_add_callback(const std::function<void()>& callback) {
        btn_issue_callback_ = callback;
    }
    void set_btn_search_callback(const std::function<void()>& callback) {
        btn_search_callback_ = callback;
    }
    
    void click_btn_release(Ref* sender);
    void click_btn_search(Ref* sender);
    void click_btn_square(Ref* sender);
    void click_btn_hot(Ref* sender);
    void click_btn_promised(Ref* sender);
    void click_btn_mine(Ref* sender);
    void click_btn_send(Ref* sender);
    void touch_image(Ref* sender, Touch* touch, Widget::TouchEventType event);
    void move_to_left();
    void move_to_right();
    
    bool touchBegan(Touch* touch, Event* event);
    void touchMoved(Touch* touch, Event* event);
    void touchEnded(Touch* touch, Event* event);
    void touchCancel(Touch* touch, Event* event);
    
    //void update_table_view();
    void update_table_view(const msg::Processor_201_GetMoment_DOWN& processor);
    
    void handle_delete_updata(cmd_data_pointer data);
    void handle_request_comment(cmd_data_pointer data);
    void handle_replay(cmd_data_pointer data);
    void handle_todo_comment(cmd_data_pointer data);
    
    void set_momment_id(const uint32_t momment_id) { momment_id_ = momment_id;}
    const uint32_t get_comment_id() { return momment_id_;  }
    
    void update_data(cmd_data_pointer);
    
public:
    
#pragma mark tableview datasource --
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    
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
    
private:
    std::function<void()> btn_issue_callback_;
    std::function<void()> btn_search_callback_;
    msg::Processor_203_PublishComment processor ;
    
    msg::MomentDisplayType dis_type_selected ;
    TableView* tableview_fri;
    events::event_dispatch dispatch_;
    
    Button* btn_square_;
    Button* btn_hot_;
    Button* btn_promised_;
    Button* btn_mine_;
    
    ListView* listview_;
    ImageView* image_bg;
    Button* btn_send;
    ImageView* input_bg;
    
    std::map<Node*, cocos2d::Point> node_about_textfileds_;
    EditBox* tf_comment_;
    msg::Processor_201_GetMoment_DOWN moment_data;

    int32_t current_select_idx_;
    cocos2d::Point began_location_;
    cocos2d::Point tableview_touch_location_;
    bool move_;
    
    int m_nTag;
    ssize_t counter;
    uint32_t momment_id_;
    bool isDown;
    
};

#endif /* defined(__pokerclient__GroupFriend__) */
