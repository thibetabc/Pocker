//  群信息编辑

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerEditName : public BaseLayer
{
public:
    //    CREATE_FUNC(LayerEditName);
    
    static LayerEditName* create(const int type);
    
    virtual bool init(const int type);
    
    void text_cancel_click(Ref* sender);
    void text_save_click(Ref* sender);
    
    void click_image_male(Ref* sender);
    void click_image_female(Ref* sender);
    typedef enum
    {
        EDIT_USER_NAME,
        EDIT_USER_PASSWORD,
        EDIT_USER_SEX,
        EDIT_USER_HEAD_PIC,
        EDIT_GROUP_NAME,
        EDIT_GROUP_NOTE,
        EDIT_GROUP_PASSWORD,
        EDIT_OTHER,
    } LAYER_EDIT_TYPE;
    
    void set_title(const std::string& title);
    void set_defalut_value(const std::string& value);
    void set_number_keyboard();
    const std::string get_value();
    const bool GetValueConfirmed();
    
    
    virtual void onExit();
    //#pragma mark EditBox delegate
    //    virtual void editBoxEditingDidBegin(EditBox* editBox);
    //    virtual void editBoxEditingDidEnd(EditBox* editBox);
    //    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    //    virtual void editBoxReturn(EditBox* editBox);
    
private:
    //
    std::string password1;
    std::string password2;
    msg::GroupInfo* group_data_;
    LAYER_EDIT_TYPE edit_type_;
    EditBox* tf_name_;
    EditBox* tf_name_2;
    EditBox* tv_note_;
    Text* text_title_;
    bool ValueConfirmed = false;
    ImageView* image_bg_;
    msg::UserDetailInfo userdetailinfo_;
    Sprite* sprite_male_select_;
    Sprite* sprite_female_select_;
    Text* text_male_;
    Text* text_female_;
    ImageView* image_bg_bottom_;
};