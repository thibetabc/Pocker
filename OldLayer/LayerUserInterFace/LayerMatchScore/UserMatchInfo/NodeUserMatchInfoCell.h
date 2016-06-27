//
//  NodeUserMatchInfoCell.h
//  pokerclient
//
//  Created by ll on 15/11/27.
//
//

#ifndef __pokerclient__NodeUserMatchInfoCell__
#define __pokerclient__NodeUserMatchInfoCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include "ShaderSprite.h"
#include "utils.h"

#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeUserMatchInfoCell : public TableViewCell {
public:
    
    CREATE_FUNC(NodeUserMatchInfoCell);
    virtual bool init();
    
    void update_data(const std::shared_ptr<msg::HistoryScore> & match_cell_data);
    std::weak_ptr<msg::HistoryScore> get_match_cell_data(){return match_cell_data_;};
    void set_date_visiable(bool isVisiable);
    void set_click_label(const std::function<void(std::string)>& click_label) {click_label_ = click_label;}
    void set_is_touch(bool is_touch);
    void set_is_group_match(bool is_group_match);
private:
    void update_ui();
    void register_download_events();
    void handle_download_events(cmd_data_pointer);
    
    std::function<void(std::string)> click_label_;
    std::weak_ptr<msg::HistoryScore> match_cell_data_;
    
    
private:
    Node* root_;

    ShaderSprite* sprite_icon_;
    
    ImageView* image_cell_bg_;
    
    ImageView* image_score_bg_;
    
    ImageView* image_player_icon_bg_;
    ImageView* image_player_icon_;
    
    ImageView* image_date_icon_;
    
    Text* text_game_date_day_;
    Text* text_game_date_time_;
    
    Text* text_room_name_;
    Text* text_score_;
    
    std::vector<Button*> btn_labels_;
    msg::UserDetailInfo userinfo;
    events::event_dispatch dispatch_;
    
};

#endif /* defined(__pokerclient__NodeUserMatchInfoCell__) */
