//
//  CreateGroupLayer.h
//  pokerclient
//  创建群 界面
//
//  Created by duanmt on 15/5/4.
//
//

#ifndef __pokerclient__CreateGroupLayer__
#define __pokerclient__CreateGroupLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "events.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;


/**
 *  Note
 *  this UI is like LayerRegister, so i use LayerRegister.csb to create.
 */

class CreateGroupLayer : public BaseLayer {
    
public:
    CREATE_FUNC(CreateGroupLayer);
    virtual bool init();
    
    
    void clickCancel(Ref* sender);
    void clickRegister(Ref* sender);
    
    void register_events();
    
    void set_cancel_callback(const std::function<void()>& callback) { cancel_callback_ = callback;}
    void handle_create_group_finished(cmd_data_pointer);
private:
    //群名称
    EditBox* tf_group_name_;
    //群备注
    EditBox* tf_group_descrption_;
    //群密码
    EditBox* tf_group_password_;
    Text* text_license_;
    Text* text_referee_note_;
    
    events::event_dispatch dispatch_;
    
    std::function<void()> cancel_callback_ = nullptr;
};

#endif /* defined(__pokerclient__CreateGroupLayer__) */
