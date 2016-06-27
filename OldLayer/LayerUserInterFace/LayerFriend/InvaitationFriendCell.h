//
//  InvaitationFriendCell.h
//  pokerclient
//
//  Created by ll on 15/8/20.
//
//

#ifndef __pokerclient__InvaitationFriendCell__
#define __pokerclient__InvaitationFriendCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "BaseFriendCell.h"
#include "ShaderSprite.h"
#include "events.h"
#include "cmd_def.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class InvaitationFriendCell : public BaseFriendCell{
public:
    CREATE_FUNC(InvaitationFriendCell);
    
    virtual bool init();

    void update_property(const msg::UserDetailInfo& friend_data);
    void update_ui(cmd_data_pointer = nullptr);
    
    void set_is_touch(bool is_touch = true);
    
private:
    ImageView* image_bg_;
    ImageView* image_icon_;
    Text* text_player_name_;
    ImageView* image_red_ball_;
    Text* text_unread_message_count_;
    events::event_dispatch dispatch_;
    
};
#endif /* defined(__pokerclient__InvaitationFriendCell__) */
