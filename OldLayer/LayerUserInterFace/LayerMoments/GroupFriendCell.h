//
//  GroupFriendCell.h
//  pokerclient
//
//  Created by qqronaldz on 15/9/1.
//
//

#ifndef __pokerclient__GroupFriendCell__
#define __pokerclient__GroupFriendCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class group_data;

class GroupFriendCell : public TableViewCell {
public:
    CREATE_FUNC(GroupFriendCell);
    
    virtual bool init();
    
    void update_property( ::msg::Moment  data);
    const std::weak_ptr<group_data>& get_group_data() { return group_data_;}
    
private:
    Text *text_nickname_;
    ImageView *image_square_head_pic;
    Text *text_publish_content;
    Text *text_content_type;
    Text *text_need_points;
    Text *text_reward_points;
    Text *text_review;
    ImageView *image_review;
    Text *text_like;
    ImageView *image_like;
    Text *text_content_time;
    
    
    
    std::weak_ptr<group_data> group_data_;
};

#endif /* defined(__pokerclient__GroupFriendCell__) */
