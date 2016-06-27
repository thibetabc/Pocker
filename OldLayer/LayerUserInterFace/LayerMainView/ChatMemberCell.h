//
//  ChatMemberCell.h
//  pokerclient
//
//  Created by ll on 15/9/23.
//
//

#ifndef __pokerclient__ChatMemberCell__
#define __pokerclient__ChatMemberCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "ShaderSprite.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class user_base;

class ChatMemberCell : public TableViewCell {
public:
    CREATE_FUNC(ChatMemberCell);
    
    virtual bool init();
    
    enum CELLTYPE{
        GROUP = 0x01,
        FRIEND = 0x02
    };
    
    void update_ui(const msg::UserDetailInfo& user);
    void update_friend_ui(const msg::UserDetailInfo& set_friend_data);
    void touch_image_head_pic(Ref* sender, Widget::TouchEventType event);
    void click_btn_delete(Ref* sender);
    
    void touch_header_image(float dt);
    
    void register_download_events();
    void handle_download_events(cmd_data_pointer);
    
    std::function<void(const bool)> callback;
    
    ImageView* image_head_pic_;
    Button* btn_delete_;
    Text* text_name_;
    msg::UserDetailInfo user_;
    msg::UserDetailInfo friend_data_;
    
    double touch_begin_timestamp_;
    double touch_end_timestamp_;
    msg::GroupInfo* group_data_;
    events::event_dispatch dispatch_;
    
    cocos2d::ShaderSprite* sprite_icon_;
    
    float is_touch_time_;
    int positionX;
    Node* root_;
    
    uint8_t cell_type;
};


#endif /* defined(__pokerclient__ChatMemberCell__) */
