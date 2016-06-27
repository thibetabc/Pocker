//
//  NodeMahjongFinalResultCell.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#ifndef NodeMahjongFinalResultCell_hpp
#define NodeMahjongFinalResultCell_hpp

#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "ShaderSprite.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeMahjongFinalResultCell : public TableViewCell , public TableViewDelegate , public TableViewDataSource
{
public:
    static NodeMahjongFinalResultCell* create(bool is_self = false);
    virtual bool init( bool is_self );
    
private:
    Node* root_;
    ImageView* image_cell_bg_;
    Node* node_player_photo_;
    Text* text_name_;
    TextAtlas* atlas_label_score_;
    ListView* list_veiw_;
    ImageView* image_score_title_;
    ImageView* image_gold_;
    
    
    TableView* tableview_;
    msg::MahjongPlayerInfo player_info_;
    
    ShaderSprite* sprite_icon_;
    events::event_dispatch dispatch_;
    bool is_self_;
public:
    void set_data( msg::MahjongPlayerInfo &player_info);
    void handle_download_event(cmd_data_pointer data);
    void addPlayPhoto(std::string picname);
#pragma mark tableview_datascource
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx){return Size(379,30);};
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
};

#endif /* NodeMahjongFinalResultCell_hpp */
