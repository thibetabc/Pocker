//
//  BaseFriendCell.h
//  pokerclient
//
//  Created by ll on 15/8/24.
//
//

#ifndef __pokerclient__BaseFriendCell__
#define __pokerclient__BaseFriendCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class BaseFriendCell : public TableViewCell{
public:
    CREATE_FUNC(BaseFriendCell);
    
    virtual bool init();
    const msg::UserDetailInfo& get_friend_data() { return friend_data_;}
    void set_friend_data(const msg::UserDetailInfo& friend_data) { friend_data_.CopyFrom(friend_data);}
    
    void virtual set_is_touch(bool is_touch = true){};
    
protected:
    msg::UserDetailInfo friend_data_;
};

#endif /* defined(__pokerclient__BaseFriendCell__) */
