//
//  FriendCell.h
//  pokerclient
//
//  Created by ll on 15/8/18.
//
//

#ifndef __pokerclient__FriendCell__
#define __pokerclient__FriendCell__

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

class FriendCell : public BaseFriendCell{
public:
    CREATE_FUNC(FriendCell);
    
    virtual bool init();
    
    void update_property(const msg::UserDetailInfo& friend_data);
    
    void set_is_touch(bool is_touch = true);
    
private:
    ImageView* image_bg_;
    ImageView* image_icon_;
    ImageView* image_red_ball_;
    Text* text_unread_message_count_;
    Text* text_player_name_;
    
    cocos2d::ShaderSprite* sprite_icon_;
    
    void update_ui();
    void register_download_events();
    void handle_download_events(cmd_data_pointer);
    
    events::event_dispatch dispatch_;
    Node* root_;
};

#endif /* defined(__pokerclient__FriendCell__) */
