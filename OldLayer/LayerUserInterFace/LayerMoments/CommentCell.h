//
//  CommentCell.h
//  pokerclient
//
//  Created by admin on 15-9-10.
//
//

#ifndef __pokerclient__CommentCell__
#define __pokerclient__CommentCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class CommentCell : public TableViewCell {
public:
    CREATE_FUNC(CommentCell);
    
    virtual bool init();
    
    void update_data();
    
    void update_property(msg::CommentInfo* comment_data);
    msg::CommentInfo*get_comment_data() { return comment_data_;}
    
private:

    ImageView* image_bg;
    ImageView* image_nickname;
    TextField* tf_nickname_;
    TextField *tf_comment;
    TextField* tf_time;
    
    msg::CommentInfo* comment_data_;
};


#endif /* defined(__pokerclient__CommentCell__) */
