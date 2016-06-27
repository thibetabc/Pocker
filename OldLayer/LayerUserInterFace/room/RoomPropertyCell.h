#pragma once


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"
using namespace cocos2d;
using namespace extension;
using namespace cocos2d::ui;


class Property;

class RoomPropertyCell : public Widget, public ui::EditBoxDelegate
{
public:
    static RoomPropertyCell* create(int AParentTag, int ACellIndex);
    
    virtual bool init();
    void update_data(msg::Property *property);
    
    void click_check_box(Ref* sender, CheckBox::EventType);
    void text_menu_click(Ref *sender);
    virtual void editBoxReturn(ui::EditBox* editBox);
    
    const uint32_t get_property_value();
    const std::string get_property_name();
    const msg::PropertyType get_property_type();
    
    
    void keyBoardWillShow(const float duration, const float distance);
    void keyBoardWillHide(const float duration, const float distance);
    
    
    
    bool select_;
    
    virtual void onEnterTransitionDidFinish();
    
private:
    CREATE_FUNC(RoomPropertyCell);
    Node* root_;
    
    Text* text_property_;
    Text* text_value_;
    Text* text_menu;
    CheckBox* check_box_;
    msg::Property *property_;
    ImageView *sprite_bg;
    ImageView *arrow;
    int parentTag, selfIndex;
    
};
