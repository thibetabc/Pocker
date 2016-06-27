//
//  LayerScoreOld.h
//  pokerclient
//
//  Created by ll on 15/10/27.
//
//

#ifndef __pokerclient__LayerScoreOld__
#define __pokerclient__LayerScoreOld__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "NodeScoreCellOld.h"
#include "ShaderSprite.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class AWARD_OBJECT//获得称号人员类
{
public:
    MATCH_OBJECT matchobject;
    std::string awardname;//称号
    
};
class LayerScoreOld : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    
    CREATE_FUNC(LayerScoreOld);
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
    
    Text* text_mark_1_;
    Text* text_mark_2_;
    Text* text_mark_3_;
    
    Text* text_mark_num_1_;
    Text* text_mark_num_2_;
    Text* text_mark_num_3_;
    
    Text* text_mark_name_1_;
    Text* text_mark_name_2_;
    Text* text_mark_name_3_;
    
    Text* text_nick_name_1_;
    Text* text_nick_name_2_;
    Text* text_nick_name_3_;
    
    TableView* tableview_;
    
    ImageView* image_avatar_1_;
    ImageView* image_avatar_2_;
    ImageView* image_avatar_3_;
    
    ImageView* image_avatar_frame_1_;
    ImageView* image_avatar_frame_2_;
    ImageView* image_avatar_frame_3_;
    
    Button* btn_back_;
    
    events::event_dispatch dispatch_;
    
    std::vector<std::shared_ptr<MATCH_OBJECT> > match_objects_;
    std::vector<std::shared_ptr<AWARD_OBJECT> > award_objects_;
    
    std::vector<ImageView*> award_avators_;
    std::vector<Text*> award_usernames_;
    
    std::vector<ShaderSprite*> shader_sprites_;
    
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

#endif /* defined(__pokerclient__LayerScoreOld__) */
