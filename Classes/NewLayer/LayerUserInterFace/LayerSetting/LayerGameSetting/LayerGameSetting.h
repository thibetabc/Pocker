//
//  LayerGameSetting.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/19.
//
//

#ifndef LayerGameSetting_hpp
#define LayerGameSetting_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "NodePropertyCell.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;
class dataSource
{
public:
    std::string name;
    msg::RoomType type;
    bool isSelected=false;
};
class LayerGameSetting:public BaseLayer,public TableViewDataSource,public TableViewDelegate
{
public:
    LayerGameSetting();
    ~LayerGameSetting();
    static LayerGameSetting *create(bool iHideLanguageSetting);
    virtual bool init();
    
    void set_change_setting_callback(const std::function<void()> change_setting_callback) { change_setting_callback_ = change_setting_callback; };
private:
    Node* root_;
    Button* btn_back_;
    Text* text_title_;
    
    ListView* list_game_type_;
    ListView* system_game_setting_;
    
    TableView* tableview_gametype_;
    
    std::vector<dataSource> data_source_;
    ssize_t current_select_index_ = 0;
    
    NodePropertyCell* cell_2_;
    NodePropertyCell* cell_3_;
    NodePropertyCell* cell_4_;
    
    std::function<void()> change_setting_callback_;
public:
    void click_btn_back(Ref* sender);
    void addDataSource();
    void click_choose_btn_func(std::string str);
    void handle_get_game_setting(cmd_data_pointer data);
    void add_game_setting_data();
    void new_card_selected_callback(msg::Property property);
    void volume_callback(msg::Property property);
    void music_callback(msg::Property property);
#pragma mark tableview_datascource
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx){return Size(380,110);};
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    bool isHideLanguageSetting;
};

#endif /* LayerGameSetting_hpp */
