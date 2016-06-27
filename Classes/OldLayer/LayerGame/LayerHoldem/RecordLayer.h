#pragma once


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "PokerRoomLayer.h"
#include "SceneManager.h"
#include "LayerPublichPriendsCircle.h"
class PokerRoom;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class RecordObject;
class StepObject;


class RecordLayer : public PokerChildLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    bool isShow;
    CREATE_FUNC(RecordLayer);
    virtual bool init();
    
    void update_data();
    void update_step();
    void set_select_handcard(bool isSelect);
    
    void click_btn_close(Ref* sender);
    bool ifchoosehandID;
    int _handID;
    void set_handID(int handID);
    void set_chooseHandID(bool chosehandID);
    
    void set_isFriHandCard(bool isFriHandCard) { isFriHandCard_ = isFriHandCard; }
    const bool get_isFriHandCard() { return isFriHandCard_; }
    const std::shared_ptr<msg::GameRecord>& get_gamerecord() { return gr_selected_; }
#pragma mark tableview datasource
    virtual cocos2d::Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    void  tableCellHighlight(TableView* table, TableViewCell* cell);
    void tableCellUnhighlight(TableView* table, TableViewCell* cell);
#pragma mark touch delegate
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    /** Callback function for touch moved.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    /** Callback function for touch ended.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    void handle_get_history();
private:
    std::shared_ptr<msg::GameRecord> gr_selected_;
    bool isFriHandCard_ = false;
    bool is_select_;
    Node* root_;
    Node* chooseNode_;
    int handIdIdx;
    TableView* tableview_record_;
    TableView* tableview_note_;
    std::vector<std::shared_ptr<msg::GameRecord> > records_;//牌局手牌记录
    std::map<uint32_t, std::shared_ptr<StepObject>> steps_;//牌局步骤记录
//    Text* text_time_;
};