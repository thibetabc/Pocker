//
//  LayerResetPassword.h
//  pokerclient
//
//  Created by lvzhangle on 16/5/19.
//
//

#ifndef __pokerclient__LayerResetPassword__
#define __pokerclient__LayerResetPassword__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "LoginManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerResetPassword : public BaseLayer, public EditBoxDelegate {
    
public:
    CREATE_FUNC(LayerResetPassword);
    virtual bool init();
    
protected:
    void register_events();
    void handle_reset_password(cmd_data_pointer data);
    void handle_send_verification(cmd_data_pointer data);
private:
    void click_btn_back(Ref * sender);
    void click_btn_verification(Ref * sender);
    void click_btn_confirm(Ref * sender);
    
    void schedule_btn_verification(float dt);
private:
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
#pragma mark UI
private:
    Button* btn_back_;
    Text* text_title_;
    
    Button* btn_verification_;
    Button* btn_confirm_;
    
    ImageView* image_account_bg_;
    ImageView* image_password_bg_;
    ImageView* image_password_confirm_bg_;

    ImageView* image_verification_bg_;
    
    EditBox* tf_account_;
    EditBox* tf_password_;
    EditBox* tf_password_confirm_;
    EditBox* tf_verification_;
    
    Node* root_;
    
private:
    float current_verification_time_ = 0;
};

#endif /* defined(__pokerclient__LayerResetPassword__) */
