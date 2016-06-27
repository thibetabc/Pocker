//
//  LayerTourmentScore.h
//  pokerclient
//
//  Created by ll on 15/12/4.
//
//

#ifndef __pokerclient__LayerTourmentScore__
#define __pokerclient__LayerTourmentScore__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "NodeScoreCellOld.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerTourmentScore : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    
    CREATE_FUNC(LayerTourmentScore);
    virtual bool init();
    void update_data();
    void handle_get_score(cmd_data_pointer data);
    void click_btn_back(Ref* sender);
private:
    void handle_download_events(cmd_data_pointer);
    void register_download_events();
    void update_ui();
private:
    Node* root_;
    
    Text* text_room_name_;
    Text* text_time_;
    
    TableView* tableview_;
    
    Button* btn_back_;
    
    events::event_dispatch dispatch_;
    
//    std::vector<std::shared_ptr<MATCH_OBJECT> > match_objects_;
    
    std::vector<ImageView*> award_avators_;
    std::vector<Text*> award_usernames_;
    
//    std::vector<ShaderSprite*> shader_sprites_;
    
    Button* btn_share_;
    
#pragma mark tableview datasource --
    virtual cocos2d::Size cellSizeForTable(TableView *table) {
        return cocos2d::Size(DESIGN_WIDTH, 135);
    };
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
};


#endif /* defined(__pokerclient__LayerTourmentScore__) */
