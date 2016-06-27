//
//  LayerChooseGameType.h
//  pokerclient
//
//  Created by lvzhangle on 16/3/27.
//
//

#ifndef __pokerclient__LayerChooseGameType__
#define __pokerclient__LayerChooseGameType__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"

#include "GameDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerChooseGameType : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    enum CHOOSE_ROOM_TYPE
    {
        NONE = 0,
        NLHOLDEM = 1,
        OFC3 = 2,
        PLO = 3,
        PLOHILO = 4,
        OFC4 = 5,
        SIChUANMAHJONG = 6,
        OMAHA = 7,
        SNG = 8,
        SNG_NLHOLDEM = 9,
        SNG_PLO = 10,
        SNG_PLOHILO = 11,
        MTT_NLHOLDEM = 12,
        MTT_PLO = 13,
        MTT_PLOHILO = 14
        
    };
    
    CREATE_FUNC(LayerChooseGameType);
    virtual bool init();
    
    void click_btn_back(Ref * sender);
    void select_choose_room_type(CHOOSE_ROOM_TYPE choose_room_type);
    void update_data(std::vector<CHOOSE_ROOM_TYPE> room_type_data);
    
    void set_choose_call_func(std::function<void(msg::RoomType, bool)> callfunc) {choose_room_type_callfunc_ = callfunc;}
    
    std::function<void(msg::RoomType, bool)> choose_room_type_callfunc_;
    
    std::vector<CHOOSE_ROOM_TYPE> room_type_datas_;
#pragma mark tableview datasource --
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
#pragma mark UI
private:
    Text* text_title_;
    
    Button* btn_back_;
    
    ImageView* image_layer_choosegametype_bg_;
    
    ListView* listview_;
    
    TableView* tableview_;
    
    Node* root_;
};

#endif /* defined(__pokerclient__LayerChooseGameType__) */