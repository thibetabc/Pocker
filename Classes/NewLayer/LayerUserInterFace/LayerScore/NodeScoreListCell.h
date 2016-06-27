//
//  NodeScoreListCell.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/11.
//
//

#ifndef NodeScoreListCellData_hpp
#define NodeScoreListCellData_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "ShaderSprite.h"
#include "events.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeScoreListCell : public TableViewCell {
public:
    
//    CREATE_FUNC(NodeScoreListCell);
    static NodeScoreListCell* create(bool is_player_rank = false , bool is_show_buy_num = true);
    virtual bool init(bool is_player_rank , bool is_show_buy_num);
    
    void update_data(const msg::PlayerScore & list_cell_data);
    void set_is_select(bool is_touch);
    void addPlayPhoto(std::string picname);
    void handle_download_event(cmd_data_pointer data);
    void set_gold_visible();
    void set_silver_visible();
    
private:
    msg::PlayerScore list_cell_data_;
    events::event_dispatch dispatch_;
    std::string picname_;
private:
    Node* root_;
    
    Text* text_room_member_name_;
    Text* text_hit_num_;
    Text* text_earn_diamond_;
    Text* text_gift_;
    Text* text_buy_num_;
   
    Node* node_room_member_photo_;
    ImageView* image_silver_;
    ImageView* image_gold_;
    ShaderSprite* sprite_icon_;
    bool is_player_rank_;
    bool is_show_buy_num_;
};
#endif /* NodeScoreListCellData_hpp */
