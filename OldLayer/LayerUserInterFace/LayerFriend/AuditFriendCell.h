//
//  AuditFriendCell.h
//  pokerclient
//
//  Created by ll on 15/8/20.
//
//

#ifndef __pokerclient__AuditFriendCell__
#define __pokerclient__AuditFriendCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "BaseFriendCell.h"
#include "events.h"
#include "BaseFriendCell.h"
#include "ShaderSprite.h"

#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class AuditFriendCell : public BaseFriendCell{
public:
    CREATE_FUNC(AuditFriendCell);
    
    virtual bool init();
    
    void update_property(const msg::Invitation& request_data);
    msg::Invitation get_invaitation_data() {return invitation_data_;}
    
    void set_is_touch(bool is_touch = true);
    
private:
    ImageView* image_bg_;
    ImageView* image_icon_;
    Text* text_player_name_;
    Text* text_player_remark_;
    Text* text_has_confirm_;
    Button* btn_confirm_;
    
    void update_ui();
    void register_download_events();
    void handle_download_events(cmd_data_pointer);
    void btn_click_confirm(Ref * sender);
    
    cocos2d::ShaderSprite* sprite_icon_;
    events::event_dispatch dispatch_;
    Node* root_;
    
    msg::Invitation invitation_data_;
};

#endif /* defined(__pokerclient__AuditFriendCell__) */
