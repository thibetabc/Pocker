//
//  NodeScoreCell.h
//  pokerclient
//
//  Created by ll on 15/10/27.
//
//

#ifndef __pokerclient__NodeScoreCell__
#define __pokerclient__NodeScoreCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "ShaderSprite.h"
#include "FileManager.h"
#include "cmd_def.h"

#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class MATCH_OBJECT
{
public:
    std::string nickname;
    std::string head_pic_name;
    uint32_t buy_amount;
    int32_t win_amount;
    uint32_t rank;
    uint32_t user_id;
    msg::RoomType room_type;
    bool is_tournament;
    std::string otherprize;
    int32_t hp_lost;
    bool is_group_user_rank;
};

class NodeScoreCell : public TableViewCell {
public:
    CREATE_FUNC(NodeScoreCell);
    
    virtual bool init();
    void update_data(const std::shared_ptr<MATCH_OBJECT>& obj);
    
private:
    void handle_download_events(cmd_data_pointer);
    void register_download_events();
    void update_ui();

private:
    Node* root_;
    
    Text* text_nickname_;        //昵称
    Text* text_buy_chip_amount_; //买入数量
    Text* text_win_chip_amount_; //盈利数量  +  得分
    Text* text_user_other_prize_;
    Text* text_user_hp_lost_;
    Text* text_rank_;
    ImageView* image_avator_;
    ImageView* image_rank_;
   
    ImageView* income_line_;
    ImageView* image_avator_frame_;
    ImageView* score_cell_bg_;
    
    ShaderSprite* sprite_icon_;
    
    std::weak_ptr<MATCH_OBJECT> obj_;
    
    
    events::event_dispatch dispatch_;
};

#endif /* defined(__pokerclient__NodeScoreCell__) */
