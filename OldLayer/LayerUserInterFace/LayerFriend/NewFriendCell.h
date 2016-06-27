//
//  NewFriendCell.h
//  pokerclient
//
//  Created by ll on 15/8/20.
//
//

#ifndef __pokerclient__NewFriendCell__
#define __pokerclient__NewFriendCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "BaseFriendCell.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NewFriendCell : public BaseFriendCell{
public:
    CREATE_FUNC(NewFriendCell);
    
    virtual bool init();
    
    void update_property(const msg::UserDetailInfo& friend_data);
    
    void set_is_touch(bool is_touch = true);
    
private:
    ImageView* image_bg_;
    ImageView* image_icon_;
    Text* text_player_name_;

};

#endif /* defined(__pokerclient__NewFriendCell__) */
