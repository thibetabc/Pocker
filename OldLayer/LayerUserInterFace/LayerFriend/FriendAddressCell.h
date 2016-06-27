//
//  FriendAddressCell.h
//  pokerclient
//
//  Created by ll on 15/8/27.
//
//

#ifndef __pokerclient__FriendAddressCell__
#define __pokerclient__FriendAddressCell__

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

class FriendAddressCell : public BaseFriendCell{
public:
    CREATE_FUNC(FriendAddressCell);
    
    virtual bool init();
    
    void update_property(const msg::UserDetailInfo& friend_data);
    
    void set_is_touch(bool is_touch = true);
    
private:
    ImageView* image_bg_;
    Text* text_player_name_;
    
};


#endif /* defined(__pokerclient__FriendAddressCell__) */
