//
//  LayerComment.h
//  pokerclient
//
//  Created by admin on 15-9-10.
//
//

#ifndef __pokerclient__LayerComment__
#define __pokerclient__LayerComment__

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

class comment_data;

class LayerComment : public BaseLayer, public TableViewDataSource, public TableViewDelegate, public EditBoxDelegate
{
public:
    
    CREATE_FUNC(LayerComment);
    virtual bool init();
    void click_btn_send(Ref* sender);
    void click_btn_cancel(Ref* sender);
    void set_btn_cancel_callback(const std::function<void()>& callback) {
        btn_cancel_callback_ = callback;
    }

#pragma mark tableview datasource --
    virtual cocos2d::Size cellSizeForTable(TableView *table);
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
    
    void keyBoardWillShow(const float duration, const float distance);
    void keyBoardWillHide(const float duration, const float distance);
    
    
    events::event_dispatch dispatch_;

    
    void handle_request_comment(cmd_data_pointer data);
    void update_table_view(const msg::Processor_204_GetComment_DOWN& processor);
    void update_data(cmd_data_pointer data);
private:
    msg::Processor_203_PublishComment processor ;
    msg::Processor_204_GetComment_DOWN commentdata;
    msg::CommentInfo* comment_data_ = nullptr;
    int counter;
    uint32_t momment_id;
    
    Button* btn_cancel;
    Button* btn_send;
    ImageView* image_bg;
    TableView* tableview_;
    ListView* listview_;
    ImageView* input_bg;
    
    EditBox* tf_comment_;
    
    std::function<void()> btn_cancel_callback_ = nullptr;
    
    std::map<Node*, cocos2d::Point> node_about_textfileds_;
    
    std::vector<std::shared_ptr<comment_data> > comment_;
};


#endif /* defined(__pokerclient__LayerComment__) */
