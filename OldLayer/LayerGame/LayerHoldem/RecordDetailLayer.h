//
//  RecordDetailLayer.hpp
//  pokerclient
//
//  Created by NaughtyDog on 16/3/10.
//
//

#ifndef RecordDetailLayer_h
#define RecordDetailLayer_h

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "PokerRoomLayer.h"
#include "LayerManager.h"
class PokerRoom;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class RecordObject;
class StepObject;

class RecordDetailLayer : public PokerChildLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    bool isShow;
    CREATE_FUNC(RecordDetailLayer);
    virtual bool init();
    
    void update_data(const msg::Processor_300_PokerGetHandHistory_DOWN& processor);
    
    void click_btn_close(Ref* sender);

    const std::shared_ptr<msg::GameRecord>& get_gamerecord() { return gr_selected_; }
    float getCellHeightForIndex(ssize_t idx);
#pragma mark tableview datasource
    //virtual cocos2d::Size cellSizeForTable(TableView *table);
    Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
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
    
    void click_btn_share(Ref* sender);
    
    void saveCallback(RenderTexture* tex, const std::string& info);
    void getSequence();
private:
    std::shared_ptr<msg::GameRecord> gr_selected_;

    Node* root_;
    Text* text_blind;
    TableView* tableview_record_;
    bool isDetail;
    std::vector<int> cellHeights;
    std::vector<int> seq;
    msg::Processor_300_PokerGetHandHistory_DOWN records;
};
#endif /* RecordDetailLayer_h */
