//
//  LayerCreateGroup.h
//  pokerclient
//  创建俱乐部 界面
//
//  Created by duanmt on 15/5/4.
//
//

#ifndef __pokerclient__LayerCreateGroup__
#define __pokerclient__LayerCreateGroup__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


/**
 *  Note
 *  this UI is like LayerRegister, so i use LayerRegister.csb to create.
 */

class LayerCreateGroup : public BaseLayer, public EditBoxDelegate{
public:
    CREATE_FUNC(LayerCreateGroup);
    virtual bool init();
    
    
    void clickCancel(Ref* sender);
    void clickRegister(Ref* sender);
    
    void register_events();
    
    void set_cancel_callback(const std::function<void()>& callback) { cancel_callback_ = callback;}
    void handle_create_group_finished(cmd_data_pointer);
    
    
    
#pragma mark - EditboxDelegate
private:
    void editBoxEditingDidBegin(EditBox* editBox){};
    void editBoxEditingDidEnd(EditBox* editBox);
    void editBoxTextChanged(EditBox* editBox, const std::string& text){};
    void editBoxReturn(EditBox* editBox){};
    
    void send_msg(string name, string remark);
    void set_language();
private:
    //俱乐部名称
    EditBox* tf_group_name_;
    //俱乐部备注
    EditBox* tf_group_remark_;
    //标题
    Text* text_title_;
    Button* btn_register;
    msg::GroupInfo groupinfo;
    std::function<void()> cancel_callback_ = nullptr;
};

#endif /* defined(__pokerclient__LayerCreateGroup__) */
