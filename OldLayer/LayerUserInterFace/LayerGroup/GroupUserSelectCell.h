//
//  GroupUserSelectCell.h
//  pokerclient
//
//  Created by ll on 15/9/9.
//
//

#ifndef __pokerclient__GroupUserSelectCell__
#define __pokerclient__GroupUserSelectCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "BaseFriendCell.h"
#include "ShaderSprite.h"


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class GroupUserSelectCell : public BaseFriendCell{
public:
    CREATE_FUNC(GroupUserSelectCell);
    
    virtual bool init();
    
    enum select_status{
        ISTRUE = 0x01,
        ISFALSE = 0x02,
        ISFORBID = 0x03
    };
    
    void update_property(const msg::UserDetailInfo& friend_data);
    
    void set_select(const uint8_t status);
    
    const uint8_t get_is_select(){return is_select_;}
    
    void set_is_touch(bool is_touch = true);
    
private:
    ImageView* image_bg_;
    ImageView* image_icon_;
    Text* text_player_name_;
    Sprite* sprite_select_;
    
    cocos2d::ShaderSprite* sprite_icon_;
    
    void update_ui();
    void register_download_events();
    void handle_download_events(cmd_data_pointer);
    
    events::event_dispatch dispatch_;
    Node* root_;
    
    uint8_t is_select_;
};


#endif /* defined(__pokerclient__GroupUserSelectCell__) */
